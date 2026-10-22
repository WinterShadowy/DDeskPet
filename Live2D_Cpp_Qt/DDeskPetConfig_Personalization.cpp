#include "DDeskPetConfig_Personalization.h"


//  ========= Config_Personalization ==========

Config_Personalization::Config_Personalization(QString qstr_FilePath) :
	Page_Config_Base(qstr_FilePath)
{
}

Config_Personalization::~Config_Personalization()
{
}

Config_Personalization& Config_Personalization::instance()
{
	static Config_Personalization ins(ConfigurationBase::instance().GetPersonalizationPageConfigFilePath());
	return ins;
}

void Config_Personalization::Initialize()
{
	this->InitInstanceData();
	//Debug_PrintJson();
	return;
}

// ========= end =========
