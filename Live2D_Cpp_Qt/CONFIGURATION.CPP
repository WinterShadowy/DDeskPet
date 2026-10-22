#include "Configuration.h"

Configuration::Configuration()
{
}

Configuration::~Configuration()
{
}

// 获取账户页面配置实例
Config_Account& Configuration::GetIns_AccountPageConfig()
{
	return Config_Account::instance();
}
// 获取聊天页面配置实例
Config_Chat& Configuration::GetIns_ChatPageConfig()
{
	return Config_Chat::instance();
}

Config_Personalization& Configuration::GetIns_PersonalizationPageConfig()
{
	return Config_Personalization::instance();
}

Config_Function& Configuration::GetIns_FunctionWindowsPageConfig()
{
	return Config_Function::instance();
}

Config_Model& Configuration::GetIns_ModelPageConfig()
{
	return Config_Model::instance();
}

Config_Developer& Configuration::GetIns_DeveloperPageConfig()
{
	return Config_Developer::instance();
}

Config_SetAI& Configuration::GetIns_SetAIPageConfig()
{
	return Config_SetAI::instance();
}

void Configuration::Initialize()
{
	// 初始化基础配置
	ConfigurationBase::instance().Initialize();

	// 初始化页面配置文件
	Config_Account::instance().Initialize();
	Config_Function::instance().Initialize();
	Config_Personalization::instance().Initialize();
	Config_SetAI::instance().Initialize();
	Config_Chat::instance().Initialize();
	Config_Model::instance().Initialize();
	Config_Developer::instance().Initialize();
	return;
}

Configuration& Configuration::instance()
{
	static Configuration ins;
	return ins;
}