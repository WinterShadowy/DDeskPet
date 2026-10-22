#include "DDeskPetConfig_SetAI.h"

// ========== Config_SetAI ==========
Config_SetAI::Config_SetAI(QString qstr_FilePath) :
	Page_Config_Base(qstr_FilePath)
{
}
Config_SetAI::~Config_SetAI()
{
}
Config_SetAI& Config_SetAI::instance()
{
	static Config_SetAI ins(ConfigurationBase::instance().GetSetAIPageConfigFilePath());
	return ins;
}
void Config_SetAI::Initialize()
{
	this->InitInstanceData();
	//Debug_PrintJson();
	return;
}
nlohmann::json& Config_SetAI::GetConfig()
{
	return this->GetJsonFromConfigFile();
}
// interface
// 获取url
QString Config_SetAI::Get_Url()
{
	// 是否存在url字段
	if (this->GetJsonFromConfigFile().contains("url_info") == true &&
		this->GetJsonFromConfigFile().at("url_info").contains("basic_url") == true &&
		this->GetJsonFromConfigFile().at("url_info").at("basic_url").contains("url") == true)
	{
		return QString::fromStdString(this->GetJsonFromConfigFile()
			.at("url_info")
			.at("basic_url")
			.at("url")
			.get<std::string>());
	}
	else
	{
		RuntimeLog_Debug("url_info、basic_url or url not found in json");
		return QString("YOUR_URL");
	}
}
QString Config_SetAI::Get_Url_Extra_Path_Chat()
{
	// 确保路径是否存在
	if (this->GetJsonFromConfigFile().contains("url_info") == true &&
		this->GetJsonFromConfigFile().at("url_info").contains("url_extra") == true &&
		this->GetJsonFromConfigFile().at("url_info").at("url_extra").contains("chat") == true &&
		this->GetJsonFromConfigFile().at("url_info").at("url_extra").at("chat").contains("path") == true)
	{
		return QString::fromStdString(this->GetJsonFromConfigFile()
			.at("url_info")
			.at("url_extra")
			.at("chat")
			.at("path")
			.get<std::string>());
	}
	else
	{
		RuntimeLog_Debug("url_info、url_extra、 chat or path not found in json");
		return QString("/v1/chat/completions");
	}
}
QString Config_SetAI::Get_Url_Extra_Path_Models()
{
	if (this->GetJsonFromConfigFile().contains("url_info") == true &&
		this->GetJsonFromConfigFile().at("url_info").contains("url_extra") == true &&
		this->GetJsonFromConfigFile().at("url_info").at("url_extra").contains("models") == true &&
		this->GetJsonFromConfigFile().at("url_info").at("url_extra").at("models").contains("path") == true)
	{
		return QString::fromStdString(this->GetJsonFromConfigFile()
			.at("url_info")
			.at("url_extra")
			.at("models")
			.at("path")
			.get<std::string>());
	}
	else
	{
		RuntimeLog_Debug("url_info、url_extra、 models or path not found in json");
		return QString("/v1/models");
	}
}
QString Config_SetAI::GetUrl_FullPath_Chat()
{
	return Get_Url() + Get_Url_Extra_Path_Chat();
}
std::string Config_SetAI::GetURL_ChatFullPath()
{
	return Get_Url().toStdString() + Get_Url_Extra_Path_Chat().toStdString();
}
QString Config_SetAI::GetUrl_FullPath_Models()
{
	try {
		std::string full_path, _url, _models;
		std::cerr << "JsonInfo: " << this->GetJsonFromConfigFile().dump(2);
		if (this->GetJsonFromConfigFile().contains("url_info") == true &&
			this->GetJsonFromConfigFile().at("url_info").contains("url_extra") == true &&
			this->GetJsonFromConfigFile().at("url_info").at("url_extra").contains("models") == true &&
			this->GetJsonFromConfigFile().at("url_info").at("url_extra").at("models").contains("path") == true &&
			this->GetJsonFromConfigFile().at("url_info").contains("basic_url") == true &&
			this->GetJsonFromConfigFile().at("url_info").at("basic_url").contains("url") == true)
		{
			_url = this->GetJsonFromConfigFile()
				.at("url_info")
				.at("basic_url")
				.at("url")
				.get<std::string>();
			_models = this->GetJsonFromConfigFile()
				.at("url_info")
				.at("url_extra")
				.at("models")
				.at("path")
				.get<std::string>();

		}
		else
		{
			_url = "YOUR_URL";
			_models = "/v1/models";
		}
		full_path = _url + _models;
		return QString::fromStdString(full_path);
	}
	catch (const std::exception& e)
	{
		RuntimeLog_Error(e.what());
		return "https://api.cpluscottage.top/v1/models";
	}
}
std::string Config_SetAI::GetURL_ModelsFullPath()
{
	if (this->GetJsonFromConfigFile().is_null() || this->GetJsonFromConfigFile().empty())
	{
		std::cerr << "JsonInfo: JsonIsEmpty";
		return "https://api.cpluscottage.top/v1/models";
	}
	try {
		std::string full_path;

		if (this->GetJsonFromConfigFile().contains("url_info") == true &&
			this->GetJsonFromConfigFile().at("url_info").contains("url_extra") == true &&
			this->GetJsonFromConfigFile().at("url_info").at("url_extra").contains("models") == true &&
			this->GetJsonFromConfigFile().at("url_info").at("url_extra").at("models").contains("path") == true &&
			this->GetJsonFromConfigFile().at("url_info").contains("basic_url") == true &&
			this->GetJsonFromConfigFile().at("url_info").at("basic_url").contains("url") == true)
		{
			std::string _url = this->GetJsonFromConfigFile()
				.at("url_info")
				.at("basic_url")
				.at("url")
				.get<std::string>();
			std::string _models = this->GetJsonFromConfigFile()
				.at("url_info")
				.at("url_extra")
				.at("models")
				.at("path")
				.get<std::string>();
			full_path = _url + _models;
		}
		else
		{
			std::string _url = "YOUR_URL";
			std::string _models = "/v1/models";
			full_path = _url + _models;
		}
		return full_path;
	}
	catch (const std::exception& e)
	{
		RuntimeLog_Error(e.what());
		return "https://api.cpluscottage.top/v1/models";
	}
}

// 获取api_key
QString Config_SetAI::Get_Api_Key()
{
	try {
		// 是否存在api_key字段
		if (this->GetJsonFromConfigFile().contains("api-key"))
		{
			return QString::fromStdString(this->GetJsonFromConfigFile().at("api-key").get<std::string>());
		}
		else
		{
			RuntimeLog_Error("api-key not found in json");
			return QString("YOUR_API_KEY");
		}
	}
	catch (const std::exception& e)
	{
		RuntimeLog_Error(e.what());
		return QString("YOUR_API_KEY");
	}
}
std::string Config_SetAI::GetApiKey()
{
	if (this->GetJsonFromConfigFile().contains("api-key"))
	{
		return this->GetJsonFromConfigFile().at("api-key").get<std::string>();
	}
	else
	{
		RuntimeLog_Debug("api-key not found in json");
		return std::string("YOUR_API_KEY");
	}
}
QStringList Config_SetAI::Get_ModelListFromConfigFile()
{
	// 是否存在model字段
	if (this->GetJsonFromConfigFile().contains("model") == true)
	{
		QStringList modelList;
		modelList.append(QString::fromStdString(this->GetJsonFromConfigFile().at("model").get<std::string>()));
		return modelList;
	}
	else
	{
		RuntimeLog_Debug("model not found in json");
		QStringList modelList;
		modelList.append(QString("gpt-3.5-turbo"));
		return modelList;
	}
}
QString Config_SetAI::Get_Model()
{
	if (this->GetJsonFromConfigFile().contains("model") == true)
	{
		return QString::fromStdString(this->GetJsonFromConfigFile().at("model").get<std::string>());
	}
	else
	{
		RuntimeLog_Debug("model not found in json");
		return QString("gpt-3.5-turbo");
	}
	return QString("gpt-3.5-turbo");
}
bool Config_SetAI::IsPromptMode()
{
	if (this->GetJsonFromConfigFile().contains("AI-Prompt") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").contains("prompt") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").at("prompt").contains("use") == true)
	{
		return this->GetJsonFromConfigFile().at("AI-Prompt").at("prompt").at("use").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("AI-Prompt, prompt or use not found in json");
		return false;
	}
}
bool Config_SetAI::IsSystemMode()
{
	if (this->GetJsonFromConfigFile().contains("AI-Prompt") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").contains("system") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").at("system").contains("use") == true)
	{
		return this->GetJsonFromConfigFile().at("AI-Prompt").at("system").at("use").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("AI-Prompt,system or use not found in json");
	}
	return false;
}
QString Config_SetAI::GetPrompt_System()
{
	if (this->GetJsonFromConfigFile().contains("AI-Prompt") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").contains("system") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").at("system").contains("value") == true)
	{
		return QString::fromStdString(this->GetJsonFromConfigFile().at("AI-Prompt").at("system").at("value").get<std::string>());
	}
	else
	{
		RuntimeLog_Debug("AI-Prompt,system or value not found in json");
	}
	return QString("You are a helpful assistant.");
}
QString Config_SetAI::GetPrompt_Prompt()
{
	if (this->GetJsonFromConfigFile().contains("AI-Prompt") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").contains("prompt") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").at("prompt").contains("value") == true)
	{
		return QString::fromStdString(this->GetJsonFromConfigFile().at("AI-Prompt").at("prompt").at("value").get<std::string>());
	}
	else
	{
		RuntimeLog_Debug("AI-Prompt,prompt or value not found in json");
	}
	return QString("You are a helpful assistant.");
}
unsigned int Config_SetAI::Get_N()
{
	if (this->GetJsonFromConfigFile().contains("n") == true &&
		this->GetJsonFromConfigFile().at("n").contains("value") == true)
	{
		return this->GetJsonFromConfigFile().at("n").at("value").get<unsigned int>();
	}
	else
	{
		RuntimeLog_Debug("n or value not found in json");
		return 1;
	}
	return 1;
}
bool Config_SetAI::GetUseNStatus()
{
	if (this->GetJsonFromConfigFile().contains("n") == true &&
		this->GetJsonFromConfigFile().at("n").contains("use_custom_values") == true)
	{
		return this->GetJsonFromConfigFile().at("n").at("use_custom_values").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("n or use_custom_values not found in json");
		return false;
	}
	return false;
}
bool Config_SetAI::GetUseAppendN()
{
	if (this->GetJsonFromConfigFile().contains("n") == true &&
		this->GetJsonFromConfigFile().at("n").contains("append_to_request") == true)
	{
		return this->GetJsonFromConfigFile().at("n").at("append_to_request").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("n or append_to_request not found in json");
		return false;
	}
	return false;
}
unsigned int Config_SetAI::Get_Max_Tokens()
{
	if (this->GetJsonFromConfigFile().contains("max_tokens") == true &&
		this->GetJsonFromConfigFile().at("max_tokens").contains("value") == true)
	{
		return this->GetJsonFromConfigFile().at("max_tokens").at("value").get<int>();
	}
	else
	{
		RuntimeLog_Debug("max_tokens or value not found in json");
		return 1000;
	}
}
bool Config_SetAI::GetUseMaxTokensStatus()
{
	if (this->GetJsonFromConfigFile().contains("max_tokens") == true &&
		this->GetJsonFromConfigFile().at("max_tokens").contains("use_custom_values") == true)
	{
		return this->GetJsonFromConfigFile().at("max_tokens").at("use_custom_values").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("max_tokens or use_custom_values not found in json");
		return false;
	}
	return false;
}
bool Config_SetAI::GetUseAppendMaxTokens()
{
	if (this->GetJsonFromConfigFile().contains("max_tokens") == true &&
		this->GetJsonFromConfigFile().at("max_tokens").contains("append_to_request") == true)
	{
		return this->GetJsonFromConfigFile().at("max_tokens").at("append_to_request").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("max_tokens or append_to_request not found in json");
		return false;
	}
	return false;
}
bool Config_SetAI::IsTemperature()
{
	if (this->GetJsonFromConfigFile().contains("randomness") == true &&
		this->GetJsonFromConfigFile().at("randomness").contains("temperature") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("temperature").contains("use") == true)
	{
		return this->GetJsonFromConfigFile().at("randomness").at("temperature").at("use").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("randomness, temperature or use not found in json");
		return false;
	}
	return false;
}
bool Config_SetAI::GetUseAppendTemperature()
{
	if (this->GetJsonFromConfigFile().contains("randomness") == true &&
		this->GetJsonFromConfigFile().at("randomness").contains("temperature") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("temperature").contains("append_to_request") == true)
	{
		return this->GetJsonFromConfigFile().at("randomness").at("temperature").at("append_to_request").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("randomness, temperature or append_to_request not found in json");
		return false;
	}
	return false;
}
bool Config_SetAI::IsTop_p()
{
	if (this->GetJsonFromConfigFile().contains("randomness") == true &&
		this->GetJsonFromConfigFile().at("randomness").contains("top_p") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("top_p").contains("use") == true)
	{
		return this->GetJsonFromConfigFile().at("randomness").at("top_p").at("use").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("randomness, top_p or use not found in json");
		return false;
	}
	return false;
}
bool Config_SetAI::GetUseAppendTop_p()
{
	if (this->GetJsonFromConfigFile().contains("randomness") == true &&
		this->GetJsonFromConfigFile().at("randomness").contains("top_p") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("top_p").contains("append_to_request") == true)
	{
		return this->GetJsonFromConfigFile().at("randomness").at("top_p").at("append_to_request").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("randomness, top_p or append_to_request not found in json");
		return false;
	}
	return false;
}
double Config_SetAI::Get_Randomness_Temperature()
{
	if (this->GetJsonFromConfigFile().contains("randomness") == true &&
		this->GetJsonFromConfigFile().at("randomness").contains("temperature") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("temperature").contains("value") == true)
	{
		return this->GetJsonFromConfigFile().at("randomness").at("temperature").at("value").get <double>();
	}
	else
	{
		RuntimeLog_Debug("randomness, temperature or value not found in json");
		return 0.0f;
	}
	return 0.0f;
}
double Config_SetAI::Get_Randomness_Top_p()
{
	if (this->GetJsonFromConfigFile().contains("randomness") == true &&
		this->GetJsonFromConfigFile().at("randomness").contains("top_p") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("top_p").contains("value") == true)
	{
		return this->GetJsonFromConfigFile().at("randomness").at("top_p").at("value").get<double>();
	}
	else
	{
		RuntimeLog_Debug("randomness, top_p or value not found in json");
		return 0.0f;
	}
	return 0.0f;
}
bool Config_SetAI::Use_Presence_Penalty()
{
	if (this->GetJsonFromConfigFile().contains("presence_penalty") == true &&
		this->GetJsonFromConfigFile().at("presence_penalty").contains("use") == true)
	{
		return this->GetJsonFromConfigFile().at("presence_penalty").at("use").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("presence_penalty or use not found in json");
		return false;
	}
	return false;
}
bool Config_SetAI::Use_Append_Presence_Penalty()
{
	if (this->GetJsonFromConfigFile().contains("presence_penalty") == true &&
		this->GetJsonFromConfigFile().at("presence_penalty").contains("append_to_request") == true)
	{
		return this->GetJsonFromConfigFile().at("presence_penalty").at("append_to_request").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("presence_penalty or append_to_request not found in json");
		return false;
	}
	return false;
}
float Config_SetAI::Get_Presence_Penalty()
{
	if (this->GetJsonFromConfigFile().contains("presence_penalty") == true &&
		this->GetJsonFromConfigFile().at("presence_penalty").contains("value") == true)
	{
		return this->GetJsonFromConfigFile().at("presence_penalty").at("value").get<float>();
	}
	else
	{
		RuntimeLog_Debug("presence_penalty or value not found in json");
		return 0.0f;
	}
	return 0.0f;
}
bool Config_SetAI::Use_Frequency_Penalty()
{
	if (this->GetJsonFromConfigFile().contains("frequency_penalty") == true &&
		this->GetJsonFromConfigFile().at("frequency_penalty").contains("use") == true)
	{
		return this->GetJsonFromConfigFile().at("frequency_penalty").at("use").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("frequency_penalty or use not found in json");
	}
	return false;
}
bool Config_SetAI::Use_Append_Frequency_Penalty()
{
	if (this->GetJsonFromConfigFile().contains("frequency_penalty") == true &&
		this->GetJsonFromConfigFile().at("frequency_penalty").contains("append_to_request") == true)
	{
		return this->GetJsonFromConfigFile().at("frequency_penalty").at("append_to_request").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("frequency_penalty or append_to_request not found in json");
	}
	return false;
}
float Config_SetAI::Get_Frequency_Penalty()
{
	if (this->GetJsonFromConfigFile().contains("frequency_penalty") == true &&
		this->GetJsonFromConfigFile().at("frequency_penalty").contains("value") == true)
	{
		return this->GetJsonFromConfigFile().at("frequency_penalty").at("value").get<float>();
	}
	else
	{
		RuntimeLog_Debug("frequency_penalty or value not found in json");
		return 0.0f;
	}
	return 0.0f;
}
QSet<QString> Config_SetAI::Get_Stop()
{
	if (this->GetJsonFromConfigFile().contains("stop") == true &&
		this->GetJsonFromConfigFile().at("stop").contains("data") == true)
	{
		QSet<QString> qset_Stop;
		for (const auto& item : this->GetJsonFromConfigFile().at("stop").at("data"))
		{
			qset_Stop.insert(QString::fromStdString(item.get<std::string>()));
		}
		return qset_Stop;
	}
	else
	{
		RuntimeLog_Debug("stop or data not found in json");
	}
	return QSet<QString>();
}
QHash<QString, QString> Config_SetAI::Get_ToolTip()
{
	if (this->GetJsonFromConfigFile().contains("tooltip") == true)
	{
		QHash<QString, QString> qhash_ToolTip;
		for (auto it = this->GetJsonFromConfigFile().at("ToolTip").begin(); it != this->GetJsonFromConfigFile().at("ToolTip").end(); ++it)
		{
			qhash_ToolTip.insert(QString::fromStdString(it.key()), QString::fromStdString(it.value().get<std::string>()));
		}
		return qhash_ToolTip;
	}
	else
	{
		RuntimeLog_Debug("tooltip not found in json");
	}
	return QHash<QString, QString>();
}
bool Config_SetAI::Use_Append_Stop()
{
	if (this->GetJsonFromConfigFile().contains("stop") == true &&
		this->GetJsonFromConfigFile().at("stop").contains("append_to_request") == true)
	{
		return this->GetJsonFromConfigFile().at("stop").at("append_to_request").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("stop or append_to_request not found in json");
	}
	return false;
}
bool Config_SetAI::GetUseCustomJson()
{
	if (this->GetJsonFromConfigFile().contains("cus_data") == true &&
		this->GetJsonFromConfigFile().at("cus_data").contains("custom_json") == true &&
		this->GetJsonFromConfigFile().at("cus_data").at("custom_json").contains("use_custom_json") == true)
	{
		return this->GetJsonFromConfigFile()
			.at("cus_data")
			.at("custom_json")
			.at("use_custom_json")
			.get<bool>();
	}
	else
	{
		RuntimeLog_Debug("use_custom_json not found in json");
		return false;
	}
	return false;
}
nlohmann::json Config_SetAI::GetCustomJson()
{
	if (this->GetJsonFromConfigFile().contains("cus_data") == true &&
		this->GetJsonFromConfigFile().at("cus_data").contains("custom_json") == true &&
		this->GetJsonFromConfigFile().at("cus_data").at("custom_json").contains("data") == true)
	{
		return this->GetJsonFromConfigFile()
			.at("cus_data")
			.at("custom_json")
			.at("data");
	}
	else
	{
		RuntimeLog_Debug("cus_data、custom_json or data not found in json");
		return nlohmann::json();
	}
	return nlohmann::json();
}

bool Config_SetAI::GetUseAppendCustomJson()
{
	if (this->GetJsonFromConfigFile().contains("cus_data") == true &&
		this->GetJsonFromConfigFile().at("cus_data").contains("append_json") == true &&
		this->GetJsonFromConfigFile().at("cus_data").at("append_json").contains("use_append_json") == true)
	{
		return this->GetJsonFromConfigFile()
			.at("cus_data")
			.at("append_json")
			.at("use_append_json")
			.get<bool>();
	}
	else
	{
		RuntimeLog_Debug("cus_data,append_json or use_append_json not found in json");
		return false;
	}
	return false;
}

nlohmann::json Config_SetAI::GetAppendCustomJson()
{
	if (this->GetJsonFromConfigFile().contains("cus_data") == true &&
		this->GetJsonFromConfigFile().at("cus_data").contains("append_json") == true &&
		this->GetJsonFromConfigFile().at("cus_data").at("append_json").contains("data") == true)
	{
		return this->GetJsonFromConfigFile()
			.at("cus_data")
			.at("append_json")
			.at("data");
	}
	else
	{
		RuntimeLog_Debug("cus_data、append_json or data not found in json");
		return nlohmann::json();
	}
	return nlohmann::json();
}

// ==== set ====
bool Config_SetAI::Set_Url(const QString& qstr_Url)
{
	if (qstr_Url.isEmpty() == true)
	{
		RuntimeLog_Error("qstr_Url is empty");
		return false;
	}
	else
	{
		this->GetJsonFromConfigFile()["url_info"]["basic_url"]["url"] = qstr_Url.toStdString();
	}

	return true;
}
bool Config_SetAI::Set_URL_APIPath(const QString& qstr_path)
{
	if (qstr_path.isEmpty() == true)
	{
		RuntimeLog_Debug("url_extra_path_chat is empty");
		return false;
	}
	if (this->GetJsonFromConfigFile().contains("url_info") == true &&
		this->GetJsonFromConfigFile().at("url_info").contains("url_extra") == true &&
		this->GetJsonFromConfigFile().at("url_info").at("url_extra").contains("chat") == true &&
		this->GetJsonFromConfigFile().at("url_info").at("url_extra").at("chat").contains("path") == true)
	{
		this->GetJsonFromConfigFile().at("url_info").at("url_extra").at("chat").at("path") = qstr_path.toStdString();
		return true;
	}
	else
	{
		RuntimeLog_Debug("url_info,url_extra, chat or path not found in json");
		this->GetJsonFromConfigFile().at("url_info").at("url_extra").at("chat").at("path") = "/v1/chat/completions";
		return false;
	}
	return true;
}
bool Config_SetAI::Set_URL_ModelsPath(const QString& qstr_path)
{
	if (qstr_path.isEmpty() == true)
	{
		return false;
	}
	if (this->GetJsonFromConfigFile().contains("url_info") == true &&
		this->GetJsonFromConfigFile().at("url_info").contains("url_extra") == true &&
		this->GetJsonFromConfigFile().at("url_info").at("url_extra").contains("models") == true &&
		this->GetJsonFromConfigFile().at("url_info").at("url_extra").at("models").contains("path") == true)
	{
		this->GetJsonFromConfigFile().at("url_info").at("url_extra").at("models").at("path") = qstr_path.toStdString();
		return true;
	}
	else
	{
		RuntimeLog_Debug("url_info, url_extra or models not found in json");
		this->GetJsonFromConfigFile().at("url_info").at("url_extra").at("models").at("path") = "/v1/models";
		return false;
	}
	return true;
}
bool Config_SetAI::Set_Api_Key(const QString& qstr_ApiKey)
{
	if (qstr_ApiKey.isEmpty() == true)
		return false;
	if (this->GetJsonFromConfigFile().contains("api-key") == true)
	{
		this->GetJsonFromConfigFile().at("api-key") = qstr_ApiKey.toStdString();
		return true;
	}
	else
	{
		RuntimeLog_Debug("api-key not found in json");
		this->GetJsonFromConfigFile().at("api-key") = "";
		return false;
	}
	return true;
}
bool Config_SetAI::Set_Model(const QString& qstr_Model)
{
	if (qstr_Model.isEmpty() == true)
		return false;
	if (this->GetJsonFromConfigFile().contains("model") == true)
	{
		this->GetJsonFromConfigFile().at("model") = qstr_Model.toStdString();
		return true;
	}
	else
	{
		RuntimeLog_Debug("model not found in json");
		this->GetJsonFromConfigFile().at("model") = "gpt-3.5-turbo";
	}
	return false;
}

bool Config_SetAI::Set_Prompt_System(const QString& qstr_System)
{
	if (qstr_System.isEmpty() == true)
		return false;
	if (this->GetJsonFromConfigFile().contains("AI-Prompt") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").contains("system") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").at("system").contains("value") == true)
	{
		this->GetJsonFromConfigFile().at("AI-Prompt").at("system").at("value") = qstr_System.toStdString();
	}
	return true;
}
bool Config_SetAI::Set_UseSystem(bool status)
{
	if (this->GetJsonFromConfigFile().contains("AI-Prompt") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").contains("system") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").at("system").contains("use") == true)
	{
		this->GetJsonFromConfigFile().at("AI-Prompt").at("system").at("use") = status;
		return true;
	}
	else
	{
		RuntimeLog_Debug("AI-Prompt or system or use not found in json");
	}
	return false;
}
bool Config_SetAI::Set_Stop(const QSet<QString>& stop)
{
	if (stop.isEmpty() == false)
	{
		nlohmann::json::array_t arr;
		for (const auto& item : stop)
		{
			arr.push_back(item.toStdString());
		}
		this->GetJsonFromConfigFile().at("stop").at("data") = arr;
		return true;
	}
	return false;
}
bool Config_SetAI::Set_Stop(const QSet<QString>& stop, bool append_stop)
{
	if (stop.isEmpty() == false)
	{
		nlohmann::json::array_t arr;
		for (const auto& item : stop)
		{
			arr.push_back(item.toStdString());
		}
		this->GetJsonFromConfigFile().at("stop").at("data") = arr;
	}
	if (this->GetJsonFromConfigFile().contains("stop") == true &&
		this->GetJsonFromConfigFile().at("stop").contains("append_to_request") == true)
	{
		this->GetJsonFromConfigFile().at("stop").at("append_to_request") = append_stop;
		return true;
	}
	else
	{
		RuntimeLog_Debug("stop or append_to_request not found in json");
	}
	return false;
}
bool Config_SetAI::Set_Append_Stop(bool append_stop)
{
	if (this->GetJsonFromConfigFile().contains("stop") == true &&
		this->GetJsonFromConfigFile().at("stop").contains("append_to_request") == true)
	{
		this->GetJsonFromConfigFile().at("stop").at("append_to_request") = append_stop;
		return true;
	}
	else
	{
		RuntimeLog_Debug("stop or append_to_request not found in json");
	}
	return false;
}
bool Config_SetAI::Set_Prompt_Prompt(const QString& qstr_Prompt)
{
	if (qstr_Prompt.isEmpty() == true)
		return false;
	if (this->GetJsonFromConfigFile().contains("AI-Prompt") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").contains("prompt") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").at("prompt").contains("value") == true)
	{
		this->GetJsonFromConfigFile().at("AI-Prompt").at("prompt").at("value") = qstr_Prompt.toStdString();
	}
	return true;
}
bool Config_SetAI::Set_UsePrompt(bool status)
{
	if (this->GetJsonFromConfigFile().contains("AI-Prompt") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").contains("prompt") == true &&
		this->GetJsonFromConfigFile().at("AI-Prompt").at("prompt").contains("use") == true)
	{
		this->GetJsonFromConfigFile().at("AI-Prompt").at("prompt").at("use") = status;
		return true;
	}
	else
	{
		RuntimeLog_Debug("AI-Prompt or prompt or use not found in json");
	}
	return false;
}
bool Config_SetAI::Set_N(const unsigned int n, bool use)
{
	if (n > 0 &&
		this->GetJsonFromConfigFile().contains("n") == true &&
		this->GetJsonFromConfigFile().at("n").contains("value") == true &&
		this->GetJsonFromConfigFile().at("n").contains("use_custom_values") == true)
	{
		this->GetJsonFromConfigFile()["n"]["value"] = n;
		this->GetJsonFromConfigFile()["n"]["use_custom_values"] = use;
		return true;
	}
	else
	{
		RuntimeLog_Debug("n range error | n,value or use_custom_values not found in json");
	}
	return false;
}
bool Config_SetAI::Set_N(const unsigned short n, bool use_n, bool use_append_n)
{
	if (n > 0 &&
		this->GetJsonFromConfigFile().contains("n") == true &&
		this->GetJsonFromConfigFile().at("n").contains("value") == true &&
		this->GetJsonFromConfigFile().at("n").contains("use_custom_values") == true &&
		this->GetJsonFromConfigFile().at("n").contains("append_to_request") == true)
	{
		this->GetJsonFromConfigFile()["n"]["value"] = static_cast<int>(n);
		this->GetJsonFromConfigFile()["n"]["use_custom_values"] = use_n;
		this->GetJsonFromConfigFile()["n"]["append_to_request"] = use_append_n;
		return true;
	}
	else
	{
		RuntimeLog_Debug("n range error | n,value or use_custom_values or use_append_n not found in json");
	}
	return false;
}
bool Config_SetAI::SetAppendN(bool use_append_n)
{
	if (this->GetJsonFromConfigFile().contains("n") == true &&
		this->GetJsonFromConfigFile().at("n").contains("append_to_request") == true)
	{
		this->GetJsonFromConfigFile()["n"]["append_to_request"] = use_append_n;
		return true;
	}
	else
	{
		RuntimeLog_Debug("n or append_to_request not found in json");
	}
	return false;
}
bool Config_SetAI::Set_Max_Tokens(const unsigned int max_tokens, bool use)
{
	if (max_tokens > 0 &&
		this->GetJsonFromConfigFile().contains("max_tokens") == true &&
		this->GetJsonFromConfigFile().at("max_tokens").contains("value") == true &&
		this->GetJsonFromConfigFile().at("max_tokens").contains("use_custom_values") == true)
	{
		this->GetJsonFromConfigFile().at("max_tokens")["value"] = max_tokens;
		this->GetJsonFromConfigFile().at("max_tokens")["use_custom_values"] = use;
		return true;
	}
	else
	{
		RuntimeLog_Debug("max_tokens or value or use_custom_values not found in json");
	}
	return false;
}
bool Config_SetAI::Set_Max_Tokens(const unsigned int max_tokens, bool use_max_tokens, bool use_append_max_tokens)
{
	if (max_tokens > 0 &&
		this->GetJsonFromConfigFile().contains("max_tokens") == true &&
		this->GetJsonFromConfigFile().at("max_tokens").contains("append_to_request") == true &&
		this->GetJsonFromConfigFile().at("max_tokens").contains("use_custom_values") == true &&
		this->GetJsonFromConfigFile().at("max_tokens").contains("value") == true)
	{
		this->GetJsonFromConfigFile()["max_tokens"]["value"] = max_tokens;
		this->GetJsonFromConfigFile()["max_tokens"]["use_custom_values"] = use_max_tokens;
		this->GetJsonFromConfigFile()["max_tokens"]["append_to_request"] = use_append_max_tokens;
		return true;
	}
	else
	{
		RuntimeLog_Debug("max_tokens or value or use_custom_values or append_to_request not found in json");
	}
	return false;
}
bool Config_SetAI::Set_Randomness_Temperature(double temperature, bool use)
{
	if (temperature < 0.0f || temperature > 2.0f)
	{
		RuntimeLog_Info("temperature out of range");
		temperature = 1.0f;
	}
	if (this->GetJsonFromConfigFile().contains("randomness") == true &&
		this->GetJsonFromConfigFile().at("randomness").contains("temperature") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("temperature").contains("value") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("temperature").contains("use") == true)
	{
		this->GetJsonFromConfigFile().at("randomness").at("temperature").at("value") = temperature;
		this->GetJsonFromConfigFile().at("randomness").at("temperature").at("use") = use;
		return true;
	}
	else
	{
		this->GetJsonFromConfigFile()["randomness"]["temperature"]["value"] = 1.0f;
		this->GetJsonFromConfigFile()["randomness"]["temperature"]["use"] = false;
		RuntimeLog_Debug("randomness/temperature/value or randomness/temperature/use not found in json");
		return false;
	}
	return false;
}
bool Config_SetAI::Set_Randomness_Temperature(double temperature, bool use, bool use_append_temperature)
{
	if (temperature < 0.0f || temperature > 2.0f)
	{
		RuntimeLog_Info("temperature out of range");
		temperature = 1.0f;
	}
	if (this->GetJsonFromConfigFile().contains("randomness") == true &&
		this->GetJsonFromConfigFile().at("randomness").contains("temperature") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("temperature").contains("use") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("temperature").contains("append_to_request") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("temperature").contains("value") == true)
	{
		this->GetJsonFromConfigFile().at("randomness").at("temperature").at("value") = temperature;
		this->GetJsonFromConfigFile().at("randomness").at("temperature").at("use") = use;
		this->GetJsonFromConfigFile().at("randomness").at("temperature").at("append_to_request") = use_append_temperature;
		return true;
	}
	else
	{
		RuntimeLog_Debug("randomness/temperature/value or randomness/temperature/use or randomness/temperature/append_to_request not found in json");
		this->GetJsonFromConfigFile().at("randomness").at("temperature").at("value") = temperature;
		this->GetJsonFromConfigFile().at("randomness").at("temperature").at("use") = use;
		this->GetJsonFromConfigFile().at("randomness").at("temperature").at("append_to_request") = use_append_temperature;
		return false;
	}

	return false;
}
bool Config_SetAI::Set_Randomness_Top_p(double top_p, bool use)
{
	if (top_p < 0.0f || top_p > 2.0f)
	{
		RuntimeLog_Info("top_p out of range");
		top_p = 0.0f;
	}
	if (this->GetJsonFromConfigFile().contains("randomness") == true &&
		this->GetJsonFromConfigFile().at("randomness").contains("top_p") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("top_p").contains("value") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("top_p").contains("use") == true)
	{
		this->GetJsonFromConfigFile().at("randomness").at("top_p").at("value") = top_p;
		this->GetJsonFromConfigFile().at("randomness").at("top_p").at("use") = use;
		return true;
	}
	else
	{
		this->GetJsonFromConfigFile()["randomness"]["top_p"]["value"] = 0.0f;
		this->GetJsonFromConfigFile()["randomness"]["top_p"]["use"] = false;
		RuntimeLog_Debug("randomness/top_p/value or randomness/top_p/use not found in json");
		return false;
	}
	return false;
}
bool Config_SetAI::Set_Randomness_Top_p(double top_p, bool use, bool use_append_top_p)
{
	if (top_p < 0.0f || top_p > 2.0f)
	{
		RuntimeLog_Info("top_p out of range");
		top_p = 0.0f;
	}
	if (this->GetJsonFromConfigFile().contains("randomness") == true &&
		this->GetJsonFromConfigFile().at("randomness").contains("top_p") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("top_p").contains("value") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("top_p").contains("use") == true &&
		this->GetJsonFromConfigFile().at("randomness").at("top_p").contains("append_to_request") == true)
	{
		this->GetJsonFromConfigFile().at("randomness").at("top_p").at("value") = top_p;
		this->GetJsonFromConfigFile().at("randomness").at("top_p").at("use") = use;
		this->GetJsonFromConfigFile().at("randomness").at("top_p").at("append_to_request") = use_append_top_p;
		return true;
	}
	else
	{
		RuntimeLog_Debug("randomness/top_p/value or randomness/top_p/use or randomness/top_p/append_to_request not found in json");
		this->GetJsonFromConfigFile().at("randomness").at("top_p").at("value") = top_p;
		this->GetJsonFromConfigFile().at("randomness").at("top_p").at("use") = use;
		this->GetJsonFromConfigFile().at("randomness").at("top_p").at("append_to_request") = use_append_top_p;
		return false;
	}
	return false;
}
bool Config_SetAI::Set_frequency_penalty(double frequency_penalty, bool status)
{
	if (frequency_penalty < -2.0f || frequency_penalty > 2.0f)
	{
		RuntimeLog_Info("frequency_penalty out of range");
		frequency_penalty = 0.0f;
	}
	if (this->GetJsonFromConfigFile().contains("frequency_penalty") == true &&
		this->GetJsonFromConfigFile().at("frequency_penalty").contains("value") == true &&
		this->GetJsonFromConfigFile().at("frequency_penalty").contains("use") == true)
	{
		this->GetJsonFromConfigFile().at("frequency_penalty").at("value") = frequency_penalty;
		this->GetJsonFromConfigFile().at("frequency_penalty").at("use") = status;
		return true;
	}
	else
	{
		RuntimeLog_Debug("frequency_penalty not found in json");
		this->GetJsonFromConfigFile()["frequency_penalty"]["value"] = frequency_penalty;
		this->GetJsonFromConfigFile()["frequency_penalty"]["use"] = false;
		return false;
	}
	return false;
}
bool Config_SetAI::Set_frequency_penalty(double frequency_penalty, bool status, bool use_append_frequency_penalty)
{
	if (frequency_penalty < -2.0f || frequency_penalty > 2.0f)
	{
		RuntimeLog_Info("frequency_penalty out of range");
		frequency_penalty = 0.0f;
	}

	if (this->GetJsonFromConfigFile().contains("frequency_penalty") == true &&
		this->GetJsonFromConfigFile().at("frequency_penalty").contains("value") == true &&
		this->GetJsonFromConfigFile().at("frequency_penalty").contains("use") == true &&
		this->GetJsonFromConfigFile().at("frequency_penalty").contains("append_to_request") == true)
	{
		this->GetJsonFromConfigFile().at("frequency_penalty").at("value") = frequency_penalty;
		this->GetJsonFromConfigFile().at("frequency_penalty").at("use") = status;
		this->GetJsonFromConfigFile().at("frequency_penalty").at("append_to_request") = use_append_frequency_penalty;
		return true;
	}
	else
	{
		RuntimeLog_Debug("frequency_penalty not found in json");
		this->GetJsonFromConfigFile()["frequency_penalty"]["value"] = frequency_penalty;
		this->GetJsonFromConfigFile()["frequency_penalty"]["use"] = status;
		this->GetJsonFromConfigFile()["frequency_penalty"]["append_to_request"] = use_append_frequency_penalty;
		return false;
	}
	return false;
}
bool Config_SetAI::Set_presence_penalty(double presence_penalty, bool status)
{
	if (presence_penalty < -2.0f || presence_penalty > 2.0f)
	{
		RuntimeLog_Info("presence_penalty out of range");
		presence_penalty = 0.0;
	}
	if (this->GetJsonFromConfigFile().contains("presence_penalty") == true &&
		this->GetJsonFromConfigFile().at("presence_penalty").contains("value") == true &&
		this->GetJsonFromConfigFile().at("presence_penalty").contains("use") == true)
	{
		this->GetJsonFromConfigFile().at("presence_penalty").at("value") = presence_penalty;
		this->GetJsonFromConfigFile().at("presence_penalty").at("use") = status;
		return true;
	}
	else
	{
		RuntimeLog_Debug("presence_penalty not found in json");
		this->GetJsonFromConfigFile()["presence_penalty"]["value"] = presence_penalty;
		this->GetJsonFromConfigFile()["presence_penalty"]["use"] = false;
		return false;
	}
	return false;
}
bool Config_SetAI::Set_presence_penalty(double presence_penalty, bool status, bool use_append_presence_penalty)
{
	if (presence_penalty < -2.0f || presence_penalty > 2.0f)
	{
		RuntimeLog_Debug("presence_penalty out of range");
		presence_penalty = 0.0;
	}
	if (this->GetJsonFromConfigFile().contains("presence_penalty") == true &&
		this->GetJsonFromConfigFile().at("presence_penalty").contains("value") == true &&
		this->GetJsonFromConfigFile().at("presence_penalty").contains("use") == true &&
		this->GetJsonFromConfigFile().at("presence_penalty").contains("append_to_request") == true)
	{
		this->GetJsonFromConfigFile().at("presence_penalty").at("value") = presence_penalty;
		this->GetJsonFromConfigFile().at("presence_penalty").at("use") = status;
		this->GetJsonFromConfigFile().at("presence_penalty").at("append_to_request") = use_append_presence_penalty;
		return true;
	}
	else
	{
		RuntimeLog_Debug("presence_penalty not found in json");
		this->GetJsonFromConfigFile()["presence_penalty"]["value"] = presence_penalty;
		this->GetJsonFromConfigFile()["presence_penalty"]["use"] = status;
		this->GetJsonFromConfigFile()["presence_penalty"]["append_to_request"] = use_append_presence_penalty;
		return false;
	}
	return false;
}
bool Config_SetAI::Set_UseCustomJson(bool status)
{
	if (this->GetJsonFromConfigFile().contains("cus_data") == true &&
		this->GetJsonFromConfigFile().at("cus_data").contains("custom_json") == true &&
		this->GetJsonFromConfigFile().at("cus_data").at("custom_json").contains("use_custom_json") == true)
	{
		this->GetJsonFromConfigFile().at("cus_data").at("custom_json").at("use_custom_json") = status;
		return true;
	}
	else
	{
		RuntimeLog_Debug("cus_data、custom_json or use_custom_json not found in json");
		this->GetJsonFromConfigFile().at("cus_data").at("custom_json").at("use_custom_json") = false;
		return false;
	}
	return false;
}
bool Config_SetAI::Set_CustomJson(std::string qstr_Json)
{
	if (this->GetJsonFromConfigFile().contains("cus_data") == true &&
		this->GetJsonFromConfigFile().at("cus_data").contains("custom_json") == true &&
		this->GetJsonFromConfigFile().at("cus_data").at("custom_json").contains("data") == true)
	{
		this->GetJsonFromConfigFile().at("cus_data").at("custom_json").at("data") = nlohmann::json::parse(qstr_Json);
		return true;
	}
	else
	{
		RuntimeLog_Debug("cus_data、custom_json or data not found in json");
		this->GetJsonFromConfigFile().at("cus_data").at("custom_json").at("data") = "{}";
		return false;
	}
	return false;
}
bool Config_SetAI::Set_UseAppendCustomJson(bool status)
{
	if (this->GetJsonFromConfigFile().contains("cus_data") == true &&
		this->GetJsonFromConfigFile().at("cus_data").contains("append_json") == true &&
		this->GetJsonFromConfigFile().at("cus_data").at("append_json").contains("use_append_json") == true)
	{
		this->GetJsonFromConfigFile().at("cus_data").at("append_json").at("use_append_json") = status;
		return true;
	}
	else
	{
		RuntimeLog_Debug("cus_data、append_json or use_append_json not found in json");
		this->GetJsonFromConfigFile().at("cus_data").at("append_json").at("use_append_json") = false;
	}
	return false;
}
bool Config_SetAI::Set_AppendCustomJson(std::string qstr_Json)
{
	if (this->GetJsonFromConfigFile().contains("cus_data") == true &&
		this->GetJsonFromConfigFile().at("cus_data").contains("append_json") == true &&
		this->GetJsonFromConfigFile().at("cus_data").at("append_json").contains("data") == true &&
		qstr_Json.empty() == false)
	{
		this->GetJsonFromConfigFile().at("cus_data").at("append_json").at("data") = nlohmann::json::parse(qstr_Json);
		return true;
	}
	else
	{
		RuntimeLog_Debug("cus_data、append_json or data not found in json");
		this->GetJsonFromConfigFile().at("cus_data").at("append_json").at("data") = "{}";
		return false;
	}
	return false;
}
// ========== end ==========