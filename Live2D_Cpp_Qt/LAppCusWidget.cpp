#include "LAppCusWidget.h"
#include <QMessageBox>
#include <QFileDialog>

// ========== ListItemWidget =========== ListItemWidget =========== ListItemWidget =========== ListItemWidget =========== ListItemWidget ===========
ListItemWidget::ListItemWidget(QWidget* parent)
    : QWidget(parent)
{
    // 插件描述
    this->m_Label_description = new QLabel("更多");
    this->m_Label_description->setAlignment(Qt::AlignCenter);
    this->m_Label_description->setFixedHeight(16);
    this->m_Label_description->setFixedWidth(25);
    SetLabelStyle(this->m_Label_description, 15);
    // 是否启用自动注册热键
    this->m_checkBox_AutoRegisterHotKey = new QCheckBox("自动注册热键");
	this->m_checkBox_AutoRegisterHotKey->setChecked(false);
    SetCheckBoxStyle(this->m_checkBox_AutoRegisterHotKey, 2); // 设置样式
    this->m_checkBox_AutoStartPLugin = new QCheckBox("启动时自动启动");
	this->m_checkBox_AutoStartPLugin->setChecked(false);
	SetCheckBoxStyle(this->m_checkBox_AutoStartPLugin, 2);
    QVBoxLayout* vLayout_Dec_AutoRegisterHotkey = new QVBoxLayout();
    vLayout_Dec_AutoRegisterHotkey->addWidget(this->m_checkBox_AutoRegisterHotKey);
    vLayout_Dec_AutoRegisterHotkey->addWidget(this->m_checkBox_AutoStartPLugin);
    
    
    this->m_Label_PluginName = new QLabel("未知");  // 插件名称
    SetLabelStyle(this->m_Label_PluginName, 3);
    SetPluginLabelSize_AdjustSize();
    QHBoxLayout* hLayout_PluginName = new QHBoxLayout();
	hLayout_PluginName->addWidget(this->m_Label_PluginName);
    hLayout_PluginName->addWidget(this->m_Label_description);

    hLayout_PluginName->addStretch();
    QVBoxLayout* vLayout_PluginName = new QVBoxLayout();
    vLayout_PluginName->addLayout(hLayout_PluginName);
	vLayout_PluginName->addLayout(vLayout_Dec_AutoRegisterHotkey);

	this->m_Label_icon = new QLabel();

    // 热键注销/注册
    this->m_btn_RegisterHotKey = new QPushButton("注册热键");
    this->m_btn_UnregisterHotKey = new QPushButton("注销热键");
    this->m_btn_SetKeyEditEnable = new QPushButton("编辑热键");
    QHBoxLayout* hLayout_btn = new QHBoxLayout();
	hLayout_btn->addWidget(this->m_btn_RegisterHotKey);
	hLayout_btn->addWidget(this->m_btn_UnregisterHotKey);
	hLayout_btn->addWidget(this->m_btn_SetKeyEditEnable);

    // 热键输入框
    this->m_keyEdit_HotKey = new QKeySequenceEdit(QKeySequence("Ctrl+F1"));
    QVBoxLayout* vLayout_AboutHotkeyRegis = new QVBoxLayout();
	vLayout_AboutHotkeyRegis->addWidget(this->m_keyEdit_HotKey);
	vLayout_AboutHotkeyRegis->addLayout(hLayout_btn);
    
    // 插件信息栏布局
    QHBoxLayout* hLayout_PluginInfo = new QHBoxLayout();
	hLayout_PluginInfo->addWidget(this->m_Label_icon);
	hLayout_PluginInfo->addLayout(vLayout_PluginName);
	hLayout_PluginInfo->addLayout(vLayout_AboutHotkeyRegis);
    
    // 热键状态
    this->m_Label_status = new QLabel("快捷键状态");
    this->m_Label_status->setFixedHeight(9);
    this->m_Label_status->adjustSize();
    this->m_Label_status->setFixedHeight(15);
    SetLabelStyle(this->m_Label_status, 20);    // 设置样式

    this->m_Label_No_Registed_HotKey = new QLabel("未注册");
    this->m_Label_No_Registed_HotKey->setFixedHeight(10);
    this->m_Label_No_Registed_HotKey->adjustSize();
    this->m_Label_status_OK = new QLabel("已注册");
    this->m_Label_status_OK->setFixedHeight(10);
    this->m_Label_status_OK->adjustSize();
    this->m_Label_status_Fail = new QLabel("注册失败");
	this->m_Label_status_Fail->setFixedHeight(10);
	this->m_Label_status_Fail->adjustSize();
    QVBoxLayout* vLayout_status = new QVBoxLayout();
	vLayout_status->addWidget(this->m_Label_No_Registed_HotKey);
	vLayout_status->addWidget(this->m_Label_status_OK);
	vLayout_status->addWidget(this->m_Label_status_Fail);
    // 热键版本
    this->m_Label_version = new QLabel("v0.0.0");
    this->m_Label_version->setFixedHeight(12);
    this->m_Label_version->adjustSize();
    SetLabelStyle(this->m_Label_version, 5);
    QFrame* lineSeparator = new QFrame();
    lineSeparator->setFrameShape(QFrame::HLine); // 设置为水平线
    lineSeparator->setFrameShadow(QFrame::Sunken); // 选择线的阴影样式
    lineSeparator->setStyleSheet("background-color: #909090;"); // 设置线的颜色
    QVBoxLayout* vLayout_status_version = new QVBoxLayout();
    // 添加 QLabel 之前，先把 lineSeparator 添加到主布局
	vLayout_status_version->addWidget(this->m_Label_status);
    vLayout_status_version->addLayout(vLayout_status);
    vLayout_status_version->addWidget(this->m_Label_version);

    // 总布局
	QHBoxLayout* hLayout = new QHBoxLayout(this);
	hLayout->addLayout(hLayout_PluginInfo);
	hLayout->addLayout(vLayout_status_version);
	this->setLayout(hLayout);

    // 信号连接
    connect(this->m_btn_RegisterHotKey, &QPushButton::clicked, this, &ListItemWidget::onRegisterHotKey);
    connect(this->m_btn_UnregisterHotKey, &QPushButton::clicked, this, &ListItemWidget::onUnregisterHotKey);
    connect(this->m_btn_SetKeyEditEnable, &QPushButton::clicked, this, &ListItemWidget::onSetKeyEditEnable);
    connect(this->m_checkBox_AutoRegisterHotKey, &QCheckBox::checkStateChanged, this, &ListItemWidget::onCheckBoxChanged_AutoRegister);
    connect(this->m_checkBox_AutoStartPLugin, &QCheckBox::checkStateChanged, this, &ListItemWidget::onCheckBoxChanged_AutoStart);
    updateHotKeyStatus(false);  // 初始化为禁用状态
}

ListItemWidget::ListItemWidget(QString hotKey, QWidget* parent) :
	ListItemWidget(parent)
{
}

bool ListItemWidget::SetIndex(short index)
{
    if(index < 0) 
        return false;
    this->m_index = index;
    return true;
}

void ListItemWidget::SetDecription(QString description)
{
    this->m_Label_description->setToolTip(description);
    return;
}

void ListItemWidget::SetPluginIcon(QString iconPath)
{
    if (!this->m_Label_icon) 
        return;

    QPixmap pixmap(iconPath); // 加载图标
    if (pixmap.isNull())
    {
        // 如果图标加载失败，可以给出相应的处理
        RuntimeLog_Debug(QString("Icon load failed:%1").arg(iconPath).toStdString().c_str());
        return;
    }

    this->m_Label_icon->setPixmap(pixmap); // 设置 label_icon 的 pixmap
    this->m_Label_icon->setScaledContents(true); // 使图像按比例缩放适应标签
    this->m_Label_icon->setFixedSize(QSize(50, 50)); // 设置 QLabel 的固定大小
    return;
}

void ListItemWidget::SetVersion(QString version)
{
    this->m_Label_version->setText(version);
	return;
}

void ListItemWidget::SetPluginName(QString pluginName)
{
    this->m_Label_PluginName->setText(pluginName);
    return;
}

void ListItemWidget::SetAutoRegisterCheckBoxStatus(bool bStatus)
{
	this->m_checkBox_AutoRegisterHotKey->setChecked(bStatus);
	return;
}

void ListItemWidget::SetAutoStartCheckBoxStatus(bool bStatus)
{
    this->m_checkBox_AutoStartPLugin->setChecked(bStatus);
    return;
}

void ListItemWidget::onRegisterHotKey()
{
    QString text = this->m_btn_RegisterHotKey->text();
    // 如果是注册热键，就注册热键
    if (text == "注册热键")
    {
        QString hotKey = this->m_keyEdit_HotKey->keySequence().toString();
        GetPluginsManagerInstance().SetHotKey(this->m_index, hotKey);
        // 保存当前状态
        if (HotKeyManager::instance()->RegisterHotKey(hotKey, this->file_Path))
        {
            SetHotKeyStatus(2);
            updateHotKeyStatus(true); // 更新状态为启用
            this->enable_Register = this->m_btn_RegisterHotKey->isEnabled();
            this->enable_Unregister = this->m_btn_UnregisterHotKey->isEnabled();
        }
        else
        {
            SetHotKeyStatus(1);
            ModelWindowPromptWindow::ShowMessageOnModelWindow(11, "注册失败啦！\n原因可能是热键已经被占用", 3000, true);
        }
    }
    else if (text == "保存编辑")
    {
        this->m_btn_RegisterHotKey->setText("注册热键");
        this->m_btn_SetKeyEditEnable->setText("编辑热键");
        updateHotKeyStatus(false);
    }
    else if (text == "保存并注册")
    {
        QString new_hotKey = this->m_keyEdit_HotKey->keySequence().toString();
        GetPluginsManagerInstance().SetHotKey(this->m_index, new_hotKey);
        this->m_btn_RegisterHotKey->setText("注册热键");
        this->m_btn_SetKeyEditEnable->setText("编辑热键");
        if (HKM_ChangeHotkey(this->m_hotkeyInfo, new_hotKey))
        {
            SetHotKeyStatus(2);
			ModelWindowPromptWindow::ShowMessageOnModelWindow(11, "热键修改成功了哦！", 3000, true);
        }
        else
        {
            SetHotKeyStatus(1);
			ModelWindowPromptWindow::ShowMessageOnModelWindow(11, "热键修改失败啦！", 3000, true);
        }
        this->m_hotkeyInfo = new_hotKey;
		updateHotKeyStatus(true);
    }
}

// 注销热键
void ListItemWidget::onUnregisterHotKey()
{
    QString hotKey = this->m_keyEdit_HotKey->keySequence().toString();
    if (HotKeyManager::instance()->UnregisterHotKey(hotKey))
    {
        this->m_hotkeyInfo = hotKey;
        SetHotKeyStatus(3);
        updateHotKeyStatus(false); // 更新状态为禁用
    }
    else
    {
        SetHotKeyStatus(2);
        // 提示注销失败
        ModelWindowPromptWindow::ShowMessageOnModelWindow(11, QString("热键注销失败啦！"), 3000, true);
    }
}

bool ListItemWidget::SetHotKeyEditInfo(QString hotKeyInfo)
{
    if (hotKeyInfo.isEmpty())
    {
        return false;
    }
	this->m_keyEdit_HotKey->setKeySequence(QKeySequence(hotKeyInfo));
    return true;
}

void ListItemWidget::SetHotKeyInfo(QString hotKeyInfo)
{
    this->m_hotkeyInfo = hotKeyInfo;
    return;
}

bool ListItemWidget::SetHotKeyStatus(unsigned char status)
{
    if (status == 1)  // 注册失败
    {
        SetLabelStyle(this->m_Label_status_Fail, 16);
        SetLabelStyle(this->m_Label_status_OK, 17);
        SetLabelStyle(this->m_Label_No_Registed_HotKey, 17);

    }
    else if (status == 2) // 注册成功
    {
        SetLabelStyle(this->m_Label_status_Fail, 17);
        SetLabelStyle(this->m_Label_status_OK, 16);
		SetLabelStyle(this->m_Label_No_Registed_HotKey, 17);
    }
    else if (status == 3) // 未注册热键
    {
		SetLabelStyle(this->m_Label_status_Fail, 17);
		SetLabelStyle(this->m_Label_status_OK, 17);
		SetLabelStyle(this->m_Label_No_Registed_HotKey, 16);
    }
    else
    {
        return false;
    }
    return true;
}

void ListItemWidget::SetPluginLabelSize_AdjustSize()
{
    QFontMetrics fm(this->m_Label_PluginName->font());
    QSize textSize = fm.size(Qt::TextSingleLine, this->m_Label_PluginName->text());
    int pad = 3 + 1;            // padding + border
    this->m_Label_PluginName->setFixedSize(textSize.width() + pad * 2, textSize.height() + pad * 2);
    return;
}

void ListItemWidget::SetItemIndex(short index)
{
    this->m_index = index;
    return;
}

void ListItemWidget::InitHotKeyOperationButtonStatus(bool bStatus)
{
    updateHotKeyStatus(bStatus);
    return;
}

void ListItemWidget::SetPluginExecutableFilePath(const QString qstr_ExecutableFilePath)
{
    this->file_Path = qstr_ExecutableFilePath;
    return;
}

void ListItemWidget::onSetKeyEditEnable()
{
    this->m_hotkeyInfo = this->m_keyEdit_HotKey->keySequence().toString();
    QString text = this->m_btn_SetKeyEditEnable->text();

    if (text == "编辑热键")
    {
        // 保存当前状态
        this->enable_Register = this->m_btn_RegisterHotKey->isEnabled();
        this->enable_Unregister = this->m_btn_UnregisterHotKey->isEnabled();
        // 设置新的按钮状态
        this->m_btn_SetKeyEditEnable->setText("取消编辑"); 
        this->m_btn_RegisterHotKey->setEnabled(true);
        if(this->enable_Register)
        {
            this->m_btn_RegisterHotKey->setText("保存编辑");
        }
        else
        {
			this->m_btn_RegisterHotKey->setText("保存并注册");
        }
        this->m_btn_UnregisterHotKey->setEnabled(false);
		this->m_keyEdit_HotKey->setEnabled(true);
	}
    else if (text == "取消编辑")
    {
        this->m_btn_RegisterHotKey->setEnabled(this->enable_Register);
        this->m_btn_UnregisterHotKey->setEnabled(this->enable_Unregister);
		this->m_btn_SetKeyEditEnable->setText("编辑热键");
        this->m_btn_RegisterHotKey->setText("注册热键");
		this->m_keyEdit_HotKey->setEnabled(false);
        this->m_keyEdit_HotKey->setKeySequence(QKeySequence(this->m_hotkeyInfo));
    }
}

// 热键自动注册
void ListItemWidget::onCheckBoxChanged_AutoRegister()
{
    bool status = this->m_checkBox_AutoRegisterHotKey->isChecked();
    if (GetPluginsManagerInstance().SetAutoRegister(this->m_index, status) == false)
    {
        RuntimeLog_Error(QString("Hotkey automatic registration failed").toStdString().c_str());
    }
}

void ListItemWidget::onCheckBoxChanged_AutoStart()
{
    bool status = this->m_checkBox_AutoStartPLugin->isChecked();
	if (GetPluginsManagerInstance().SetAutoStartStatus(this->m_index, status) == false)
	{
		RuntimeLog_Error(QString("Auto start setting failed").toStdString().c_str());
	}
    return;
}

void ListItemWidget::SetLabelStyle(QLabel* label, int index)
{
    static const QString tbl[25] = {
        // 0
        "QLabel{ background:#ffffff; color:#202020; font:8pt; border:none; }",
        // 1
        "QLabel{ background:#f5f5f5; color:#202020; font:8pt; border-radius:4px; padding:4px; }",
        // 2
        "QLabel{ background:#e3f2fd; color:#1565c0; font:8pt; border:1px solid #90caf9; border-radius:0px; padding:0px; }",
        // 3
        "QLabel{ background:#e8f5e9; color:#2e7d32; font:8pt; border:1px solid #a5d6a7; border-radius:0px; padding:0px; }",
        // 4
        "QLabel{ background:#fff8e1; color:#f57c00; font:8pt; border:1px solid #ffe082; border-radius:0px; padding:0px; }",
        // 5
        "QLabel{ background:#ffebee; color:#c62828; font:7pt; border:1px solid #ef9a9a; padding:0px; }",
        // 6
        "QLabel{ background:transparent; color:#303030; font: bold 12pt; }",
        // 7
        "QLabel{ background:#dcdcdc; max-height:1px; }",
        // 8
        "QLabel{ background:#424242; color:#fafafa; font:9pt; border-radius:4px; padding:4px; }",
        // 9
        "QLabel{ background:#1976d2; color:#ffffff; font:9pt; border-radius:4px; padding:4px; }",
        //10
        "QLabel{ background:#e0e0e0; color:#424242; font:9pt; border-radius:8px; padding:4px 8px; }",
        //11
        "QLabel{ background:#fff; color:#757575; font:9pt; border:1px solid #bdbdbd; border-radius:3px; }",
        //12
        "QLabel{ background:#fafafa; color:#202020; font:9pt; border:1px solid #e0e0e0; border-radius:3px; padding:3px; }",
        //13
        "QLabel{ background:transparent; color:#202020; font:9pt; border-bottom:1px solid #e0e0e0; padding:4px 0px; }",
        //14
        "QLabel{ background:qlineargradient(x1:0,y1:0,x2:1,y2:0,stop:0 #64b5f6,stop:1 #1976d2); color:#ffffff; font: bold 7pt; border-radius:4px; padding:2px; }",
        // 15: "?" Label`s tooltip style
        "QLabel{ border: 1px solid #d0d0d0; border-radius: 4px; padding: 1px; background-color: #f5f5f5; font-size: 10px;}",
        // 16 运行/启动
        "QLabel{ background:#4caf50; color:#ffffff; font:bold 6pt; border:1px solid #388e3c; border-radius:12px;}",
        // 17 停止/离线
        "QLabel{ background:#9e9e9e; color:#ffffff; font:bold 6pt; border:1px solid #616161; border-radius:12px;}",
        // 18 告警
        "QLabel{ background:#ff9800; color:#ffffff; font:bold 6pt; border:1px solid #f57c00; border-radius:12px;}",
        // 19 故障
        "QLabel{ background:#f44336; color:#ffffff; font:bold 6pt; border:1px solid #d32f2f; border-radius:12px;}",
        // 20 未初始化
        "QLabel{ background:#eceff1; color:#546e7a; font:bold 8pt; border:1px solid #90a4ae; border-radius:12px; qproperty-alignment: AlignCenter;}"
    };

    if (index >= 0 && index < 21)
        label->setStyleSheet(tbl[index]);
}

void ListItemWidget::SetCheckBoxStyle(QCheckBox* checkBox, int index)
{
    /* 可选：如果有勾图标，取消注释并把路径改为 :/icons/check_white.png */
            // QCheckBox::indicator:checked {
            //     image: url(:/icons/check_white.png);
            // }
    QString styles[25] = {
        "QCheckBox { color: #333333; font-size: 13px; spacing: 8px; } \
             QCheckBox:unchecked { color: #6b6b6b; } \
             QCheckBox:checked { color: #1f6feb; font-weight: 600; } \
             QCheckBox::indicator { width: 18px; height: 18px; border-radius: 4px; border: 1px solid #cfcfcf; background: #ffffff; } \
             QCheckBox::indicator:hover { border: 1px solid #b3b3b3; } \
             QCheckBox::indicator:checked { border: 1px solid #1f6feb; background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4ba0ff, stop:1 #1f6feb); } \
             QCheckBox:disabled { color: #9a9a9a; } \
             QCheckBox::indicator:disabled { background: #f5f5f5; border: 1px solid #e0e0e0;}",
        R"(
            QCheckBox { color: #2c2c2c; font-size: 13px; spacing: 8px; }
            QCheckBox:unchecked { color: #7a7a7a; }
            QCheckBox:checked { color: #239a45; font-weight: 600; }
            QCheckBox::indicator { width: 16px; height: 16px; border-radius: 3px; border: 1px solid #dcdcdc; background: #ffffff; }
            QCheckBox::indicator:checked { background: #27ae60; border: 1px solid #1f8f4a; }
        )",
        // 2: 极简圆形（低调）
        R"(
            QCheckBox { color: #333333; font-size: 13px; spacing: 8px; }
            QCheckBox:unchecked { color: #8c8c8c; }
            QCheckBox::indicator { width: 14px; height: 14px; border-radius: 8px; border: 1px solid #d6d6d6; background: #ffffff; }
            QCheckBox::indicator:checked { background: #50577a; border: 1px solid #3f4460; }
        )",

        // 3: 开关样式（模拟轻量开关）
        R"(
            QCheckBox { color: #333333; font-size: 13px; spacing: 10px; }
            QCheckBox::indicator { width: 34px; height: 18px; border-radius: 9px; background: #e9e9e9; border: 1px solid #dcdcdc; }
            QCheckBox::indicator:checked { background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 #7bd389, stop:1 #3db048); border: 1px solid #38a444; }
            /* 圆点：使用 background-position 模拟（较简单）*/
            QCheckBox::indicator { background-repeat: no-repeat; background-position: left center; background-image: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:0,
                stop:0 rgba(255,255,255,1) ); }
            QCheckBox::indicator:checked { background-position: right center; }
        )",

        // 4: 中性灰 + 柔和蓝（适合设置页）
        R"(
            QCheckBox { color: #444444; font-size: 13px; spacing: 8px; }
            QCheckBox:unchecked { color: #888888; }
            QCheckBox::indicator { width: 17px; height: 17px; border-radius: 4px; border: 1px solid #e0e0e0; background: #fbfbfb; }
            QCheckBox::indicator:checked { background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #cfe9ff, stop:1 #8cc5ff); border: 1px solid #7ab6ff; }
        )",

        // 5: 单色暗调（稳重、专业）
        R"(
            QCheckBox { color: #2f2f2f; font-size: 13px; spacing: 8px; }
            QCheckBox:unchecked { color: #6f6f6f; }
            QCheckBox::indicator { width: 16px; height: 16px; border-radius: 3px; border: 1px solid #bfbfbf; background: #fff; }
            QCheckBox::indicator:checked { background: #444444; border: 1px solid #333333; }
        )",
        // 6: 薄荷绿
        "QCheckBox { color: #333; font-size: 13px; spacing: 8px; } \
            QCheckBox::indicator { width: 18px; height: 18px; border-radius: 4px; border: 1px solid #c1d7c0; background: #ffffff; } \
            QCheckBox::indicator:hover { border-color: #8bc34a; } \
            QCheckBox::indicator:checked { border: 1px solid #66bb6a; background: qlineargradient(x1:0 y1:0 x2:0 y2:1 stop:0 #81c784 stop:1 #66bb6a); } \
            QCheckBox:checked { color: #2e7d32; font-weight: 600; } \
            QCheckBox:disabled { color: #9e9e9e; } \
            QCheckBox::indicator:disabled { background: #f5f5f5; border: 1px solid #e0e0e0; }",

        // 7 烟灰极简
        "QCheckBox { color: #424242; font-size: 13px; spacing: 8px; } \
             QCheckBox::indicator { width: 18px; height: 18px; border-radius: 4px; border: 1px solid #bdbdbd; background: #fafafa; } \
             QCheckBox::indicator:hover { background: #eeeeee; } \
             QCheckBox::indicator:checked { background: #9e9e9e; border-color: #757575; } \
             QCheckBox:checked { color: #212121; font-weight: 600; } \
             QCheckBox:disabled { color: #9e9e9e; } \
             QCheckBox::indicator:disabled { background: #f5f5f5; border: 1px solid #e0e0e0; }",

        // 8 柔雾紫
        "QCheckBox { color: #333; font-size: 13px; spacing: 8px; } \
            QCheckBox::indicator { width: 18px; height: 18px; border-radius: 4px; border: 1px solid #d1c4e9; background: #ffffff; } \
            QCheckBox::indicator:hover { border-color: #9575cd; } \
            QCheckBox::indicator:checked { border: 1px solid #7e57c2; background: qlineargradient(x1:0 y1:0 x2:0 y2:1 stop:0 #b39ddb stop:1 #7e57c2); } \
            QCheckBox:checked { color: #5e35b1; font-weight: 600; } \
            QCheckBox:disabled { color: #9e9e9e; } \
            QCheckBox::indicator:disabled { background: #f5f5f5; border: 1px solid #e0e0e0; }",

        // 9 暖橙活力
        "QCheckBox { color: #333; font-size: 13px; spacing: 8px; } \
             QCheckBox::indicator { width: 18px; height: 18px; border-radius: 4px; border: 1px solid #ffcc80; background: #ffffff; } \
             QCheckBox::indicator:hover { border-color: #ffa726; } \
             QCheckBox::indicator:checked { border: 1px solid #fb8c00; background: qlineargradient(x1:0 y1:0 x2:0 y2:1 stop:0 #ffb74d stop:1 #fb8c00); } \
             QCheckBox:checked { color: #ef6c00; font-weight: 600; } \
             QCheckBox:disabled { color: #9e9e9e; } \
             QCheckBox::indicator:disabled { background: #f5f5f5; border: 1px solid #e0e0e0; }"
    };
    if (index < 0 || index >= 25)
    {
        return;
    }
    checkBox->setStyleSheet(styles[index]);
    return;
}

void ListItemWidget::updateHotKeyStatus(bool isEnabled)
{
    // 更新按钮状态
    this->m_btn_RegisterHotKey->setEnabled(!isEnabled);
    this->m_btn_UnregisterHotKey->setEnabled(isEnabled);
}

// ========== end ========== end ========== end =========== end =========== end =========== end ========== end =========== end ==========

// ========== ListItemWidget_Live2DModel =========== ListItemWidget_Live2DModel =========== ListItemWidget_Live2DModel ===========
ListItemWidget_Live2DModel::ListItemWidget_Live2DModel(const QString& qstr_LModelFileName, const QString& qstr_LModelFolderName , QWidget* parent) :
    QWidget(parent)
{
    this->m_Label_Icon_Folder = new QLabel();
    this->SetIcon_Folder(Config_Model::instance().GetIcon_Folder());
    this->m_Label_ModelFolderName = new MarqueeLabel(this, 2);
    this->m_Label_ModelFolderName->setText(qstr_LModelFolderName);
    LAppStyleManager::SetLabelStyle(this->m_Label_ModelFolderName, 23);

    QHBoxLayout* hlayout_FolderName = new QHBoxLayout();
    hlayout_FolderName->addWidget(this->m_Label_Icon_Folder);
    hlayout_FolderName->addWidget(this->m_Label_ModelFolderName);
    
    QWidget* leftWidget = new QWidget();
    leftWidget->setLayout(hlayout_FolderName);
    leftWidget->setFixedWidth(190);

    this->m_Label_Icon_File = new QLabel();
    this->SetIcon_File(Config_Model::instance().GetIcon_File());
    this->m_Label_ModelFileName = new MarqueeLabel(this, 2);
    this->m_Label_ModelFileName->setText(qstr_LModelFileName);
    LAppStyleManager::SetLabelStyle(this->m_Label_ModelFileName, 23);
	QHBoxLayout* hlayout_FileName = new QHBoxLayout(this);
	hlayout_FileName->addWidget(this->m_Label_Icon_File);
	hlayout_FileName->addWidget(this->m_Label_ModelFileName);

    QWidget* rightWidget = new QWidget();
	rightWidget->setLayout(hlayout_FileName);
    rightWidget->setFixedWidth(190);
    
    this->m_splitter = new QSplitter(Qt::Horizontal, this);
    this->m_splitter->addWidget(leftWidget);
    this->m_splitter->addWidget(rightWidget);
    this->m_splitter->setStretchFactor(0, 1);   // 左右都可伸缩
    this->m_splitter->setStretchFactor(1, 1);
    this->m_splitter->setHandleWidth(2);
    this->m_splitter->setStyleSheet(
        "QSplitter::handle { background: #aaa; width: 2px; }"
        "QSplitter::handle:horizontal { height: 2px; }");

    this->m_pushButton_SetModel = new QPushButton("使用模型");
    LAppStyleManager::SetPushButtonStyle(this->m_pushButton_SetModel, 0);

    // 总布局
    QHBoxLayout* hlayout = new QHBoxLayout(this);
	hlayout->addWidget(this->m_splitter, 4);
	hlayout->addWidget(this->m_pushButton_SetModel, 1);
    hlayout->setContentsMargins(1, 1, 1, 1);
    connect(this->m_pushButton_SetModel, &QPushButton::clicked, this, &ListItemWidget_Live2DModel::OnPushButton_SetModel);
}

ListItemWidget_Live2DModel::~ListItemWidget_Live2DModel()
{
}

void ListItemWidget_Live2DModel::SetIndex(const short index)
{
    this->m_index = index;
    return;
}

void ListItemWidget_Live2DModel::SetFolderName(const QString& qstr_FolderName)
{
    if(this->m_Label_ModelFolderName)
    {
        this->m_Label_ModelFolderName->setText(qstr_FolderName);
    }
    return;
}

void ListItemWidget_Live2DModel::SetFileName(const QString& qstr_FileName)
{
    if(this->m_Label_ModelFileName)
    {
        this->m_Label_ModelFileName->setText(qstr_FileName);
    }
    return;
}

void ListItemWidget_Live2DModel::SetIcon_Folder(const QString& qstr_IconPath)
{
    if (!this->m_Label_Icon_Folder)
        return;

    QPixmap pixmap(qstr_IconPath); // 加载图标
    if (pixmap.isNull())
    {
        // 如果图标加载失败，可以给出相应的处理
        RuntimeLog_Debug(QString("Icon load failed: %1").arg(qstr_IconPath).toStdString().c_str());
        return;
    }

    this->m_Label_Icon_Folder->setPixmap(pixmap); // 设置 label_icon 的 pixmap
    this->m_Label_Icon_Folder->setScaledContents(true); // 使图像按比例缩放适应标签
    this->m_Label_Icon_Folder->setFixedSize(QSize(25, 25)); // 设置 QLabel 的固定大小
    return;
}

void ListItemWidget_Live2DModel::SetIcon_File(const QString& qstr_IconPath)
{
    if (!this->m_Label_Icon_File)
        return;

    QPixmap pixmap(qstr_IconPath); // 加载图标
    if (pixmap.isNull())
    {
        // 如果图标加载失败，可以给出相应的处理
        RuntimeLog_Debug(QString("Icon load failed: %1").arg(qstr_IconPath).toStdString().c_str());
        return;
    }

    this->m_Label_Icon_File->setPixmap(pixmap); // 设置 label_icon 的 pixmap
    this->m_Label_Icon_File->setScaledContents(true); // 使图像按比例缩放适应标签
    this->m_Label_Icon_File->setFixedSize(QSize(25, 25)); // 设置 QLabel 的固定大小
    return;
}

void ListItemWidget_Live2DModel::OnPushButton_SetModel()
{
    if (GetLModelManagerInstance().SetDisplayLive2DModel(this->m_index) == false)
    {
        RuntimeLog_Debug(QString("failed: set display live2d model").toStdString().c_str());
		return;
    }
    else
    {
		ModelWindowPromptWindow::ShowMessageOnModelWindow(11, QString("模型已切换成功！"), 3000, true);
    }
}
// ========== end ========== end ========== end =========== end =========== end =========== end ========== end =========== end ==========

// ========== DisplayChatHistoryWidget ========== DisplayChatHistoryWidget ========== DisplayChatHistoryWidget ========== DisplayChatHistoryWidget ===========
#include <QJsonArray>
#include <QJsonValue>
#include <QLabel>
#include <QPixmap>
#include <QMovie>
#include <QDateTime>
#include <QMimeDatabase>
#include <QMimeType>
#include <QStyleOption>
#include <QPainter>

static bool isMine(const QJsonObject& msg)
{
    return msg.value("sender").toString() == "User";
}

DisplayChatHistoryWidget::DisplayChatHistoryWidget(QWidget* parent)
    : QWidget(parent)
{
    this->m_scrollArea = new QScrollArea(this);
    this->m_scrollArea->setWidgetResizable(true);
    this->m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    this->m_contentWidget = new QWidget;
    this->m_mainLay = new QVBoxLayout(m_contentWidget);
    this->m_mainLay->addStretch();
    this->m_contentWidget->setLayout(m_mainLay);

    this->m_scrollArea->setWidget(m_contentWidget);

    QVBoxLayout* outerLay = new QVBoxLayout(this);
    outerLay->setContentsMargins(0, 0, 0, 0);
    outerLay->addWidget(m_scrollArea);
    setLayout(outerLay);

    m_mainLay->setDirection(QBoxLayout::BottomToTop);
}

void DisplayChatHistoryWidget::Update(const QJsonObject& dayRecord)
{
    // 清空的是内容窗口
    QLayoutItem* child;
    while ((child = m_mainLay->takeAt(0)) != nullptr) 
    {
        if (child->widget()) 
            delete child->widget();
        delete child;
    }
    m_mainLay->addStretch();   // 重新加到底

    QJsonArray msgs = dayRecord.value("messages").toArray();
    
    m_msgCount = msgs.size();
    int end = qMax(0, msgs.size() - 500);
    for (int i = msgs.size() - 1; i >= end; --i) 
    {
        QWidget* w = CreateMessageItem(msgs.at(i).toObject());
        if (w)
            m_mainLay->insertWidget(m_mainLay->count() - 1, w); // 插在 stretch 前面
    }
}

int DisplayChatHistoryWidget::MessageCount() const
{
    return m_msgCount;
}

void DisplayChatHistoryWidget::ClearLayout()
{
    while (QLayoutItem* item = m_mainLay->takeAt(0)) 
    {
        if (item->widget()) 
            delete item->widget();
        delete item;
    }
    m_mainLay->addStretch();
}

QWidget* DisplayChatHistoryWidget::CreateMessageItem(const QJsonObject& msg)
{
    const QString qstr_msgType = msg.value("type").toString(); // "text" | "image" | "video"
    const QString qstr_body = msg.value("body").toString(); // 文本或 本地路径
    const bool bIsUser = isMine(msg);
    const QString time = QDateTime::fromMSecsSinceEpoch(
        msg.value("ts").toInteger()).toString("hh:mm");

    QWidget* row = new QWidget;
    QHBoxLayout* rowLay = new QHBoxLayout(row);
    rowLay->setContentsMargins(5, 3, 5, 3);

    QLabel* bubble = nullptr;
    if (qstr_msgType == "text") 
    {
        bubble = new QLabel(qstr_body);  
        const QString bg = bIsUser ? "#07c160" : "#ffffff";    // user=我(绿)  对方=白 
        const QString text = bIsUser ? "#ffffff" : "#141415";
        bubble->setStyleSheet(QString(
            "QLabel{"
            "    background:%1;"
            "    color:%2;"               /* 字体与背景反差 */
            "    padding:8px 12px;"
            "    border-radius:16px;"
            "    max-width:480px;"
            "    selection-background-color:%3;"  /* 选中时背景再深一点 */
            "    selection-color:#ffffff;"
            "}").arg(bg, text, bIsUser ? "#059e55" : "#e5e5e5"));

        bubble->setWordWrap(true);
        bubble->setTextInteractionFlags(Qt::TextSelectableByMouse);
    }
    else if (qstr_msgType == "image")
    {
        bubble = new QLabel;
        bubble->setScaledContents(true);
        bubble->setFixedSize(160, 160);
        bubble->setPixmap(QPixmap(qstr_body).scaled(160, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        bubble->setStyleSheet("QLabel{border:1px solid #ddd;border-radius:8px;}");
    }
    else if (qstr_msgType == "video")
    {
        bubble = new QLabel("📹 " + QFileInfo(qstr_body).fileName());
        bubble->setStyleSheet("QLabel{color:#1e90ff;font:10pt;}");
    }
    else 
    {
        return nullptr; // 未知类型忽略
    }

    QLabel* timeLab = new QLabel(time);
    timeLab->setStyleSheet("QLabel{color:#999;font:8pt;}");

    QVBoxLayout* side = new QVBoxLayout;
    side->addWidget(bubble);
    side->addWidget(timeLab);
    side->setSpacing(2);

    if (bIsUser)
    {
        rowLay->addStretch();
        rowLay->addLayout(side);
    }
    else 
    {
        rowLay->addLayout(side);
        rowLay->addStretch();
    }
    return row;
}

// ========== end ========== end ========== end =========== end =========== end =========== end ========== end =========== end ==========
