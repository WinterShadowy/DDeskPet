#include "LAppChatHistoryManager.h"

static QMutex g_queueMutex;          // 队列锁
static QMutex g_ioMutex;             // 文件读写锁

/*
 ============================================================================
 Function: instance
 Description: 获取聊天记录管理器的单例
 Parameters:
     - 无参数: 无释义
 Return: 返回一个聊天记录管理器的单例引用
 ============================================================================
*/
ChatHistoryManager& ChatHistoryManager::instance()
{
    static ChatHistoryManager ins;
    return ins;
}

/*
 ============================================================================
 Function: LoadChatHistory
 Description: 加载聊天记录
 Parameters:
     - int: 年
	 - int: 月
	 - int: 日
 Return: 返回加载的聊天记录 - QJsonObject
 ============================================================================
*/
QJsonObject ChatHistoryManager::LoadChatHistory(int year, int month, int day)
{
    return _LoadChatHistory(QDate(year, month, day));
}

/*
 ============================================================================
 Function: LoadChatHistory
 Description: 加载聊天记录
 Parameters:
     - const QDate&: 日期
 Return: 返回加载的聊天记录 - QJsonObject
 ============================================================================
*/
QJsonObject ChatHistoryManager::LoadChatHistory(const QDate& date)
{
    return _LoadChatHistory(date);
}

/*
 ============================================================================
 Function: InsertMessage
 Description: 插入聊天记录
 Parameters:
     - const QString&: 消息类型
	 - const QString&: 消息内容
	 - const QString&: 发送者
	 - const QString&: 发送时间
 Return: 返回是否插入成功
 ============================================================================
*/
bool ChatHistoryManager::InsertMessage(const QString& msgType, const QString& msg, const QString& sender, const QString& ts)
{
    {
        QMutexLocker locker(&g_queueMutex);
        queue_Messages.enqueue({ msgType, msg, sender, ts });
    }
    LaunchWorker();   // 保证有且仅有一个后台线程在跑
    return true;
}

int ChatHistoryManager::GetMessageCount(int year, int month, int day)
{
    QJsonObject obj = LoadChatHistory(year, month, day);
    if (obj.contains("messages") == false)
    {
        RuntimeLog_Debug("The message field does not exist");
        return -1;
    }
    return obj["messages"].toArray().size();
}

void ChatHistoryManager::Initilize(const QString& rootPath)
{
    m_root.setPath(rootPath);
    if (!m_root.exists())
        m_root.mkpath(".");
    ReadDefaultMessage();
}

ChatHistoryManager::ChatHistoryManager(QObject* parent)
    : QObject(parent)
{
}

ChatHistoryManager::~ChatHistoryManager()
{
    // 等待队列清空（简单阻塞，可再优化）
    while (!queue_Messages.isEmpty())
        QThread::msleep(50);
}

void ChatHistoryManager::ReadDefaultMessage()
{
    QString filePath = MakeFilePath(QDate(2019, 7, 24));
    QFile file(filePath);
	if (file.exists() && file.open(QIODevice::ReadOnly))
	{
		QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
		file.close();
		this->m_emptyData = doc.object();
        return;
	}
    else
    {
		RuntimeLog_Warn("ChatHistoryManager read fail");
        return;
    }
}

QJsonObject ChatHistoryManager::_LoadChatHistory(const QDate& date)
{
    QMutexLocker locker(&g_ioMutex);
    QString filePath = MakeFilePath(date);
    QFile file(filePath);
    if (!file.exists())
        return this->m_emptyData;

    if (file.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        return doc.object();
    }
    qWarning() << "ChatHistoryManager read fail" << filePath;
    return this->m_emptyData;
}

/*
 ============================================================================
 Function: _InsertMessgaeToJson
 Description: 插入聊天记录
 Parameters:
     - int: 年
	 - int: 月
	 - int: 日
	 - const ChatMessageInfo&: 聊天记录
 Return: <返回值描述>
 ============================================================================
*/
bool ChatHistoryManager::_InsertMessgaeToJson(int year, int month, int day, const ChatMessageInfo& msgInfo)
{
    QDate targetDate(year, month, day);
    QJsonObject root = _LoadChatHistory(targetDate);

    // 确保根对象里有 date 字段（只需要一次
    if (!root.contains("date"))
        root["date"] = targetDate.toString("yyyy-MM-dd");

    // 追加消息
    QJsonArray msgs = root["messages"].toArray();
    msgs.append(QJsonObject{
        {"type",   msgInfo.msgType},
        {"body",   msgInfo.msg},
        {"sender", msgInfo.sender},
        {"ts",     msgInfo.ts.toLongLong()}
        });
    root["messages"] = msgs;

    // 写回磁盘
    bool ok = SaveData(targetDate, MakeFilePath(targetDate), root);

    return ok;
}

/*
 ============================================================================
 Function: SaveData
 Description: 保存聊天记录
 Parameters:
     - const QDate&: 日期
	 - const QString&: 聊天记录路径
	 - const QJsonObject&: 聊天记录
 Return: 执行成功返回true，失败返回false
 ============================================================================
*/
bool ChatHistoryManager::SaveData(const QDate& date, const QString& filePath, const QJsonObject& content)
{
    QMutexLocker locker(&g_ioMutex);
    QJsonDocument doc(content);
    QFile file(filePath);

    auto _MakeJsonBytes = [](const QJsonObject& root) -> QByteArray {
		QJsonDocument doc(root);
		return doc.toJson(QJsonDocument::Indented);
	};

    if (file.open(QIODevice::WriteOnly))
    {
        file.write(_MakeJsonBytes(content));
        file.close();
        return true;
    }

    RuntimeLog_Fatal("ChatHistoryManager save fail");
    return false;
}

/*
 ============================================================================
 Function: MakeFilePath
 Description: 生成文件路径
 Parameters:
     - const QDate&: 日期
 Return: 返回文件路径
 ============================================================================
*/
QString ChatHistoryManager::MakeFilePath(const QDate& date) const
{
    return m_root.absoluteFilePath(date.toString("yyyy-MM-dd") + ".chathistory.json");
}

/*
 ============================================================================
 Function: GetTodayDate
 Description: 获取今天的日期
 Parameters:
     - 无参数: 无释义
 Return: 返回今天的日期
 ============================================================================
*/
QDate ChatHistoryManager::GetTodayDate()
{
    return QDate::currentDate();
}

static std::atomic_bool g_workerRunning{ false };   // 后台线程生命周期标志

/*
 ============================================================================
 Function: LaunchWorker
 Description: 启动后台线程
 Parameters:
     - 无参数: 无释义
 Return: 无返回值
 ============================================================================
*/
void ChatHistoryManager::LaunchWorker()
{
    // 已有工作者
    if (g_workerRunning.load())
        return;          

    g_workerRunning = true;
    std::thread([this] {
        while (g_workerRunning.load()) 
        {
            ChatMessageInfo task;
            // 取任务
            {   
                QMutexLocker locker(&g_queueMutex);
                if (queue_Messages.isEmpty())
                {
                    g_workerRunning = false;
                    break;
                }
                task = queue_Messages.dequeue();
            }
            // 真正写文件（线程安全）
            _InsertMessgaeToJson(GetTodayDate().year(),
                GetTodayDate().month(),
                GetTodayDate().day(),
                task);
        }
        }).detach();
}