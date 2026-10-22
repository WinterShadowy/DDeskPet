
/*
 ============================================================================
 Header: ServerAI.h
 Description: 服务器AI模块
 Start time: 2025.11.01
 End time: 2026.01.07
 Maintainer: WangSZ
 ============================================================================
*/

#pragma once

#include <QComBoBox>
#include <QLabel>
#include <QThread>
#include <QString>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QPointer>
#include <QMetaObject>

#include "AIManager.h"
#include "LAppDefine.hpp"
#include "Configuration.h"

class ServerAI
{
public:
	ServerAI();
	~ServerAI();

	// 获取单例
	static ServerAI& instance();

    // 更新AI页ComboBox中的内容
	void UpdateSetAIPage_ModelsComBoboxInfo(QComboBox* comBobox_Models, QLabel* qlabel_updatestatus);

	bool ReloadAIConfig(std::string str_url_path, std::string str_api_key);
private:
	// 从服务器获取模型列表
	QStringList GetModelsFromServer();
	
	QStringList GetModelsFromServerV3();
private:
	
};