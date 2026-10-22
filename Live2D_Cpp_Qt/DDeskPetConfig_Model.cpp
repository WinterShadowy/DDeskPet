#include "DDeskPetConfig_Model.h"

// ========== Config_Model ==========
Config_Model::Config_Model(QString qstr_FilePath) :
	Page_Config_Base(qstr_FilePath)
{
}

Config_Model::~Config_Model()
{
}

QString Config_Model::GetDisplayModelPath()
{
	if (this->GetJsonFromConfigFile().contains("model_folder_name") == true)
	{
		QString path = QDir::toNativeSeparators(BaseConfig::instance().GetModelFolderPath() +
			QString::fromStdString(this->GetJsonFromConfigFile().at("model_folder_name").get<std::string>()) + "/");
		return path;
	}
	return QString();
}

QString Config_Model::GetDisplayModelName()
{
	if (this->GetJsonFromConfigFile().contains("model_json_name") == true)
	{
		return QString::fromStdString(this->GetJsonFromConfigFile().at("model_json_name").get<std::string>());
	}
	return QString();
}

void Config_Model::SetDisplayModelInfo(const QString& modelPath, const QString& modelName)
{
	// 设置展示的模型文件夹名
	if (this->GetJsonFromConfigFile().contains("model_folder_name") == true)
	{
		this->GetJsonFromConfigFile()["model_folder_name"] = modelPath.toStdString();
	}
	else
	{
		RuntimeLog_Debug("model_folder_name not found in json");
	}
	// 设置展示的模型文件名
	if (this->GetJsonFromConfigFile().contains("model_json_name") == true)
	{
		this->GetJsonFromConfigFile().at("model_json_name") = std::string(modelName.toStdString());
	}
	else
	{
		RuntimeLog_Debug("model_json_name not found in json");
	}
	return;
}

QString Config_Model::GetIcon_Folder()
{
	if (this->GetJsonFromConfigFile().contains("imgs_file_name") == false)
	{
		RuntimeLog_Debug("imgs_file_name not found in json");
		return QString();

	}
	if (this->GetJsonFromConfigFile().at("imgs_file_name").contains("Folder") == false)
	{
		RuntimeLog_Debug("Folder not found in json");
		return QString();
	}
	return QDir::toNativeSeparators(QString(BaseConfig::instance().GetImgFilePath() +
		QString::fromStdString(this->GetJsonFromConfigFile().at("imgs_file_name").at("Folder").get<std::string>())));
}

QString Config_Model::GetIcon_File()
{
	if (this->GetJsonFromConfigFile().contains("imgs_file_name") == false)
	{
		RuntimeLog_Debug("imgs_file_name not found in json");
		return QString("");
	}
	if (this->GetJsonFromConfigFile().at("imgs_file_name").contains("File") == false)
	{
		RuntimeLog_Debug("File not found in json");
		return QString("");
	}
	return QDir::toNativeSeparators(QString(BaseConfig::instance().GetImgFilePath() +
		QString::fromStdString(this->GetJsonFromConfigFile().at("imgs_file_name").at("File").get<std::string>())));
}

Config_Model& Config_Model::instance()
{
	static Config_Model ins(ConfigurationBase::instance().GetModelPageConfigFilePath());
	return ins;
}

void Config_Model::Initialize()
{
	this->InitInstanceData();
	return;
}
// ========== end ==========