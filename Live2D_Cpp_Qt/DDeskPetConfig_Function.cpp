#include "DDeskPetConfig_Function.h"

//  ========= Config_Function ==========
Config_Function::Config_Function(QString qstr_FilePath) :
	Page_Config_Base(qstr_FilePath)
{
}

Config_Function::~Config_Function()
{
}

bool Config_Function::IsMouseMoveFollow()
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").contains("mouse_follow") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").contains("use_mouse_follow") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").contains("use") == true)
	{
		return this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").at("use").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_live2D_model、mouse_follow or use_mouse_follow not found in json");
	}
	return false;
}

int Config_Function::GetMouseMoveFollowMode()
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").contains("mouse_follow") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").contains("follow_model") == true)
	{
		return this->GetJsonFromConfigFile()
			.at("about_live2D_model")
			.at("mouse_follow")
			.at("follow_model")
			.get<int>();	// 1: local, 2: global
	}
	else
	{
		RuntimeLog_Debug("about_live2D_model、mouse_follow or follow_model not found in json");
	}
	return 1;
}

bool Config_Function::IsMouseMoveFollowGlobal()
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").contains("mouse_follow") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").contains("use_mouse_follow") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").contains("follow_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").at("follow_model").contains("global") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").at("follow_model").at("global").contains("use") == true)
	{
		return this->GetJsonFromConfigFile()
			.at("about_live2D_model")
			.at("mouse_follow")
			.at("use_mouse_follow")
			.at("follow_model")
			.at("global")
			.at("use")
			.get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_live2D_model、mouse_follow or use_mouse_follow not found in json");
	}
	return false;
}

bool Config_Function::IsMouseMoveFollowLocal()
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").contains("mouse_follow") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").contains("use_mouse_follow") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").contains("follow_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").at("follow_model").contains("local") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").at("follow_model").at("local").contains("use") == true)
	{
		return this->GetJsonFromConfigFile()
			.at("about_live2D_model")
			.at("mouse_follow")
			.at("use_mouse_follow")
			.at("follow_model")
			.at("local")
			.at("use")
			.get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_live2D_model、mouse_follow or use_mouse_follow not found in json");
	}
	return false;
}

bool Config_Function::IsRuntimeErrorLog()
{
	if (this->GetJsonFromConfigFile().contains("about_other") == true &&
		this->GetJsonFromConfigFile().at("about_other").contains("use_error_logs") == true &&
		this->GetJsonFromConfigFile().at("about_other").at("use_error_logs").contains("use") == true)
	{
		return this->GetJsonFromConfigFile().at("about_other").at("use_error_logs").at("use").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_other、use_error_logs or use not found in json");
	}
	return false;
}

int Config_Function::GetLogLevel()
{
	if (this->GetJsonFromConfigFile().contains("about_other") == true &&
		this->GetJsonFromConfigFile().at("about_other").contains("use_error_logs") == true &&
		this->GetJsonFromConfigFile().at("about_other").at("use_error_logs").contains("log_level") == true &&
		this->GetJsonFromConfigFile().at("about_other").at("use_error_logs").at("log_level").contains("value") == true)
	{
		return this->GetJsonFromConfigFile().at("about_other").at("use_error_logs").at("log_level").at("value").get<int>();
	}
	else
	{
		RuntimeLog_Debug("about_other、log_level not found in json");
	}
	return 0;
}

bool Config_Function::IsUseVA()
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").contains("voice_assistant") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("voice_assistant").contains("use_voice_assistant") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("voice_assistant").at("use_voice_assistant").contains("use") == true)
	{
		return this->GetJsonFromConfigFile().at("about_live2D_model").at("voice_assistant").at("use_voice_assistant").at("use").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_live2D_model、voice_assistant、use_voice_assistant or use not found in json");
	}
	return false;
}

bool Config_Function::IsShortcut_Key_Activate_VA()
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").contains("voice_assistant") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("voice_assistant").contains("hotkey_info") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("voice_assistant").at("hotkey_info").contains("hotkey") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("voice_assistant").at("hotkey_info").at("hotkey").contains("value") == true)
	{
		return this->GetJsonFromConfigFile()
			.at("about_live2D_model")
			.at("voice_assistant")
			.at("hotkey_info")
			.at("hotkey")
			.at("value")
			.get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_live2D_model、voice_assistant、hotkey_info、hotkey or value not found in json");
	}
	return false;
}

bool Config_Function::IsAutoLogin()
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_window") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").contains("use_auto_login") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").at("use_auto_login").contains("use") == true)
	{
		return this->GetJsonFromConfigFile().at("about_live2D_window").at("use_auto_login").at("use").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_live2D_window、 use_auto_login or use not found in json");
	}
	return false;
}

bool Config_Function::IsAutoSaveChatRecord()
{
	if (this->GetJsonFromConfigFile().contains("about_ai") == true &&
		this->GetJsonFromConfigFile().at("about_ai").contains("auto_save_chat_content") == true &&
		this->GetJsonFromConfigFile().at("about_ai").at("auto_save_chat_content").contains("use_auto_save_chat_content") == true)
	{
		return this->GetJsonFromConfigFile().at("about_ai").at("auto_save_chat_content").at("use_auto_save_chat_content").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_ai、auto_save_chat_content or use_auto_save_chat_content not found in json");
	}
	return false;
}

bool Config_Function::IsShow_CPU_Usage()
{
	if (this->GetJsonFromConfigFile().contains("about_show_message") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").contains("tab_cpu") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").contains("show_cpu_usage") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").at("show_cpu_usage").contains("use") == true)
	{
		return this->GetJsonFromConfigFile()
			.at("about_show_message")
			.at("tab_cpu")
			.at("show_cpu_usage")
			.at("use")
			.get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_show_message、tab_cpu、show_cpu_usage、use not found in json");
	}
	return false;
}

bool Config_Function::IsShow_Memory_Usage()
{
	if (this->GetJsonFromConfigFile().contains("about_show_message") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").contains("tab_memory") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").contains("show_memory_usage") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").at("show_memory_usage").contains("use") == true)
	{
		return this->GetJsonFromConfigFile()
			.at("about_show_message")
			.at("tab_memory")
			.at("show_memory_usage")
			.at("use")
			.get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_show_message、tab_memory、show_memory_usage、use not found in json");
	}
	return false;
}

bool Config_Function::IsUse_MultiThread_CPU_Usage()
{
	if (this->GetJsonFromConfigFile().contains("about_show_message") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").contains("tab_cpu") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").contains("show_cpu_usage") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").at("show_cpu_usage").contains("use_thread") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").at("show_cpu_usage").at("use_thread").contains("use") == true)
	{
		return this->GetJsonFromConfigFile()
			.at("about_show_message")
			.at("tab_cpu")
			.at("show_cpu_usage")
			.at("use_thread")
			.at("use")
			.get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_show_message、tab_cpu、show_cpu_usage、use_thread、use not found in json");
	}
	return false;
}

unsigned int Config_Function::GetThreadFlushTime_CPU()
{
	if (this->GetJsonFromConfigFile().contains("about_show_message") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").contains("tab_cpu") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").contains("show_cpu_usage") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").at("show_cpu_usage").contains("use_thread") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").at("show_cpu_usage").at("use_thread").contains("thread_flush_time") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").at("show_cpu_usage").at("use_thread").at("thread_flush_time").contains("flush_time") == true)
	{
		int _flush_time = this->GetJsonFromConfigFile()
			.at("about_show_message")
			.at("tab_cpu")
			.at("show_cpu_usage")
			.at("use_thread")
			.at("thread_flush_time")
			.at("flush_time")
			.get<int>();
		// 确保时间大于0
		return (unsigned int)(_flush_time <= 1 ? 3 : _flush_time);
	}
	else
	{
		RuntimeLog_Debug("tab_cpu、show_cpu_usage、use_thread、thread_flush_time not found in json");
	}
	return 3;
}

bool Config_Function::IsUse_MultiThread_Memory_Usage()
{
	if (this->GetJsonFromConfigFile().contains("about_show_message") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").contains("tab_memory") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").contains("show_memory_usage") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").at("show_memory_usage").contains("use_thread") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").at("show_memory_usage").at("use_thread").contains("use") == true)
	{
		return this->GetJsonFromConfigFile()
			.at("about_show_message")
			.at("tab_memory")
			.at("show_memory_usage")
			.at("use_thread")
			.at("use")
			.get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_show_message、tab_memory、show_memory_usage、use_thread、use not found in json");
	}
	return false;
}

unsigned int Config_Function::GetThreadFlushTime_Memory()
{
	if (this->GetJsonFromConfigFile().contains("about_show_message") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").contains("tab_memory") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").contains("show_memory_usage") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").at("show_memory_usage").contains("use_thread") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").at("show_memory_usage").at("use_thread").contains("thread_flush_time") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").at("show_memory_usage").at("use_thread").at("thread_flush_time").contains("flush_time") == true)
	{
		int _flush_time = this->GetJsonFromConfigFile()
			.at("about_show_message")
			.at("tab_memory")
			.at("show_memory_usage")
			.at("use_thread")
			.at("thread_flush_time")
			.at("flush_time")
			.get<int>();
		return (unsigned int)(_flush_time <= 1 ? 3 : _flush_time);
	}
	else
	{
		RuntimeLog_Debug("about_show_message、tab_memory、show_memory_usage、use_thread、thread_flush_time not found in json");
	}
	return 3;
}

/*
 ============================================================================
 Function: GetShow_Message_Style
 Description: 获取显示消息的窗口的样式
 Parameters:
	 - 无参数: 无释义
 Return: int类型, 表示显示消息的窗口的样式
 ============================================================================
*/
int Config_Function::GetShow_Message_Style()
{
	if (this->GetJsonFromConfigFile().contains("about_show_message") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").contains("show_message_style") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("show_message_style").contains("style") == true)
	{
		return this->GetJsonFromConfigFile().at("about_show_message").at("show_message_style").at("style").get<int>();
	}
	else
	{
		RuntimeLog_Debug("about_show_message、show_message_style、style not found in json");
	}
	return 0;
}

bool Config_Function::IsUse_Model_lip_shape()
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").contains("lip_shape") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("lip_shape").contains("use_lip_shape") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("lip_shape").at("use_lip_shape").contains("use") == true)
	{
		return this->GetJsonFromConfigFile().at("about_live2D_model").at("lip_shape").at("use_lip_shape").at("use").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_live2D_model、lip_shape、use_lip_shape、use not found in json");
	}
	return false;
}

bool Config_Function::IsUse_Context()
{
	if (this->GetJsonFromConfigFile().contains("about_ai") == true &&
		this->GetJsonFromConfigFile().at("about_ai").contains("chat_context") == true &&
		this->GetJsonFromConfigFile().at("about_ai").at("chat_context").contains("use_chat_context") == true &&
		this->GetJsonFromConfigFile().at("about_ai").at("chat_context").at("use_chat_context").contains("use") == true)
	{
		return this->GetJsonFromConfigFile().at("about_ai").at("chat_context").at("use_chat_context").at("use").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_ai、chat_context、use_chat_context、use not found in json");
	}
	return false;
}

bool Config_Function::IsUseDeveloperMode()
{
	if (this->GetJsonFromConfigFile().contains("about_other") == true &&
		this->GetJsonFromConfigFile().at("about_other").contains("use_developer_mode") == true &&
		this->GetJsonFromConfigFile().at("about_other").at("use_developer_mode").contains("use") == true)
	{
		return this->GetJsonFromConfigFile().at("about_other").at("use_developer_mode").at("use").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_other、use_developer_mode、use not found in json");
	}
	return false;
}

/*
 ============================================================================
 Function: IsModelWindowAllwaysOnTop
 Description: 获取模型窗口是否总在最前
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个bool，true为总在最前（置顶窗口），false为不总在最前
 ============================================================================
*/
bool Config_Function::IsModelWindowAllwaysOnTop()
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_window") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").contains("top_window") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").at("top_window").contains("use") == true)
	{
		return this->GetJsonFromConfigFile().at("about_live2D_window").at("top_window").at("use").get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_live2D_window、top_window、use not found in json");
	}
	return false;
}

/*
 ============================================================================
 Function: IsUseDefaultModelWindowSize
 Description: 获取是否使用默认模型窗口尺寸
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个bool，true为使用默认尺寸，false为不使用
 ============================================================================
*/
bool Config_Function::IsUseDefaultModelWindowSize()
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_window") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").contains("model_window_scale_ratio") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").contains("default_size") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").at("default_size").contains("use_default_size") == true)
	{
		return this->GetJsonFromConfigFile()
			.at("about_live2D_window")
			.at("model_window_scale_ratio")
			.at("default_size")
			.at("use_default_size")
			.get<bool>();
	}
	else
	{
		RuntimeLog_Debug("about_live2D_window、model_window_scale_ratio、default_size、use_default_size not found in json");
	}
	return true;
}

/*
 ============================================================================
 Function: GetWindowZoomFactor
 Description: 获取窗口的缩放因子
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个float，表示窗口的缩放因子
 ============================================================================
*/
float Config_Function::GetWindowZoomFactor()
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_window") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").contains("model_window_scale_ratio") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").contains("window_zoom_factor") == true)
	{
		float factor = this->GetJsonFromConfigFile()
			.at("about_live2D_window")
			.at("model_window_scale_ratio")
			.at("window_zoom_factor")
			.get<float>();
		return (factor < 0.0f || factor > 3.0f) ? 1.0f : factor;
	}
	else
	{
		RuntimeLog_Debug("about_live2D_window、model_window_scale_ratio、window_zoom_factor not found in json");
	}
	return 1.0f;
}

/*
 ============================================================================
 Function: GetDefaultModelWindowHeight
 Description: 获取默认模型窗口高度
 Parameters:
	 - 无参数: 无释义
 Return: 返回一个int，表示默认模型窗口高度的像素值
 ============================================================================
*/
int Config_Function::GetDefaultModelWindowHeight()
{
	// 判断是否使用默认模型窗口尺寸，如果使用则直接返回json中配置的默认高度
	// 否则根据缩放因子计算并返回高度（false）
	if (IsUseDefaultModelWindowSize() == true)
	{
		if (this->GetJsonFromConfigFile().contains("about_live2D_window") == true &&
			this->GetJsonFromConfigFile().at("about_live2D_window").contains("model_window_scale_ratio") == true &&
			this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").contains("default_size") == true &&
			this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").at("default_size").contains("default_height") == true)
		{
			return this->GetJsonFromConfigFile()
				.at("about_live2D_window")
				.at("model_window_scale_ratio")
				.at("default_size")
				.at("default_height")
				.get<int>();
		}
		else
		{
			RuntimeLog_Debug("about_live2D_window、model_window_scale_ratio、default_size、default_height not found in json");
		}
	}
	else
	{
		if (this->GetJsonFromConfigFile().contains("about_live2D_window") == true &&
			this->GetJsonFromConfigFile().at("about_live2D_window").contains("model_window_scale_ratio") == true &&
			this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").contains("default_size") == true &&
			this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").at("default_size").contains("default_height") == true)
		{
			return static_cast<int>(static_cast<float>(this->GetJsonFromConfigFile()
				.at("about_live2D_window")
				.at("model_window_scale_ratio")
				.at("default_size")
				.at("default_height")
				.get<int>()) *
				GetWindowZoomFactor());
		}
		else
		{
			RuntimeLog_Debug("about_live2D_window、model_window_scale_ratio、default_size、default_height not found in json");
		}
	}
	return 600;
}

int Config_Function::GetDefaultModelWindowWidth()
{
	// 判断是否使用默认模型窗口尺寸，如果使用则直接返回json中配置的默认宽度
	// 否则根据缩放因子计算并返回宽度（false）
	if (IsUseDefaultModelWindowSize() == true)
	{
		if (this->GetJsonFromConfigFile().contains("about_live2D_window") == true &&
			this->GetJsonFromConfigFile().at("about_live2D_window").contains("model_window_scale_ratio") == true &&
			this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").contains("default_size") == true &&
			this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").at("default_size").contains("default_width") == true)
		{
			return this->GetJsonFromConfigFile()
				.at("about_live2D_window")
				.at("model_window_scale_ratio")
				.at("default_size")
				.at("default_width")
				.get<int>();
		}
		else
		{
			RuntimeLog_Debug("about_live2D_window、model_window_scale_ratio、default_size、default_width not found in json");
		}
	}
	else
	{
		if (this->GetJsonFromConfigFile().contains("about_live2D_window") == true &&
			this->GetJsonFromConfigFile().at("about_live2D_window").contains("model_window_scale_ratio") == true &&
			this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").contains("default_size") == true &&
			this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").at("default_size").contains("default_width") == true)
		{
			return static_cast<int>(static_cast<float>(this->GetJsonFromConfigFile()
				.at("about_live2D_window")
				.at("model_window_scale_ratio")
				.at("default_size")
				.at("default_width")
				.get<int>()) *
				GetWindowZoomFactor());
		}
		else
		{
			RuntimeLog_Debug("about_live2D_window、model_window_scale_ratio、default_size、default_width not found in json");
		}
	}

	return 800;
}

void Config_Function::SetUseMouseMoveFollow(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").contains("mouse_follow") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").contains("use_mouse_follow") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").contains("use") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_live2D_model")
			.at("mouse_follow")
			.at("use_mouse_follow")
			.at("use") = status;
	}
	else
	{
		RuntimeLog_Debug("about_live2D_model、mouse_follow or use_mouse_follow not found in json");
	}
	return;
}

void Config_Function::SetMouseMoveFollowMode(int mode)
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").contains("mouse_follow") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").contains("follow_model") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_live2D_model")
			.at("mouse_follow")
			.at("follow_model") = mode;
	}
	else
	{
		RuntimeLog_Debug("about_live2D_model、mouse_follow or follow_model not found in json");
	}
	return;
}

void Config_Function::SetMouseMoveFollowGlobal(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_model") == true &&
		this->GetJsonFromConfigFile().at("about_Live2D_model").contains("mouse_follow") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").contains("use_mouse_follow") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").contains("follow_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").at("follow_model").contains("global") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").at("follow_model").at("global").contains("use") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_live2D_model")
			.at("mouse_follow")
			.at("use_mouse_follow")
			.at("follow_model")
			.at("global")
			.at("use") = status;
	}
	else
	{
		RuntimeLog_Debug("about_live2D_model、mouse_follow or use_mouse_follow、follow_model、global or use not found in json");
	}
	return;
}

void Config_Function::SetMouseMoveFollowLocal(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").contains("mouse_follow") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").contains("use_mouse_follow") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").contains("follow_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").at("follow_model").contains("local") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("mouse_follow").at("use_mouse_follow").at("follow_model").at("local").contains("use") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_live2D_model")
			.at("mouse_follow")
			.at("use_mouse_follow")
			.at("follow_model")
			.at("local")
			.at("use") = status;
	}
	else
	{
		RuntimeLog_Debug("about_live2D_model、mouse_follow or use_mouse_follow、follow_model、local or use not found in json");
	}
	return;
}

void Config_Function::SetUseRuntimeErrorLog(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_other") == true &&
		this->GetJsonFromConfigFile().at("about_other").contains("use_error_logs") == true &&
		this->GetJsonFromConfigFile().at("about_other").at("use_error_logs").contains("use") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_other")
			.at("use_error_logs")
			.at("use") = status;
	}
	else
	{
		RuntimeLog_Debug("about_other、use_error_logs or use not found in json");
	}
	return;
}

void Config_Function::SetRuntimeLogLevel(QString level)
{
	int _level = 0;
	if (level == "Debug")
	{
		_level = 1;
	}
	else if (level == "Info")
	{
		_level = 2;
	}
	else if (level == "Warning")
	{
		_level = 3;
	}
	else if (level == "Error")
	{
		_level = 4;
	}
	else if (level == "Fatal")
	{
		_level = 5;
	}
	else
	{
		RuntimeLog_Debug("Invalid log level: " + level.toStdString());
		_level = 0;
	}
	if (this->GetJsonFromConfigFile().contains("about_other") == true &&
		this->GetJsonFromConfigFile().at("about_other").contains("use_error_logs") == true &&
		this->GetJsonFromConfigFile().at("about_other").at("use_error_logs").contains("log_level") == true &&
		this->GetJsonFromConfigFile().at("about_other").at("use_error_logs").at("log_level").contains("value") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_other")
			.at("use_error_logs")
			.at("log_level")
			.at("value") = _level;
	}
	else
	{
		RuntimeLog_Debug("about_other、log_level or level not found in json");
		this->GetJsonFromConfigFile()
			.at("about_other")
			.at("use_error_logs")
			.at("log_level")["value"] = _level;
	}
}

void Config_Function::SetUseVA(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").contains("voice_assistant") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("voice_assistant").contains("use_voice_assistant") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("voice_assistant").at("use_voice_assistant").contains("use") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_live2D_model")
			.at("voice_assistant")
			.at("use_voice_assistant")
			.at("use") = status;
	}
	else
	{
		RuntimeLog_Debug("about_live2D_model、voice_assistant、use_voice_assistant or use not found in json");
	}
	return;
}

void Config_Function::SetUseShortcut_Key_Activate_VA(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").contains("voice_assistant") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("voice_assistant").contains("hotkey_info") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("voice_assistant").at("hotkey_info").contains("use_hotkey") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_live2D_model")
			.at("voice_assistant")
			.at("hotkey_info")
			.at("use_hotkey") = status;
	}
	else
	{
		RuntimeLog_Debug("about_live2D_model、voice_assistant、hotkey_info or use_hotkey not found in json");
	}
	return;
}

void Config_Function::SetUseAutoLogin(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_window") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").contains("use_auto_login") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").at("use_auto_login").contains("use") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_live2D_window")
			.at("use_auto_login")
			.at("use") = status;
	}
	else
	{
		RuntimeLog_Debug("about_live2D_window、use_auto_login or use not found in json");
	}
	return;
}

void Config_Function::SetUseAutoSaveChatRecord(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_ai") == true &&
		this->GetJsonFromConfigFile().at("about_ai").contains("chat_context") == true &&
		this->GetJsonFromConfigFile().at("about_ai").at("chat_context").contains("use_chat_context") == true &&
		this->GetJsonFromConfigFile().at("about_ai").at("chat_context").at("use_chat_context").contains("use") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_ai")
			.at("chat_context")
			.at("use_chat_context")
			.at("use") = status;
	}
	else
	{
		RuntimeLog_Debug("about_ai、chat_context、use_chat_context or use not found in json");
	}
	return;
}

void Config_Function::SetShow_CPU_Usage(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_show_message") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").contains("tab_cpu") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").contains("show_cpu_usage") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").at("show_cpu_usage").contains("use") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_show_message")
			.at("tab_cpu")
			.at("show_cpu_usage")
			.at("use") = status;
	}
	else
	{
		RuntimeLog_Debug("about_show_message、tab_cpu、show_cpu_usage or use not found in json");
	}
	return;
}

void Config_Function::SetShow_Memory_Usage(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_show_message") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").contains("tab_memory") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").contains("show_memory_usage") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").at("show_memory_usage").contains("use") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_show_message")
			.at("tab_memory")
			.at("show_memory_usage")
			.at("use") = status;
	}
	else
	{
		RuntimeLog_Debug("about_show_message、tab_memory、show_memory_usage or use not found in json");
	}
	return;
}

void Config_Function::SetUse_MultiThread_CPU_Usage(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_show_message") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").contains("tab_cpu") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").contains("show_cpu_usage") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").at("show_cpu_usage").contains("use_thread") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").at("show_cpu_usage").at("use_thread").contains("use") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_show_message")
			.at("tab_cpu")
			.at("show_cpu_usage")
			.at("use_thread")
			.at("use") = status;
	}
	else
	{
		RuntimeLog_Debug("about_show_message、tab_cpu、show_cpu_usage、use_thread or use not found in json");
	}
	return;
}

void Config_Function::SetThreadFlushTime_CPU(unsigned int time)
{
	if (this->GetJsonFromConfigFile().contains("about_show_message") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").contains("tab_cpu") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").contains("show_cpu_usage") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").at("show_cpu_usage").contains("use_thread") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").at("show_cpu_usage").at("use_thread").contains("thread_flush_time") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_cpu").at("show_cpu_usage").at("use_thread").at("thread_flush_time").contains("flush_time") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_show_message")
			.at("tab_cpu")
			.at("show_cpu_usage")
			.at("use_thread")
			.at("thread_flush_time")
			.at("flush_time") = time;
	}
	else
	{
		RuntimeLog_Debug("about_show_message、tab_cpu、show_cpu_usage、use_thread、thread_flush_time or flush_time not found in json");
	}
	return;
}

void Config_Function::SetUse_MultiThread_Memory_Usage(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_show_message") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").contains("tab_memory") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").contains("show_memory_usage") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").at("show_memory_usage").contains("use_thread") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").at("show_memory_usage").at("use_thread").contains("use") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_show_message")
			.at("tab_memory")
			.at("show_memory_usage")
			.at("use_thread")
			.at("use") = status;
	}
	else
	{
		RuntimeLog_Debug("about_show_message, tab_memory, show_memory_usage, use_thread, thread_flush_time or use not found in json");
	}
	return;
}

void Config_Function::SetThreadFlushTime_Memory(unsigned int time)
{
	if (this->GetJsonFromConfigFile().contains("about_show_message") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").contains("tab_memory") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").contains("show_memory_usage") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").at("show_memory_usage").contains("use_thread") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").at("show_memory_usage").at("use_thread").contains("thread_flush_time") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("tab_memory").at("show_memory_usage").at("use_thread").at("thread_flush_time").contains("flush_time") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_show_message")
			.at("tab_memory")
			.at("show_memory_usage")
			.at("use_thread")
			.at("thread_flush_time")
			.at("flush_time") = time;
	}
	else
	{
		RuntimeLog_Debug("about_show_message, tab_memory, show_memory_usage, use_thread, thread_flush_time or flush_time not found in json");
	}
	return;
}

/*
 ============================================================================
 Function: SetShowMessageStyle
 Description: 设置显示消息的窗口的样式
 Parameters:
	 - int: 显示风格
 Return: 无
 ============================================================================
*/
void Config_Function::SetShowMessageStyle(int style)
{
	if (this->GetJsonFromConfigFile().contains("about_show_message") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").contains("show_message_style") == true &&
		this->GetJsonFromConfigFile().at("about_show_message").at("show_message_style").contains("style") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_show_message")
			.at("show_message_style")
			.at("style") = style;
		return;
	}
	else
	{
		this->GetJsonFromConfigFile()["Show_Message_Style"] = style;
		RuntimeLog_Debug("about_show_message、show_message_style or style not found in json");
		return;
	}
	return;
}

void Config_Function::SetUse_Model_lip_shape(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_model") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").contains("lip_shape") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("lip_shape").contains("use_lip_shape") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_model").at("lip_shape").at("use_lip_shape").contains("use") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_live2D_model")
			.at("lip_shape")
			.at("use_lip_shape")
			.at("use") = status;
	}
	else
	{
		RuntimeLog_Debug("about_live2D_model、lip_shape、use_lip_shape or use not found in json");
	}
	return;
}

void Config_Function::SetUse_Context(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_ai") == true &&
		this->GetJsonFromConfigFile().at("about_ai").contains("chat_context") == true &&
		this->GetJsonFromConfigFile().at("about_ai").at("chat_context").contains("use_chat_context") == true &&
		this->GetJsonFromConfigFile().at("about_ai").at("chat_context").at("use_chat_context").contains("use") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_ai")
			.at("chat_context")
			.at("use_chat_context")
			.at("use") = status;
	}
	else
	{
		RuntimeLog_Debug("about_ai、chat_context、use_chat_context or use not found in json");
	}
	return;
}

void Config_Function::SetUseDeveloperMode(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_other") == true &&
		this->GetJsonFromConfigFile().at("about_other").contains("use_developer_mode") == true &&
		this->GetJsonFromConfigFile().at("about_other").at("use_developer_mode").contains("use") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_other")
			.at("use_developer_mode")
			.at("use") = status;
	}
	else
	{
		RuntimeLog_Debug("about_other、use_developer_mode or use not found in json");
	}
	return;
}

/*
 ============================================================================
 Function: SetModelWindowAllwaysOnTop
 Description: 设置模型窗口是否一直置顶
 Parameters:
	 - bool: 一个设置状态
 Return: 无返回值
 ============================================================================
*/
void Config_Function::SetModelWindowAllwaysOnTop(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_window") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").contains("top_window") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").at("top_window").contains("use") == true)
	{
		this->GetJsonFromConfigFile().at("about_live2D_window").at("top_window").at("use") = status;
	}
	else
	{
		this->GetJsonFromConfigFile()["about_live2D_window"]["top_window"]["use"] = status;
		RuntimeLog_Debug("about_live2D_window、top_window or use not found in json");
	}
	return;
}

/*
 ============================================================================
 Function: SetUseDefaultModelWindowSize
 Description: 设置是否使用默认模型窗口大小
 Parameters:
	 - bool: 一个设置状态
 Return: 无返回值
 ============================================================================
*/
void Config_Function::SetUseDefaultModelWindowSize(bool status)
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_window") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").contains("model_window_scale_ratio") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").contains("default_size") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").at("default_size").contains("use_default_size") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_live2D_window")
			.at("model_window_scale_ratio")
			.at("default_size")
			.at("use_default_size") = status;
	}
	else
	{
		this->GetJsonFromConfigFile()["about_live2D_window"]["model_window_scale_ratio"]["default_size"]["use_default_size"] = status;
		RuntimeLog_Debug("about_live2D_window、model_window_scale_ratio、default_size or use_default_size not found in json");
	}
	return;
}

/*
 ============================================================================
 Function: SetWindowZoomFactor
 Description: 设置窗口缩放因子
 Parameters:
	 - float: 缩放因子
 Return: 无返回值
 ============================================================================
*/
void Config_Function::SetWindowZoomFactor(float factor)
{
	if (this->GetJsonFromConfigFile().contains("about_live2D_window") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").contains("model_window_scale_ratio") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").contains("window_zoom_factor") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_live2D_window")
			.at("model_window_scale_ratio")
			.at("window_zoom_factor") = factor;
	}
	else
	{
		this->GetJsonFromConfigFile()["about_live2D_window"]["model_window_scale_ratio"]["window_zoom_factor"] = factor;
		RuntimeLog_Debug("about_live2D_window、model_window_scale_ratio or window_zoom_factor not found in json");
	}
	return;
}

/*
 ============================================================================
 Function: SetDefaultModelWindowHeight
 Description: 设置默认模型窗口高度
 Parameters:
	 - int: 高度
 Return: 无返回值
 ============================================================================
*/
void Config_Function::SetDefaultModelWindowHeight(int height)
{
	if (height <= 75 || height > 900)
		height = 300;
	if (this->GetJsonFromConfigFile().contains("about_live2D_window") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").contains("model_window_scale_ratio") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").contains("default_size") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").at("default_size").contains("default_height") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_live2D_window")
			.at("model_window_scale_ratio")
			.at("default_size")
			.at("default_height") = height;
	}
	else
	{
		this->GetJsonFromConfigFile()["about_live2D_window"]["model_window_scale_ratio"]["default_size"]["default_height"] = height;
		RuntimeLog_Debug("about_live2D_window、model_window_scale_ratio、default_size or default_height not found in json");
	}
	return;
}

/*
 ============================================================================
 Function: SetDefaultModelWindowWidth
 Description: 设置默认模型窗口宽度
 Parameters:
	 - int: 宽度
 Return: 无返回值
 ============================================================================
*/
void Config_Function::SetDefaultModelWindowWidth(int width)
{
	if (width <= 100 || width > 1200)
		width = 400;
	if (this->GetJsonFromConfigFile().contains("about_live2D_window") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").contains("model_window_scale_ratio") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").contains("default_size") == true &&
		this->GetJsonFromConfigFile().at("about_live2D_window").at("model_window_scale_ratio").at("default_size").contains("default_width") == true)
	{
		this->GetJsonFromConfigFile()
			.at("about_live2D_window")
			.at("model_window_scale_ratio")
			.at("default_size")
			.at("default_width") = width;
	}
	else
	{
		this->GetJsonFromConfigFile()["about_live2D_window"]["model_window_scale_ratio"]["default_size"]["default_width"] = width;
		RuntimeLog_Debug("about_live2D_window、model_window_scale_ratio、default_size or default_width not found in json");
	}
	return;
}
Config_Function& Config_Function::instance()
{
	static Config_Function ins(ConfigurationBase::instance().GetFunctionWindowsPageConfigFilePath());
	return ins;
}

void Config_Function::Initialize()
{
	this->InitInstanceData();
	//Debug_PrintJson();
	return;
}


// ========= end ==========