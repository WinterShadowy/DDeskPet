
#include <atomic>
#include <condition_variable>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <functional>
#include <deque>
#include <future>
#include <chrono>

#include <QSslSocket>
#include <QSslCipher>
#include <QHostAddress>
#include <QByteArray>
#include <QDataStream>
#include <QSslConfiguration>
#include <QSslCertificate>
#include <QSslKey>
#include <QFile>
#include <QDebug>
#include <QElapsedTimer>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

class EasyOpenSSL_Qt {
public:
    EasyOpenSSL_Qt();
    ~EasyOpenSSL_Qt();

    // 初始化 SSL 支持。
    // 在 Qt 应用已经创建 QCoreApplication/QApplication 之后调用。
    // 返回值：
    //  - true  表示当前平台支持 SSL，后续可调用 Connect；
    //  - false 表示当前平台不支持 SSL，无法建立 TLS 连接。
    bool Initialize();

    // 与 Initialize 对应的清理接口。
    // 当前实现仅重置内部标记，不主动关闭 socket，实际资源释放由 Disconnect 完成。
    void Cleanup();

    // 建立到服务器的 TLS 连接。
    // 参数说明：
    //  - host       : 服务器 IP 或域名（用于 TCP 连接）；
    //  - port       : 服务器监听的 TLS 端口；
    //  - server_name: SNI/证书主机名校验使用的域名，留空则使用 host；
    //  - caPath     : 额外 CA 证书（PEM）路径，留空则仅使用系统信任根；
    //  - clientCert : 客户端证书（PEM，可选，用于双向 TLS）；
    //  - clientKey  : 与 clientCert 匹配的客户端私钥（PEM，可选）。
    // 行为与返回值：
    //  - 成功返回 true，表示 TCP 连接 + TLS 握手 + 证书校验均通过；
    //  - 失败返回 false，并打印详细的错误与证书信息到调试输出。
    bool Connect(const std::string& host, int port,
        const std::string& server_name = std::string(),
        const std::string& caPath = std::string(),
        const std::string& clientCert = std::string(),
        const std::string& clientKey = std::string());

    // 主动断开当前连接并释放 QSslSocket 资源。
    // 若当前未连接，则该函数为空操作。
    void Disconnect();

    // 发送一条 JSON 请求。
    //  - 会自动为本次请求分配一个全局自增的 request_id 字段并写入 JSON；
    //  - 实际发送的数据格式为：[4 字节大端长度][JSON 文本]；
    //  - out_request_id 用于后续在 WaitForResponse 中匹配对应的响应。
    bool SendJson(const json& j, uint32_t& out_request_id);

    // 阻塞等待指定 request_id 对应的 JSON 响应。
    //  - timeout_ms < 0 表示无限等待；
    //  - 收到的其它 request_id 的消息会被暂存到缓存 map 中，供之后查询；
    //  - 成功返回 true，并在 out 中输出完整 JSON；超时或连接异常返回 false。
    bool WaitForResponse(uint32_t request_id, json& out, int timeout_ms = -1);

    // 当前 TLS 连接是否处于已建立状态。
    // 注意：仅表示逻辑上的“已连接”，并不主动探测对端是否已经关闭。
    bool IsConnected() const;

private:
    // 从套接字中读取一条完整的 JSON 消息。
    //  - 先读取 4 字节长度前缀（大端），再读取指定长度的正文；
    //  - 内部会处理分包/粘包问题；
    //  - 返回 false 表示读取失败或连接已不可用。
    bool readOneMessage(json& out_msg, int wait_ms);

    // 底层 TLS 套接字，仅在 m_socketmutex_ 保护下访问。
    QSslSocket* m_socket = nullptr;
    // 保护 m_socket 以及 write/读操作，避免多线程并发访问。
    std::mutex m_socket_mutex;

    // 缓存「非当前请求」的其它响应，按 request_id 进行索引。
    std::mutex m_esponses_mutex;
    std::condition_variable m_response_cv;
    std::map<uint32_t, json> m_responses;

    // request_id 全局递增计数器，从 1 开始分配。
    std::atomic<uint32_t> m_request_id_counter{ 1 };
    // 当前连接状态标记，仅表示逻辑上的“已建立连接”。
    std::atomic<bool> m_connection_active{ false };
    // 是否已经成功调用 Initialize。
    bool m_initialized = false;
};

// ---------------- Implementation ----------------

inline EasyOpenSSL_Qt::EasyOpenSSL_Qt() 
{
}

inline EasyOpenSSL_Qt::~EasyOpenSSL_Qt()
{
    Disconnect();
    Cleanup();
}

inline bool EasyOpenSSL_Qt::Initialize()
{
    if (this->m_initialized)
        return true;
    // 注意：调用前需要已经创建 QCoreApplication/QApplication，
    // 否则部分 Qt 网络功能可能不可用。
    if (!QSslSocket::supportsSsl())
    {
        qWarning("QSslSocket: SSL not supported on this platform");
        return false;
    }
    this->m_initialized = true;
    return true;
}

inline void EasyOpenSSL_Qt::Cleanup()
{
    this->m_initialized = false;
}

inline bool EasyOpenSSL_Qt::Connect(const std::string& host, int port, const std::string& server_name,
    const std::string& caPath, const std::string& clientCert,
    const std::string& clientKey)
{
    // 确保已经完成 SSL 能力检测。
    if (!this->m_initialized)
    {
        if (!Initialize())
            return false;
    }

    if (this->m_connection_active)
        return true;

    std::lock_guard<std::mutex> lock(this->m_socket_mutex);
    if (this->m_socket)
    {
        Disconnect();
    }

    this->m_socket = new QSslSocket();

    // 配置 SSL 参数：CA、客户端证书、私钥、协议版本等。
    QSslConfiguration cfg = this->m_socket->sslConfiguration();
    bool hasCA = false;
    if (!caPath.empty()) 
    {
        QList<QSslCertificate> cas = QSslCertificate::fromPath(QString::fromStdString(caPath));
        if (!cas.isEmpty()) 
        {
            for (const QSslCertificate& c : cas)
                cfg.addCaCertificate(c);
            hasCA = true;
        }
        else 
        {
            qDebug() << "EasyOpenSSL_Qt::Connect warning - failed to load CA from" << QString::fromStdString(caPath);
        }
        qDebug() << "EasyOpenSSL_Qt::Connect debug - loaded CA count:" << cas.size();
        for (const QSslCertificate& ca : cas)
        {
            qDebug() << "  CA subject:" << ca.subjectDisplayName();
        }
    }
    if (hasCA) 
    {
        cfg.setPeerVerifyMode(QSslSocket::VerifyPeer);
    }
    else 
    {
        // 没有CA时，依然要求校验，但会失败（安全）
        cfg.setPeerVerifyMode(QSslSocket::VerifyPeer);
    }
    if (!clientCert.empty() && !clientKey.empty()) 
    {
        QList<QSslCertificate> loc = QSslCertificate::fromPath(QString::fromStdString(clientCert));
        if (!loc.isEmpty()) 
            cfg.setLocalCertificate(loc.first());
        QFile keyFile(QString::fromStdString(clientKey));
        if (keyFile.open(QIODevice::ReadOnly)) 
        {
            QByteArray keyData = keyFile.readAll();
            keyFile.close();
            QSslKey k(keyData, QSsl::Rsa);
            if (!k.isNull()) 
                cfg.setPrivateKey(k);
            else 
                qDebug() << "EasyOpenSSL_Qt::Connect warning - failed to parse client private key";
        }
        else 
        {
            qDebug() << "EasyOpenSSL_Qt::Connect warning - failed to open client key file" << QString::fromStdString(clientKey);
        }
    }
    this->m_socket->setSslConfiguration(cfg);
    this->m_socket->setProtocol(QSsl::AnyProtocol);

    // 设置 SNI 与证书主机名校验的目标域名。
   
    QString sni = server_name.empty() ? QString::fromStdString(host) : QString::fromStdString(server_name);
    this->m_socket->connectToHostEncrypted(QString::fromStdString(host), port, sni);

    // ========= TODO ========= 阻塞了主进程
    // 将握手等待时间设置为 15 秒，避免网络较慢时过早超时。 
    if (!this->m_socket->waitForEncrypted(15000))
    {
        qDebug() << "EasyOpenSSL_Qt::Connect error - encrypted handshake failed:" << this->m_socket->errorString();
        qDebug() << "  socket state:" << this->m_socket->state();
        qDebug() << "  peer name used for handshake:" << sni;
        QList<QSslError> errs;
        this->m_socket->sslErrors(errs);
        for (const auto& e : errs) 
        {
            qDebug() << "  sslError:" << e.errorString();
        }
        QSslConfiguration cur = this->m_socket->sslConfiguration();
        qDebug() << "  protocol:" << cur.protocol();
        QList<QSslCipher> ciphers = cur.ciphers();
        QStringList cipherNames;
        for (const QSslCipher& c : ciphers)
            cipherNames << c.name();
        qDebug() << "  ciphers:" << cipherNames.join(",");
        // 输出本地CA证书信息
        QList<QSslCertificate> caList = cur.caCertificates();
        qDebug() << "  loaded CA certs:" << caList.size();
        for (const QSslCertificate& ca : caList)
        {
            qDebug() << "    CA subject:" << ca.subjectDisplayName();
        }
        // 输出服务端证书信息
        QSslCertificate peerCert = this->m_socket->peerCertificate();
        if (!peerCert.isNull()) 
        {
            qDebug() << "  peer certificate subject:" << peerCert.subjectDisplayName();
            qDebug() << "  peer certificate issuer:" << peerCert.issuerDisplayName();
            qDebug() << "  peer certificate effective date:" << peerCert.effectiveDate();
            qDebug() << "  peer certificate expiry date:" << peerCert.expiryDate();
        }
        else 
        {
            qDebug() << "  peer certificate is null (no certificate received)";
        }
        delete this->m_socket;
        this->m_socket = nullptr;
        return false;
    }

    // 检查证书校验结果（Qt 6 无 isValid，需要手动判断有效期）。
    if (this->m_socket->peerVerifyMode() == QSslSocket::VerifyPeer) 
    {
        const QSslCertificate cert = this->m_socket->peerCertificate();
        if (cert.isNull()) 
        {
            qDebug() << "EasyOpenSSL_Qt::Connect error - peer certificate is null!";
            Disconnect();
            return false;
        }
        // 检查有效期
        QDateTime now = QDateTime::currentDateTimeUtc();
        if (cert.effectiveDate() > now || cert.expiryDate() < now) 
        {
            qDebug() << "EasyOpenSSL_Qt::Connect error - peer certificate expired or not yet valid!";
            Disconnect();
            return false;
        }
    }

    this->m_connection_active = true;
    return true;
}

inline void EasyOpenSSL_Qt::Disconnect()
{
    if (!this->m_connection_active)
        return;

    std::lock_guard<std::mutex> lock(this->m_socket_mutex);
    if (this->m_socket)
    {
        this->m_socket->disconnectFromHost();
        this->m_socket->waitForDisconnected(2000);
        delete this->m_socket;
        this->m_socket = nullptr;
    }
    this->m_connection_active = false;
}

inline bool EasyOpenSSL_Qt::SendJson(const json& j, uint32_t& out_request_id)
{
    if (!this->m_connection_active || !this->m_socket)
        return false;

    json msg = j;
    uint32_t rid = this->m_request_id_counter.fetch_add(1);
    msg["request_id"] = rid;
    out_request_id = rid;

    std::string s = msg.dump();
    uint32_t len = static_cast<uint32_t>(s.size());

    std::lock_guard<std::mutex> lock(this->m_socket_mutex);
    if (!this->m_socket)
        return false;

    QByteArray out;
    QByteArray headerBytes;
    QDataStream headerStream(&headerBytes, QIODevice::WriteOnly);
    headerStream.setByteOrder(QDataStream::BigEndian);
    headerStream << quint32(len);

    out.append(headerBytes);
    out.append(s.c_str(), s.size());

    qint64 written = this->m_socket->write(out);
    if (written != out.size()) 
        return false;
    if (!this->m_socket->waitForBytesWritten(5000)) 
        return false;

    return true;
}

inline bool EasyOpenSSL_Qt::readOneMessage(json& out_msg, int wait_ms)
{
    if (!this->m_connection_active || !this->m_socket)
        return false;

    // 注意：wait_ms 代表整帧（长度+正文）的最大等待时间，而不是单次调用超时。
    // 使用 QElapsedTimer 追踪总耗时，避免在读到一半时因为单次 waitForReadyRead 超时
    // 而中途放弃，导致协议帧被拆坏。

    if (wait_ms < 0)
        wait_ms = 30000; // 默认最多等 30 秒

    QElapsedTimer timer;
    timer.start();

    auto remaining = [&]() -> int {
        qint64 elapsed = timer.elapsed();
        qint64 left = wait_ms - elapsed;
        return left > 0 ? static_cast<int>(left) : 0;
        };

    // 1) 读取长度前缀（4 字节大端）
    while (this->m_socket->bytesAvailable() < (int)sizeof(uint32_t))
    {
        int left = remaining();
        if (left <= 0)
        {
            qDebug() << "EasyOpenSSL_Qt::readOneMessage timeout waiting header, bytesAvailable="
                << this->m_socket->bytesAvailable();
            return false;
        }
        if (!this->m_socket->waitForReadyRead(left))
        {
            // 再次检查是否有数据到达，防止刚好在边界上醒来
            if (this->m_socket->bytesAvailable() < (int)sizeof(uint32_t))
            {
                qDebug() << "EasyOpenSSL_Qt::readOneMessage waitForReadyRead header timeout, bytesAvailable="
                    << this->m_socket->bytesAvailable();
                return false;
            }
            break;
        }
    }

    QByteArray lenbuf = this->m_socket->read(sizeof(uint32_t));
    while (lenbuf.size() < (int)sizeof(uint32_t))
    {
        int left = remaining();
        if (left <= 0)
        {
            qDebug() << "EasyOpenSSL_Qt::readOneMessage timeout completing header, got=" << lenbuf.size();
            return false;
        }
        if (!this->m_socket->waitForReadyRead(left))
        {
            if (this->m_socket->bytesAvailable() == 0)
            {
                qDebug() << "EasyOpenSSL_Qt::readOneMessage waitForReadyRead header(2) timeout, got="
                    << lenbuf.size();
                return false;
            }
        }
        QByteArray more = this->m_socket->read(sizeof(uint32_t) - lenbuf.size());
        if (more.isEmpty())
        {
            qDebug() << "EasyOpenSSL_Qt::readOneMessage read header returned empty, state="
                << this->m_socket->state() << " error=" << this->m_socket->errorString();
            return false;
        }
        lenbuf.append(more);
    }

    if (lenbuf.size() != (int)sizeof(uint32_t))
        return false;

    QDataStream lenStream(lenbuf);
    lenStream.setByteOrder(QDataStream::BigEndian);
    quint32 len = 0;
    lenStream >> len;
    if (len == 0 || len > 50 * 1024 * 1024)
    {
        qDebug() << "EasyOpenSSL_Qt::readOneMessage invalid length:" << len;
        return false;
    }

    // 2) 按长度读取完整消息体
    QByteArray body;
    body.resize(len);
    qint64 got = 0;
    while (got < (qint64)len)
    {
        int left = remaining();
        if (left <= 0)
        {
            qDebug() << "EasyOpenSSL_Qt::readOneMessage timeout reading body, got=" << got << "len=" << len;
            return false;
        }

        if (this->m_socket->bytesAvailable() == 0)
        {
            if (!this->m_socket->waitForReadyRead(left))
            {
                if (this->m_socket->bytesAvailable() == 0)
                {
                    qDebug() << "EasyOpenSSL_Qt::readOneMessage waitForReadyRead body timeout, got="
                        << got << "len=" << len;
                    return false;
                }
            }
        }

        qint64 r = this->m_socket->read(body.data() + got, len - got);
        if (r <= 0)
        {
            qDebug() << "EasyOpenSSL_Qt::readOneMessage body read failed, r=" << r
                << " state=" << this->m_socket->state() << " error=" << this->m_socket->errorString();
            return false;
        }
        got += r;
    }

    try 
    {
        std::string s(body.constData(), body.size());
        out_msg = json::parse(s);
        return true;
    }
    catch (...) 
    {
        qDebug() << "EasyOpenSSL_Qt::readOneMessage json parse failed";
        return false;
    }
}

inline bool EasyOpenSSL_Qt::WaitForResponse(uint32_t request_id, json& out, int timeout_ms)
{
    if (!this->m_connection_active || !this->m_socket) 
        return false;

    int wait_ms = (timeout_ms < 0) ? 30000 : timeout_ms;
    json msg;
    if (!readOneMessage(msg, wait_ms))
        return false;

    out = std::move(msg);
    return true;
}

inline bool EasyOpenSSL_Qt::IsConnected() const
{
    return this->m_connection_active.load();
}
