/*
 ============================================================================
 Header: ServerChildProcessManager.h
 Description: 子进程管理 - 对服务器子进程进行管理
 Start time: 2025.11.15
 End time: 2026.01.07
 Maintainer: WangSZ
 ============================================================================
*/

#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QProcess>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <functional>

#include "AIManager.h"
#include "LAppDefine.hpp"
#include "LAppModelMessage.h"
#include "ServerRequestProcessor.h"

#define _LApp_Handler_QJsonObject   std::function<QJsonObject(const QJsonObject&)>
#define _LApp_Handler_void          std::function<void(const QJsonObject&)>

#define _LApp_RETURN_QJSONOBJ     static_cast<unsigned char>(1) // 1
#define _LApp_RETURN_VOID         static_cast<unsigned char>(0) // 2
// 消息类型
// AI请求
#define REQUEST_AI              "request_AI"
// 测试AI请求
#define TEST_REQUEST_AI         "test_request_AI"
// 记录聊天记录
#define RECORD_CHAT             "record_chat"
// 在模型窗口上显示消息
#define SHOW_MSG                "show_msg"
// 测试在模型窗口上显示消息
#define TEST_SHOW_MSG           "test_show_msg"
// 获取进程信息
#define GET_PROCESS_INFO        "get_process_info"
// 获取服务器信息
#define GET_SERVER_INFO         "get_server_info"
// 获取账号信息
#define GET_ACCOUNT_INFO        "get_account_info"
// 获取json模板
#define GET_JSON_TEMPLATE       "get_json_template"
// 未定义的消息
#define UNDEFINED_MSG           "undefined_msg"
// 客户端收到消息返回消息
// 收到模板时返回的消息
#define RETURN_MSG              "return_msg"
// 初始化AI模块后的消息
#define INIT_AI_MSG             "init_ai_msg"

class ChildManager : public QObject
{
    Q_OBJECT
public:
    static ChildManager* instance();
    // 启动子进程并建立连接；可重复调用，会先清理旧进程
    bool RunChildProcess(const QString& exePath);

private:
    explicit ChildManager(QObject* parent = nullptr);
    ~ChildManager();
    // 内容处理 
    void ContentProcessing(QJsonObject& obj);  
    // 真正 fork 子进程
    void KickChild();  
    // 清理 socket & 进程
    void CloseAll(); 
    // 父->子
    void SendJson(const QJsonObject& obj); 
    // 注册功能
    void RegisterLAppHandler(QString key, _LApp_Handler_QJsonObject _lapp_handler);
    // 删除注册功能
	void UnRegisterHandler(QString key);
    // 清理注册功能
    void ClearAllLappHandler();

private:
    // ------ 网络 ------
    QTcpServer* m_server = nullptr;
    QTcpSocket* m_socket = nullptr;   // 当前已建立的连接
    quint16 PORT = 19724;
    static constexpr int MAX_JSON = 100000;

    // ------ 进程 ------
    QProcess* m_proc = nullptr;
    int m_restartCnt = 0;
    static constexpr int MAX_RESTART = 2;

    // ------ 协议解析 ------
    qint32 m_expectBytes = 0;
    QByteArray m_buffer;

    // ------ 可执行文件 ------
    QString m_exePath;
    // ------ 处理收到的消息 ------
    using msgType = char;
    QHash<QString, _LApp_Handler_QJsonObject> m_handlerMap;
    QHash<QString, _LApp_Handler_void> m_handlerMap_void;
    QHash<QString, msgType> m_MessageHandler;

private slots:
    void onNewConnection();
    void onSocketReadyRead();
    void onSocketDisconnected();
    void onProcFinished(int exitCode, QProcess::ExitStatus);
};