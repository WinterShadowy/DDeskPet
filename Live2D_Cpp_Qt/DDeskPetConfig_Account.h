#pragma once

#include "DDeskPetConfigInterfaceBase.h"
#include "DDeskPetConfig_Base.h"

// ========== 账号页面配置 ==========
class Config_Account : virtual public Page_Config_Base {
public:
	Config_Account() = default;
	Config_Account(QString qstr_FilePath);
	~Config_Account();

	static Config_Account& instance();
	virtual void Initialize() override;
	nlohmann::json& GetJson();
	void SetJsonFromSettingPage(nlohmann::json json);
	// 获取显示/隐藏密码的图标
	QString GetIcon_ShowPassword();
	QString GetIcon_HidePassword();

	QString Get_Username();
	QString Get_Password();
};
// =========== end =========