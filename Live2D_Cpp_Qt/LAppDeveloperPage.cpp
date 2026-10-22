#include "LAppDeveloperPage.h"
#include "LAppLive2DManager.hpp"

// ========== Developer_Lips_Shape ========== Developer_Lips_Shape ========== Developer_Lips_Shape ==========

/*
 ============================================================================
 Function: Developer_Lips_Shape
 Description: 构造函数
 Parameters:
     - QWidget* parent: 父窗口的指针
 Return: 无
 ============================================================================
*/
Developer_Lips_Shape::Developer_Lips_Shape(QWidget* parent)
{
    // 嘴唇形状
    this->m_label_ParamMouthForm = new QLabel("嘴唇形状:");
    this->m_label_ParamMouthForm_Value = new QLabel("0.0");
    this->m_slider_ParamMouthForm = new QSlider();
    this->m_slider_ParamMouthForm->setRange(0, 10);
    this->m_slider_ParamMouthForm->setSingleStep(1);
    this->m_slider_ParamMouthForm->setOrientation(Qt::Horizontal);
    QHBoxLayout* hlayout_ParamMouthForm = new QHBoxLayout();
    hlayout_ParamMouthForm->addWidget(this->m_label_ParamMouthForm);
	hlayout_ParamMouthForm->addWidget(this->m_slider_ParamMouthForm);
	hlayout_ParamMouthForm->addWidget(this->m_label_ParamMouthForm_Value);

    // 嘴唇开度
    this->m_label_ParamMouthOpenY = new QLabel("嘴唇开度:");
	this->m_label_ParamMouthOpenY_Value = new QLabel("0.0");
    this->m_slider_ParamMouthOpenY = new QSlider();
    this->m_slider_ParamMouthOpenY->setRange(0, 10);
    this->m_slider_ParamMouthOpenY->setSingleStep(1);
    this->m_slider_ParamMouthOpenY->setOrientation(Qt::Horizontal);
    QHBoxLayout* hlayout_ParamMouthOpenY = new QHBoxLayout();
	hlayout_ParamMouthOpenY->addWidget(this->m_label_ParamMouthOpenY);
    hlayout_ParamMouthOpenY->addWidget(this->m_slider_ParamMouthOpenY);
	hlayout_ParamMouthOpenY->addWidget(this->m_label_ParamMouthOpenY_Value);

    // wav文件
    this->m_edit_wavFile = new QLineEdit();
    this->m_button_chooseWavFile = new QPushButton("选择wav文件");
    this->m_button_debug = new QPushButton("调试");
    QHBoxLayout* hlayout_edit = new QHBoxLayout();
    hlayout_edit->addWidget(this->m_edit_wavFile);
    hlayout_edit->addWidget(this->m_button_chooseWavFile);
    hlayout_edit->addWidget(this->m_button_debug);

    // 布局
    QVBoxLayout* vlayout_lips_shape = new QVBoxLayout();
    vlayout_lips_shape->addLayout(hlayout_ParamMouthForm);
    vlayout_lips_shape->addLayout(hlayout_ParamMouthOpenY);
    vlayout_lips_shape->addLayout(hlayout_edit);
    vlayout_lips_shape->addStretch();

    this->m_groupbox_main = new QGroupBox("嘴唇参数调试");
    this->m_groupbox_main->setLayout(vlayout_lips_shape);

    QVBoxLayout* mainLay = new QVBoxLayout(this);
    mainLay->addWidget(m_groupbox_main);

    // 连接信号
    connect(this->m_slider_ParamMouthForm,      &QSlider::valueChanged,  this, &Developer_Lips_Shape::on_slider_ParamMouthForm_valueChanged);
    connect(this->m_slider_ParamMouthOpenY,     &QSlider::valueChanged,  this, &Developer_Lips_Shape::on_slider_ParamMouthOpenY_valueChanged);
    connect(this->m_button_chooseWavFile,       &QPushButton::clicked,   this, &Developer_Lips_Shape::on_button_chooseWavFile_clicked);
    connect(this->m_button_debug,               &QPushButton::clicked,   this, &Developer_Lips_Shape::on_button_debug_clicked);
}

/*
 ============================================================================
 Function: ~Developer_Lips_Shape
 Description: 析构函数
 Parameters:
     - 无参数: 无释义
 Return: 无
 ============================================================================
*/
Developer_Lips_Shape::~Developer_Lips_Shape()
{
}

/*
 ============================================================================
 Function: on_slider_ParamMouthForm_valueChanged
 Description: 槽函数 - 负责嘴唇形状的控制
 Parameters:
     - int: 嘴唇形状的参数
 Return: 无
 ============================================================================
*/
void Developer_Lips_Shape::on_slider_ParamMouthForm_valueChanged(int value)
{
    QString value_str = QString("%1").arg(value / 10.0);
    this->m_label_ParamMouthForm_Value->setText(value_str);
}

/*
 ============================================================================
 Function: on_slider_ParamMouthOpenY_valueChanged
 Description: 槽函数 - 负责嘴唇开度的控制
 Parameters:
     - int: 嘴唇开度的参数
 Return: 无
 ============================================================================
*/
void Developer_Lips_Shape::on_slider_ParamMouthOpenY_valueChanged(int value)
{
    QString value_str = QString("%1").arg(value / 10.0);
    this->m_label_ParamMouthOpenY_Value->setText(value_str);
}

/*
 ============================================================================
 Function: on_button_chooseWavFile_clicked
 Description: 槽函数 - 负责选择wav文件
 Parameters:
     - 无参数: 无释义
 Return: 无
 ============================================================================
*/
void Developer_Lips_Shape::on_button_chooseWavFile_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "选择文件", QDir::homePath(), "WAV文件 (*.wav)");
    if (file_name.isEmpty())
    {
        return;
    }
    else
    {
        this->m_edit_wavFile->setText(file_name);
    }
}

/*
 ============================================================================
 Function: on_button_debug_clicked
 Description: 槽函数 - 负责调试进行
 Parameters:
     - 无参数: 无释义
 Return: 无
 ============================================================================
*/
void Developer_Lips_Shape::on_button_debug_clicked()
{
    if (this->m_edit_wavFile == nullptr)
        return;
    QString file_path = this->m_edit_wavFile->text();
    LAppLive2DManager::GetInstance()->StartLipSync(QDir::toNativeSeparators(file_path).toStdString().c_str());
    return;
}

// ========== End ========== End ========== End =========== End =========== End =========== End ==========

// ========== Developer_Debug_AI_Page ========== Developer_Debug_AI_Page ========== Developer_Debug_AI_Page ==========

/*
 ============================================================================
 Function: Developer_Debug_AI_Page
 Description: 构造函数
 Parameters:
     - QWidget* parent: 父窗口的指针
 Return: 无
 ============================================================================
*/
Developer_Debug_AI_Page::Developer_Debug_AI_Page(QWidget* parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QVBoxLayout* groupLayout = new QVBoxLayout();
    QVBoxLayout* vlayout_req_temp = new QVBoxLayout();

    // 请求模板相关
    {
        this->m_checkbox_minimum_request_tempate = new QCheckBox("是否使用最小请求模板");
        this->m_checkbox_minimum_request_tempate->setChecked(Config_Developer::instance().GetUseAI());
        QHBoxLayout* hlayout_request_template = new QHBoxLayout();
        hlayout_request_template->addWidget(this->m_checkbox_minimum_request_tempate);

        vlayout_req_temp->addLayout(hlayout_request_template);

        connect(this->m_checkbox_minimum_request_tempate, &QCheckBox::clicked, this, &Developer_Debug_AI_Page::onCheckBoxClicked_MimiReqTemplate);
    }

    groupLayout->addLayout(vlayout_req_temp);

    this->m_groupbox_main = new QGroupBox("AI设置调试");
    this->m_groupbox_main->setLayout(groupLayout);

    mainLayout->addWidget(this->m_groupbox_main);

    
}

/*
 ============================================================================
 Function: ~Developer_Debug_AI_Page
 Description: 析构函数
 Parameters:
     - 无参数: 无释义
 Return: 无
 ============================================================================
*/
Developer_Debug_AI_Page::~Developer_Debug_AI_Page()
{

}

/*
 ============================================================================
 Function: onCheckBoxClicked_MimiReqTemplate
 Description: 槽函数 - 处理最小请求模板调试点击
 Parameters:
     - bool: 是否选中
 Return: 无
 ============================================================================
*/
void Developer_Debug_AI_Page::onCheckBoxClicked_MimiReqTemplate(bool checked)
{
    Configuration::GetIns_DeveloperPageConfig().SetUseMinTemplate(checked);
    return;
}

// ========== End ========== End ========== End =========== End =========== End =========== End ==========

// ========== Developer_Debug_NetService ========== Developer_Debug_NetService ========== Developer_Debug_NetService ==========
/*
 ============================================================================
 Function: Developer_Debug_NetService
 Description: 构造函数
 Parameters:
     - QWidget* parent: 父窗口的指针
 Return: 无
 ============================================================================
*/
Developer_Debug_NetService::Developer_Debug_NetService(QWidget* parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QVBoxLayout* groupLayout = new QVBoxLayout();
    QVBoxLayout* vlayout_widget = new QVBoxLayout();

    // 监听相关
    {
        this->m_label_prompt_listen_port = new QLabel("监听端口");
        
        this->m_lineEdit_listen_port = new QLineEdit();
        QIntValidator* validator = new QIntValidator(1, 65535, this);
        this->m_lineEdit_listen_port->setValidator(validator);
        this->m_lineEdit_listen_port->setText(QString::number(Config_Developer::instance().GetListenPort()));
        QHBoxLayout* hLayout_listen_port = new QHBoxLayout();
        hLayout_listen_port->addWidget(this->m_label_prompt_listen_port);
        hLayout_listen_port->addWidget(this->m_lineEdit_listen_port);

        vlayout_widget->addLayout(hLayout_listen_port);

        // 连接端口修改信号槽
        connect(this->m_lineEdit_listen_port, &QLineEdit::textChanged, this, &Developer_Debug_NetService::onLineEditListenPort_valueChanged);
    }

    // NetService相关
    {
        this->m_checkBox_no_NetService = new QCheckBox("启动时不启动NetService进程");
        this->m_checkBox_no_NetService->setChecked(!Config_Developer::instance().GetUseNetService());
        vlayout_widget->addWidget(this->m_checkBox_no_NetService);

        // 连接启动时是否启动NetService进程信号槽
		connect(this->m_checkBox_no_NetService, &QCheckBox::clicked, this, &Developer_Debug_NetService::onCheckBoxClicked_NoNetService);
    }

    groupLayout->addLayout(vlayout_widget);

    this->m_groupbox_main = new QGroupBox("网络服务");
    this->m_groupbox_main->setLayout(groupLayout);

    mainLayout->addWidget(this->m_groupbox_main);
}

/*
 ============================================================================
 Function: ~Developer_Debug_NetService
 Description: 析构函数
 Parameters:
     - 无参数: 无释义
 Return: 无
 ============================================================================
*/
Developer_Debug_NetService::~Developer_Debug_NetService()
{
}

/*
 ============================================================================
 Function: onCheckBoxClicked_NoNetService
 Description: 槽函数 - 处理启动时是否启动NetService进程
 Parameters:
     - bool: 是否启动
 Return: 无
 ============================================================================
*/
void Developer_Debug_NetService::onCheckBoxClicked_NoNetService(bool checked)
{
    //qDebug() << "onCheckBoxClicked_NoNetService:" << checked;
    Config_Developer::instance().SetUseNetService(checked);
    return;
}

/*
 ============================================================================
 Function: onLineEditListenPort_valueChanged
 Description: 槽函数 - 处理端口修改
 Parameters:
     - const QString&: 端口信息
 Return: 无
 ============================================================================
*/
void Developer_Debug_NetService::onLineEditListenPort_valueChanged(const QString& text)
{
    int port = text.toInt();
    Config_Developer::instance().SetListenPort(port);
    return;
}

// ========== End ========== End ========== End =========== End =========== End =========== End ==========

// ========== Developer_Debug_Plugins ========== Developer_Debug_Plugins ========== Developer_Debug_Plugins ==========

/*
 ============================================================================
 Function: Developer_Debug_Plugins
 Description: 构造函数
 Parameters:
     - QWidget* parent: 父窗口的指针
 Return: 无
 ============================================================================
*/
Developer_Debug_Plugins::Developer_Debug_Plugins(QWidget* parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QVBoxLayout* groupLayout = new QVBoxLayout();
    QVBoxLayout* vlayout_widget = new QVBoxLayout();

	// 插件页加载
    {
        this->m_checkBox_no_plugin_page = new QCheckBox("不加载插件页");
		this->m_checkBox_no_plugin_page->setChecked(!Config_Developer::instance().GetUsePlugins());

		vlayout_widget->addWidget(this->m_checkBox_no_plugin_page);

		// 连接插件页加载信号槽
		connect(this->m_checkBox_no_plugin_page, &QCheckBox::clicked, this, &Developer_Debug_Plugins::onCheckBoxClicked_NoPluginsPage);
    }

    // 插件加载
    {
        this->m_checkBox_no_load_page = new QCheckBox("不加载插件");
		this->m_checkBox_no_load_page->setChecked(Config_Developer::instance().GetLoadPlugins());

		vlayout_widget->addWidget(this->m_checkBox_no_load_page);

		// 连接插件加载信号槽
		connect(this->m_checkBox_no_load_page, &QCheckBox::clicked, this, &Developer_Debug_Plugins::onCheckBoxClicked_NoLoadPlugins);
    }

    // 插件路径
    {
        this->m_checkBox_no_launch_any_plugins = new QCheckBox("不启动任何插件");
        this->m_checkBox_no_launch_any_plugins->setChecked(Config_Developer::instance().GetAutoStartPlugins());

		vlayout_widget->addWidget(this->m_checkBox_no_launch_any_plugins);

		// 连接插件路径信号槽
		connect(this->m_checkBox_no_launch_any_plugins, &QCheckBox::clicked, this, &Developer_Debug_Plugins::onCheckBoxClicked_NoLaunchAnyPlugins);
    }

    groupLayout->addLayout(vlayout_widget);

    this->m_groupbox_main = new QGroupBox("插件服务");
    this->m_groupbox_main->setLayout(groupLayout);

    mainLayout->addWidget(this->m_groupbox_main);
}

/*
 ============================================================================
 Function: ~Developer_Debug_Plugins
 Description: 析构函数
 Parameters:
     - 无参数: 无释义
 Return: 无
 ============================================================================
*/
Developer_Debug_Plugins::~Developer_Debug_Plugins()
{
}

/*
 ============================================================================
 Function: onCheckBoxClicked_NoPluginsPage
 Description: 槽函数 - 处理插件页加载
 Parameters:
     - bool: 勾选状态
 Return: 无
 ============================================================================
*/
void Developer_Debug_Plugins::onCheckBoxClicked_NoPluginsPage(bool checked)
{
    Config_Developer::instance().SetUsePlugins(checked);

    // 如果不加载插件页，那么不加载插件被勾选，不自动启动任何插件也被勾选，且被设置为true
    // 避免不必要的消耗
    if (checked)
    {
        this->m_checkBox_no_load_page->setChecked(checked);
        this->m_checkBox_no_launch_any_plugins->setChecked(checked);
        Config_Developer::instance().SetLoadPlugins(checked);
		Config_Developer::instance().SetAutoStartPlugins(checked);
    }
    this->m_checkBox_no_load_page->setEnabled(!checked);
	this->m_checkBox_no_launch_any_plugins->setEnabled(!checked);
    return;
}

/*
 ============================================================================
 Function: onCheckBoxClicked_NoLoadPlugins
 Description: 槽函数 - 处理插件加载
 Parameters:
     - bool: 勾选状态
 Return: 无
 ============================================================================
*/
void Developer_Debug_Plugins::onCheckBoxClicked_NoLoadPlugins(bool checked)
{
    Config_Developer::instance().SetLoadPlugins(checked);

    // 如果不加载插件被勾选，则不自动启动任何插件也被勾选，且被设置为true   
    if(checked)
    {
         this->m_checkBox_no_launch_any_plugins->setChecked(checked);
         Config_Developer::instance().SetAutoStartPlugins(checked);
    }
    this->m_checkBox_no_launch_any_plugins->setEnabled(!checked);
}

/*
 ============================================================================
 Function: onCheckBoxClicked_NoLaunchAnyPlugins
 Description: 槽函数 - 处理插件启动
 Parameters:
     - bool: 勾选状态
 Return: 无
 ============================================================================
*/
void Developer_Debug_Plugins::onCheckBoxClicked_NoLaunchAnyPlugins(bool checked)
{
    Config_Developer::instance().SetAutoStartPlugins(checked);
    return;
}
// ========== End ========== End ========== End =========== End =========== End =========== End ==========

// ========== Developer_Debug_Models ========== Developer_Debug_Models ========== Developer_Debug_Models ==========

/*
 ============================================================================
 Function: Developer_Debug_Models
 Description: 构造函数
 Parameters:
     - QWidget* parent: 父窗口的指针
 Return: 无
 ============================================================================
*/
Developer_Debug_Models::Developer_Debug_Models(QWidget* parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QVBoxLayout* groupLayout = new QVBoxLayout();
    QVBoxLayout* vlayout_widget = new QVBoxLayout();

    // 模型页是否加载
    {
        this->m_checkBox_no_models_page = new QCheckBox("不加载模型页");
        this->m_checkBox_no_models_page->setChecked(!Config_Developer::instance().GetUseModels());

		vlayout_widget->addWidget(this->m_checkBox_no_models_page);
        
        // 连接模型页是否加载信号槽
		connect(this->m_checkBox_no_models_page, &QCheckBox::clicked, this, &Developer_Debug_Models::onCheckBoxClicked_NoModelsPage);
    }

    // 不加载模型
    {
        this->m_checkBox_no_load_models = new QCheckBox("不加载模型");
		this->m_checkBox_no_load_models->setChecked(!Config_Developer::instance().GetLoadModels());

		vlayout_widget->addWidget(this->m_checkBox_no_load_models);

		// 连接加载模型信号槽
		connect(this->m_checkBox_no_load_models, &QCheckBox::clicked, this, &Developer_Debug_Models::onCheckBoxClicked_NoLoadModels);
    }

    groupLayout->addLayout(vlayout_widget);

    this->m_groupbox_main = new QGroupBox("模型服务");
    this->m_groupbox_main->setLayout(groupLayout);

    mainLayout->addWidget(this->m_groupbox_main);
}

/*
 ============================================================================
 Function: ~Developer_Debug_Models
 Description: 析构函数
 Parameters:
     - 无参数: 无释义
 Return: 无
 ============================================================================
*/
Developer_Debug_Models::~Developer_Debug_Models()
{
}

/*
 ============================================================================
 Function: onCheckBoxClicked_NoModelsPage
 Description: 槽函数 - 处理模型页是否加载
 Parameters:
     - bool: 勾选状态
 Return: 无
 ============================================================================
*/
void Developer_Debug_Models::onCheckBoxClicked_NoModelsPage(bool checked)
{
    Config_Developer::instance().SetUseModels(checked);

    // 如果不加载模型页，那么不模型加载也默认勾选
    if (checked)
    {
        this->m_checkBox_no_load_models->setChecked(checked);
        Config_Developer::instance().SetLoadModels(checked);
    }
    this->m_checkBox_no_load_models->setEnabled(!checked);
    return;
}

/*
 ============================================================================
 Function: onCheckBoxClicked_MoLoadModels
 Description: 槽函数 - 处理模型是否加载
 Parameters:
     - bool: 勾选状态
 Return: 无
 ============================================================================
*/
void Developer_Debug_Models::onCheckBoxClicked_NoLoadModels(bool checked)
{
    Config_Developer::instance().SetLoadModels(checked);
}

// ========== End ========== End ========== End =========== End =========== End =========== End ==========

// ========== Developer_Debug_ChatHistory ========== Developer_Debug_ChatHistory ========== Developer_Debug_ChatHistory ==========

/*
 ============================================================================
 Function: Developer_Debug_ChatHistory
 Description: 构造函数
 Parameters:
     - QWidget*: 一个父widget指针
 Return: 无
 ============================================================================
*/
Developer_Debug_ChatHistory::Developer_Debug_ChatHistory(QWidget* parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QVBoxLayout* groupLayout = new QVBoxLayout();
    QVBoxLayout* vlayout_widget = new QVBoxLayout();

    // 模型页是否加载
    {
        this->m_checkBox_no_models_page = new QCheckBox("不加载聊天记录页");
        this->m_checkBox_no_models_page->setChecked(!Config_Developer::instance().GetUseChatHistory());

        vlayout_widget->addWidget(this->m_checkBox_no_models_page);

        // 连接模型页是否加载信号槽
        connect(this->m_checkBox_no_models_page, &QCheckBox::clicked, this, &Developer_Debug_ChatHistory::onCheckBoxClicked_no_chat_history);
    }

    groupLayout->addLayout(vlayout_widget);

    this->m_groupbox_main = new QGroupBox("聊天记录相关");
    this->m_groupbox_main->setLayout(groupLayout);

    mainLayout->addWidget(this->m_groupbox_main);
}

/*
 ============================================================================
 Function: Developer_Debug_ChatHistory
 Description: 析构函数
 Parameters:
     - 无参数: 无释义
 Return: 无
 ============================================================================
*/
Developer_Debug_ChatHistory::~Developer_Debug_ChatHistory()
{
}

/*
 ============================================================================
 Function: Developer_Debug_ChatHistory
 Description: 槽函数 - 处理模型是否加载
 Parameters:
     - bool: 勾选状态
 Return: 无
 ============================================================================
*/
void Developer_Debug_ChatHistory::onCheckBoxClicked_no_chat_history(bool checked)
{
    Config_Developer::instance().SetUseChatHistory(checked);
    return;
}

// ========== End ========== End ========== End =========== End =========== End =========== End ==========

// ========== Developer_Debug_ModelMessage =========== Developer_Debug_ModelMessage ========== Developer_Debug_ModelMessage ========== 
Developer_Debug_ModelMessage::Developer_Debug_ModelMessage(QWidget* parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QVBoxLayout* groupLayout = new QVBoxLayout();
    QVBoxLayout* vlayout_widget = new QVBoxLayout();

    {
        this->m_radioLive2D = new QRadioButton("Live2D消息显示");
        this->m_radioLive2D->setChecked(true);
        this->m_radioNetService = new QRadioButton("网络服务消息");
        this->m_radioNetService->setChecked(false);
        QVBoxLayout* vlayout_forward_to = new QVBoxLayout();
        vlayout_forward_to->addWidget(this->m_radioLive2D);
        vlayout_forward_to->addWidget(this->m_radioNetService);

        connect(m_radioLive2D, &QRadioButton::toggled, this, &Developer_Debug_ModelMessage::onRadioChanged);

        this->m_btnShowMessage = new QPushButton("发送");
        this->m_comboMessageTemp = new QComboBox();
        InitComboBox(true);
        QVBoxLayout* vlayout_processmsg = new QVBoxLayout();
        vlayout_processmsg->addWidget(this->m_btnShowMessage);
        vlayout_processmsg->addWidget(this->m_comboMessageTemp);
        connect(this->m_btnShowMessage, &QPushButton::clicked, this, &Developer_Debug_ModelMessage::onSendButtonClicked);
        connect(m_comboMessageTemp, &QComboBox::currentIndexChanged, this, &Developer_Debug_ModelMessage::onComboboxChanged);

        QHBoxLayout* hlayout_process = new QHBoxLayout();
        hlayout_process->addLayout(vlayout_forward_to);
        hlayout_process->addLayout(vlayout_processmsg);

        this->m_textEditMessage = new QPlainTextEdit();
        this->m_textEditMessage->setMinimumHeight(200);

        vlayout_widget->addLayout(hlayout_process);
        vlayout_widget->addWidget(this->m_textEditMessage);
    }

    groupLayout->addLayout(vlayout_widget);

    this->m_groupbox_main = new QGroupBox("消息相关设置");
    this->m_groupbox_main->setLayout(groupLayout);

    mainLayout->addWidget(this->m_groupbox_main);

    // 连接信号到消息显示窗口
    connect(this, &Developer_Debug_ModelMessage::showMessage,
        &BubbleManager::instance(), qOverload<const QString&>(&BubbleManager::ShowMessage));
}

Developer_Debug_ModelMessage::~Developer_Debug_ModelMessage()
{
}


void Developer_Debug_ModelMessage::onComboboxChanged(int index)
{
    nlohmann::json jsonTemplate = Config_Developer::instance().GetMessageTemplate(index);
	std::string jsonString = jsonTemplate.dump(4); // 4表示缩进为4个空格
    this->m_textEditMessage->clear();
	this->m_textEditMessage->setPlainText(QString::fromStdString(jsonString));
}

void Developer_Debug_ModelMessage::onRadioChanged(bool checked)
{
    InitComboBox(checked);
}

void Developer_Debug_ModelMessage::onSendButtonClicked()
{
    if (this->m_radioLive2D->isChecked())
    {
        QString _jsonStr = this->m_textEditMessage->toPlainText();
        emit showMessage(_jsonStr);
    }
    else
    {
        QString _jsonStr = this->m_textEditMessage->toPlainText();
        emit pluginMessage(_jsonStr);
    }
}

void Developer_Debug_ModelMessage::InitComboBox(bool isLive2D)
{
    this->m_comboMessageTemp->clear();
    if (isLive2D) 
    {
        this->m_comboMessageTemp->addItem(QString("不显示任何消息"));
        this->m_comboMessageTemp->addItem(QString("流式显示(打字机效果)"));
        this->m_comboMessageTemp->addItem(QString("直接显示消息"));
    }
    else
    {
        this->m_comboMessageTemp->addItem(QString("request_ai"));
        this->m_comboMessageTemp->addItem(QString("forward(转发给自己)"));
        this->m_comboMessageTemp->addItem(QString("intercept_ai_response"));
        this->m_comboMessageTemp->addItem(QString("open_settings_page"));
    }
}

// ========== End ========== End ========== End =========== End =========== End =========== End ==========