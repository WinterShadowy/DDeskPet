#include "ServerChildProcessManager.h"
#include <QCoreApplication>
#include <QHostAddress>
#include <QDebug>

ChildManager* ChildManager::instance()
{
    static ChildManager inst;
    return &inst;
}

ChildManager::ChildManager(QObject* parent)
    : QObject(parent)
{
    m_server = new QTcpServer(this);
   
        
    // qInfo() << "Parent listening on" << PORT;
    // 注册处理函数，处理来自子进程的消息，此类函数需要向服务端返回消息
    RegisterLAppHandler(REQUEST_AI,             RecordChatContent); // 弃用
    RegisterLAppHandler(RECORD_CHAT,            RecordChatContent);
    RegisterLAppHandler(TEST_REQUEST_AI,        RecordChatContent_Test);    // 弃用
    RegisterLAppHandler(SHOW_MSG,               ShowMessage);
    RegisterLAppHandler(TEST_SHOW_MSG,          ShowMessage_Test);
    RegisterLAppHandler(UNDEFINED_MSG,          UndefinedMessage);
    RegisterLAppHandler(GET_PROCESS_INFO,       _GetProcessID);
    // 注册返回消息，此类函数不需要向服务端返回消息
    this->m_handlerMap_void.insert(RETURN_MSG,      ReturnMsg);
    this->m_handlerMap_void.insert(INIT_AI_MSG,     InitAIMsg);

    this->m_MessageHandler.insert(REQUEST_AI,           _LApp_RETURN_QJSONOBJ);
	this->m_MessageHandler.insert(TEST_REQUEST_AI,      _LApp_RETURN_QJSONOBJ);
	this->m_MessageHandler.insert(SHOW_MSG,             _LApp_RETURN_QJSONOBJ);
	this->m_MessageHandler.insert(TEST_SHOW_MSG,        _LApp_RETURN_QJSONOBJ);
	this->m_MessageHandler.insert(UNDEFINED_MSG,        _LApp_RETURN_QJSONOBJ);
    this->m_MessageHandler.insert(GET_PROCESS_INFO,     _LApp_RETURN_QJSONOBJ);
    this->m_MessageHandler.insert(RETURN_MSG,           _LApp_RETURN_VOID);
	this->m_MessageHandler.insert(INIT_AI_MSG,          _LApp_RETURN_VOID);
}

ChildManager::~ChildManager()
{
    CloseAll();
}

bool ChildManager::RunChildProcess(const QString& exePath)
{
    if (m_proc)          // 已有进程，先清理
        CloseAll();

    m_restartCnt = 0;
    if (exePath.isEmpty())
    {
        RuntimeLog_Warn("exePath is empty");
		return false;
    }
    else
    {
        m_exePath = exePath;
    }

    if(Config_Developer::instance().GetUseListenPort() == true)
    {
        connect(m_server, &QTcpServer::newConnection, this, &ChildManager::onNewConnection);
        this->PORT = Config_Developer::instance().GetListenPort();
        if (!m_server->listen(QHostAddress::LocalHost, this->PORT))
        {
            /*qFatal("ChildManager: cannot listen on 19724");*/
            RuntimeLog_Error("cannot listen on 19724");
            QMetaObject::invokeMethod(qApp, &QCoreApplication::quit, Qt::QueuedConnection);
        }
        // 确保 server 已经在监听
        if (!m_server->isListening())
        {
            RuntimeLog_Error("Server not listening, cannot start child");
            return false;
        }
        else
        {
            qDebug() << "监听已经启动" << this->PORT;
        }
    }

    if (Config_Developer::instance().GetUseNetService() == true)
    {
        KickChild();         // 启动进程
    }
    return true;
}

void ChildManager::ContentProcessing(QJsonObject& obj)
{
    QString type;
    if (obj.contains("type"))
        type = obj["type"].toString();
    else
        type = UNDEFINED_MSG;

    QHash<QString, msgType>::iterator it = this->m_MessageHandler.find(type);
    bool hasMsgBody = obj.contains("message_body");
    if (hasMsgBody)
    {
        switch (it.value())
        {
        case _LApp_RETURN_QJSONOBJ:
            SendJson(this->m_handlerMap[type](obj));
            return;
        case _LApp_RETURN_VOID:
            this->m_handlerMap_void[type](obj);
            return;
        }
    }
    else
    {
        QJsonObject reply = obj;
		reply.insert("da_mie_da_mie", "?奇怪的Json增加了[emoji - 知识增加]");
		SendJson(reply);
    }
    return;
}

void ChildManager::KickChild()
{
    Q_ASSERT(!m_proc);
    m_proc = new QProcess(this);
    m_proc->setProgram(m_exePath);
    //m_proc->start();
    // 把父进程监听端口通过命令行参数告诉子进程，方便它连接
    m_proc->setArguments({ QString::number(PORT) });

    connect(m_proc, &QProcess::errorOccurred, [](QProcess::ProcessError e) {
		RuntimeLog_Error(QString("child error:%1").arg(e).toStdString().c_str());
        });
    connect(m_proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &ChildManager::onProcFinished);

    m_proc->start();
    if (!m_proc->waitForStarted(3000))
    {
		RuntimeLog_Error(QString("child start failed").toStdString().c_str());
        m_proc->deleteLater();
        m_proc = nullptr;
    }
    else 
    {
        qInfo() << "child started, pid =" << m_proc->processId();
    }
}

void ChildManager::CloseAll()
{
    if (m_socket)
    {
        m_socket->close();   // 会触发 onSocketDisconnected
        m_socket = nullptr;
    }
    if (m_proc) 
    {
        m_proc->terminate();
        if (!m_proc->waitForFinished(3000))
            m_proc->kill();
        m_proc->deleteLater();
        m_proc = nullptr;
    }
}

// 父子 发送 JSON
void ChildManager::SendJson(const QJsonObject& obj)
{
    if (!m_socket || m_socket->state() != QAbstractSocket::ConnectedState)
        return;
    QJsonDocument doc(obj);
    QByteArray payload = doc.toJson(QJsonDocument::Compact);
    if (payload.size() > MAX_JSON)
    {
        RuntimeLog_Warn("json too large");
        return;
    }
    quint32 len = qToBigEndian<quint32>(payload.size());
    m_socket->write(reinterpret_cast<const char*>(&len), 4);
    m_socket->write(payload);
    m_socket->flush();
    return;
}

void ChildManager::RegisterLAppHandler(QString key, _LApp_Handler_QJsonObject _lapp_handler)
{
	this->m_handlerMap.insert(key, _lapp_handler);
	return;
}

void ChildManager::UnRegisterHandler(QString key)
{
    this->m_handlerMap.remove(key);
    return;
}

void ChildManager::ClearAllLappHandler()
{
    this->m_handlerMap.clear();
    return;
}

// ---------- 网络槽函数 ----------
void ChildManager::onNewConnection()
{
    while (m_server->hasPendingConnections())
    {
        QTcpSocket* s = m_server->nextPendingConnection();
        if (m_socket)
        {
			RuntimeLog_Warn(QString("extra connection dropped").toStdString().c_str());
            s->close();
            s->deleteLater();
            continue;
        }
        m_socket = s;
        connect(m_socket, &QTcpSocket::readyRead, this, &ChildManager::onSocketReadyRead);
        connect(m_socket, &QTcpSocket::disconnected, this, &ChildManager::onSocketDisconnected);

        RuntimeLog_Info(QString("child socket connected").toStdString().c_str());
        qDebug() << "连接到子进程：" << m_socket->peerAddress().toString() << ":" << m_socket->peerPort();

        // 连接成功后发送模板
        /*QString strJson  = QString::fromStdString(AIManager::instance().GetAIJsonTemplate_NJson().dump());
        QJsonObject jsonBuffer = QJsonDocument::fromJson(strJson.toUtf8()).object();*/
        SendJson(AIManager::instance().GetAIJsonTemplate());
    }
}

void ChildManager::onSocketDisconnected()
{
    RuntimeLog_Error(QString("child socket lost").toStdString().c_str());
    if (m_socket) 
    {
        m_socket->deleteLater();
        m_socket = nullptr;
    }
}

void ChildManager::onSocketReadyRead()
{
    while (m_socket->bytesAvailable() > 0) 
    {
        if (m_expectBytes == 0) 
        {   // 等待长度头
            if (m_socket->bytesAvailable() < 4) 
                return;
            quint32 net;
            m_socket->read(reinterpret_cast<char*>(&net), 4);
            m_expectBytes = qFromBigEndian<quint32>(net);
            if (m_expectBytes > MAX_JSON) 
            {   // 防恶意
                //qWarning() << "json length too large" << m_expectBytes;
				RuntimeLog_Warn(QString("json length too large:%1").arg(m_expectBytes).toStdString().c_str());
                m_socket->close();
                return;
            }
        }
        if (m_socket->bytesAvailable() < m_expectBytes) 
            return;
        m_buffer = m_socket->read(m_expectBytes);
        m_expectBytes = 0;

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(m_buffer, &err);
        if (err.error != QJsonParseError::NoError) 
        {
            continue;
        }
        QJsonObject obj = doc.object();
        //qDebug() << "parent recv" << obj;

        this->ContentProcessing(obj); // 业务处理 obj
    }
}

void ChildManager::onProcFinished(int exitCode, QProcess::ExitStatus st)
{
    m_proc->deleteLater();
    m_proc = nullptr;
    if (m_restartCnt < MAX_RESTART) 
    {
        ++m_restartCnt;
        qInfo() << "restart child" << m_restartCnt;
        KickChild();
    }
}