#include "ServerUser.h"

UserRequest& UserRequest::Instance()
{
	static UserRequest inst;
	return inst;
}

void UserRequest::ConfigureServer(const std::string& host, int port, const std::string& server_name, const std::string& caPath, const std::string& clientCert, const std::string& clientKey)
{
	{
		std::lock_guard<std::mutex> lock(m_configMutex);
		m_host = host;
		m_port = port;
		m_serverName = server_name;
		m_caPath = caPath;
		m_clientCert = clientCert;
		m_clientKey = clientKey;
	}
	ensureWorkerStarted();
}

void UserRequest::SerServerInfo(const std::string& _domain, const int port, const std::string caPath, const std::string clientCert, const std::string clientKey)
{
	{
		std::lock_guard<std::mutex> lock(m_configMutex);
		

		// 解析域名到 IP
		QHostInfo hostInfo = QHostInfo::fromName("deskpet.cpluscottage.top");
		if (hostInfo.error() != QHostInfo::NoError)
		{
			qDebug() << "Lookup failed:" << hostInfo.errorString();
			return;
		}

		// 获取第一个 IPv4 地址
		QHostAddress serverIp;
		for (const QHostAddress& address : hostInfo.addresses())
		{
			if (address.protocol() == QAbstractSocket::IPv4Protocol)
			{
				serverIp = address;
				break;
			}
		}

		m_host = serverIp.toString().toStdString();
		qDebug() << "serverIpStr:" << this->m_host;
		m_port = port;
		m_serverName = _domain;  // SNI用域名，证书校验用域名
		m_caPath = caPath;
		m_clientCert = clientCert;
		m_clientKey = clientKey; m_port = port;
		m_serverName = _domain;
		m_caPath = caPath;
		m_clientCert = clientCert;
		m_clientKey = clientKey;
	}
}

void UserRequest::ConnectAsync(AsyncCallback callback)
{
	ensureWorkerStarted();

	RequestTask task;
	task.isConnectOnly = true;
	task.timeout_ms = 0;
	task.callback = std::move(callback);

	{
		std::lock_guard<std::mutex> lock(m_queueMutex);
		// 防止极端情况下队列无限增长
		if (m_queue.size() >= MAX_PENDING)
		{
			// 队列已满，立即在当前线程回调失败
			if (task.callback)
			{
				task.callback(false, nlohmann::json{});
			}
			return;
		}
		m_queue.push_back(std::move(task));
	}
	m_queueCv.notify_one();
}

void UserRequest::SendRequestAsync(const nlohmann::json& request, AsyncCallback callback, int timeout_ms)
{
	ensureWorkerStarted();

	RequestTask task;
	task.request = request;
	task.timeout_ms = timeout_ms;
	task.callback = std::move(callback);

	{
		std::lock_guard<std::mutex> lock(m_queueMutex);
		// 简单防护：限制待处理请求数量，避免用户疯狂点击导致内存无限增长
		if (m_queue.size() >= MAX_PENDING)
		{
			if (task.callback)
			{
				// 直接回调失败，提示上层稍后重试即可。
				task.callback(false, nlohmann::json{});
			}
			return;
		}
		m_queue.push_back(std::move(task));
	}
	m_queueCv.notify_one();
}

bool UserRequest::InitConnect(std::string _host, 
	int _port,
	const std::string& server_name,
	const std::string& caPath,
	const std::string& clientCert,
	const std::string& clientKey)
{
	ConfigureServer(_host, _port, server_name, caPath, clientCert, clientKey);
	// 这里不再主动阻塞等待连接，只返回参数设置是否合理。
	if (_host.empty() || _port == 0)
	{
		qDebug() << "UserRequest::InitConnect error: host/port invalid";
		return false;
	}
	return true;
}

bool UserRequest::RequestVersionInfo(nlohmann::json& response)
{
	nlohmann::json req;
	req["type"] = "version_query";
	req["cmd"] = "get_version";
	req["user_id"] = 12345;
	return sendRequestSync(req, response, 5000);
}

bool UserRequest::RequestAuth(const std::string& account, const std::string& password, nlohmann::json& response)
{
	nlohmann::json req;
	req["type"] = "auth";
	req["account"] = account;
	req["password"] = password;
	return sendRequestSync(req, response, 5000);
}

bool UserRequest::RequestAnnouncement(nlohmann::json& response)
{
	nlohmann::json req;
	req["type"] = "announcement";
	return sendRequestSync(req, response, 5000);
}


void UserRequest::SendRequestToServer(nlohmann::json _request, nlohmann::json& _response)
{
	sendRequestSync(_request, _response, 5000);
}

UserRequest::UserRequest()
{
}

UserRequest::~UserRequest()
{
	// 通知工作线程退出
	m_stopFlag.store(true);
	m_queueCv.notify_all();
	if (m_worker.joinable())
	{
		m_worker.join();
	}
}

void UserRequest::ensureWorkerStarted()
{
	bool expected = false;
	if (m_workerStarted.compare_exchange_strong(expected, true))
	{
		m_worker = std::thread([this]() { 
			this->workerLoop(); 
			});
	}
}

void UserRequest::workerLoop()

{
	// 所有网络操作都在该线程内完成，避免阻塞 UI 线程。
	m_client.Initialize();

	while (!m_stopFlag.load())
	{
		RequestTask task;
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			m_queueCv.wait(lock, [this]() {
				return m_stopFlag.load() || !m_queue.empty();
				});

			if (m_stopFlag.load() && m_queue.empty())
				break;

			task = std::move(m_queue.front());
			m_queue.pop_front();
		}

		// 若仅仅是“连接测试”任务，则不发送任何业务数据。
		if (task.isConnectOnly)
		{
			bool ok = ensureConnectedInWorker();
			if (task.callback)
			{
				task.callback(ok, nlohmann::json{});
			}
			continue;
		}

		// 确保连接（若当前未连接，则在本次请求前尝试连接一次）。
		if (!ensureConnectedInWorker())
		{
			if (task.callback)
			{
				task.callback(false, nlohmann::json{});
			}
			continue;
		}

		uint32_t rid = 0;
		bool ok = m_client.SendJson(task.request, rid);
		nlohmann::json response;
		if (ok)
		{
			// 这里使用 request_id==0，表示“读取任意一条响应即可”，
			// 避免因服务端对 request_id 处理不一致导致匹配失败。
			ok = m_client.WaitForResponse(0 /*any*/, response, task.timeout_ms);
		}

		if (!ok)
		{
			// 当前请求失败时，主动断开连接，
			// 不做立即重连，等下一次有请求再重连以减小服务器压力。
			m_client.Disconnect();
			m_isConnect.store(false);
		}

		if (task.callback)
		{
			task.callback(ok, response);
		}
	}

	// 线程退出前清理连接。
	if (m_client.IsConnected())
	{
		m_client.Disconnect();
	}
}

bool UserRequest::ensureConnectedInWorker()
{
	if (m_isConnect.load() && m_client.IsConnected())
		return true;

	std::string host;
	int port = 0;
	std::string serverName;
	std::string caPath;
	std::string clientCert;
	std::string clientKey;
	{
		std::lock_guard<std::mutex> lock(m_configMutex);
		host = m_host;
		port = m_port;
		serverName = m_serverName;
		caPath = m_caPath;
		clientCert = m_clientCert;
		clientKey = m_clientKey;
	}

	if (host.empty() || port == 0)
	{
		qDebug() << "UserRequest::ensureConnectedInWorker error - host/port not configured";
		return false;
	}

	std::string sn = serverName.empty() ? host : serverName;
	bool ok = m_client.Connect(host, port, sn, caPath, clientCert, clientKey);
	if (ok && m_client.IsConnected())
	{
		// 按需求：连接建立完成后阻塞 0.5 秒，
		// 避免“刚连上立刻发包”可能带来的问题。
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		m_isConnect.store(true);
	}
	else
	{
		m_isConnect.store(false);
	}
	if (!ok)
	{
		qDebug() << "UserRequest::ensureConnectedInWorker error - Connect() failed";
	}
	return m_isConnect.load();
}

bool UserRequest::sendRequestSync(const nlohmann::json& request, nlohmann::json& response, int timeout_ms)
{
	// 使用 shared_ptr 确保 promise 的生命周期至少持续到回调执行完毕，
	// 避免在超时返回后回调里访问已销毁的 promise 导致崩溃。
	std::shared_ptr<std::promise<std::pair<bool, nlohmann::json>>> prom = std::make_shared<std::promise<std::pair<bool, nlohmann::json>>>();
	std::future<std::pair<bool, nlohmann::json>> fut = prom->get_future();

	SendRequestAsync(request,
		[prom](bool ok, const nlohmann::json& resp)
		{
			try
			{
				prom->set_value(std::make_pair(ok, resp));
			}
			catch (const std::future_error&)
			{
				// 若共享状态已无效或已被设置，忽略异常以防止线程终止。
			}
		},
		timeout_ms);

	if (fut.wait_for(std::chrono::milliseconds(timeout_ms)) == std::future_status::timeout)
	{
		return false;
	}

	auto result = fut.get();
	response = std::move(result.second);
	return result.first;
}