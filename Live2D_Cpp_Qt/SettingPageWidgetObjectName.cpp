#include "SettingPageWidgetObjectName.h"


namespace Page_ObjectNameList {
	// 账户页面
	namespace Account_ObjNameList {
		QString lineEdit_UserName				= "lineEdit_UserName";
		QString lineEdit_Password				= "lineEdit_Password";

		QString plainTextEdit_UserFeedback		= "FeedbackTextEdit";
		QString plainTextEdit_Announcement		= "AnnouncementTextEdit";

		QString pushButton_Login				= "pushButton_Login";
		QString pushButton_Register				= "pushButton_Register";

		QString label_UserName					= "label_UserName";
		QString label_UID						= "label_UID";
		QString label_Email						= "label_Email";
	}
	// 个性化页面
	namespace Personalization_ObjNameList {

	}

	namespace Funtion_ObjNameList {
		QString checkBox_show_cpu				= "checkBox_show_cpu";
		QString checkBox_show_gpu				= "checkBox_show_gpu";
		QString checkBox_show_memory			= "checkBox_show_memory";
		QString checkBox_show_cpu_thread		= "checkBox_show_cpu_thread";
		QString checkBox_show_gpu_thread		= "checkBox_show_gpu_thread";
		QString checkBox_show_memory_thread		= "checkBox_show_memory_thread";
		QString checkBox_mouse_follow			= "checkBox_mouse_follow";
		QString checkBox_Model_lip_shape		= "checkBox_Model_lip_shape";
		QString checkBox_Model_VA				= "checkBox_Model_VA";
		QString checkBox_Model_USE_HotKey		= "checkBox_Model_USE_HotKey";
		QString checkBox_errorLog				= "checkBox_errorLog";
		QString checkBox_dev					= "checkBox_dev";
		QString checkBox_useCustomJson			= "checkBox_useCustomJson";
		QString checkBox_model_window_top		= "checkBox_model_window_top";

		QString spinBox_flushtime_cpu			= "spinBox_flushtime_cpu";
		QString spinBox_flushtime_gpu			= "spinBox_flushtime_gpu";
		QString spinBox_flushtime_memory		= "spinBox_flushtime_memory";

		QString radioButton_mouse_follow_global = "radioButton_mouse_follow_global";
		QString radioButton_mouse_follow_local	= "radioButton_mouse_follow_local";

		QString ksEdit_hotkey					= "ksEdit_hotkey";

		QString pushButton_RegisterHotkey		= "pushButton_RegisterHotkey";

		QString qtabWidget_More					= "qtabWidget_More";

		QString combobox_show_message_style		= "combobox_show_message_style";
		QString comboBox_logLevel				= "comboBox_logLevel";

		QString label_show_message_style		= "label_show_message_style";
	}

	// 设置AI页面
	namespace SetAI_ObjNameList {
		QString pushButton_SaveConfig			= "pushButton_SaveConfig";
		QString pushButton_RestoreDefaultConfig = "pushButton_RestoreDefaultConfig";
		QString pushButton_SaveConfigToServer	= "pushButton_SaveConfigToServer";
		QString pushbutton_refresh				= "pushbutton_refresh";
		QString pushbutton_reload_ai			= "pushbutton_reload_ai";

		QString lineEdit_url					= "lineEdit_Url";
		QString lineEdit_url_extrapath			= "lineEdit_Url_extrapath";
		QString lineEdit_url_extrapath_model	= "lineEdit_Url_extrapath_model";
		QString lineEdit_api_key				= "lineEdit_ApiKey";
		QString lineEdit_n						= "lineEdit_n";
		QString lineEdit_maxtokens				= "lineEdit_maxtokens";

		QString plainTextEdit_prompt			= "plainTextEdit_prompt";
		QString plainTextEdit_system			= "plainTextEdit_system";
		QString plainTextEdit_n					= "plainTextEdit_n";
		QString plainTextEdit_maxtokens			= "plainTextEdit_maxtokens";
		QString plainTextEdit_URL				= "plainTextEdit_URL";
		QString plainTextEdit_ApiKey			= "plainTextEdit_ApiKey";
		QString plainTextEdit_customJson		= "plainTextEdit_customJson";
		QString plainTextEdit_addCustomField	= "plainTextEdit_addCustomField";

		QString comboBox_ModelList				= "comboBox_ModelList";

	    QString spinBox_n						= "spinBox_n";
		QString spinBox_maxtokens				= "spinBox_maxtokens";
		QString spinBox_Temperature				= "spinBox_Temperature";
		QString spinBox_top_p					= "spinBox_top_p";
		QString spinBox_frequency_penalty		= "spinBox_frequency_penalty";
		QString spinBox_presence_penalty		= "spinBox_presence_penalty";

		QString label_comboBoxStatus			= "label_comboBoxStatus";

		QString dspinBox_Temperature			= "dspinBox_Temperature";
		QString dspinBox_top_p					= "dspinBox_top_p";
		QString dspinBox_frequency_penalty		= "dspinBox_frequency_penalty";
		QString dspinBox_presence_penalty		= "dspinBox_presence_penalty";

		QString checkBox_n						= "checkBox_n";
		QString checkBox_append_n				= "checkBox_append_n";
		QString checkBox_maxtokens				= "checkBox_maxtokens";
		QString checkBox_append_maxtokens		= "checkBox_append_maxtokens";
		QString checkBox_AIPrompt_prompt		= "checkBox_AIPrompt_prompt";
		QString checkBox_AIPrompt_system		= "checkBox_AIPrompt_system";
		QString checkBox_frequency_penalty		= "checkBox_frequency_penalty";
		QString checkBox_append_frequency_penalty	= "checkBox_append_frequency_penalty";
		QString checkBox_presence_penalty		= "checkBox_presence_penalty";
		QString checkBox_append_presence_penalty	= "checkBox_append_presence_penalty";
		QString checkBox_useCustomJson			= "checkBox_useCustomJson";
		QString checkBox_AddCustomField			= "checkBox_AddCustomField";
		QString checkBox_temperature			= "checkBox_temperature";
		QString checkBox_top_p					= "checkBox_top_p";
		QString checkBox_append_temperature		= "checkBox_append_temperature"	;
		QString checkBox_append_top_p			= "checkBox_append_top_p";
		QString checkBox_append_stop			= "checkBox_append_stop";

		QString radioButton_temperature			= "radioButton_temperature";
		QString radioButton_top_p				= "radioButton_top_p";

		QString qtableWidget_Stop				= "qtabelWidget_stopwords";

		QString intSlider_n						= "intSlider_n";
		QString intSlider_max_tokens			= "intSlider_max_tokens";
	}
}