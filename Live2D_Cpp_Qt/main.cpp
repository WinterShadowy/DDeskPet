#include "GLCore.h"
#include "LAppTray.hpp"
#include <QtWidgets/QApplication>
#include <QDebug>

int __cdecl MyReportHook(int reportType, char* msg, int* retVal)
{
    // 只要是 _CRT_ASSERT 就立即断点
    if (reportType == _CRT_ASSERT)
    {
        __debugbreak();          // 调试器立刻中断
        *retVal = 1;             // 告诉 CRT "已处理"
        return TRUE;             // 不再弹窗
    }
    return FALSE;                // 其他报告走默认流程
}

int main(int argc, char* argv[])
{
#if ((defined(DEBUG) | defined(_DEBUG) ) && defined(_MSC_VER))
    _CrtSetReportHook(MyReportHook);   // 安装钩子
    // 打开全检查标志
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif

    QApplication app(argc, argv);
    qDebug() << "Application Dir Path: " << QCoreApplication::applicationDirPath();
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    // 初始化基础配置
    if (SystemTrayManager::instance().InitBasicConfiguration() == false)
    {
        return 0;
    }
    else
    {
        qDebug() << "Init Basic Configuration completed.";
    }
    // 初始化日志文件路径
    //SystemTrayManager::instance().InitRuntimeErrorLog();
    SystemTrayManager::instance().InitLogSystem();
    qDebug() << "Init Runtime Error Log completed.";

    // 初始化配置信息 & 读取配置文件
    SystemTrayManager::instance().InitConfig();
    qDebug() << "Init Config completed.";

    // 设置运行时日志
    //SystemTrayManager::instance().SetRuntimErrorLog();
    //qDebug() << "Set Runtime Error Log completed.";

    // 运行服务
    SystemTrayManager::instance().RunServer();
    qDebug() << "Run Server completed.";

    // 初始化用户接口信息
    SystemTrayManager::instance().WriteProcessInfoToInterfaceJsonFile();
    qDebug() << "Write Process Info To Interface Json File completed.";

    // 初始化快捷键
    SystemTrayManager::instance().RegisterHotkeyFromPluginsInfo();
    qDebug() << "Register Hotkey From Plugins Info completed.";

    // 初始化聊天记录
    SystemTrayManager::instance().InitChatRecord();
    qDebug() << "Init Chat Record completed.";

    // 初始化AI
    SystemTrayManager::instance().InitAI();
    qDebug() << "Init AI completed.";

    if (SystemTrayManager::instance().GetUserConfig()->GetIns_FunctionWindowsPageConfig().GetMouseMoveFollowMode() == MOUSE_VIEW_TOUCHESMOVED_GLOBAL_SCALE)
    {
        //qDebug() << "Global mouse hook will be installed.";
        // 安装全局鼠标钩子
        if (!GlobalMouseHook::instance().install())
        {
            RuntimeLog_Error("Failed to install global mouse hook");
            return 0;
        }
    }

    // 初始化窗口并创建托盘
    SystemTrayManager::instance().init();
    qDebug() << "Init Tray completed.";
    //ModelWindowAgent::Instance().SetGLCoreInstance();
    SystemTrayManager::instance().AutomaticallyStartPlugins();
    qDebug() << "Automatically Start Plugins completed.";

    // 应用程序退出时卸载钩子
    QObject::connect(&app, &QApplication::aboutToQuit, [=]() {
        if (SystemTrayManager::instance().GetUserConfig()->GetIns_FunctionWindowsPageConfig().GetMouseMoveFollowMode() == MOUSE_VIEW_TOUCHESMOVED_GLOBAL_SCALE)
        {
            GlobalMouseHook::instance().uninstall();
        }
        });
    return app.exec();
}