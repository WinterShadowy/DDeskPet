#include "AIManager.h"
#include <QMutexLocker>


AIManager& AIManager::instance()
{
	static AIManager ins;
	return ins;
}

void AIManager::InitAIManager(const QString qstr_url, const QString qstr_apiKey)
{
	QMutexLocker locker(&m_mutex);
	if (qstr_url.isEmpty() || qstr_apiKey.isEmpty())
	{
		RuntimeLog_Debug("url or apiKey is empty");
		return;
	}
	this->m_url = qstr_url.toStdString();
	this->m_api_key = qstr_apiKey.toStdString();
#ifdef _ALL_AI_VERSION_V3_
	this->m_ai.SetKey(this->m_api_key);
	this->m_ai.SetURL(this->m_url);
	this->m_ai.SetErrorThrow(ALL_AI::ALL_AI_ErrorThrow::ALL_AI_NO_ERROR_THROW);
	this->m_ai.SetHttpTransport(std::make_shared<ALL_AI::HttpTransport::CurlHttpTransport>());
	if (this->m_ai.InitAI())
	{
		RuntimeLog_Info("AIManager init success");
	}
	else
	{
		RuntimeLog_Error("AIManager init fail");
	}
#elif _ALL_AI_VERSION_V2_
	this->m_ai.InitAI(this->m_url, this->m_api_key);
#endif
	return;
}

void AIManager::InitAIManager(const std::string& str_url, const std::string& str_apiKey)
{
	QMutexLocker locker(&m_mutex);
	if (str_url.empty() || str_apiKey.empty())
	{
		RuntimeLog_Info("url or apiKey is empty");
		return;
	}
#ifdef _ALL_AI_VERSION_V3_
	this->m_api_key = str_apiKey;
	this->m_url = str_url;
	_Load_ALL_AI();
#elif defined(_ALL_AI_VERSION_V2_)
	this->m_url = str_url;
	this->m_api_key = str_apiKey;
	this->m_ai.InitAI(str_url, str_apiKey);
#endif
	return;
}

bool AIManager::ReinitAIManager(const std::string& str_url, const std::string& str_apiKey)
{
	QMutexLocker locker(&m_mutex);
	if (str_url.empty() || str_apiKey.empty())
	{
		RuntimeLog_Debug("url or apiKey is empty");
		return false;
	}
#ifdef _ALL_AI_VERSION_V3_
	this->m_url = str_url;
	this->m_api_key = str_apiKey;
	return _Load_ALL_AI();
#elif defined(_ALL_AI_VERSION_V2_)
	this->m_ai.Reinitialize(this->m_url, this->m_api_key);
#endif
	return false;
}

void AIManager::SetURL(const QString& qstr_url)
{
	QMutexLocker locker(&m_mutex);
	this->m_url = qstr_url.toStdString();
	return;
}

void AIManager::ResetURL(const QString& qstr_url)
{
	QMutexLocker locker(&m_mutex);
	this->m_url = qstr_url.toStdString();
#ifdef _ALL_AI_VERSION_V3_
	this->m_ai.SetURL(this->m_url);
#elif defined(_ALL_AI_VERSION_V2_)
	this->m_ai.Reinitialize(this->m_url, this->m_api_key);
#endif
	return;
}

void AIManager::SetAPIKey(const QString& qstr_apiKey)
{
	QMutexLocker locker(&m_mutex);
	this->m_api_key = qstr_apiKey.toStdString();
	return;
}

void AIManager::ResetAPIKey(const QString& qstr_apiKey)
{
	QMutexLocker locker(&m_mutex);
	this->m_api_key = qstr_apiKey.toStdString();
#ifdef _ALL_AI_VERSION_V3_
	this->m_ai.ReloadAI(this->m_url, this->m_api_key, std::make_shared<ALL_AI::HttpTransport::CurlHttpTransport>());
#elif defined(_ALL_AI_VERSION_V2_)
	this->m_ai.Reinitialize(this->m_url, this->m_api_key);
#endif
}

AIManager::AI_STATUS AIManager::GetRequestStatus()
{
	QMutexLocker locker(&m_mutex);
	return this->m_ai_status;
}

QStringList AIManager::GetModels()
{
	QMutexLocker locker(&m_mutex);

	qDebug() << "[debug] init: " << this->m_is_init;

	nlohmann::json json_buffer;
#ifdef _ALL_AI_VERSION_V3_
	json_buffer = this->m_ai.SendRequestFromBuilder_Get();
#elif defined(_ALL_AI_VERSION_V2_)
	json_buffer = this->m_ai.SendRequest(this->m_ai.Set_ModelList()->GetRequestBuilderJson(), AI_MODEL_LIST);
#endif
	std::string _buffer_info = json_buffer.dump(2);
	qDebug() << "[debug] json_buffer_info:" << _buffer_info.c_str();
	this->m_list_models.clear();
	if (json_buffer.contains("data"))
	{
		for (auto& buffer : json_buffer["data"])
		{
			if (buffer.contains("id"))
			{
				std::string str = buffer.at("id").is_string() ? buffer.at("id").get<std::string>() : "";
				this->m_list_models.push_back(QString::fromStdString(str));
			}
		}
	}
	return this->m_list_models;
}

std::string AIManager::GetAIReply(const QString& qstr_question)
{
	QMutexLocker locker(&m_mutex);

	// 判断问题是否为空
	if (!qstr_question.isEmpty())
	{
#ifdef _ALL_AI_VERSION_V3_
		// 判断是否使用自定义json
		if (!Config_SetAI::instance().GetUseCustomJson())
		{
			// 已弃用
		}
		else
		{
			// 已弃用
		}
#elif defined(_ALL_AI_VERSION_V2_)
		if (!Config_SetAI::instance().GetUseCustomJson())
		{
			// 设置N
			this->m_ai.Set_Chat()->SetJson()->N(Config_SetAI::instance().Get_N());
			// 设置MaxTokens
			this->m_ai.Set_Chat()->SetJson()->MaxTokens(Config_SetAI::instance().Get_Max_Tokens());
			// 设置Model
			this->m_ai.Set_Chat()->SetJson()->Model(Config_SetAI::instance().Get_Model().toStdString());
			// 设置stream
			this->m_ai.Set_Chat()->SetJson()->Stream(false);
			// 设置提示 - prompt
			if (Config_SetAI::instance().IsPromptMode())
			{
				this->m_ai.Set_Chat()->SetJson()->AddCustomFieldToJson("prompt", Config_SetAI::instance().GetPrompt_Prompt().toStdString());
			}
			// 设提示 - system
			if (Config_SetAI::instance().IsSystemMode())
			{
				this->m_ai.Set_Chat()->SetJson()->Push_Back_Message("system", Config_SetAI::instance().GetPrompt_System().toStdString());
			}
			// 设置问题
			this->m_ai.Set_Chat()->SetJson()->Push_Back_Message("user", qstr_question.toStdString());
			// 设置temperature
			if (Config_SetAI::instance().IsTemperature())
			{
				this->m_ai.Set_Chat()->SetJson()->Temperature(Config_SetAI::instance().Get_Randomness_Temperature());
			}
			// 设置top_p
			if (Config_SetAI::instance().IsTop_p())
			{
				this->m_ai.Set_Chat()->SetJson()->TopP(Config_SetAI::instance().Get_Randomness_Top_p());
			}
			// 设置 stop
			if (Config_SetAI::instance().Get_Stop().empty() == false)
			{
				QSet<QString> qset_Stop = Config_SetAI::instance().Get_Stop();
				nlohmann::json::array_t json_array;
				for (auto& str : qset_Stop)
				{
					json_array.push_back(str.toStdString());
				}
				this->m_ai.Set_Chat()->SetJson()->Stop(json_array);
			}
			// 设置 FrequencyPenalty
			if (Config_SetAI::instance().Use_Frequency_Penalty())
			{
				this->m_ai.Set_Chat()->SetJson()->FrequencyPenalty(Config_SetAI::instance().Get_Frequency_Penalty());
			}
			// 设置 PresencePenalty
			if (Config_SetAI::instance().Use_Presence_Penalty())
			{
				this->m_ai.Set_Chat()->SetJson()->PresencePenalty(Config_SetAI::instance().Get_Presence_Penalty());
			}
		}
		else  // 使用自定义的json
		{
			this->m_ai.Set_Chat()->SetJson()->CustomJson(Config_SetAI::instance().GetCustomJson());
			this->m_ai.Set_Chat()->SetJson()->Push_Back_Message("user", qstr_question.toStdString());
			this->m_ai.Set_Chat()->SetJson()->Stream(false);
		}
		return this->m_ai.Set_Chat()->GetReply().GetContent().Reply<std::string>();
#endif
	}
	return "";
}

QJsonObject AIManager::GetAIJsonTemplate()
{
	QJsonObject jsonBuffer;
	if (!Config_SetAI::instance().GetUseCustomJson())
	{
		// 基本字段：N, max_tokens, model, stream
		jsonBuffer.insert("n", QJsonValue::fromVariant(Config_SetAI::instance().Get_N()));
		jsonBuffer.insert("max_tokens", QJsonValue::fromVariant(Config_SetAI::instance().Get_Max_Tokens()));
		jsonBuffer.insert("model", QJsonValue(Config_SetAI::instance().Get_Model()));
		jsonBuffer.insert("stream", QJsonValue(false));
		jsonBuffer.insert("url", Config_SetAI::instance().GetUrl_FullPath_Chat());
		jsonBuffer.insert("api_key", Config_SetAI::instance().Get_Api_Key());
		// messages 数组（按顺序推入 system/user）
		QJsonArray messages;
		if (Config_SetAI::instance().IsSystemMode())
		{
			QJsonObject sysObj;
			sysObj.insert("role", "system");
			sysObj.insert("content", Config_SetAI::instance().GetPrompt_System());
			messages.append(sysObj);
		}
		jsonBuffer.insert("messages", messages);

		// 可选参数：temperature, top_p
		if (false && Config_SetAI::instance().IsTemperature())
			jsonBuffer.insert("temperature",
				QJsonValue::fromVariant(Config_SetAI::instance().Get_Randomness_Temperature()));

		if (false && Config_SetAI::instance().IsTop_p())
			jsonBuffer.insert("top_p",
				QJsonValue::fromVariant(Config_SetAI::instance().Get_Randomness_Top_p()));

		// stop 数组（来自 QSet<QString>）
		if (!Config_SetAI::instance().Get_Stop().empty())
		{
			QSet<QString> qset_Stop = Config_SetAI::instance().Get_Stop();
			QJsonArray stopArr;
			for (const QString& s : qset_Stop)
				stopArr.append(QJsonValue(s));
			jsonBuffer.insert("stop", stopArr);
		}

		// frequency_penalty / presence_penalty
		if (Config_SetAI::instance().Use_Frequency_Penalty())
			jsonBuffer.insert("frequency_penalty",
				QJsonValue::fromVariant(Config_SetAI::instance().Get_Frequency_Penalty()));

		if (Config_SetAI::instance().Use_Presence_Penalty())
			jsonBuffer.insert("presence_penalty",
				QJsonValue::fromVariant(Config_SetAI::instance().Get_Presence_Penalty()));
	}
	else
	{
		// 使用自定义 JSON 字符串：尝试解析成 QJsonObject，然后在其上附加 user 消息与 stream=false
		QString custom = QString::fromStdString(Config_SetAI::instance().GetCustomJson().dump()); // 假定返回 QString 包含 JSON 文本
		QJsonDocument doc = QJsonDocument::fromJson(custom.toUtf8());
		if (!doc.isNull() && doc.isObject()) 
		{
			jsonBuffer = doc.object();
		}
		else 
		{
			// 解析失败：仍返回基本结构或记录日志
			jsonBuffer = QJsonObject();
		}
	}
	jsonBuffer.insert("stream", QJsonValue(false));
	qDebug() << QJsonDocument(jsonBuffer).toJson(QJsonDocument::Compact);
	return QJsonObject({ {"type", "template"}, {"body", jsonBuffer} });
}

nlohmann::json AIManager::GetAIJsonTemplate_NJson()
{
	nlohmann::json jsonBuffer;
	if (!Config_SetAI::instance().GetUseCustomJson())
	{
		// 基本字段：N, max_tokens, model, stream
		jsonBuffer["N"] = Config_SetAI::instance().Get_N();
		jsonBuffer["max_tokens"] = Config_SetAI::instance().Get_Max_Tokens();
		jsonBuffer["model"] = Config_SetAI::instance().Get_Model().toStdString();
		jsonBuffer["stream"] = false;
		jsonBuffer["url"] = Config_SetAI::instance().GetUrl_FullPath_Chat().toStdString();
		jsonBuffer["api_key"] = Config_SetAI::instance().Get_Api_Key().toStdString();

		// messages 数组（按顺序推入 system/user）
		nlohmann::json::array_t arrayMessages;

		if (Config_SetAI::instance().IsSystemMode())
		{
			arrayMessages.push_back({ "role", "system" });
			arrayMessages.push_back({ "content", Config_SetAI::instance().GetPrompt_System().toStdString()});
		}
		jsonBuffer["messages"] = arrayMessages;

		// 可选参数：temperature, top_p
		if (Config_SetAI::instance().IsTemperature())
			jsonBuffer["temperature"] = Config_SetAI::instance().Get_Randomness_Temperature();

		if (Config_SetAI::instance().IsTop_p())
			jsonBuffer["top_p"] = Config_SetAI::instance().Get_Randomness_Top_p();

		// stop 数组（来自 QSet<QString>）
		if (!Config_SetAI::instance().Get_Stop().empty())
		{
			QSet<QString> qset_Stop = Config_SetAI::instance().Get_Stop();
			nlohmann::json::array_t stopArr;
			for (const QString& s : qset_Stop)
				stopArr.push_back(s.toStdString());
			jsonBuffer["stop"] = stopArr;
		}

		// frequency_penalty / presence_penalty
		if (Config_SetAI::instance().Use_Frequency_Penalty())
			jsonBuffer["frequency_penalty"] = Config_SetAI::instance().Get_Frequency_Penalty();

		if (Config_SetAI::instance().Use_Presence_Penalty())
			jsonBuffer["presence_penalty"] = Config_SetAI::instance().Get_Presence_Penalty();
	}
	else
	{
		// 使用自定义 JSON 字符串：尝试解析成 QJsonObject，然后在其上附加 user 消息与 stream=false
	    jsonBuffer = Config_SetAI::instance().GetCustomJson(); // 假定返回 QString 包含 JSON 文本

		jsonBuffer["stream"] = false;
	}

	return nlohmann::json({ {"type", "template"}, {"body", jsonBuffer} });
}

AIManager::AIManager()
{
}

AIManager::~AIManager()
{
}

bool AIManager::_Load_ALL_AI()
{
	// 如何已经初始化过，则使用重新初始化
	this->m_ai.SetKey(this->m_api_key);
	this->m_ai.SetURL(this->m_url);
	this->m_ai.SetHttpTransport(std::make_shared<ALL_AI::HttpTransport::CurlHttpTransport>());
	this->m_ai.SetErrorThrow(ALL_AI::ALL_AI_ErrorThrow::ALL_AI_NO_ERROR_THROW);
	if(this->m_is_init == false)
	{
		bool bSuccess = this->m_ai.InitAI();
		if (bSuccess)
		{
			RuntimeLog_Info("AIManager init success");
		}
		else
		{
			RuntimeLog_Error("AIManager init fail");
		}
		this->m_is_init = true;
		return bSuccess;
	}
	else
	{
		bool bSuccess = this->m_ai.ReloadAI();
		if (bSuccess)
		{
			RuntimeLog_Info("AIManager Reinit success");
		}
		else
		{
			RuntimeLog_Error("AIManager Reinit fail");
		}
		return bSuccess;
	}
}
