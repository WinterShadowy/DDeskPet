#pragma once

#include "DDeskPetConfigInterfaceBase.h"
#include "DDeskPetConfig_Base.h"

// ========== 个性化页面配置 ==========
class Config_Personalization : virtual public Page_Config_Base {
public:
	Config_Personalization() = default;
	Config_Personalization(QString qstr_FilePath);
	~Config_Personalization();

	static Config_Personalization& instance();
	virtual void Initialize() override;
};
// ========== end ==========