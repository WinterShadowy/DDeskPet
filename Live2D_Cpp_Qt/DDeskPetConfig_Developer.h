#pragma once

#include "DDeskPetConfigInterfaceBase.h"
#include "DDeskPetConfig_Base.h"

// ========== 开发者页面配置 ==========
class Config_Developer : virtual public Page_Config_Base {
public:
	Config_Developer() = default;
	Config_Developer(QString qstr_FilePath);
	~Config_Developer();

	static Config_Developer& instance();
	virtual void Initialize() override;

	// ===== NetService =====

	// 获取监听端口
	int GetListenPort();
	// 设置监听端口
	void SetListenPort(int port);

	// 获取启动是否使用NetService - 是否启动子进程
	bool GetUseNetService();
	// 启动时是否监听端口 - 是否启动监听
	bool GetUseListenPort();
	// 设置启动时不适用NetService
	void SetUseNetService(bool status);
	// 设置启动时是否监听端口
	void SetUseListenPort(bool status);

	nlohmann::json GetMessageTemplate(int index);

	// ===== end =====

	// ===== AI =====

	// 获取AI是否启动
	bool GetUseAI();
	// 设置AI是否启动
	void SetUseAI(bool status);

	// 获取是否使用最小模板
	bool GetUseMinTemplate();
	// 设置是否使用最小模板
	void SetUseMinTemplate(bool status);

	// ===== end =====

	// ===== BaseFunctions =====

	// 获取是否使用CPU占用 - 强制
	bool GetUseCPUUsage();
	// 设置是否使用CPU占用
	void SetUseCPUUsage(bool status);

	// 获取是否使用GPU占用 - 强制
	bool GetUseGPUUsage();
	// 设置是否使用GPU占用
	void SetUseGPUUsage(bool status);

	// 获取是否使用内存占用 - 强制
	bool GetUseMemoryUsage();
	// 设置是否使用内存占用
	void SetUseMemoryUsage(bool status);

	// ===== end =====

	// ===== ChatHistory =====

	// 获取是否加载聊天记录页面
	bool GetUseChatHistory();
	// 设置是否加载聊天记录页面
	void SetUseChatHistory(bool status);

	// ===== end =====

	// ===== models ======

	// 获取是否加载模型页面
	bool GetUseModels();
	// 设置是否加载模型页面
	void SetUseModels(bool status);

	// 获取是否加载模型
	bool GetLoadModels();
	// 设置是否加载模型
	void SetLoadModels(bool status);

	// ===== end =====

	// ==== Plugins =====

	// 获取是否加载插件页面
	bool GetUsePlugins();
	// 设置是否加载插件页面
	void SetUsePlugins(bool status);

	// 获取是否加载插件
	bool GetLoadPlugins();
	// 设置是否加载插件
	void SetLoadPlugins(bool status);

	// 设置是否自动启动插件 - 对所有插件生效
	bool GetAutoStartPlugins();
	// 设置是否自动启动插件
	void SetAutoStartPlugins(bool status);

	// ===== end =====
};
// =========== end ===========