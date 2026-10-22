#include "DDeskPetConfig_Chat.h"

// ========== Config_Chat ==========
Config_Chat::Config_Chat(QString qstr_FilePath) :
	Page_Config_Base(qstr_FilePath)
{
}

Config_Chat::~Config_Chat()
{
}

Config_Chat& Config_Chat::instance()
{
	static Config_Chat ins(ConfigurationBase::instance().GetChatPageConfigFilePath());
	return ins;
}

void Config_Chat::Initialize()
{
	this->InitInstanceData();
	//Debug_PrintJson();
	return;
}

// ========== end ==========