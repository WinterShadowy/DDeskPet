
#pragma once

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <memory>

#include "SettingDialog.h"
#include "Configuration.h"
#include "CubismWindowStyle.h"
#include "SettingReusablePage.h"
#include "LAppPluginsManager.h"
#include "LAppChatHistoryManager.h"
#include "AIManager.h"
#include "ServerChildProcessManager.h"
#include "ConvenientTool.hpp"
#include "LAppModelMessage.h"

class SystemTrayManager : public QObject
{
    Q_OBJECT
public:
    static SystemTrayManager& instance();
    void init(GLCore* target = nullptr);   // target 就是 GLCore
    void showMessage(const QString& msg);

    // 初始化最基础的配置
    bool InitBasicConfiguration();
	// 初始化配置
    void InitConfig();
    void InitRuntimeErrorLog();
    // 初始化日志系统
    void InitLogSystem();
    // 读取配置文件的配置信息
    void SetRuntimErrorLog();
    void WriteProcessInfoToInterfaceJsonFile();
    // 初始化快捷键
    void RegisterHotkeyFromPluginsInfo();
    // 初始化聊天记录
    void InitChatRecord();
    //  初始化弹窗信息
    void InitModleMessageLimit();
    // 初始化AI
    void InitAI();
    // 运行服务器端口
    void RunServer();

    // 自动启动插件
    void AutomaticallyStartPlugins();

	// 销毁托盘
    void Destroy();

    // 设置窗口尺寸
    void ResizeCoreWindow(int width, int height);

    /*
        获取某些信息
    */
    // 获取配置信息
    Configuration* GetUserConfig();
private:
    SystemTrayManager() = default;
    ~SystemTrayManager() override;

    SystemTrayManager(const SystemTrayManager&) = delete;
    SystemTrayManager& operator=(const SystemTrayManager&) = delete;

    // 设置界面
    void showSettingsDialog();          // 统一的入口
    void onSettingsDialogDestroyed();   // 对话框析构时回调

private:
    QSystemTrayIcon* trayIcon = nullptr;    // 托盘
    QMenu* trayMenu = nullptr;
    QAction* showAction = nullptr;      // 展示窗口
    QAction* hideAction = nullptr;      // 隐藏窗口
    QAction* quitAction = nullptr;      // 退出
    QAction* settingsAction = nullptr;  // 菜单项
    SettingsDialog* m_SettingMenu = nullptr;
};