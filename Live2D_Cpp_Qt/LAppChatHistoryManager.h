
/*
 ============================================================================
 Header: LAppChatHistoryManager.h
 Description: 聊天记录管理类
 Start time: 2025.10.25
 End time: 2026.01.07
 Maintainer: WangSZ
 ============================================================================
*/

#pragma once

#include "LAppDefine.hpp"

#include <QObject>
#include <QJsonObject>
#include <QDate>
#include <QDir>
#include <QQueue>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <QDebug>


typedef struct {
    QString msgType;
	QString msg;
	QString sender;
	QString ts;
}ChatMessageInfo;

class ChatHistoryManager : public QObject
{
    Q_OBJECT
public:
    static ChatHistoryManager& instance();
    // 加载聊天记录
    QJsonObject LoadChatHistory(int year, int month, int day);
    QJsonObject LoadChatHistory(const QDate& date);
    // 插入聊天记录 - 只插入今天的
    bool InsertMessage(const QString& msgType, const QString& msg, const QString& sender, const QString& ts);
    // 获取指定日期的聊天记录数量
    int GetMessageCount(int year, int month, int day);
    // 初始化路径
    void Initilize(const QString& rootPath);
private:
    // 构造函数
    explicit ChatHistoryManager(QObject* parent = nullptr);
    // 析构函数
    ~ChatHistoryManager();
    // 读取默认消息
    void ReadDefaultMessage();
    // 插入聊天记录
    bool _InsertMessgaeToJson(int year, int month, int day, const ChatMessageInfo& msgInfo);
    // 加载聊天记录实现
    QJsonObject _LoadChatHistory(const QDate& date);
    // 保存聊天记录
    bool SaveData(const QDate& date, const QString& qstr_filePath, const QJsonObject& content);
    // 获取文件路路径 - 日期
    QString MakeFilePath(const QDate& date) const;
    // 获取今天的日期
    QDate GetTodayDate();
    // 启动线程
    void LaunchWorker();

private:
    QDir m_root;   // 聊天记录存放文件夹路径
    QQueue<ChatMessageInfo> queue_Messages;   // 消息队列
    QJsonObject m_emptyData;        // 查找到的消息为空时，就使用此数据
};

#define GetChatHistoryManagerInstance() \
    ChatHistoryManager::instance()