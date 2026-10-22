#pragma once

#include "DDeskPetConfigInterfaceBase.h"
#include "DDeskPetConfig_Base.h"

typedef char _bit8_int;
typedef unsigned char _bit8_uint;

typedef _bit8_uint CharacterMode;		// 高4位 人设模式
typedef _bit8_uint RandomnessMode;		// 低4位 随机度模式

// ========== 设置AI页面配置 ==========
class Config_SetAI : virtual public Page_Config_Base {
public:
	Config_SetAI() = default;
	Config_SetAI(QString qstr_FilePath);
	~Config_SetAI();
public:
	static Config_SetAI& instance();
	virtual void Initialize() override;

	nlohmann::json& GetConfig();

	// ===== 获取设置 =====
	// 获取Url
	QString Get_Url();
	// 获取URL额外路径 - chat
	QString Get_Url_Extra_Path_Chat();
	// 获取URL额外路径 - models
	QString Get_Url_Extra_Path_Models();
	// 获取完整的Chat路径
	QString GetUrl_FullPath_Chat();
	std::string GetURL_ChatFullPath();
	// 获取完整的Models路径
	QString GetUrl_FullPath_Models();
	std::string GetURL_ModelsFullPath();

	// 获取Api Key
	QString Get_Api_Key();
	std::string GetApiKey();
	// 获取模型
	QStringList Get_ModelListFromConfigFile();
	QString Get_Model();
	// 人设设置
	bool IsPromptMode();
	bool IsSystemMode();
	// 获取提示-System
	QString GetPrompt_System();
	// 获取提示-Prompt
	QString GetPrompt_Prompt();
	// 获取回复数量
	unsigned int Get_N();
	bool GetUseNStatus();
	// 获取是否追加n的值
	bool GetUseAppendN();
	// 获取最大token
	unsigned int Get_Max_Tokens();
	// 获取是否使用Max_Tokens
	bool GetUseMaxTokensStatus();
	// 获取是否追加Max_Tokens
	bool GetUseAppendMaxTokens();

	bool IsTemperature();
	bool GetUseAppendTemperature();
	bool IsTop_p();
	bool GetUseAppendTop_p();

	// 获取随机性—Temperature
	double Get_Randomness_Temperature();

	// 获取随机性—Top_p
	double Get_Randomness_Top_p();

	// 获取 presence_penalty
	bool Use_Presence_Penalty();
	bool Use_Append_Presence_Penalty();
	float Get_Presence_Penalty();

	// frequency_penalty
	bool Use_Frequency_Penalty();
	bool Use_Append_Frequency_Penalty();
	float Get_Frequency_Penalty();

	// 获取停止词
	QSet<QString> Get_Stop();
	// 获取提示词
	QHash<QString, QString> Get_ToolTip();
	bool Use_Append_Stop();

	// 是否使用自定义json
	bool GetUseCustomJson();
	// 获取自定义json
	nlohmann::json GetCustomJson();
	// 是否使用自定义追加json
	bool GetUseAppendCustomJson();
	// 获取自定义追加json
	nlohmann::json GetAppendCustomJson();

	// ====== 设置配置 ======
	bool Set_Url(const QString& qstr_Url);
	// 设置API路径
	bool Set_URL_APIPath(const QString& qstr_path);
	// 设置API模型路径
	bool Set_URL_ModelsPath(const QString& qstr_path);
	bool Set_Api_Key(const QString& qstr_ApiKey);
	bool Set_Model(const QString& qstr_Model);
	bool Set_Prompt_System(const QString& qstr_System);
	bool Set_UseSystem(bool status);
	// 设置停止词
	bool Set_Stop(const QSet<QString>& stop);
	bool Set_Stop(const QSet<QString>& stop, bool append_stop = false);
	bool Set_Append_Stop(bool append_stop);

	bool Set_Prompt_Prompt(const QString& qstr_Prompt);
	bool Set_UsePrompt(bool status);
	// 设置回复数量
	bool Set_N(const unsigned int n, bool use);
	bool Set_N(const unsigned short n, bool use_n, bool use_append_n);
	bool SetAppendN(bool use_append_n);
	// 设置最大token数
	bool Set_Max_Tokens(const unsigned int max_tokens, bool use);
	bool Set_Max_Tokens(const unsigned int max_tokens, bool use_max_tokens, bool use_append_max_tokens);
	bool Set_Randomness_Temperature(double temperature, bool use);
	bool Set_Randomness_Temperature(double temperature, bool use, bool use_append_temperature);
	bool Set_Randomness_Top_p(double top_p, bool use);
	bool Set_Randomness_Top_p(double top_p, bool use, bool use_append_top_p);
	// 设置频率惩罚
	bool Set_frequency_penalty(double frequency_penalty, bool status);
	bool Set_frequency_penalty(double frequency_penalty, bool status, bool use_append_frequency_penalty);
	// 设置presence惩罚
	bool Set_presence_penalty(double presence_penalty, bool status);
	bool Set_presence_penalty(double presence_penalty, bool status, bool use_append_presence_penalty);

	// 设置自定义json
	bool Set_UseCustomJson(bool status);
	bool Set_CustomJson(std::string qstr_Json);
	// 设置自定义追加json
	bool Set_UseAppendCustomJson(bool status);
	bool Set_AppendCustomJson(std::string qstr_Json);

private:
	RandomnessMode _randomnessMode;
	CharacterMode _promptMode;

};
// =========== end ==========