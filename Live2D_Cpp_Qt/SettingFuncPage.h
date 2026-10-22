#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif


#include "ServerUser.h"
#include "Configuration.h"
#include "SettingReusablePage.h"
#include "SettingPageWidgetObjectName.h"
#include "LAppDefine.hpp"
#include "LAppModelMessage.h"
#include "qhotkey.h"
#include "qhotkey_Manager.h"
#include "LAppCusWidget.h"
#include "LAppDeveloperPage.h"

// Server
#include "ServerAI.h" 

// Manager
#include "LAppQWidgetStyleManager.h"
#include "LAppLive2DModelsManager.h"
#include "LAppPluginsManager.h"
#include "LAppChatHistoryManager.h"
#include "ConvenientTool.hpp"

#include <QWidget>
#include <QStackedWidget>
#include <QGroupBox>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QSpinBox>
#include <QComboBox>
#include <QTabBar>
#include <QTabWidget>
#include <QListWidget>
#include <QToolBox>
// button
#include <QToolButton>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QAbstractButton>
// edit
#include <QLineEdit>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QKeySequenceEdit>
// layout
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QScrollArea>
#include <QDesktopServices>
#include <QUrl>

// date
#include <QDateEdit>
#include <QDate>
#include <QSizePolicy>
#include <QIntValidator>
#include <QFrame>
#include <QSpacerItem>
#include <QStyle>
#include <QFont>
//#include <cmath>


#define UNDEFINED_PAGE -1

//namespace PageObejectName {
//	namespace Page_Account_ObjectName {
//		QString Login("LoginPage");
//		QString Register("RegisterPage");
//		QString UserInfo("UserInfoPage");
//		QString FeedbackTextEdit("FeedbackTextEdit");
//	}
//}

// 初始化数据
class Setting_InitializePageWidgetData {
public:
	Setting_InitializePageWidgetData() {};
	~Setting_InitializePageWidgetData() = default;

	template <typename _T, typename... Args>
	static void _SetWidgetData(QWidget* parent, const QString& objectName, Args... args);
};
#define SetPageWidgetData(_Widget_Type, parent, objectName, ...) \
	Setting_InitializePageWidgetData::_SetWidgetData<_Widget_Type>(parent, objectName, __VA_ARGS__)
// ========== end ==========

// 初始化页面 & 保存页面
class SettingPublicFunction {
public:
	SettingPublicFunction() {};
	virtual ~SettingPublicFunction() = 0;

	// 初始化页面
	virtual void InitPageFromJson() = 0;
	// 保存页面参数
	virtual void SavePageToJson() = 0;
};

// 账户
class Page_Account : public QWidget, public SettingPublicFunction
{
	Q_OBJECT
public:
	explicit Page_Account(QWidget* parent = nullptr);
	~Page_Account() = default;

	void InitLoginPage();
	// 重写父类部分
	virtual void InitPageFromJson() override;
	virtual void SavePageToJson() override;

private:
	Config_Account& GetPageConfigInstance();
	QWidget* CreateLoginPage();
	QWidget* CreateRegisterPage();
	QWidget* CreateUserInfoPage();

	void SetPwdDisplayStatus_IconSwitching(QLineEdit* lineEdit_UserPasswd, QAction* QAction_toggleAction, bool isVisible);
private slots:
	void ShowLoginPage(int nFromPageIndex);
	void ShowRegisterPage();
	void DoLogin();
	void OpenForgotPasswd_Account();
	void SendFeedback();
	void GetNewAnnouncement();
	
private:
	QStackedWidget* m_stackedWidget = nullptr;	// 页面切换

	nlohmann::json _user_info;
};

// 个性化
class Page_Personalization : public QWidget
{
	Q_OBJECT
public:
	explicit Page_Personalization(QWidget* parent = nullptr);
	~Page_Personalization() = default;

private:
	QWidget* CreatePage_ListWidget();

private:
	QGroupBox* groupBox_ModelWindow = nullptr;
	QGroupBox* groupBox_Mouse = nullptr;
	QGroupBox* groupBox_Keyboard = nullptr;
};

// 功能窗口
class Page_FuntionWindows : public QWidget, public SettingPublicFunction
{
	Q_OBJECT
public:
	explicit Page_FuntionWindows(QWidget* parent = nullptr);
	~Page_FuntionWindows() = default;

	virtual void InitPageFromJson() override;
	virtual void SavePageToJson() override;

	Config_Function& GetPageConfigInstance();

private:
	template <typename _T>
	void SetJsonDataFromWidget(int index, _T _t_value);

private:
	QWidget* CreatePage();

	QWidget* CreatePage_ShowMessage();
	QWidget* CreatePage_About_Live2D();

	// 关于窗口
	QWidget* CreatePage_About_Window();
	QWidget* CreatePage_About_AI();
	QWidget* CreatePage_More();
};

// 设置AI
class Page_SetAI : public QWidget
{
	Q_OBJECT
public:
	explicit Page_SetAI(QWidget* parent = nullptr);
	~Page_SetAI() = default;

public:
	QWidget* CreateSetAIPage();
	QGroupBox* CreateBaseSettingPage();
	QWidget* CreateMoreSettingPage();
	QWidget* CreateCustomSettingPage();
	QWidget* CreateCustomFieldPage();
	QWidget* CreateConfigurationOperationPage();

private slots:
	void OnCheckBoxChanged(QCheckBox* checkBox, int index);
	void OnPushButtonClicked_StopTable(QPushButton* button, int nPushButtonID);
	// 保存按钮点击时
	void OnSaveBtnClicked();
	// 恢复默认按钮点击时
	void OnRestoreDefaultBtnClicked();
private:
};

// 聊天记录
class Page_Chat : public QWidget
{
	Q_OBJECT
public:
	explicit Page_Chat(QWidget* parent = nullptr);
	~Page_Chat() = default;

private:
	QWidget* CreatePage();
	QWidget* CreateDateSelectWidget();  // 创建日期选择页面
	QWidget* CreateChatRecordWidget();  // 创建聊天记录页面

private:
	DisplayChatHistoryWidget* m_displayChatHistoryWidget = nullptr;
};

// 模型
class Page_Model : public QWidget
{
	Q_OBJECT
public:
	explicit Page_Model(QWidget* parent = nullptr);
	~Page_Model() = default;

	QWidget* CreateDisplayModelInfoPage();

private:
	void ScanfModels();

private:
	QFileInfoList* m_fileInfoList = nullptr;
};

// 开发者选项
class Page_Developer : public QWidget
{
	Q_OBJECT
public:
	explicit Page_Developer(QWidget* parent = nullptr);
	~Page_Developer() = default;

private:
	Developer_Lips_Shape* m_developer_Lips_Shape = nullptr;
	Developer_Debug_AI_Page* m_developer_Debug_AI_Page = nullptr;
	Developer_Debug_Plugins* m_developer_Debug_Plugins = nullptr;		// Debug - 插件
	Developer_Debug_Models* m_developer_Debug_Models = nullptr;			// Debug - 模型
	Developer_Debug_ChatHistory* m_developer_Debug_ChatHistory = nullptr; // Debug - 聊天记录
	Developer_Debug_NetService* m_developer_Debug_NetService = nullptr;
	Developer_Debug_ModelMessage* m_developer_Debug_ModelMessage = nullptr;

	QGroupBox* m_groupbox_main = nullptr;
	QScrollArea* m_scrollArea = nullptr;
};

// 页面工厂
class Page_Factory
{
public:
	Page_Factory();
	~Page_Factory() = default;

	static Page_Factory& Instance();
public:
	// 账户页面
	QWidget* CreatePageAccount(QWidget* parent)
	{
		return new Page_Account(parent);
	}
	// 个性化页面
	QWidget* CreatePagePersonalization(QWidget* parent)
	{
		return new Page_Personalization(parent);
	}
	// 功能页面
	QWidget* CreatePageFuntionWindows(QWidget* parent)
	{
		return new Page_FuntionWindows(parent);
	}
	
	// 设置AI
	QWidget* CreatePageSetAI(QWidget* parent)
	{
		return new Page_SetAI(parent);
	}
	// 聊天
	QWidget* CreatePageChat(QWidget* parent)
	{
		return new Page_Chat(parent);
	}
	// 其他页面 - 模型页面
	QWidget* CreatePageModel(QWidget* parent)
	{
		return new Page_Model(parent);
	}
	//开发者选项
	QWidget* CreatePageDeveloper(QWidget* parent)
	{
		return new Page_Developer;
	}
};