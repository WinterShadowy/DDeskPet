#include "DDeskPetConfigInterfaceBase.h"

// ========== Config_SetAI_Base ==========
Page_Config_Base::Page_Config_Base(std::string& str_FilePath)
{
	this->_m_qstr_ConfigFilePath = QString::fromStdString(str_FilePath);
	//ReadConfigFromJson(this->_m_qstr_ConfigFilePath);
}

Page_Config_Base::Page_Config_Base(QString qstr_FilePath)
{
	this->_m_qstr_ConfigFilePath = qstr_FilePath;
	//ReadConfigFromJson(this->_m_qstr_ConfigFilePath);
}

Page_Config_Base::~Page_Config_Base()
{
	WriteConfigToJson(this->_m_qstr_ConfigFilePath);
}

void Page_Config_Base::InitInstanceData()
{
	// 防止重复初始化
	if (this->IsInit)
		return;
	ReadConfigFromJson(this->_m_qstr_ConfigFilePath);
	this->IsInit = true;
	return;
}

void Page_Config_Base::Debug_PrintJson(int nIndex)
{
	qDebug() << "Debug_PrintJson:" << this->_m_json_config.dump(nIndex).c_str() << Qt::endl;
	return;
}

void Page_Config_Base::ReadConfigFromJson(const QString& qstr_ConfigFilePath)
{
	QString qstr_FilePathBuffer = QDir(QDir::toNativeSeparators(qstr_ConfigFilePath)).absolutePath();
	if (qstr_ConfigFilePath.isEmpty())
	{
		RuntimeLog_Warn("Empty config path");
		return;
	}
	QFile file(qstr_FilePathBuffer);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		RuntimeLog_Warn(("Failed to open the target configuration file. Path:" + qstr_FilePathBuffer).toStdString());
		return;
	}
	try {
		QByteArray byteArray = file.readAll();
		file.close();
		std::string str_Json = byteArray.toStdString();
		this->_m_json_config = nlohmann::json::parse(str_Json);
	}
	catch (nlohmann::json::parse_error& e)
	{
		RuntimeLog_Fatal((std::string("Failed to open the target configuration file. details: ") + e.what()));
	}
	return;
}

void Page_Config_Base::WriteConfigToJson(const QString& qstr_ConfigFilePath)
{
	QString qstr_FilePathBuffer = QDir::toNativeSeparators(qstr_ConfigFilePath);
	QFile file(qstr_FilePathBuffer);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		std::string str_err_msg = ("Failed to open the target configuration file. details: details: " + qstr_ConfigFilePath).toStdString();
		RuntimeLog_Warn(str_err_msg);
		return;
	}

	// 将 JSON 对象转为字符串
	QByteArray jsonString = QString::fromStdString(this->_m_json_config.dump(4)).toUtf8(); // 4表示缩进空格数

	// 写入文件
	file.write(jsonString);
	file.close();
	return;
}

void Page_Config_Base::WirteConfigToFile()
{
	WriteConfigToJson(this->_m_qstr_ConfigFilePath);
}

void Page_Config_Base::SetInitStatus(bool status)
{
	this->IsInit = status;
}

void Page_Config_Base::SetJson_BaseClass(nlohmann::json _json)
{
	this->_m_json_config = _json;
}

QString Page_Config_Base::GetConfigFilePath()
{
	return this->_m_qstr_ConfigFilePath;
}

bool Page_Config_Base::GetInitStatus()
{
	return this->IsInit;
}

nlohmann::json& Page_Config_Base::GetJsonFromConfigFile()
{
	return this->_m_json_config;
}

// ========== end ==========

// ========= Window_Config_Base ==========
Window_Config_Base::Window_Config_Base(QString qstr_FilePath) :
	_m_qstr_ConfigFilePath(qstr_FilePath)
{
}

Window_Config_Base::~Window_Config_Base()
{
	WriteConfigToJson(this->_m_qstr_ConfigFilePath);
}

QString Window_Config_Base::GetConfigFilePath()
{
	return this->_m_qstr_ConfigFilePath;
}

nlohmann::json& Window_Config_Base::GetJsonFromConfigFile()
{
	return this->_m_json_config;
}

bool Window_Config_Base::ReadConfigFromJson(const QString& qstr_ConfigFilePath)
{
	qDebug() << "Window_Config_Base::ReadConfigFromJson:" << qstr_ConfigFilePath;
	nlohmann::json jsonBuffer = _FIO::ReadFilleFromJson(qstr_ConfigFilePath);
	if (jsonBuffer.contains("Error") == true)
	{
		return false;
	}
	this->_m_json_config = jsonBuffer;
	//qDebug() << "JsonInfo: " << this->_m_json_config.dump(2);
	return true;
}
void Window_Config_Base::WriteConfigToJson(const QString& qstr_ConfigFilePath)
{
	QString qstr_FilePathBuffer = QDir::toNativeSeparators(qstr_ConfigFilePath);
	QFile file(qstr_FilePathBuffer);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		return;
	}

	// 将 JSON 对象转为字符串
	QByteArray jsonString = QString::fromStdString(this->_m_json_config.dump(4)).toUtf8(); // 4表示缩进空格数

	// 写入文件
	file.write(jsonString);
	file.close();
	return;
}
// ========== end ==========


