#include "SettingDialog.h"
#include <QResizeEvent>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("设置");
    if (ConfigurationBase::instance().GetSettingPageIcon().isEmpty() == false)
    {
        setWindowIcon(QIcon(ConfigurationBase::instance().GetSettingPageIcon()));
    }
    
    // 仅配置服务器参数并启动后台工作线程（非阻塞），不立即建立连接。
    try
    {
        std::string caPath; // 为空表示使用系统 CA
       
        QtConcurrent::run([caPath]() {
            //UserRequest::Instance().ConfigureServer("103.23.149.189", 20134, "deskpet.cpluscottage.top", caPath);
            UserRequest::Instance().SerServerInfo("deskpet.cpluscottage.top", 20134, caPath);
            });
    }
    catch (...)
    {
        qDebug() << "配置服务器失败";
        RuntimeLog_Error("Server configuration failed");
    }
    
    // 侧边栏
    m_vtab_Side = new VTabPageBar(this);
    m_vtab_Side->SetSideBarBorder();
    m_vtab_Side->SetButtonSpacing(0);

    // count 统计页面数量
    // 页面数量用于计算侧边栏的高度 - 防止侧边栏高度不够
    int count = 0;

    // 各页面

    // 账户页
    try
    {
        m_widget_PageUser = Page_Factory::Instance().CreatePageAccount(this);  // 创建账户页
        ++count;
        if (ConfigurationBase::instance().GetSettingPageIcon_Account().isEmpty())
        {
            m_vtab_Side->AddItem("  账户", m_widget_PageUser);
        }
        else
        {
            m_vtab_Side->AddItem("  账户", m_widget_PageUser, ConfigurationBase::instance().GetSettingPageIcon_Account());
        }
    }
    catch (...)
    {
		qDebug() << "创建账户页失败";
	    RuntimeLog_Fatal("Failed to create account page");
    }

    // 插件页 - (早期项目命名问题所以叫Personalization)
    try {
        if (Config_Developer::instance().GetUsePlugins() == true)
        {
            m_widget_PagePersonalization = Page_Factory::Instance().CreatePagePersonalization(this);    // 创建个性化页
            ++count;
            if (ConfigurationBase::instance().GetSettingPageIcon_Plugin().isEmpty())
            {
                m_vtab_Side->AddItem("  插件", m_widget_PagePersonalization);
            }
            else
            {
                m_vtab_Side->AddItem("  插件", m_widget_PagePersonalization, ConfigurationBase::instance().GetSettingPageIcon_Plugin());
            }
        }
    }
    catch (...)
    {
		qDebug() << "创建插件页失败";
        RuntimeLog_Fatal("Failed to create plugin page");
    }

    // 模型页
    try {
        if (Config_Developer::instance().GetUseModels() == true)
        {
            m_widget_PageModel = Page_Factory::Instance().CreatePageModel(this);    // 创建模型页
            ++count;
            if (ConfigurationBase::instance().GetSettingPageIcon_Model().isEmpty())
            {
                m_vtab_Side->AddItem("  模型", m_widget_PageModel);
            }
            else
            {
                m_vtab_Side->AddItem("  模型", m_widget_PageModel, ConfigurationBase::instance().GetSettingPageIcon_Model());
            }
        }
    }
    catch (...)
    {
		qDebug() << "创建模型页失败";
        RuntimeLog_Fatal("Failed to create model page");
    }

    // 功能页
    try
    {
        m_widget_PageFuntionWindows = Page_Factory::Instance().CreatePageFuntionWindows(this);  // 创建功能页
        ++count;
        if (ConfigurationBase::instance().GetSettingPageIcon_MoreFunction().isEmpty())
        {
            m_vtab_Side->AddItem("  基础功能", m_widget_PageFuntionWindows);
        }
        else
        {
            m_vtab_Side->AddItem("  基础功能", m_widget_PageFuntionWindows, ConfigurationBase::instance().GetSettingPageIcon_MoreFunction());
        }
    }
    catch (...)
    {
		qDebug() << "创建功能页失败";
        RuntimeLog_Fatal("Failed to create function page");
    }

    // 判断是否创建AI设置页
    try {
        if (Config_Developer::instance().GetUseAI() == true)
        {
            m_widget_PageSetAI = Page_Factory::Instance().CreatePageSetAI(this);
            ++count;
            if (ConfigurationBase::instance().GetSettingPageIcon_SetAI().isEmpty())
            {
                m_vtab_Side->AddItem("  设置AI", m_widget_PageSetAI);
            }
            else
            {
                m_vtab_Side->AddItem("  设置AI", m_widget_PageSetAI, ConfigurationBase::instance().GetSettingPageIcon_SetAI());
            }
        }
    }
    catch (...)
    {
		qDebug() << "创建AI设置页失败";
        RuntimeLog_Fatal("Failed to create AI setting page");
    }

    // 判断是否创建聊天记录页
    try {
        if (Config_Developer::instance().GetUseChatHistory() == true)
        {
            m_widget_PageChat = Page_Factory::Instance().CreatePageChat(this);
            ++count;
            if (ConfigurationBase::instance().GetSettingPageIcon_Chat().isEmpty())
            {
                m_vtab_Side->AddItem("  聊天记录", m_widget_PageChat);
            }
            else
            {
                m_vtab_Side->AddItem("  聊天记录", m_widget_PageChat, ConfigurationBase::instance().GetSettingPageIcon_Chat());
            }
        }
    }
    catch (...)
    {
		qDebug() << "创建聊天记录页失败";
        RuntimeLog_Fatal("Failed to create chat history page");
    }

    // 判断是否创建开发者页
    try {
        if (Configuration::GetIns_FunctionWindowsPageConfig().IsUseDeveloperMode() == true)
        {
            m_widget_PageDeveloper = Page_Factory::Instance().CreatePageDeveloper(this);
            ++count;
            if (ConfigurationBase::instance().GetSettingPageIcon_Developer().isEmpty())
            {
                m_vtab_Side->AddItem("  开发者页", m_widget_PageDeveloper);
            }
            else
            {
                m_vtab_Side->AddItem("  开发者页", m_widget_PageDeveloper, ConfigurationBase::instance().GetSettingPageIcon_Developer());
            }
        }
    }
    catch (...)
    {
		qDebug() << "创建开发者页失败";
		RuntimeLog_Fatal("Failed to create developer page");
    }

    // 用布局管理器，让侧边栏占满整个对话框
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_vtab_Side, 1);
    setLayout(mainLayout);
    
    resize(820, (70 * count < 280 ? 280 : 70 * count));
    m_vtab_Side->SetAllButtonStyle(SettingsDialog::CB_SetToolButtonStyle);
    // 将所有页面加入页面池
    this->m_SettingPageInfoPool = m_vtab_Side->GetAllWidget();
}

void SettingsDialog::resizeEvent(QResizeEvent* event)
{
    QDialog::resizeEvent(event);
}

void SettingsDialog::CB_SetToolButtonStyle(QPushButton* btn)
{
    LAppStyleManager::SetPushButtonStyle(btn, 5);
    return;
    btn->setStyleSheet(
        "QPushButton {"
        "   text-align: left;"                // 文字居左
        "   font-size: 14px;"                  // 字体大小
        "   background: rgb(255, 255, 255);"   // 正常背景
        "   border: none;"                     // 去掉边框
        "   padding-left:6px;"
        "   padding-right:12px;"
        "}"
        "QPushButton:checked {"
        "   font: bold 14px;"
        "   background: rgb(240, 240, 240);"
        "   padding-left:6px;"
        "   padding-right:12px;"
        "}");
}
