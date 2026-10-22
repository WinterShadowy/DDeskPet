#pragma once

#include "DDeskPetConfigInterfaceBase.h"
#include "DDeskPetConfig_Base.h"

// =========== 功能页面配置 ==========
class Config_Function : virtual public Page_Config_Base {
public:
	Config_Function() = default;
	Config_Function(QString qstr_FilePath);
	~Config_Function();

	//  ==== Get ====

	// 是否使用鼠标跟随
	bool IsMouseMoveFollow();
	// 鼠标跟随模式 
	int GetMouseMoveFollowMode();
	// 是否是全局追踪模式
	bool IsMouseMoveFollowGlobal();
	// 是否是局部追踪模式
	bool IsMouseMoveFollowLocal();
	// 是否使用运行时错误日志
	bool IsRuntimeErrorLog();
	// 获取日志级别
	int GetLogLevel();
	// 是否使用语音助手
	bool IsUseVA();
	// 是否使用快捷键唤醒语音助手
	bool IsShortcut_Key_Activate_VA();
	// 是否使用自动登录
	bool IsAutoLogin();
	// 是否自动保存聊天记录
	bool IsAutoSaveChatRecord();
	// 是否在提示框下方展示CPU占用
	bool IsShow_CPU_Usage();
	// 是否在提示框下方展示内存占用
	bool IsShow_Memory_Usage();
	// 是否使用多线程计算CPU占用
	bool IsUse_MultiThread_CPU_Usage();
	// 多线程计算CPU占用刷新时间
	unsigned int GetThreadFlushTime_CPU();
	// 是否使用多线程计算内存占用
	bool IsUse_MultiThread_Memory_Usage();
	// 多线程计算内存占用刷新时间
	unsigned int GetThreadFlushTime_Memory();
	// 显示消息的样式
	int GetShow_Message_Style();
	// 是否使用唇形同步
	bool IsUse_Model_lip_shape();
	// AI对话是否使用上下文
	bool IsUse_Context();
	// 是否使用开发者模式
	bool IsUseDeveloperMode();
	// 是否模型窗口置顶
	bool IsModelWindowAllwaysOnTop();
	// 是否使用默认的模型窗口大小
	bool IsUseDefaultModelWindowSize();
	// 获取窗口缩放因子
	float GetWindowZoomFactor();
	// 获取默认窗口高度
	int GetDefaultModelWindowHeight();
	// 获取默认窗口宽度
	int GetDefaultModelWindowWidth();


	// ==== Set ====
	// 设置是否使用鼠标跟随
	void SetUseMouseMoveFollow(bool status);
	// 设置鼠标跟随模式
	void SetMouseMoveFollowMode(int mode);
	// 设置是否是全局追踪模式
	void SetMouseMoveFollowGlobal(bool status);
	// 设置是否是局部追踪模式
	void SetMouseMoveFollowLocal(bool status);
	// 设置是否使用运行时错误日志
	void SetUseRuntimeErrorLog(bool status);
	// 设置日志级别
	void SetRuntimeLogLevel(QString level);
	// 设置是否使用语音助手
	void SetUseVA(bool status);
	// 设置是否使用快捷键唤醒语音助手
	void SetUseShortcut_Key_Activate_VA(bool status);
	// 设置是否使用自动登录
	void SetUseAutoLogin(bool status);
	// 设置是否自动保存聊天记录
	void SetUseAutoSaveChatRecord(bool status);
	// 设置是否在提示框下方展示CPU占用
	void SetShow_CPU_Usage(bool status);
	// 设置是否在提示框下方展示内存占用
	void SetShow_Memory_Usage(bool status);
	// 设置是否使用多线程计算CPU占用
	void SetUse_MultiThread_CPU_Usage(bool status);
	// 设置多线程计算CPU占用刷新时间
	void SetThreadFlushTime_CPU(unsigned int time);
	// 设置是否使用多线程计算内存占用
	void SetUse_MultiThread_Memory_Usage(bool status);
	// 设置多线程计算内存占用刷新时间
	void SetThreadFlushTime_Memory(unsigned int time);
	// 设置显示消息的样式
	void SetShowMessageStyle(int style);
	// 设置是否使用唇形同步
	void SetUse_Model_lip_shape(bool status);
	// AI对话是否使用上下文
	void SetUse_Context(bool status);
	// 设置使用开发者模式
	void SetUseDeveloperMode(bool status);
	// 设置模型窗口置顶
	void SetModelWindowAllwaysOnTop(bool status);
	// 设置使用默认的模型窗口大小
	void SetUseDefaultModelWindowSize(bool status);
	// 设置窗口缩放因子
	void SetWindowZoomFactor(float factor);
	// 设置默认窗口高度
	void SetDefaultModelWindowHeight(int height);
	// 设置默认窗口宽度
	void SetDefaultModelWindowWidth(int width);

	static Config_Function& instance();

	virtual void Initialize() override;
};
// ========== end ==========