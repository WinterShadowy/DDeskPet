
/*
 ============================================================================
 Header: AIManager.h
 Description: AI操作管理器
 Start time: 2025.11.01
 End time: 2026.01.07
 Maintainer: WangSZ
 ============================================================================
*/
#pragma once

#define _ALL_AI_VERSION_V3_ 0L

#ifdef _ALL_AI_VERSION_V3_
#include "ALL-AI-V3.hpp"
#elif defined(_ALL_AI_VERSION_V2_)
#include "ALL-AI.hpp"
#endif

#include "json.hpp"
#include "LAppDefine.hpp"
#include "Configuration.h"

#include <QStringList>
#include <QString>
#include <string>
#include <QJsonObject>
#include <QJsonDocument>

#include <QMutex>

class AIManager {
public:
	enum AI_STATUS {
		AI_RUNNING = 0,
		AI_IDLE = 1
	};

	static AIManager& instance();
	
	// 初始化
	void InitAIManager(const QString qstr_url , const QString qstr_apiKey);
	void InitAIManager(const std::string& str_url, const std::string& str_apiKey);
	bool ReinitAIManager(const std::string & str_url, const std::string & str_apiKey);
	// 设置URL
	void SetURL(const QString& qstr_url);
	// 重新设置url
	void ResetURL(const QString& qstr_url);
	// 设置api-key
	void SetAPIKey(const QString& qstr_apiKey);
	// 重设api key
	void ResetAPIKey(const QString& qstr_apiKey);
	// 获取请求状态
	AI_STATUS GetRequestStatus();
	// 获取模型列表
	QStringList GetModels();
	// 获取回复
	std::string GetAIReply(const QString& qstr_question);
	QJsonObject GetAIJsonTemplate();
	nlohmann::json GetAIJsonTemplate_NJson();
private:
	AIManager();
	~AIManager();
	// 加载ALL_AI
	bool _Load_ALL_AI();
private:
	
	ALL_AI::AI m_ai;

	std::string m_url;
	std::string m_api_key;

	AI_STATUS m_ai_status = AI_IDLE;
	QStringList m_list_models;

	bool m_is_init = false;

	QMutex m_mutex;
};