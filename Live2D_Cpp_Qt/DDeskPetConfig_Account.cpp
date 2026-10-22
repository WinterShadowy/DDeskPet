#include "DDeskPetConfig_Account.h"

// ========== Config_Account ==========
Config_Account::Config_Account(QString qstr_FilePath) :
	Page_Config_Base(qstr_FilePath)
{
}

Config_Account::~Config_Account()
{
}

Config_Account& Config_Account::instance()
{
	static Config_Account ins(ConfigurationBase::instance().GetAccountPageConfigFilePath());
	return ins;
}

void Config_Account::Initialize()
{
	this->InitInstanceData();
	return;
}

nlohmann::json& Config_Account::GetJson()
{
	return this->GetJsonFromConfigFile();
}

void Config_Account::SetJsonFromSettingPage(nlohmann::json json)
{
	SetJson_BaseClass(std::move(json));
}

// 获取图标 - 显示密码
QString Config_Account::GetIcon_ShowPassword()
{
	if (this->GetJson().contains("imgs_file_name") == true &&
		this->GetJson().at("imgs_file_name").contains("show_passwd") == true)
	{
		return QString(BaseConfig::instance().GetImgFilePath() +
			QString::fromStdString(this->GetJson().at("imgs_file_name").at("show_passwd").get<std::string>()));
	}
	else
	{
		std::string err_msg = "imgs_file_name or show_passwd not found in json";
		RuntimeLog_Debug(err_msg);
	}
	return QString();
}

// 获取图标 - 隐藏密码
QString Config_Account::GetIcon_HidePassword()
{
	if (this->GetJson().contains("imgs_file_name") == true &&
		this->GetJson().at("imgs_file_name").contains("hide_passwd") == true)
	{
		return QString(BaseConfig::instance().GetImgFilePath() +
			QString::fromStdString(this->GetJson().at("imgs_file_name").at("hide_passwd").get<std::string>()));
	}
	else
	{
		std::string err_msg = "imgs_file_name or hide_passwd not found in json";
		RuntimeLog_Debug(err_msg);
	}
	return QString();
}

QString Config_Account::Get_Username()
{
	if (this->GetJsonFromConfigFile().contains("account") == true)
	{
		return std::move(QString::fromStdString(this->GetJsonFromConfigFile().at("account").get<std::string>()));
	}
	else
	{
		std::string err_msg = "account not found in json";
		RuntimeLog_Debug(err_msg);
	}
	return QString("");
}

QString Config_Account::Get_Password()
{
	if (this->GetJsonFromConfigFile().contains("password") == true)
	{
		return std::move(QString::fromStdString(this->GetJsonFromConfigFile().at("password").get<std::string>()));
	}
	return QString("");
}
// ========== end ==========