#pragma once

#include "DDeskPetConfigInterfaceBase.h"
#include "DDeskPetConfig_Base.h"

// 模型页面配置
class Config_Model : virtual public Page_Config_Base {
public:
	Config_Model() = default;
	Config_Model(QString qstr_FilePath);
	~Config_Model();

	// 获取展示的模型路径
	QString GetDisplayModelPath();
	// 获取展示的模型的名称
	QString GetDisplayModelName();
	// 设置展示的模型
	void SetDisplayModelInfo(const QString& modelPath, const QString& modelName);
	// 获取图标 - Folder
	QString GetIcon_Folder();
	// 获取图标 - File
	QString GetIcon_File();

	static Config_Model& instance();
	virtual void Initialize() override;
};
// =========== end ==========