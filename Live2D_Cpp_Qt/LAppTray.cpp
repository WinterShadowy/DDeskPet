#include "LAppTray.hpp"
#include <QApplication>

SystemTrayManager& SystemTrayManager::instance()
{
    static SystemTrayManager inst;
    return inst;
}

void SystemTrayManager::init(GLCore* target)
{
    // 设置退出时不关闭最后一个窗口
    qApp->setQuitOnLastWindowClosed(false);
    if (target != nullptr)
    {
        target->show();
        target->resize(Configuration::GetIns_FunctionWindowsPageConfig().GetDefaultModelWindowHeight(),
            Configuration::GetIns_FunctionWindowsPageConfig().GetDefaultModelWindowWidth());

		QPoint currentPos = target->pos();
        currentPos = ConvenientTool::CheckModelWindowOffScreen(currentPos, target->width(), target->height());

        ModelWindowPromptWindow::SetWindowNowPosition(currentPos.x(), currentPos.y(), target->width(), target->height());
    }
    else
    {
        ModelWindowAgent::Instance().LoadLive2DModel();
		qDebug() << "GLCore instance is null, loaded model window through ModelWindowAgent.";
    }

    this->trayMenu = new QMenu(target);
    this->showAction = new QAction(QString("显示"), trayMenu);
    this->showAction->setIcon(QIcon(ConfigurationBase::instance().GetTrayIconPath_ShowIcon()));
    this->hideAction = new QAction(QString("隐藏"), trayMenu);
    this->hideAction->setIcon(QIcon(ConfigurationBase::instance().GetTrayIconPath_HideIcon()));
    this->quitAction = new QAction(QString("退出"), trayMenu);
    this->quitAction->setIcon(QIcon(ConfigurationBase::instance().GetTrayIconPath_ExitIcon()));
    this->settingsAction = new QAction(QString("设置"), trayMenu);
    this->settingsAction->setIcon(QIcon(ConfigurationBase::instance().GetTrayIconPath_SettingIcon()));

    trayMenu->addAction(settingsAction);
    trayMenu->addSeparator();
    trayMenu->addAction(showAction);
    trayMenu->addAction(hideAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    this->trayIcon = new QSystemTrayIcon();
    this->trayIcon->setIcon(QIcon(ConfigurationBase::instance().GetTrayIconPath()));
    this->trayIcon->setContextMenu(this->trayMenu);
    this->trayIcon->show();

    if(target != nullptr)
    {
        connect(showAction, &QAction::triggered, target, &QWidget::show);
        connect(hideAction, &QAction::triggered, target, &QWidget::hide);
        connect(this->trayIcon, &QSystemTrayIcon::activated, this, [target](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::Trigger && (QApplication::mouseButtons() & Qt::LeftButton) && target != nullptr)
            {
                target->isVisible() ? target->hide() : target->show();
            }
            });
    }
    else
    {
        connect(showAction, &QAction::triggered, this, []{
			ModelWindowAgent::Instance().SetVisible(true);
            });
        connect(hideAction, &QAction::triggered, this, [] {
            ModelWindowAgent::Instance().SetVisible(false);
            });
        connect(this->trayIcon, &QSystemTrayIcon::activated, this, [](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::Trigger) 
            {
                ModelWindowAgent::Instance().SetVisible(!ModelWindowAgent::Instance().IsVisible());
            }
            });
    }
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(settingsAction, &QAction::triggered, this, &SystemTrayManager::showSettingsDialog);

    return;
}

bool SystemTrayManager::InitBasicConfiguration()
{
    bool _init_base = BaseConfig::instance().Initialize();
    Configuration::instance().Initialize();
    return _init_base;
}

void SystemTrayManager::InitConfig()
{
    // 初始化配置
    ConfigurationBase::instance().Initialize();
}

void SystemTrayManager::InitRuntimeErrorLog()
{
    // 设置日志路径
    //RuntimeLog::instance().SetLogFilePath(BaseConfig::instance().GetUserLogFilePath());
}

void SystemTrayManager::InitLogSystem()
{
    qDebug() << "Initializing Log System with file path:" << BaseConfig::instance().GetUserLogFilePath().toStdString().c_str();
    Logger::init(BaseConfig::instance().GetUserLogFilePath().toStdString().c_str(),
        Logger::valueToLogLevel(BaseConfig::instance().GetUserLogLevel()));
}

void SystemTrayManager::SetRuntimErrorLog()
{
    return;
    if (Configuration::GetIns_FunctionWindowsPageConfig().IsRuntimeErrorLog() == true)
    {
        // 设置日志初始化
        RuntimeLog::instance().InitializeRuntimeLog();
    }
    return;
}
#include "LAppUserInterface.h"
void SystemTrayManager::WriteProcessInfoToInterfaceJsonFile()
{
    InitilizeUserWindowInterface(ConfigurationBase::instance().GetInterfaceFilePath());
    return;
}

void SystemTrayManager::RegisterHotkeyFromPluginsInfo()
{
    PluginsManager::instance().InitPluginsInfo(ConfigurationBase::instance().GetPluginsPathFromJson());
    return;
}

void SystemTrayManager::InitChatRecord()
{
    GetChatHistoryManagerInstance().Initilize(BaseConfig::instance().GetChatHistoryFilePath());
    return;
}

void SystemTrayManager::InitModleMessageLimit()
{
    BubbleManager::instance().SetMaxBubbles(3);
}

void SystemTrayManager::InitAI()
{
    std::string _str_url = Config_SetAI::instance().GetURL_ModelsFullPath();
    std::string _api_key = Config_SetAI::instance().GetApiKey();
	qDebug() << "Initializing AI Manager with URL:" << QString::fromStdString(_str_url) << "and API Key:" << QString::fromStdString(_api_key);
	return;
}

void SystemTrayManager::RunServer()
{
    ChildManager::instance()->RunChildProcess(BaseConfig::instance().GetServerFilePath());
    return;
}

void SystemTrayManager::AutomaticallyStartPlugins()
{
    PluginsManager::instance().AutomaticallyStartPlugins();
    return;
}

void SystemTrayManager::Destroy()
{
    delete this->trayMenu;
    this->trayMenu = nullptr;
    delete this->trayIcon;
    this->trayIcon = nullptr;
}

void SystemTrayManager::ResizeCoreWindow(int width, int height)
{
    return;
}

Configuration* SystemTrayManager::GetUserConfig()
{
    return &Configuration::instance();
}

SystemTrayManager::~SystemTrayManager()
{
	if (trayIcon)
	{
		trayIcon->hide();
        trayIcon->deleteLater();
		trayIcon = nullptr;
	}
}

void SystemTrayManager::showSettingsDialog()
{
    try {
        if (!this->m_SettingMenu)
        {
            this->m_SettingMenu = new SettingsDialog();   // 创建
            NotificationWidget::installDialog(this->m_SettingMenu);
            this->m_SettingMenu->setAttribute(Qt::WA_DeleteOnClose); // 关闭即 delete
            connect(this->m_SettingMenu, &QObject::destroyed, this, &SystemTrayManager::onSettingsDialogDestroyed);
        }

        this->m_SettingMenu->show();
        this->m_SettingMenu->raise();          // 置顶
        //this->m_SettingMenu->activateWindow(); // 抢焦点
    }
    catch (...)
    {
        qDebug() << "Error Info: SystemTrayManager::showSettingsDialog()";
        RuntimeLog_Fatal("Show Settings Dialog Error");
    }
}

void SystemTrayManager::onSettingsDialogDestroyed()
{
    //delete this->m_SettingMenu;
    this->m_SettingMenu = nullptr;   // 指针复位
}