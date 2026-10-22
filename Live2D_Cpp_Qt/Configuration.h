
/*
 ============================================================================
 Header: Configuration.h
 Description: 配置文件管理器
 Start time: 2025.07.21
 End time: 2026.01.07
 Maintainer: WangSZ
 ============================================================================
*/

#pragma once

// function
// +-----------+
// |   parent  | <----------------+
// +-----+-----+                  |
//       |                        |
//       |---------------+        |
// +-----+-----+   +-----+-----+  |
// |  child_1  | ~ |  child_n  |  |
// +-----+-----+   +-----+-----+  |
//       |			     |        |
//       |			     |        |
// +-----+-----+   +-----+-----+  |
// | interface1|   | interface2|  |
// +-----+-----+   +-----+-----+  |
//       |               |        |
//       +--Effectuate---+--------+



#include "DDeskPetConfigInterfaceBase.h"

#include "DDeskPetConfig_Account.h"
#include "DDeskPetConfig_Chat.h"
#include "DDeskPetConfig_Developer.h"
#include "DDeskPetConfig_Function.h"
#include "DDeskPetConfig_Model.h"
#include "DDeskPetConfig_Personalization.h"
#include "DDeskPetConfig_SetAI.h"

#include "DDeskPetConfig_Base.h"


class Configuration {
public:
	// 单例
	Configuration();
	~Configuration();

	static Config_Account& GetIns_AccountPageConfig();
	static Config_Personalization& GetIns_PersonalizationPageConfig();
	static Config_Function& GetIns_FunctionWindowsPageConfig();
	static Config_Model& GetIns_ModelPageConfig();
	static Config_Developer& GetIns_DeveloperPageConfig();
	static Config_SetAI& GetIns_SetAIPageConfig();
	static Config_Chat& GetIns_ChatPageConfig();

	void Initialize();

	static Configuration& instance();

private:
	Configuration(const Configuration&) = delete;
	Configuration& operator=(const Configuration&) = delete;
};