#pragma once
#include <QString>

// 页面组件对象名称列表
namespace Page_ObjectNameList {
	// 账户页面
	namespace Account_ObjNameList {
		extern QString lineEdit_UserName;
		extern QString lineEdit_Password;

		extern QString plainTextEdit_UserFeedback;
		extern QString plainTextEdit_Announcement;

		extern QString pushButton_Login;
		extern QString pushButton_Register;

		extern QString label_UserName;
		extern QString label_UID;
		extern QString label_Email;
	}
	namespace Personalization_ObjNameList {

	}

	namespace Funtion_ObjNameList {
		extern QString checkBox_show_cpu;
		extern QString checkBox_show_gpu;
		extern QString checkBox_show_memory;
		extern QString checkBox_show_cpu_thread;
		extern QString checkBox_show_gpu_thread;
		extern QString checkBox_show_memory_thread;
		extern QString checkBox_mouse_follow;
		extern QString checkBox_Model_lip_shape;
		extern QString checkBox_Model_VA;
		extern QString checkBox_Model_USE_HotKey;
		extern QString checkBox_errorLog;
		extern QString checkBox_dev;
		extern QString checkBox_model_window_top;

		extern QString spinBox_flushtime_cpu;
		extern QString spinBox_flushtime_gpu;
		extern QString spinBox_flushtime_memory;

		extern QString radioButton_mouse_follow_global;
		extern QString radioButton_mouse_follow_local;

		extern QString ksEdit_hotkey;

		extern QString pushButton_RegisterHotkey;

		extern QString combobox_show_message_style;
		extern QString comboBox_logLevel;
		extern QString label_show_message_style;
	}
	// 设置AI页面
	namespace SetAI_ObjNameList {
		// AI配置操作页面 - objectName
		extern QString pushButton_SaveConfig;
		extern QString pushButton_RestoreDefaultConfig;
		extern QString pushButton_SaveConfigToServer;
		extern QString pushbutton_refresh;
		extern QString pushbutton_reload_ai;

		extern QString lineEdit_url;
		extern QString lineEdit_url_extrapath;
		extern QString lineEdit_url_extrapath_model;
		extern QString lineEdit_api_key;
		extern QString lineEdit_n;
		extern QString lineEdit_maxtokens;

		extern QString plainTextEdit_prompt;
		extern QString plainTextEdit_system;
		extern QString plainTextEdit_n;
		extern QString plainTextEdit_maxtokens;
		extern QString plainTextEdit_URL;
		extern QString plainTextEdit_ApiKey;
		extern QString plainTextEdit_customJson;
		extern QString plainTextEdit_addCustomField;

		extern QString comboBox_ModelList;
		
		extern QString spinBox_n;
		extern QString spinBox_maxtokens;
		extern QString spinBox_temperature;
		extern QString spinBox_top_p;
		extern QString spinBox_frequency_penalty;
		extern QString spinBox_presence_penalty;

		extern QString label_comboBoxStatus;

		extern QString dspinBox_Temperature;
		extern QString dspinBox_top_p;
		extern QString dspinBox_frequency_penalty;
		extern QString dspinBox_presence_penalty;

		extern QString checkBox_n;
		extern QString checkBox_append_n;
		extern QString checkBox_maxtokens;
		extern QString checkBox_append_maxtokens;
		extern QString checkBox_AIPrompt_prompt;
		extern QString checkBox_AIPrompt_system;
		extern QString checkBox_frequency_penalty;
		extern QString checkBox_append_frequency_penalty;
		extern QString checkBox_presence_penalty;
		extern QString checkBox_append_presence_penalty;
		extern QString checkBox_useCustomJson;
		extern QString checkBox_AddCustomField;
		extern QString checkBox_temperature;
		extern QString checkBox_top_p;
		extern QString checkBox_append_temperature;
		extern QString checkBox_append_top_p;
		extern QString checkBox_append_stop;

		extern QString radioButton_temperature;
		extern QString radioButton_top_p;

		extern QString qtableWidget_Stop;

		extern QString qtabWidget_More;

		extern QString intSlider_n;
		extern QString intSlider_max_tokens;
	}
}

// 别名 太长辣~~~~~我为什么要这样命名QAQ
namespace _PONL = Page_ObjectNameList;