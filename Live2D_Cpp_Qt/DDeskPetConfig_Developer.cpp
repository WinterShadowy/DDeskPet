#include "DDeskPetConfig_Developer.h"

// ========= Config_Developer ========== Config_Developer ========= Config_Developer ========= Config_Developer =========
Config_Developer::Config_Developer(QString qstr_FilePath) :
	Page_Config_Base(qstr_FilePath)
{
}

Config_Developer::~Config_Developer()
{
}

/*
 ============================================================================
 Function: instance
 Description: 获取一个Config_Developer单例
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个Config_Developer单例的引用
 ============================================================================
*/
Config_Developer& Config_Developer::instance()
{
	static Config_Developer ins(ConfigurationBase::instance().GetDeveloperPageConfigFilePath());
	return ins;
}

/*
 ============================================================================
 Function: Initialize
 Description: 初始化单例数据
 Parameters:
	 - 无参数: 无释义
 Return: 无
 ============================================================================
*/
void Config_Developer::Initialize()
{
	this->InitInstanceData();
	//Debug_PrintJson();
	return;
}

/*
 ============================================================================
 Function: GetListenPort
 Description: 获取监听端口
 Parameters:
	 - 无参数: 无释义
 Return: 返回监听端口
 ============================================================================
*/
int Config_Developer::GetListenPort()
{
	if (this->GetJsonFromConfigFile().contains("NetService") == true)
	{
		nlohmann::json _json = this->GetJsonFromConfigFile().at("NetService");
		if (_json.contains("listen_port") == true)
		{
			return _json.at("listen_port").get<int>();
		}
		else
		{
			RuntimeLog_Debug("listen_port not found in json");
			return 19072;
		}
	}
	else
	{
		RuntimeLog_Debug("NetService not found in json");
		return 19724;
	}
	return 19724;
}

/*
 ============================================================================
 Function: SetListenPort
 Description: 设置监听端口
 Parameters:
	 - int: 监听端口
 Return: 无
 ============================================================================
*/
void Config_Developer::SetListenPort(int port)
{
	if (this->GetJsonFromConfigFile().contains("NetService") == true)
	{
		nlohmann::json& _json = this->GetJsonFromConfigFile().at("NetService");

		if (_json.contains("listen_port") == true)
		{
			_json.at("listen_port") = port;
		}
		else
		{
			RuntimeLog_Debug("listen_port not found in json");
			_json["listen_port"] = port;
		}
	}
	else
	{
		RuntimeLog_Debug("NetService not found in json");
		this->GetJsonFromConfigFile()["NetService"]["listen_port"] = port;
	}
}

/*
 ============================================================================
 Function: GetUseNetService
 Description: 获取是否使用网络服务
 Parameters:
	 - 无参数: 无释义
 Return: 无
 ============================================================================
*/
bool Config_Developer::GetUseNetService()
{
	if (this->GetJsonFromConfigFile().contains("NetService") == true)
	{
		nlohmann::json _json = this->GetJsonFromConfigFile().at("NetService");
		if (_json.contains("no_net_service") == true)
		{
			return !_json.at("no_net_service").get<bool>();
		}
		else
		{
			RuntimeLog_Debug("no_net_service not found in json");
			return true;
		}
	}
	else
	{
		RuntimeLog_Debug("NetService not found in json");
		return false;
	}
	return false;
}

/*
 ============================================================================
 Function: GetUseListenPort
 Description: 获取是否使用监听端口
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个bool，true为使用，false为不使用
 ============================================================================
*/
bool Config_Developer::GetUseListenPort()
{
	if (this->GetJsonFromConfigFile().contains("NetService") == true)
	{
		nlohmann::json _json = this->GetJsonFromConfigFile().at("NetService");
		if (_json.contains("no_listen_port") == true)
		{
			return !_json.at("no_listen_port").get<bool>();
		}
		else
		{
			RuntimeLog_Debug("no_listen_port not found in json");
			return true;
		}
	}
	else
	{
		RuntimeLog_Debug("NetService not found in json");
		return false;
	}
	return false;
}

/*
 ============================================================================
 Function: SetUseNetService
 Description: 设置是否使用网络服务
 Parameters:
	 - bool: 是否使用网络服务
 Return: 无
 ============================================================================
*/
void Config_Developer::SetUseNetService(bool status)
{
	if (this->GetJsonFromConfigFile().contains("NetService") == true)
	{
		nlohmann::json& _json = this->GetJsonFromConfigFile().at("NetService");
		if (_json.contains("no_net_service") == true)
		{
			_json.at("no_net_service") = status;
		}
		else
		{
			RuntimeLog_Debug("no_net_service not found in json");
			_json["no_net_service"] = status;
		}
	}
	else
	{
		RuntimeLog_Debug("NetService not found in json");
		this->GetJsonFromConfigFile()["NetService"]["no_net_service"] = status;
	}
	return;
}

/*
 ============================================================================
 Function: SetUseListenPort
 Description: 设置是否使用监听端口
 Parameters:
	 - bool: 是否使用监听端口
 Return: 无
 ============================================================================
*/
void Config_Developer::SetUseListenPort(bool status)
{
	if (this->GetJsonFromConfigFile().contains("NetService") == true)
	{
		nlohmann::json& _json = this->GetJsonFromConfigFile().at("NetService");
		if (_json.contains("no_listen_port") == true)
		{
			_json.at("no_listen_port") = status;
		}
		else
		{
			RuntimeLog_Debug("no_listen_port not found in json");
			_json["no_listen_port"] = status;
		}
	}
	else
	{
		RuntimeLog_Debug("NetService not found in json");
		this->GetJsonFromConfigFile()["NetService"]["no_listen_port"] = status;
	}
	return;
}

nlohmann::json Config_Developer::GetMessageTemplate(int index)
{
	if (this->GetJsonFromConfigFile().contains("message_template") == true &&
		this->GetJsonFromConfigFile().at("message_template").contains("message_temp") == true)
	{
		auto& arr = this->GetJsonFromConfigFile().at("message_template").at("message_temp");
		if (index >= arr.size())
			return {};
		return arr[index];
	}
	return nlohmann::json();
}

/*
 ============================================================================
 Function: GetUseAI
 Description: 获取是否使用AI
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个bool，true为使用，false为不使用
 ============================================================================
*/
bool Config_Developer::GetUseAI()
{
	if (this->GetJsonFromConfigFile().contains("ai") == true &&
		this->GetJsonFromConfigFile().at("ai").contains("no_set_ai_page") == true)
	{
		return !this->GetJsonFromConfigFile().at("ai").at("no_set_ai_page").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("no_set_ai_page not found in json");
		return true;
	}
	return true;
}

/*
 ============================================================================
 Function: SetUseAI
 Description: 设置获取是否使用AI
 Parameters:
	 - bool: 是否使用AI
 Return: 无
 ============================================================================
*/
void Config_Developer::SetUseAI(bool status)
{
	if (this->GetJsonFromConfigFile().contains("ai") == true)
	{
		nlohmann::json& _json = this->GetJsonFromConfigFile().at("ai");
		if (_json.contains("no_set_ai_page") == true)
		{
			_json.at("no_set_ai_page") = status;
		}
		else
		{
			RuntimeLog_Debug("no_set_ai_page not found in json");
			_json["no_set_ai_page"] = status;
		}
	}
	else
	{
		RuntimeLog_Debug("ai not found in json");
		this->GetJsonFromConfigFile()["ai"]["no_set_ai_page"] = status;
	}
	return;
}

/*
 ============================================================================
 Function: GetUseMinTemplate
 Description: 获取是否使用AI最小模板
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个bool，true为使用，false为不使用
 ============================================================================
*/
bool Config_Developer::GetUseMinTemplate()
{
	if (this->GetJsonFromConfigFile().contains("ai") == true &&
		this->GetJsonFromConfigFile().at("ai").contains("minimum_request_template") == true)
	{
		return this->GetJsonFromConfigFile().at("ai").at("minimum_request_template").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("minimum_request_template not found in json");
		return false;
	}
	return false;
}

/*
 ============================================================================
 Function: SetUseMinTemplate
 Description: 设置是否使用AI最小模板
 Parameters:
	 - bool: 是否使用
 Return: 无
 ============================================================================
*/
void Config_Developer::SetUseMinTemplate(bool status)
{
	if (this->GetJsonFromConfigFile().contains("ai") == true)
	{
		nlohmann::json& _json = this->GetJsonFromConfigFile().at("ai");
		if (_json.contains("minimum_request_template") == true)
		{
			_json.at("minimum_request_template") = status;
		}
		else
		{
			RuntimeLog_Debug("minimum_request_template not found in json");
			_json["minimum_request_template"] = status;
		}
	}
	else
	{
		RuntimeLog_Debug("ai not found in json");
		this->GetJsonFromConfigFile()["ai"]["minimum_request_template"] = status;
	}
	return;
}

/*
 ============================================================================
 Function: GetUseCPUUsage
 Description: 获取是否使用CPU占用
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个bool，true为使用，false为不使用
 ============================================================================
*/
bool Config_Developer::GetUseCPUUsage()
{
	// TODO
	return false;
}

/*
 ============================================================================
 Function: SetUseCPUUsage
 Description: 设置是否使用CPU占用功能
 Parameters:
	 - bool: 是否使用
 Return: 无
 ============================================================================
*/
void Config_Developer::SetUseCPUUsage(bool status)
{
	// TODO
}

/*
 ============================================================================
 Function: GetUseGPUUsage
 Description: 获取是否使用GPU占用功能
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个bool，true为使用，false为不使用
 ============================================================================
*/
bool Config_Developer::GetUseGPUUsage()
{
	// TODO
	return false;
}

/*
 ============================================================================
 Function: SetUseGPUUsage
 Description: 设置是否使用GPU占用
 Parameters:
	 - bool: 是否使用
 Return: 无
 ============================================================================
*/
void Config_Developer::SetUseGPUUsage(bool status)
{
	// TODO
}

/*
 ============================================================================
 Function: GetUseMemoryUsage
 Description: 设置是否使用内存占用
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个bool，true为使用，false为不使用
 ============================================================================
*/
bool Config_Developer::GetUseMemoryUsage()
{
	// TODO
	return false;
}

/*
 ============================================================================
 Function: SetUseMemoryUsage
 Description: 设置是否使用内存占用
 Parameters:
	 - bool: 是否使用
 Return: 无
 ============================================================================
*/
void Config_Developer::SetUseMemoryUsage(bool status)
{
	// TODO
}

/*
 ============================================================================
 Function: GetUseChatHistory
 Description: 获取是否使用聊天记录界面
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个bool，true为使用，false为不使用
 ============================================================================
*/
bool Config_Developer::GetUseChatHistory()
{
	// 查询相关的字段是否存在
	if (this->GetJsonFromConfigFile().contains("chat_history") == true &&
		this->GetJsonFromConfigFile().at("chat_history").contains("no_load_chat_history") == true)
	{
		return !this->GetJsonFromConfigFile().at("chat_history").at("no_load_chat_history").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("no_load_chat_history or chat_history not found in json");
		return true;
	}
	return true;
}

/*
 ============================================================================
 Function: SetUseChatHistory
 Description: 设置是否使用聊天记录界面
 Parameters:
	 - bool: 是否使用
 Return: 无
 ============================================================================
*/
void Config_Developer::SetUseChatHistory(bool status)
{
	if (this->GetJsonFromConfigFile().contains("chat_history") == true)
	{
		nlohmann::json& _json = this->GetJsonFromConfigFile().at("chat_history");
		if (_json.contains("no_load_chat_history") == true)
		{
			_json.at("no_load_chat_history") = status;
		}
		else
		{
			RuntimeLog_Debug("no_load_chat_history not found in json");
			_json["no_load_chat_history"] = status;
		}
	}
	else
	{
		RuntimeLog_Debug("chat_history not found in json");
		this->GetJsonFromConfigFile()["chat_history"]["no_load_chat_history"] = status;
	}
}

/*
 ============================================================================
 Function: GetUseModels
 Description: 获取是否使用模型页面
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个bool，true为使用，false为不使用
 ============================================================================
*/
bool Config_Developer::GetUseModels()
{
	if (this->GetJsonFromConfigFile().contains("models") == true &&
		this->GetJsonFromConfigFile().at("models").contains("no_model_page") == true)
	{
		return !this->GetJsonFromConfigFile().at("models").at("no_model_page").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("no_model_page or models not found in json");
		return true;
	}
	return true;
}

/*
 ============================================================================
 Function: SetUseModels
 Description: 设置是否使用模型页面
 Parameters:
	 - bool: 是否使用
 Return: 无
 ============================================================================
*/
void Config_Developer::SetUseModels(bool status)
{
	if (this->GetJsonFromConfigFile().contains("models") == true)
	{
		nlohmann::json& _json = this->GetJsonFromConfigFile().at("models");
		if (_json.contains("no_model_page") == true)
		{
			_json.at("no_model_page") = status;
		}
		else
		{
			RuntimeLog_Debug("no_model_page not found in json");
			_json["no_load_models"] = status;
		}
	}
	else
	{
		RuntimeLog_Debug("models not found in json");
		this->GetJsonFromConfigFile()["models"]["no_load_models"] = status;
	}
	return;
}

/*
 ============================================================================
 Function: GetLoadModels
 Description: 设置是否加载模型
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个bool，true为加载，false为不加载
 ============================================================================
*/
bool Config_Developer::GetLoadModels()
{
	if (this->GetJsonFromConfigFile().contains("models") == true &&
		this->GetJsonFromConfigFile().at("models").contains("no_load_models") == true)
	{
		return !this->GetJsonFromConfigFile().at("models").at("no_load_models").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("no_load_models or models not found in json");
		return true;
	}
	return false;
}

/*
 ============================================================================
 Function: SetLoadModels
 Description: 设置是否加载模型
 Parameters:
	 - bool: 是否使用
 Return: 无
 ============================================================================
*/
void Config_Developer::SetLoadModels(bool status)
{
	if (this->GetJsonFromConfigFile().contains("models") == true)
	{
		nlohmann::json& _json = this->GetJsonFromConfigFile().at("models");
		if (_json.contains("no_load_models") == true)
		{
			_json.at("no_load_models") = status;
		}
		else
		{
			RuntimeLog_Debug("no_load_models not found in json");
			_json["no_load_models"] = status;
		}
	}
	else
	{
		RuntimeLog_Debug("models not found in json");
		this->GetJsonFromConfigFile()["models"]["no_load_models"] = status;
	}
}

/*
 ============================================================================
 Function: GetUsePlugins
 Description: 获取是否使用插件
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个bool，true为使用，false为不使用
 ============================================================================
*/
bool Config_Developer::GetUsePlugins()
{
	if (this->GetJsonFromConfigFile().contains("plugins") == true &&
		this->GetJsonFromConfigFile().at("plugins").contains("no_load_plugins") == true)
	{
		return !this->GetJsonFromConfigFile().at("plugins").at("no_load_plugins").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("no_load_plugins or plugins not found in json");
		return true;
	}
	return true;
}

/*
 ============================================================================
 Function: SetUsePlugins
 Description: 设置是否使用插件
 Parameters:
	 - bool: 是否使用
 Return: 无
 ============================================================================
*/
void Config_Developer::SetUsePlugins(bool status)
{
	if (this->GetJsonFromConfigFile().contains("plugins") == true)
	{
		nlohmann::json& _json = this->GetJsonFromConfigFile().at("plugins");
		if (_json.contains("no_load_plugins") == true)
		{
			_json.at("no_load_plugins") = status;
		}
		else
		{
			RuntimeLog_Debug("no_load_plugins not found in json");
			_json["no_load_plugins"] = status;
		}
	}
	else
	{
		RuntimeLog_Debug("plugins not found in json");
		this->GetJsonFromConfigFile()["plugins"]["no_load_plugins"] = status;
	}
	return;
}

/*
 ============================================================================
 Function: GetLoadPlugins
 Description: 获取是否加载插件
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个bool，true为加载，false为不加载
 ============================================================================
*/
bool Config_Developer::GetLoadPlugins()
{
	if (this->GetJsonFromConfigFile().contains("plugins") == true &&
		this->GetJsonFromConfigFile().at("plugins").contains("no_launch_any_plugins") == true)
	{
		return this->GetJsonFromConfigFile().at("plugins").at("no_launch_any_plugins").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("no_launch_any_plugins or plugins not found in json");
		return false;
	}
	return false;
}

/*
 ============================================================================
 Function: SetLoadPlugins
 Description: 设置是否加载插件
 Parameters:
	 - bool: 是否加载
 Return: 无
 ============================================================================
*/
void Config_Developer::SetLoadPlugins(bool status)
{
	if (this->GetJsonFromConfigFile().contains("plugins") == true)
	{
		nlohmann::json& _json = this->GetJsonFromConfigFile().at("plugins");
		if (_json.contains("no_launch_any_plugins") == true)
		{
			_json.at("no_launch_any_plugins") = status;
		}
		else
		{
			RuntimeLog_Debug("no_launch_any_plugins not found in json");
			_json["no_launch_any_plugins"] = status;
		}
	}
	else
	{
		RuntimeLog_Debug("plugins not found in json");
		this->GetJsonFromConfigFile()["plugins"]["no_launch_any_plugins"] = status;
	}
	return;
}

/*
 ============================================================================
 Function: GetAutoStartPlugins
 Description: 获取是否自动加载插件
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个bool，true为自动加载，false为不自动加载
 ============================================================================
*/
bool Config_Developer::GetAutoStartPlugins()
{
	if (this->GetJsonFromConfigFile().contains("plugins") == true &&
		this->GetJsonFromConfigFile().at("plugins").contains("automatically_start_plugin") == true)
	{
		return this->GetJsonFromConfigFile().at("plugins").at("automatically_start_plugin").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("automatically_start_plugin or plugins not found in json");
		return false;
	}
	return false;
}

/*
 ============================================================================
 Function: SetAutoStartPlugins
 Description: 设置是否自动加载插件
 Parameters:
	 - bool: 是否自动加载
 Return: 无
 ============================================================================
*/
void Config_Developer::SetAutoStartPlugins(bool status)
{
	if (this->GetJsonFromConfigFile().contains("plugins") == true)
	{
		nlohmann::json& _json = this->GetJsonFromConfigFile().at("plugins");
		if (_json.contains("automatically_start_plugin") == true)
		{
			_json.at("automatically_start_plugin") = status;
		}
		else
		{
			RuntimeLog_Debug("automatically_start_plugin not found in json");
			_json["automatically_start_plugin"] = status;
		}
	}
	else
	{
		RuntimeLog_Debug("plugins not found in json");
		this->GetJsonFromConfigFile()["plugins"]["automatically_start_plugin"] = status;
	}
	return;
}

// ======== end ========= end ========= end ========= end ========= end ========= end =========
