#include "DDeskPetConfig_Base.h"

// ========== 程序初始化的基础配置读取 ==========
BaseConfig::BaseConfig(QString qstr_FilePath) :
	Window_Config_Base(qstr_FilePath)
{
}

BaseConfig::~BaseConfig()
{
}

BaseConfig& BaseConfig::instance()
{
	static BaseConfig ins(QDir::toNativeSeparators(QDir("./BasicConfiguration.json").absolutePath()));
	return ins;
}

QString BaseConfig::GetUserLogFilePath()
{
	if (this->GetJsonFromConfigFile().contains("user_profile_path") == true &&
		this->GetJsonFromConfigFile().contains("logs_path") == true)
	{
		return QDir::toNativeSeparators(QDir(this->GetValueFromJson<QString>("user_profile_path") +
			this->GetValueFromJson<QString>("logs_path")).absolutePath() + "/");
	}
	return QString();
}

int BaseConfig::GetUserLogLevel()
{
	if (this->GetJsonFromConfigFile().contains("logs_level"))
	{
		return this->GetJsonFromConfigFile().at("logs_level").get<int>();
	}
	return 0;
}

QString BaseConfig::GetUserConfigFilePath()
{
	if (this->GetJsonFromConfigFile().contains("user_profile_path") == true &&
		this->GetJsonFromConfigFile().contains("config_path") == true)
	{
		return QDir::toNativeSeparators(QDir(this->GetValueFromJson<QString>("user_profile_path") +
			this->GetValueFromJson<QString>("config_path")).absolutePath() + "/");
	}
	return QString();
}

QString BaseConfig::GetPluginsFilePathFromBaseConfig()
{
	if (this->GetJsonFromConfigFile().contains("user_profile_path") == true &&
		this->GetJsonFromConfigFile().contains("plugin_path") == true)
	{
		return QDir::toNativeSeparators(QDir(this->GetValueFromJson<QString>("user_profile_path") +
			this->GetValueFromJson<QString>("plugin_path")).absolutePath() + "/");
	}
	return QString();
}

QString BaseConfig::GetUserFilePath()
{
	if (this->GetJsonFromConfigFile().contains("user_profile_path") == true)
	{
		return this->GetValueFromJson<QString>("user_profile_path");
	}
	return QString();
}

QString BaseConfig::GetImgFilePath()
{
	if (this->GetJsonFromConfigFile().contains("user_profile_path") &&
		this->GetJsonFromConfigFile().contains("Img_path"))
	{
		return QDir::toNativeSeparators(QDir(this->GetValueFromJson<QString>("user_profile_path") +
			this->GetValueFromJson<QString>("Img_path")).absolutePath() + "/");
	}
	return QString();
}

template<typename _T>
inline _T BaseConfig::GetValueFromJson(const QString& qstr_key)
{
	if (this->GetJsonFromConfigFile().contains(qstr_key.toStdString()) == true)
	{
		if constexpr (std::is_same_v<_T, QString>)
		{
			return QString::fromStdString(this->GetJsonFromConfigFile().at(qstr_key.toStdString()).get<std::string>());
		}
		else if constexpr (std::is_same_v<_T, std::string>)
		{
			return this->GetJsonFromConfigFile().at(qstr_key.toStdString()).get<std::string>();
		}
		else
		{
			return this->GetJsonFromConfigFile().at(qstr_key.toStdString()).get<_T>();
		}
	}
	else
	{
		return _T{};
	}
}

// 获取文件模型文件夹
QString BaseConfig::GetModelFolderPath()
{
	if (this->GetJsonFromConfigFile().contains("user_models_path") == true &&
		this->GetJsonFromConfigFile().contains("models_folder_name") == true)
	{
		return QDir::toNativeSeparators(QDir(this->GetValueFromJson<QString>("user_models_path") +
			this->GetValueFromJson<QString>("models_folder_name")).absolutePath() + "/");
	}
	return QString();
}

QString BaseConfig::GetChatHistoryFilePath()
{
	if (this->GetJsonFromConfigFile().contains("user_profile_path") == true &&
		this->GetJsonFromConfigFile().contains("chat_history_path") == true)
	{
		return QDir::toNativeSeparators(QDir(this->GetValueFromJson<QString>("user_profile_path") +
			this->GetValueFromJson<QString>("chat_history_path")).absolutePath() + "/");
	}
	return QString();
}

QString BaseConfig::GetServerFilePath()
{
	if (this->GetJsonFromConfigFile().contains("NetServer_excutable_file_path") == true)
	{
		return QDir::toNativeSeparators(this->GetValueFromJson<QString>("NetServer_excutable_file_path"));
	}
	return QString();
}

QPoint BaseConfig::GetWindowPosition()
{
	if (this->GetJsonFromConfigFile().contains("Model_Window_Position") == true &&
		this->GetJsonFromConfigFile().at("Model_Window_Position").contains("x") == true &&
		this->GetJsonFromConfigFile().at("Model_Window_Position").contains("y") == true)
	{
		QPoint pos;
		int x = this->GetJsonFromConfigFile().at("Model_Window_Position").at("x").get<int>();
		int y = this->GetJsonFromConfigFile().at("Model_Window_Position").at("y").get<int>();
		pos.setX(x);
		pos.setY(y);
		return pos;
	}
	return QPoint();
}

QSize BaseConfig::GetWindowSize()
{
	return QSize();
}

void BaseConfig::SetWindowPosition(const QPoint& pos)
{
	// 设置x
	if (this->GetJsonFromConfigFile().contains("Model_Window_Position") == true &&
		this->GetJsonFromConfigFile().at("Model_Window_Position").contains("x") == true)
	{
		this->GetJsonFromConfigFile().at("Model_Window_Position").at("x") = pos.x();
	}
	else
	{
		this->GetJsonFromConfigFile().at("Model_Window_Position").at("x") = -1;
	}
	// 设置y
	if (this->GetJsonFromConfigFile().contains("Model_Window_Position") == true &&
		this->GetJsonFromConfigFile().at("Model_Window_Position").contains("y") == true)
	{
		this->GetJsonFromConfigFile().at("Model_Window_Position").at("y") = pos.y();
	}
	else
	{
		this->GetJsonFromConfigFile().at("Model_Window_Position").at("y") = -1;
	}
}

void BaseConfig::SetWindowSize(const QSize& size)
{
}

bool BaseConfig::Initialize()
{
	return ReadConfigFromJson(this->GetConfigFilePath());
}

// ========== end ==========


// ========== Configuration ==========
ConfigurationBase::ConfigurationBase(QWidget* parent)
{
}

ConfigurationBase::ConfigurationBase(const QString& qstr_ConfigFilePath) :
	Page_Config_Base(qstr_ConfigFilePath)
{
}

ConfigurationBase::~ConfigurationBase()
{
}

/*
 ============================================================================
 Function: GetPageIconField
 Description: 获取页面图标的字段Json
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个nlohmann::json，表示页面图标的字段Json
 ============================================================================
*/
nlohmann::json ConfigurationBase::GetPageIconField()
{
	nlohmann::json _json = ConfigurationBase::GetJsonFromConfigFile();
	nlohmann::json _json_page_icon;

	if (_json.contains("pape_icon"))
	{
		_json_page_icon = _json.at("pape_icon");
		return _json_page_icon;
	}
	else
	{
		/*
		"page_account": "icon_account.png",
		"page_developer": "icon_developer.png",
		"page_function": "icon_function.png",
		"page_history": "icon_chat_history.png",
		"page_model": "icon_models.png",
		"page_plugin": "icon_plugins.png",
		"page_set_ai": "icon_ai.png",
		"page_setting": "icon_setting_page.png"
		*/
		_json_page_icon["page_account"] = "icon_account.png";
		_json_page_icon["page_developer"] = "icon_developer.png";
		_json_page_icon["page_function"] = "icon_function.png";
		_json_page_icon["page_history"] = "icon_chat_history.png";
		_json_page_icon["page_model"] = "icon_models.png";
		_json_page_icon["page_plugin"] = "icon_plugins.png";
		_json_page_icon["page_set_ai"] = "icon_ai.png";
		_json_page_icon["page_setting"] = "icon_setting_page.png";
		return _json_page_icon;
	}
	return nlohmann::json();
}

void ConfigurationBase::Initialize()
{
	if (this->GetInitStatus() == true)
		return;
	this->InitInstanceData();
	// 从基础配置文件中获取其余配置文件路径
	this->m_configFilePath = GetConfigFilesPathFromJson();
}

ConfigurationBase& ConfigurationBase::instance()
{
	static ConfigurationBase ins(BaseConfig::instance().GetUserConfigFilePath() + "Config.json");
	return ins;
}


// 获取配置文件的信息
template <typename _T>
_T ConfigurationBase::GetConfigFileValueFromJson(const QString& qstr_Key)
{
	static_assert((
		std::is_same_v<_T, QString> ||
		std::is_same_v<_T, std::string> ||
		std::is_same_v<_T, nlohmann::json>),
		"The type of _T must be QString,std::string or nlohmann::json");
	std::string str_Key = qstr_Key.toStdString();
	if (this->GetJsonFromConfigFile().contains(str_Key) == false)
		return _T{};
	if constexpr (std::is_same_v<_T, QString>)
	{
		return QString::fromStdString(this->GetJsonFromConfigFile().at(str_Key).get<std::string>());
	}
	else if constexpr (std::is_same_v<_T, std::string>)
	{
		return this->GetJsonFromConfigFile().at(str_Key).get<std::string>();
	}
	else if constexpr (std::is_same_v<_T, nlohmann::json>)
	{
		return this->GetJsonFromConfigFile().at(str_Key).get<nlohmann::json>();
	}
	else
	{
		return _T{};
	}
}

template <typename _T_Page_Config_Subclass>
_T_Page_Config_Subclass* ConfigurationBase::GetConfigInstanceeFromConfiguration()
{
	try {
		return &_T_Page_Config_Subclass::instance();
	}
	catch (std::exception e) {

		RuntimeLog::instance().WriteLogInfo("_T_Page_Config_Subclass& ConfigurationBase::GetConfigInstanceeFromConfiguration()",
			QString::fromStdString(e.what()));
		return nullptr;
	}
}

QString ConfigurationBase::GetUserLogFilePath()
{
	QString qstr_FilePath = QDir::toNativeSeparators(GetConfigFileValueFromJson<QString>("logs_path"));
	return qstr_FilePath;
}
QString ConfigurationBase::GetInterfaceFilePath()
{
	QString qstr_FilePath = QDir::toNativeSeparators(GetConfigFileValueFromJson<QString>("InterfacePath") +
		GetConfigFileValueFromJson<QString>("Interfacce_FileName"));
	return qstr_FilePath;
}


QString ConfigurationBase::GetConfigFilesPathFromJson()
{
	// 获取配置文件路径
	return BaseConfig::instance().GetUserConfigFilePath();
}

QString ConfigurationBase::GetPluginsPathFromJson()
{
	// 获取插件路径
	return BaseConfig::instance().GetPluginsFilePathFromBaseConfig();
}

QString ConfigurationBase::GetDefaultIconPath()
{
	return GetConfigFileValueFromJson<QString>("default_icon_plugins");
}

QString ConfigurationBase::GetAccountPageConfigFilePath()
{
	QString qstr_FilePath = QDir::toNativeSeparators(this->m_configFilePath +
		ConfigurationBase::instance().GetConfigFileValueFromJson<QString>("AccountPageConfig"));
	return qstr_FilePath;
}

QString ConfigurationBase::GetChatPageConfigFilePath()
{
	QString qstr_FilePath = QDir::toNativeSeparators(this->m_configFilePath +
		ConfigurationBase::instance().GetConfigFileValueFromJson<QString>("ChatPageConfig"));
	return qstr_FilePath;
}

QString ConfigurationBase::GetConfigPathFilePath()
{
	QString qstr_FilePath = QDir::toNativeSeparators(this->m_configFilePath +
		ConfigurationBase::instance().GetConfigFileValueFromJson<QString>("ConfigPath"));
	return QString();
}

QString ConfigurationBase::GetDeveloperPageConfigFilePath()
{
	QString qstr_FilePath = QDir::toNativeSeparators(this->m_configFilePath +
		ConfigurationBase::instance().GetConfigFileValueFromJson<QString>("DeveloperPageConfig"));
	return qstr_FilePath;
}

QString ConfigurationBase::GetFunctionWindowsPageConfigFilePath()
{
	QString qstr_FilePath = QDir::toNativeSeparators(this->m_configFilePath +
		ConfigurationBase::instance().GetConfigFileValueFromJson<QString>("FunctionWindowsPageConfig"));
	return qstr_FilePath;
}

QString ConfigurationBase::GetPersonalizationPageConfigFilePath()
{
	QString qstr_FilePath = QDir::toNativeSeparators(this->m_configFilePath +
		ConfigurationBase::instance().GetConfigFileValueFromJson<QString>("PersonalizationPageConfig"));
	return qstr_FilePath;
}

QString ConfigurationBase::GetModelPageConfigFilePath()
{

	QString qstr_FilePath = QDir::toNativeSeparators(this->m_configFilePath +
		ConfigurationBase::instance().GetConfigFileValueFromJson<QString>("ModelPageConfig"));
	return qstr_FilePath;
}

QString ConfigurationBase::GetSetAIPageConfigFilePath()
{
	QString qstr_FilePath = QDir::toNativeSeparators(this->m_configFilePath +
		ConfigurationBase::instance().GetConfigFileValueFromJson<QString>("SetAIPageConfig"));
	return qstr_FilePath;
}

/*
 ============================================================================
 Function: GetTrayIconPath
 Description: 获取托盘图标路径
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个QString，表示托盘图标路径
 ============================================================================
*/
QString ConfigurationBase::GetTrayIconPath()
{
	nlohmann::json _tray_json = ConfigurationBase::instance().GetConfigFileValueFromJson<nlohmann::json>("tray");
	QString qstr_FilePath;
	if (_tray_json.contains("tray_icon"))
	{
		qstr_FilePath = QDir::toNativeSeparators(BaseConfig::instance().GetImgFilePath() +
			QString::fromStdString(_tray_json.at("tray_icon").get<std::string>()));
	}

	//qDebug() << "tray_icon：" << qstr_FilePath;
	return qstr_FilePath;
}

QString ConfigurationBase::GetTrayIconPath_SettingIcon()
{
	nlohmann::json _tray_json = ConfigurationBase::instance().GetConfigFileValueFromJson<nlohmann::json>("tray");
	QString qstr_FilePath;
	if (_tray_json.contains("tray_icon_setting"))
	{
		qstr_FilePath = QDir::toNativeSeparators(BaseConfig::instance().GetImgFilePath() +
			QString::fromStdString(_tray_json.at("tray_icon_setting").get<std::string>()));
	}

	return qstr_FilePath;
}

QString ConfigurationBase::GetTrayIconPath_ExitIcon()
{
	nlohmann::json _tray_json = ConfigurationBase::instance().GetConfigFileValueFromJson<nlohmann::json>("tray");

	QString qstr_FilePath;
	if (_tray_json.contains("tray_icon_exit"))
	{
		qstr_FilePath = QDir::toNativeSeparators(BaseConfig::instance().GetImgFilePath() +
			QString::fromStdString(_tray_json.at("tray_icon_exit").get<std::string>()));
	}

	return qstr_FilePath;
}

QString ConfigurationBase::GetTrayIconPath_HideIcon()
{
	nlohmann::json _tray_json = ConfigurationBase::instance().GetConfigFileValueFromJson<nlohmann::json>("tray");

	QString qstr_FilePath;
	if (_tray_json.contains("tray_icon_hide"))
	{
		qstr_FilePath = QDir::toNativeSeparators(BaseConfig::instance().GetImgFilePath() +
			QString::fromStdString(_tray_json.at("tray_icon_hide").get<std::string>()));
	}

	return qstr_FilePath;
}

QString ConfigurationBase::GetTrayIconPath_ShowIcon()
{
	return ConfigurationBase::GetTrayIconPath();
}

/*
 ============================================================================
 Function: GetSettingPageIcon
 Description: 获取设置页面图标
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个QString, 表示设置页面图标的路径
 ============================================================================
*/
QString ConfigurationBase::GetSettingPageIcon()
{
	nlohmann::json _json_page_icon = ConfigurationBase::GetPageIconField();
	if (_json_page_icon.contains("page_setting") == true)
	{
		QString file_name = QString::fromStdString(_json_page_icon.at("page_setting").get<std::string>());
		return QDir::toNativeSeparators(BaseConfig::instance().GetImgFilePath() + file_name);
	}
	return QString();
}

/*
 ============================================================================
 Function: GetSettingPageIcon_Account
 Description: 获取账户选项图标
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个QString, 表示账户选项图标路径
 ============================================================================
*/
QString ConfigurationBase::GetSettingPageIcon_Account()
{
	nlohmann::json _json_page_icon = ConfigurationBase::GetPageIconField();
	if (_json_page_icon.contains("page_account") == true)
	{
		QString file_name = QString::fromStdString(_json_page_icon.at("page_account").get<std::string>());
		return QDir::toNativeSeparators(BaseConfig::instance().GetImgFilePath() + file_name);
	}
	return QString();
}

/*
 ============================================================================
 Function: GetSettingPageIcon_Plugin
 Description: 获取插件选项图标
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个QString, 表示插件选项图标的路径
 ============================================================================
*/
QString ConfigurationBase::GetSettingPageIcon_Plugin()
{
	nlohmann::json _json_page_icon = ConfigurationBase::GetPageIconField();
	if (_json_page_icon.contains("page_plugin") == true)
	{
		QString file_name = QString::fromStdString(_json_page_icon.at("page_plugin").get<std::string>());
		return QDir::toNativeSeparators(BaseConfig::instance().GetImgFilePath() + file_name);
	}
	return QString();
}

/*
 ============================================================================
 Function: GetSettingPageIcon_Model
 Description: 获取模型设置选项图标
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个QString, 表示模型设置选项的图标路径
 ============================================================================
*/
QString ConfigurationBase::GetSettingPageIcon_Model()
{
	nlohmann::json _json_page_icon = ConfigurationBase::GetPageIconField();
	if (_json_page_icon.contains("page_model") == true)
	{
		QString file_name = QString::fromStdString(_json_page_icon.at("page_model").get<std::string>());
		return QDir::toNativeSeparators(BaseConfig::instance().GetImgFilePath() + file_name);
	}
	return QString();
}

/*
 ============================================================================
 Function: GetSettingPageIcon_MoreFunction
 Description: 获取更多功能选项图标
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个QString, 表示更多功能选项的图标路径
 ============================================================================
*/
QString ConfigurationBase::GetSettingPageIcon_MoreFunction()
{
	nlohmann::json _json_page_icon = ConfigurationBase::GetPageIconField();
	if (_json_page_icon.contains("page_function") == true)
	{
		QString file_name = QString::fromStdString(_json_page_icon.at("page_function").get<std::string>());
		return QDir::toNativeSeparators(BaseConfig::instance().GetImgFilePath() + file_name);
	}
	return QString();
}

/*
 ============================================================================
 Function: GetSettingPageIcon_SetAI
 Description: 获取设置AI选项图标
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个QString, 表示设置AI选项的图标路径
 ============================================================================
*/
QString ConfigurationBase::GetSettingPageIcon_SetAI()
{
	nlohmann::json _json_page_icon = ConfigurationBase::GetPageIconField();
	if (_json_page_icon.contains("page_set_ai") == true)
	{
		QString file_name = QString::fromStdString(_json_page_icon.at("page_set_ai").get<std::string>());
		return QDir::toNativeSeparators(BaseConfig::instance().GetImgFilePath() + file_name);
	}
	return QString();
}

/*
 ============================================================================
 Function: GetSettingPageIcon_Chat
 Description: 获取聊天记录选项图标
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个QString, 表示聊天记录选项的图标路径
 ============================================================================
*/
QString ConfigurationBase::GetSettingPageIcon_Chat()
{
	nlohmann::json _json_page_icon = ConfigurationBase::GetPageIconField();
	qDebug() << _json_page_icon.dump(2);
	if (_json_page_icon.contains("page_chat_history") == true)
	{
		QString file_name = QString::fromStdString(_json_page_icon.at("page_chat_history").get<std::string>());
		return QDir::toNativeSeparators(BaseConfig::instance().GetImgFilePath() + file_name);
	}
	return QString();
}

/*
 ============================================================================
 Function: GetSettingPageIcon_Developer
 Description: 获取开发者页选项图标
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个QString, 表示开发者页选项的图标路径
 ============================================================================
*/
QString ConfigurationBase::GetSettingPageIcon_Developer()
{
	nlohmann::json _json_page_icon = ConfigurationBase::GetPageIconField();
	if (_json_page_icon.contains("page_developer") == true)
	{
		QString file_name = QString::fromStdString(_json_page_icon.at("page_developer").get<std::string>());
		return QDir::toNativeSeparators(BaseConfig::instance().GetImgFilePath() + file_name);
	}
	return QString();
}

QString ConfigurationBase::GetCurrentVersion()
{
	return QString("v1.0.0.1");
}

QString ConfigurationBase::GetPublisher()
{
	return QString("Anonymous");
}

// ========== end =========
