#pragma once

#pragma disable : 4996

#include "qhotkey_Manager.h"
#include "LAppPluginsManager.h"
#include "LAppLive2DModelsManager.h"
#include "LAppModelMessage.h"
#include "LAppQWidgetStyleManager.h"
#include "LAppCustomLabel.h"
#include "LAppCustomSlider.h"

#include <QFont>
#include <QWidget>
#include <QPushButton>
#include <QStyle>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QKeySequenceEdit>
#include <QIntValidator>
#include <QSplitter>
#include <QJsonObject>
#include <QDate>
#include <QScrollArea>

// ========== 插件列表项 ===========
class ListItemWidget : public QWidget
{
    Q_OBJECT
public:
    ListItemWidget(QWidget* parent = nullptr);
    explicit ListItemWidget(QString hotKey, QWidget* parent = nullptr);
    ~ListItemWidget() = default;

    bool SetIndex(short index);

    // Label
    // 设置插件描述信息
    void SetDecription(QString description);
    // 设置插件图标
    void SetPluginIcon(QString iconPath);
    // 设置插件版本号
    void SetVersion(QString version);
    // 设置插件名称
	void SetPluginName(QString pluginName);
    // 设置自动注册热键CheckBox的状态
    void SetAutoRegisterCheckBoxStatus(bool bStatus);
    // 设置自动启动热键CheckBox的状态
    void SetAutoStartCheckBoxStatus(bool bStatus);
    // QKeySequence
    bool SetHotKeyEditInfo(QString hotKeyInfo);
    // 设置hotkeyinfo成员
    void SetHotKeyInfo(QString hotKeyInfo);
    // 设置热键的注册状态
    bool SetHotKeyStatus(unsigned char status);
    // 动态调整Label大小
    void SetPluginLabelSize_AdjustSize();
    // 设置Index
    void SetItemIndex(short index);
    // 初始化热键操作按钮的状态
    void InitHotKeyOperationButtonStatus(bool bStatus);
    // 设置可执行文件路径
    void SetPluginExecutableFilePath(const QString  qstr_ExecutableFilePath);
public slots:
    void onRegisterHotKey();
    void onUnregisterHotKey();
    void onCheckBoxChanged_AutoRegister();
    void onCheckBoxChanged_AutoStart();
	void onSetKeyEditEnable();

private:
    void SetLabelStyle(QLabel* label, int index);
    void SetCheckBoxStyle(QCheckBox* checkBox, int index);
    void updateHotKeyStatus(bool isEnabled);
   
private:
    QLabel* m_Label_icon = nullptr;             // 图标
    QLabel* m_Label_PluginName = nullptr;       // 插件名称
    QLabel* m_Label_status = nullptr;           // 状态
    QLabel* m_Label_status_OK = nullptr;        // 状态 OK
    QLabel* m_Label_status_Fail = nullptr;      // 状态 Fail
    QLabel* m_Label_No_Registed_HotKey = nullptr;     // 未注册快捷键
	QLabel* m_Label_version = nullptr;			// 版本号
    QLabel* m_Label_description = nullptr;      // 描述   
    QPushButton* m_btn_RegisterHotKey = nullptr;    // 注册快捷键
	QPushButton* m_btn_UnregisterHotKey = nullptr;  // 取消注册快捷键   
    QPushButton* m_btn_SetKeyEditEnable = nullptr;   // 设置快捷键编辑框可用
    QCheckBox* m_checkBox_AutoRegisterHotKey = nullptr; // 自动注册
    QCheckBox* m_checkBox_AutoStartPLugin = nullptr;    // 自动启动
    QKeySequenceEdit* m_keyEdit_HotKey = nullptr;     // 快捷键编辑框
	QHotkey* m_hotkey = nullptr;
    QString icon_path;  // 图标路径
    QString file_Path;  // 可执行文件路径

    QString m_hotkeyInfo;
    bool enable_Register;
    bool enable_Unregister;

    short m_index = -1;
};
// ========== end ==========

// ========== 模型列表Widget ==========
class ListItemWidget_Live2DModel : public QWidget
{
    Q_OBJECT
public:
    ListItemWidget_Live2DModel() = delete;
    explicit ListItemWidget_Live2DModel(const QString& qstr_LModelFileName, const QString& qstr_LModelFolderName, QWidget* parent = nullptr);
    ~ListItemWidget_Live2DModel();
    
    // 设置下标
    void SetIndex(const short index);
    // 设置文件夹名
    void SetFolderName(const QString& qstr_FolderName);
    // 设置文件名
    void SetFileName(const QString& qstr_FileName);
	// 设置图标 - 文件夹
	void SetIcon_Folder(const QString& qstr_IconPath);
    // 设置图标 - 文件
    void SetIcon_File(const QString& qstr_IconPath);
private slots:
    void OnPushButton_SetModel();

private:
    MarqueeLabel* m_Label_ModelFolderName = nullptr;
    MarqueeLabel* m_Label_ModelFileName = nullptr;
    QLabel* m_Label_Icon_Folder = nullptr;
    QLabel* m_Label_Icon_File = nullptr;
    QPushButton* m_pushButton_SetModel = nullptr;
    QSplitter* m_splitter = nullptr;

    short m_index = -1;
};

// ========== end =========

// ========== 聊天记录页面 ===========
class DisplayChatHistoryWidget : public QWidget {
    Q_OBJECT
public:
    explicit DisplayChatHistoryWidget(QWidget* parent = nullptr);
    // 更新聊天记录
    void Update(const QJsonObject& dayRecord);
    // 返回聊天记录条数
    int MessageCount() const;

private:
    void ClearLayout();          // 清空旧消息
    QWidget* CreateMessageItem(const QJsonObject& msg); // 单条消息控件

private:
    QWidget* m_contentWidget = nullptr;
    QScrollArea* m_scrollArea = nullptr;
    QVBoxLayout* m_mainLay = nullptr;
    int m_msgCount = 0;
};

// ========== end ==========

