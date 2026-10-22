
/*
 ============================================================================
 Header: ServerUser.h
 Description: 用户请求类
 Start time: 2025.10.30
 End time: 2026.01.07
 Maintainer: WangSZ
 ============================================================================
*/

#pragma once

#include "EasyOpenSSL.hpp"
#include "LAppDefine.hpp"

#include <string>
#include <QString>
#include <QMessageBox>
#include <QHostInfo>
#include <QHostAddress>

class UserRequest
{
public:
    static UserRequest& Instance();

    // ================= 新接口设计（多线程 + 按需重连） =================

    // 配置服务器连接参数，并启动后台工作线程（如尚未启动）。
    // 这个函数本身是非阻塞的，不会去做真实的网络握手，只是保存参数。
    void ConfigureServer(const std::string& host, 
        int port,
        const std::string& server_name = std::string(),
        const std::string& caPath = std::string(),
        const std::string& clientCert = std::string(),
        const std::string& clientKey = std::string());

    void SerServerInfo(const std::string& _domain,
        const int port,
        const std::string caPath = std::string(),
        const std::string clientCert = std::string(),
        const std::string clientKey = std::string());

    using AsyncCallback = std::function<void(bool, const nlohmann::json&)>;

    // 仅进行一次“建立连接”的异步操作，不发送任何业务数据。
    //  - 在工作线程中调用 ensureConnectedInWorker()；
    //  - 连接成功/失败后通过 callback 把结果通知调用方；
    //  - 适合用于“连接并初始化”按钮，只更新 UI 状态。
    void ConnectAsync(AsyncCallback callback);

    // 异步发送任意 JSON 请求：
    //  - 在内部工作线程中完成 Connect/SendJson/WaitForResponse；
    //  - 若当前未连接，线程会在处理本次请求前尝试连接；
    //  - 若连接中途断开，只会断开当前连接，不会立即重连；
    //    下一次有新的请求进入队列时才会重新尝试连接。
    //  - callback 在线程中执行，你若在 Qt UI 中更新控件，
    //    请使用 QMetaObject::invokeMethod 切回主线程。
    void SendRequestAsync(const nlohmann::json& request, AsyncCallback callback, int timeout_ms = 5000);

    // ========== 向下兼容的同步接口（仍然会阻塞当前线程） ==========
    // 注意：为了保证 UI 不被阻塞，请勿在界面线程直接调用这些同步函数，
    // 建议全部改用 SendRequestAsync。

    bool InitConnect(std::string _host = "103.23.149.189", int _port = 20134,
        const std::string& server_name = std::string(),
        const std::string& caPath = std::string(),
        const std::string& clientCert = std::string(),
        const std::string& clientKey = std::string());

    // 版本查询（同步封装，不推荐在 UI 线程直接使用）
    bool RequestVersionInfo(nlohmann::json& response);

    // 账户验证（同步封装，不推荐在 UI 线程直接使用）
    bool RequestAuth(const std::string& account, const std::string& password, nlohmann::json& response);

    // 公告获取（同步封装，不推荐在 UI 线程直接使用）
    bool RequestAnnouncement(nlohmann::json& response);

    // 原有的同步发送接口（不推荐在 UI 线程直接使用）
    void SendRequestToServer(nlohmann::json _request, nlohmann::json& _response);

private:
    UserRequest();
    ~UserRequest();

    UserRequest(const UserRequest&) = delete;

private:
    // ========== 工作线程与任务队列 ==========
    struct RequestTask
    {
        nlohmann::json request;
        int timeout_ms = 5000;
        AsyncCallback callback;
        bool isConnectOnly = false;
    };

    void ensureWorkerStarted();

    void workerLoop();

    bool ensureConnectedInWorker();

    // 同步封装：基于异步接口 + promise 实现，避免重复代码。
    bool sendRequestSync(const nlohmann::json& request, nlohmann::json& response, int timeout_ms);

private:
    EasyOpenSSL_Qt m_client;

    std::atomic<bool> m_isConnect{ false };

    // 连接配置（仅在 m_configMutex 保护下访问）
    std::mutex m_configMutex;
    std::string m_host;
    int m_port = 0;
    std::string m_serverName;
    std::string m_caPath;
    std::string m_clientCert;
    std::string m_clientKey;

    // 任务队列
    std::mutex m_queueMutex;
    std::condition_variable m_queueCv;
    std::deque<RequestTask> m_queue;

    // 防止极端情况下任务堆积过多
    static constexpr size_t MAX_PENDING = 64;

    // 工作线程相关
    std::thread m_worker;
    std::atomic<bool> m_stopFlag{ false };
    std::atomic<bool> m_workerStarted{ false };
};
