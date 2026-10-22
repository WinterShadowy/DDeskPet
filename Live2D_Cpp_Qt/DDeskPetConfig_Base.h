#pragma once

#include "DDeskPetConfigInterfaceBase.h"

// ========== 程序初始化的基础配置读取 ==========
class BaseConfig : virtual public Window_Config_Base {
public:
	BaseConfig() = default;
	BaseConfig(QString qstr_FilePath);
	~BaseConfig();
	static BaseConfig& instance();

	// 获取用户日志文件路径
	QString GetUserLogFilePath();
	// 获取用户日志级别
	int GetUserLogLevel();
	// 获取用户配置文件路径
	QString GetUserConfigFilePath();
	// 获取插件文件夹路径
	QString GetPluginsFilePathFromBaseConfig();
	// 获取User文件夹的路径
	QString GetUserFilePath();
	// 获取图标文件路径 - 只获取存储路径
	QString GetImgFilePath();
	// 获取模型文件路径
	QString GetModelFolderPath();
	// 获取聊天记录文件路径
	QString GetChatHistoryFilePath();
	// 获取Server可执行文件路径
	QString GetServerFilePath();
	// 获取窗口在关闭前的位置
	QPoint GetWindowPosition();
	// 获取窗口在关闭前的尺寸
	QSize GetWindowSize();
	// 设置窗口在关闭前的位置
	void SetWindowPosition(const QPoint& pos);
	// 设置窗口在关闭前的尺寸
	void SetWindowSize(const QSize& size);


	virtual bool Initialize() override;
private:
	template<typename _T>
	_T GetValueFromJson(const QString& qstr_key);
};
// ========== end ==========

// =========== 配置文件读取 ==========
class ConfigurationBase : virtual public Page_Config_Base
{
private:
	ConfigurationBase(QWidget* parent = nullptr);
	~ConfigurationBase();

	// 禁止拷贝构造和赋值操作
	ConfigurationBase(const ConfigurationBase& other) = delete;
	ConfigurationBase& operator=(const ConfigurationBase& other) = delete;

	// 获取页面图标字段
	nlohmann::json GetPageIconField();
	
public:
	static ConfigurationBase& instance();

	ConfigurationBase(const QString& qstr_ConfigFilePath);

	// 初始化
	void Initialize() override;

	

	template <typename _T>
	_T GetConfigFileValueFromJson(const QString& qstr_Key);

	template <typename _T_Page_Config_Subclass>
	_T_Page_Config_Subclass* GetConfigInstanceeFromConfiguration();

	// 获取用户日志文件路径
	QString GetUserLogFilePath();
	// 获取接口文件路径
	QString GetInterfaceFilePath();
	// 获取配置文件路径
	QString GetConfigFilesPathFromJson();
	//获取插件路径
	QString GetPluginsPathFromJson();
	// 获取默认图标
	QString GetDefaultIconPath();
	// 获取绝对路径
	QString GetAccountPageConfigFilePath();
	QString GetChatPageConfigFilePath();
	QString GetConfigPathFilePath();
	QString GetDeveloperPageConfigFilePath();
	QString GetFunctionWindowsPageConfigFilePath();
	QString GetPersonalizationPageConfigFilePath();
	QString GetModelPageConfigFilePath();
	QString GetSetAIPageConfigFilePath();

	// 获取托盘图标路径
	QString GetTrayIconPath();
	// 获取托盘设置图标的路径
	QString GetTrayIconPath_SettingIcon();
	// 获取托盘退出图标的路径
	QString GetTrayIconPath_ExitIcon();
	// 获取托盘隐藏模型图标路径
	QString GetTrayIconPath_HideIcon();
	// 获取托盘显示模型的图标路径
	QString GetTrayIconPath_ShowIcon();

	// 获取设置页图标
	QString GetSettingPageIcon();
	// 获取账户图标
	QString GetSettingPageIcon_Account();
	// 获取插件图标
	QString GetSettingPageIcon_Plugin();
	// 获取模型设置页图标
	QString GetSettingPageIcon_Model();
	// 获取基础功能页图标
	QString GetSettingPageIcon_MoreFunction();
	// 获取设置AI页图标
	QString GetSettingPageIcon_SetAI();
	// 获取聊天记录页图标
	QString GetSettingPageIcon_Chat();
	// 获取开发者页的图标
	QString GetSettingPageIcon_Developer();

	// 获取当前版本
	QString GetCurrentVersion();
	// 获取发布者
	QString GetPublisher();

private:
	QString m_configFilePath;
};
// =========== end ============