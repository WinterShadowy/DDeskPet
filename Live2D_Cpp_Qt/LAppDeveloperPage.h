
/*
 ============================================================================
 Header: LAppDeveloperPage.h
 Description: 开发者界面
 Start time: 2026.01.12
 End time: 2026.01.12
 Maintainer: WangSZ
 ============================================================================
*/

#pragma once

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QIntValidator>
#include <QPlainTextEdit>
#include <QRadioButton>
#include <QComboBox>

#include "LAppModelMessage.h"
#include "Configuration.h"

/*
 ============================================================================
 Class Name: Developer_Lips_Shape
 Description: 开发者界面 - 唇形相关的部分
 Start time: 2026.01.12
 End time: 2026.01.12
 Maintainer: WangSZ
 ============================================================================
*/
class Developer_Lips_Shape : public QWidget
{
	Q_OBJECT
public:
	Developer_Lips_Shape(QWidget* parent = nullptr);
	~Developer_Lips_Shape();

private slots:
	// slots - 唇形形状参数
	void on_slider_ParamMouthForm_valueChanged(int value);
	// slots - 嘴巴开度
	void on_slider_ParamMouthOpenY_valueChanged(int value);
	// slots - 选择WAV文件
	void on_button_chooseWavFile_clicked();
	// slots - 调试
	void on_button_debug_clicked();

private:
	QLabel* m_label_ParamMouthForm;		// 唇形形状参数
	QLabel* m_label_ParamMouthOpenY;	// 嘴巴开度
	QLabel* m_label_ParamMouthForm_Value; // 唇形形状 - 用于显示值
	QLabel* m_label_ParamMouthOpenY_Value; // 嘴巴开度 - 用于显示值
	QSlider* m_slider_ParamMouthForm;	// 唇形形状参数
	QSlider* m_slider_ParamMouthOpenY;	// 嘴巴开度参数

	QLineEdit* m_edit_wavFile;			// 用于显示路径的Edit
	QPushButton* m_button_chooseWavFile;	// 选择WAV文件
	QPushButton* m_button_debug;			// 调试嘴唇相关按钮

	QGroupBox* m_groupbox_main;	// 主布局
};

/*
 ============================================================================
 Class Name: Developer_Debug_AI_Page
 Description: 开发者界面 - 调试AI
 Start time: 2026.01.20
 End time: 2026.01.21
 Maintainer: WangSZ
 ============================================================================
*/
class Developer_Debug_AI_Page : public QWidget {
	Q_OBJECT
public:
	Developer_Debug_AI_Page(QWidget* parent = nullptr);
	~Developer_Debug_AI_Page();

private slots:
	void onCheckBoxClicked_MimiReqTemplate(bool checked);

private:
	QCheckBox* m_checkbox_minimum_request_tempate = nullptr;	// 最小请求模板
	QGroupBox* m_groupbox_main = nullptr;		// 主布局
};

/*
 ============================================================================
 Class Name: Developer_Debug_NetService
 Description: 开发者界面 - 调试AI
 Start time: 2026.01.20
 End time: 2026.01.21
 Maintainer: WangSZ
 ============================================================================
*/
class Developer_Debug_NetService : public QWidget {
	Q_OBJECT
public:
	Developer_Debug_NetService(QWidget* parent = nullptr);
	~Developer_Debug_NetService();

private slots:
	void onCheckBoxClicked_NoNetService(bool checked);
	void onLineEditListenPort_valueChanged(const QString& text);
private:
	
	QCheckBox* m_checkBox_no_NetService = nullptr;  // 是否启动网络服务进程
	QLabel* m_label_prompt_listen_port = nullptr;	// 提示 - 监听端口
	QLineEdit* m_lineEdit_listen_port = nullptr;	// 监听端口编辑
	QGroupBox* m_groupbox_main = nullptr;		// 主布局
};

/*
 ============================================================================
 Class Name: Developer_Debug_Plugins
 Description: 开发者界面 - 调试插件
 Start time: 2026.01.22
 End time: 2026.01.22
 Maintainer: WangSZ
 ============================================================================
*/
class Developer_Debug_Plugins : public QWidget {
	Q_OBJECT
public:
	Developer_Debug_Plugins(QWidget* parent = nullptr);
	~Developer_Debug_Plugins();

private slots:

	void onCheckBoxClicked_NoPluginsPage(bool checked);
	void onCheckBoxClicked_NoLoadPlugins(bool checked);
	void onCheckBoxClicked_NoLaunchAnyPlugins(bool checked);
private:

	QCheckBox* m_checkBox_no_plugin_page = nullptr;  // 是否启动网络服务进程
	QCheckBox* m_checkBox_no_load_page = nullptr;	// 是否加载插件
	QCheckBox* m_checkBox_no_launch_any_plugins = nullptr;	// 不启动任何插件
	QGroupBox* m_groupbox_main = nullptr;		// 主布局
};

/*
 ============================================================================
 Class Name: Developer_Debug_Models
 Description: 开发者界面 - 调试模型
 Start time: 2026.01.22
 End time: 2026.01.22
 Maintainer: WangSZ
 ============================================================================
*/
class Developer_Debug_Models : public QWidget {
	Q_OBJECT
public:
	Developer_Debug_Models(QWidget* parent = nullptr);
	~Developer_Debug_Models();

private slots:

	void onCheckBoxClicked_NoModelsPage(bool checked);
	void onCheckBoxClicked_NoLoadModels(bool checked);
private:

	QCheckBox* m_checkBox_no_models_page = nullptr;  // 是否启动网络服务进程
	QCheckBox* m_checkBox_no_load_models = nullptr;	// 是否加载插件
	QGroupBox* m_groupbox_main = nullptr;		// 主布局
};

/*
 ============================================================================
 Class Name: Developer_Debug_Models
 Description: 开发者界面 - 调试模型
 Start time: 2026.01.22
 End time: 2026.01.22
 Maintainer: WangSZ
 ============================================================================
*/
class Developer_Debug_ChatHistory : public QWidget {
	Q_OBJECT
public:
	Developer_Debug_ChatHistory(QWidget* parent = nullptr);
	~Developer_Debug_ChatHistory();

private slots:

	void onCheckBoxClicked_no_chat_history(bool checked);
private:

	QCheckBox* m_checkBox_no_models_page = nullptr;  // 是否加载聊天记录
	QGroupBox* m_groupbox_main = nullptr;		// 主布局
};

/*
============================================================================
 Class Name: Developer_Debug_ModelMessage
 Description: 开发者界面 - 消息调试
 Start time: 2026.08.10
 End time: 2026.08.12
 Maintainer: WangSZ
 ============================================================================
*/
class Developer_Debug_ModelMessage : public QWidget {
	Q_OBJECT
public:
	Developer_Debug_ModelMessage(QWidget* parent = nullptr);
	~Developer_Debug_ModelMessage();

signals:
	void showMessage(const QString& _jsonStr);
	void pluginMessage(const QString& _jsonStr);

private slots:
	void onComboboxChanged(int index);
	void onRadioChanged(bool checked);
	void onSendButtonClicked();		// 发送按钮点击

private:
	void InitComboBox(bool isLive2D);

private: 
	QGroupBox* m_groupbox_main = nullptr;
	QPushButton* m_btnShowMessage = nullptr;
	QComboBox* m_comboMessageTemp = nullptr;
	QPlainTextEdit* m_textEditMessage = nullptr;
	QRadioButton* m_radioLive2D = nullptr;
	QRadioButton* m_radioNetService = nullptr;
};