#pragma once

#include "DDeskPetConfigInterfaceBase.h"
#include "DDeskPetConfig_Base.h"

// =========== AIÁÄÌì ==========
class Config_Chat : virtual public Page_Config_Base {
public:
	Config_Chat() = default;
	Config_Chat(QString qstr_FilePath);
	~Config_Chat();

	static Config_Chat& instance();
	virtual void Initialize() override;
};
// =========== end ==========