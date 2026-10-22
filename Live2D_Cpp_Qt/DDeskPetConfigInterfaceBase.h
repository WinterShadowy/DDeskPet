#pragma once

#include "LAppDefine.hpp"
#include "LAppFuncDefine.hpp"
#include "json.hpp"

#include <QWidget>
#include <QDir>
#include <QFile>
#include <QString>
#include <string>
#include <iostream>
#include <fstream>
#include <QJsonArray>



// 配置文件基类 构造函数会读取配置文件
// 读取到的配置文件会保存在json对象中
// 析构时会将json对象写入配置文件
class Page_Config_Base {
public:
	Page_Config_Base() = default;
	Page_Config_Base(std::string& str_FilePath);
	Page_Config_Base(QString qstr_FilePath);
	~Page_Config_Base();

	void InitInstanceData();
	void SetInitStatus(bool status);

	QString GetConfigFilePath();
	bool GetInitStatus();

	nlohmann::json& GetJsonFromConfigFile();

	virtual void Initialize() = 0;

	void WirteConfigToFile();
protected:
	void Debug_PrintJson(int nIndent = 2);
	void ReadConfigFromJson(const QString& qstr_ConfigFilePath);
	void WriteConfigToJson(const QString& qstr_ConfigFilePath);

	void SetJson_BaseClass(nlohmann::json _json);
private:
	nlohmann::json _m_json_config;
	QString _m_qstr_ConfigFilePath;
	bool IsInit = false;
};
// ========== end ==========

// ========== 窗口配置基类 ==========
class Window_Config_Base {
public:
	Window_Config_Base() = default;
	Window_Config_Base(QString qstr_FilePath);
	~Window_Config_Base();

	// 获取配置文件路径 - 绝对路径
	QString GetConfigFilePath();
	nlohmann::json& GetJsonFromConfigFile();

	virtual bool Initialize() = 0;

protected:
	bool ReadConfigFromJson(const QString& qstr_ConfigFilePath);
	void WriteConfigToJson(const QString& qstr_ConfigFilePath);
private:
	nlohmann::json _m_json_config;
	QString _m_qstr_ConfigFilePath;
};
// ========== end ==========



