#include "SettingFuncPage.h"
#include <tuple>

// ========== SettingInitializePageData ============
template<typename _T, typename... Args>
inline void Setting_InitializePageWidgetData::_SetWidgetData(QWidget* parent, const QString& objectName, Args... args)
{
    _T* widget = parent->findChild<_T*>(objectName);
    if (widget != nullptr && sizeof...(args) > 0)
    {
        std::tuple<Args...> argsTuple = std::make_tuple(args...); // 将参数封装成 tuple
        constexpr size_t argCount = std::tuple_size<decltype(argsTuple)>::value;

        // 确定是否是 QCheckBox
        if constexpr (std::is_same_v<_T, QCheckBox>) 
        {
            widget->setChecked(std::get<0>(argsTuple));
        }
        else if constexpr (std::is_same_v<_T, QLabel>) 
        {
            widget->setText(std::get<0>(argsTuple));
        }
        else if constexpr (std::is_same_v<_T, QSpinBox>) 
        {
            widget->setValue(std::get<0>(argsTuple));
            if constexpr (argCount == 3)
            {
                widget->setRange(std::get<1>(argsTuple), std::get<2>(argsTuple));
            }
        }
		else if constexpr (std::is_same_v<_T, QDoubleSpinBox>)
		{
			widget->setValue(std::get<0>(argsTuple));
            if constexpr(argCount == 3)
            {
                widget->setRange(std::get<1>(argsTuple), std::get<2>(argsTuple));
            }
            else if constexpr(argCount > 3)
            {
                widget->setRange(std::get<1>(argsTuple), std::get<2>(argsTuple));
                widget->setSingleStep(std::get<3>(argsTuple));
                widget->setDecimals(std::get<4>(argsTuple));
            }
		}
		else if constexpr (std::is_same_v<_T, QLineEdit>)
		{
			widget->setText(std::get<0>(argsTuple));
		}
		else if constexpr (std::is_same_v<_T, QTextEdit>)
		{
			widget->setPlainText(std::get<0>(argsTuple));
		}
		else if constexpr (std::is_same_v<_T, QPlainTextEdit>)
		{
			widget->setPlainText(std::get<0>(argsTuple));
		}
		else if constexpr (std::is_same_v<_T, QKeySequenceEdit>)
		{
			widget->setKeySequence(QKeySequence(std::get<0>(argsTuple)));
		}
        else if constexpr (std::is_same_v<_T, QPushButton>) 
        {
            widget->setText(std::get<0>(argsTuple));
        }
        else if constexpr (std::is_same_v<_T, QComboBox>) 
		{
			widget->setCurrentIndex(std::get<0>(argsTuple));
		}
		else if constexpr (std::is_same_v<_T, QSlider>) 
		{
			widget->setValue(std::get<0>(argsTuple));
			widget->setRange(std::get<1>(argsTuple), std::get<2>(argsTuple));
		}
        else if constexpr (std::is_same_v<_T, QRadioButton>)
        {
			widget->setChecked(std::get<0>(argsTuple));
        }
    }
    else 
    {
        RuntimeLog_Fatal(QString("findChild<%1*>(%2) is nullptr").arg(QString::fromStdString(typeid(_T).name())).arg(objectName));
    }
}
// ========== end ==========

// ========== SettingPublicFunction ==========
SettingPublicFunction::~SettingPublicFunction()
{
}
// ========== end ==========

// ========== class Page_Account ==========
Page_Account::Page_Account(QWidget* parent) :
	QWidget(parent)
{
    this->m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 登录页面
    QWidget* pageBuffer = this->CreateLoginPage();
    if (pageBuffer)
    {
        this->m_stackedWidget->addWidget(pageBuffer);
    }
    // 用户信息页面
	pageBuffer = this->CreateUserInfoPage();
	if (pageBuffer)
	{
		this->m_stackedWidget->addWidget(pageBuffer);
	}
	// 注册页面
	pageBuffer = this->CreateRegisterPage();
	if (pageBuffer)
	{
		this->m_stackedWidget->addWidget(pageBuffer);
	}

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_stackedWidget); // 填满整个区域
    
    setLayout(mainLayout);
    this->InitPageFromJson();
}

void Page_Account::InitLoginPage()
{
	this->m_stackedWidget->setCurrentIndex(0);
}

void Page_Account::InitPageFromJson()
{
    QLineEdit* lineEdit_Buffer = this->findChild<QLineEdit*>(_PONL::Account_ObjNameList::lineEdit_UserName);
    if (lineEdit_Buffer)
    {
        lineEdit_Buffer->setText(this->GetPageConfigInstance().Get_Username());
    }
    else
    {
        RuntimeLog_Debug("lineEdit_UserName not found in json");
    }
}

void Page_Account::SavePageToJson()
{
    return;
}

Config_Account& Page_Account::GetPageConfigInstance()
{
    return Configuration::GetIns_AccountPageConfig();
}

QWidget* Page_Account::CreateLoginPage()
{
    QWidget* page = new QWidget(this);

    // ========== 创建 GroupBox ==========
    QGroupBox* box = new QGroupBox(page);
    box->setObjectName("loginGroupBox");

    // ========== 登录表单内容 ==========
    QLineEdit* lineEdit_UserName = new QLineEdit();
    lineEdit_UserName->setObjectName(_PONL::Account_ObjNameList::lineEdit_UserName);
    lineEdit_UserName->setPlaceholderText("请输入账号");
    lineEdit_UserName->setFixedWidth(200);  // 固定宽度，避免拉伸时过宽
    QRegularExpression rx_UserNmae("^[A-Za-z0-9]*$");
    lineEdit_UserName->setValidator(new QRegularExpressionValidator(rx_UserNmae, lineEdit_UserName));

    QLineEdit* lineEdit_UserPasswd = new QLineEdit();
    lineEdit_UserPasswd->setObjectName(_PONL::Account_ObjNameList::lineEdit_Password);
    lineEdit_UserPasswd->setPlaceholderText("请输入密码");
    lineEdit_UserPasswd->setEchoMode(QLineEdit::Password);
    lineEdit_UserPasswd->setFixedWidth(200);
    QAction* QAction_toggleAction = lineEdit_UserPasswd->addAction(
        QIcon(Config_Account::instance().GetIcon_HidePassword()),
        QLineEdit::TrailingPosition);
    QRegularExpression rx_UserPasswd("^[A-Za-z0-9!\"#$%&'()*+,\\-./:;<=>?@[\\]^_`{|}~]*$");
    lineEdit_UserPasswd->setValidator(new QRegularExpressionValidator(rx_UserPasswd, lineEdit_UserPasswd));

    connect(QAction_toggleAction, &QAction::triggered, this, [this, lineEdit_UserPasswd, QAction_toggleAction]() {
        bool isVisible = lineEdit_UserPasswd->echoMode() == QLineEdit::Password;
        this->SetPwdDisplayStatus_IconSwitching(lineEdit_UserPasswd, QAction_toggleAction, isVisible);
        });

    QFormLayout* form = new QFormLayout();
    form->addRow("账号:", lineEdit_UserName);
    form->addRow("密码:", lineEdit_UserPasswd);

    QPushButton* btn_Login = new QPushButton("登录");
    //btn_Login->setFixedWidth(200);
    QPushButton* btn_Register = new QPushButton("注册");
    //btn_Register->setFixedWidth(200);
    connect(btn_Login, &QPushButton::clicked, this, &Page_Account::DoLogin);
    connect(btn_Register, &QPushButton::clicked, this, &Page_Account::ShowRegisterPage);

    QPushButton* btn_ForgotPwd = new QPushButton("忘记密码");
    QPushButton* btn_ForgotAccount = new QPushButton("忘记账户");
    btn_ForgotPwd->setFlat(true);
    btn_ForgotAccount->setFlat(true);
    btn_ForgotPwd->setStyleSheet("QPushButton{border:none; padding-right:1px; margin-right:0px; color:#666;}");
    btn_ForgotAccount->setStyleSheet("QPushButton{border:none; padding-left:1px; margin-left:0px; color:#666;}");
    connect(btn_ForgotPwd, &QPushButton::clicked, this, &Page_Account::OpenForgotPasswd_Account);
    connect(btn_ForgotAccount, &QPushButton::clicked, this, &Page_Account::OpenForgotPasswd_Account);

    QLabel* sep = new QLabel(" | ");
    sep->setStyleSheet("color:#999;");

    QHBoxLayout* forgotLayout = new QHBoxLayout();
    forgotLayout->setSpacing(1);
    forgotLayout->addWidget(btn_ForgotPwd);
    forgotLayout->addWidget(sep);
    forgotLayout->addWidget(btn_ForgotAccount);
    forgotLayout->setAlignment(Qt::AlignCenter);

    // 登录内容容器
    QWidget* loginWidget = new QWidget();
    QVBoxLayout* loginContentLayout = new QVBoxLayout(loginWidget);
    loginContentLayout->addLayout(form);
    loginContentLayout->addWidget(btn_Login);
    loginContentLayout->addWidget(btn_Register);
    loginContentLayout->addLayout(forgotLayout);
    loginContentLayout->setAlignment(Qt::AlignCenter);
    loginContentLayout->setSpacing(10);

    // ========== 版本信息（左下角）==========
    QLabel* label_current_version = new QLabel("当前版本: ");
	QString text = QString("%1 - %2")
        .arg(ConfigurationBase::instance().GetCurrentVersion())
        .arg(ConfigurationBase::instance().GetPublisher());
    QLabel* label_current_version_detail = new QLabel(text);
    QLabel* label_latest_version = new QLabel("最新版本: ");
    QLabel* label_latest_version_detail = new QLabel("未知");

    // 用 QPointer 包装，用于 Lambda 捕获
    QPointer<QLabel> ptr_current_detail(label_current_version_detail);
    QPointer<QLabel> ptr_latest_detail(label_latest_version_detail);
    QPointer<QObject> selfGuard(this);

    QFont smallFont;
    smallFont.setPointSize(8);
    label_current_version->setFont(smallFont);
    label_current_version_detail->setFont(smallFont);
    label_latest_version->setFont(smallFont);
    label_latest_version_detail->setFont(smallFont);

    QString versionStyle = "color: #888888;";
    label_current_version->setStyleSheet(versionStyle);
    label_current_version_detail->setStyleSheet(versionStyle);
    label_latest_version->setStyleSheet(versionStyle);
    label_latest_version_detail->setStyleSheet(versionStyle);

    QHBoxLayout* versionLayout_current = new QHBoxLayout();
    versionLayout_current->addWidget(label_current_version);
    versionLayout_current->addWidget(label_current_version_detail);
    versionLayout_current->addStretch();

    QHBoxLayout* versionLayout_latest = new QHBoxLayout();
    versionLayout_latest->addWidget(label_latest_version);
    versionLayout_latest->addWidget(label_latest_version_detail);
    versionLayout_latest->addStretch();

    QVBoxLayout* versionLayout = new QVBoxLayout();
    versionLayout->addLayout(versionLayout_current);
    versionLayout->addLayout(versionLayout_latest);
    versionLayout->setSpacing(2);
    versionLayout->setContentsMargins(1, 0, 0, 5);

    // 版本号容器（固定在底部）
    QWidget* versionWidget = new QWidget();
    versionWidget->setLayout(versionLayout);

    // ========== GroupBox 布局：使用 QGridLayout 实现居中 + 底部固定 ==========
    QGridLayout* gridLayout = new QGridLayout(box);

    // 登录内容放在中心，占据所有空间
    gridLayout->addWidget(loginWidget, 0, 0, Qt::AlignCenter);

    // 版本号放在左下角
    gridLayout->addWidget(versionWidget, 0, 0, Qt::AlignLeft | Qt::AlignBottom);

    // 让单元格可以拉伸，但内容保持对齐
    gridLayout->setRowStretch(0, 1);
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setContentsMargins(2, 2, 2, 2);

    // ========== 页面总布局 ==========
    QVBoxLayout* pageLayout = new QVBoxLayout(page);
    pageLayout->addWidget(box);
    pageLayout->setContentsMargins(3, 3, 3, 3);

	// ========== 连接 ==========
    {
        nlohmann::json _requestJson;
        _requestJson["type"] = "version_query";
        _requestJson["cmd"] = "get_version";
		_requestJson["user_id"] = 114514; // 这个接口不需要用户ID，随便填一个（单纯懒得改服务器端）
        UserRequest::Instance().SendRequestAsync(
            _requestJson,
            [this, selfGuard, ptr_current_detail, ptr_latest_detail](bool ok, const nlohmann::json& response)
            {
				// 检查是否销毁,销毁则不执行QMataObject::invokeMethod，避免访问野指针
                if (!selfGuard)
                {
                    return;
                }

                QMetaObject::invokeMethod(qApp, [this, ok, response, selfGuard, ptr_current_detail, ptr_latest_detail]()
                    {
                        if (!selfGuard)
                        {
                            return;  // Page_Account 已被销毁
                        }

                        // 检查 QLabel 是否还存在
                        if (!ptr_current_detail || !ptr_latest_detail) 
                        {
                            return;  // QLabel 已被销毁
                        }

                        if (!ok || response.empty())
                        {
                            ModelWindowPromptWindow::ShowMessageOnModelWindow(11,
                                QString("验证失败：服务器未连接或存在未错误 - 获取版本信息失败！"),
                                3000,
                                true);
                            QString text = "未知 - 未知发布者";
                            if(ptr_latest_detail)
                            {
                                ptr_latest_detail->setText(text);
                            }
							RuntimeLog_Debug("_responseJson is empty");
                            return;
                        }
                        else
                        {
                            if (response.contains("data") &&
                                response.at("data").contains("publisher") &&
                                response.at("data").contains("latest_version"))
                            {
                                QString _latest_version = QString::fromStdString(response.at("data").at("latest_version").get<std::string>());
								QString _publisher = QString::fromStdString(response.at("data").at("publisher").get<std::string>());
                                QString text = _latest_version + " - " + _publisher;
                                int cmp_res = ConvenientTool::CompareVersion(_latest_version.toStdString(), ConfigurationBase::instance().GetCurrentVersion().toStdString());
                                QString versionStyle;

                                // 最新版小于客户端版本 - 一般是测试版
                                if (cmp_res == -1)
                                {
                                    versionStyle = "color: #f47b92;";
                                } 
                                else if (cmp_res == 0)
                                {
                                    versionStyle = "color: #78cdf2;";
                                    if(ptr_current_detail != nullptr)
                                    {
                                        ptr_current_detail->setStyleSheet(versionStyle);
                                    }
                                }
                                else if (cmp_res == 1)
                                {
                                    versionStyle = "color: #61e326;";
                                }
                                else if (cmp_res == 2)
                                {
									RuntimeLog_Debug("Version number anomaly");
                                }
                                qDebug() << "cmp_res: " << cmp_res;
                                if (ptr_latest_detail != nullptr)
                                {
                                    ptr_latest_detail->setStyleSheet(versionStyle);
                                    ptr_latest_detail->setText(text);
                                }
                                return;
                            }
                            else
                            {
                                if (response.contains("user_info") && response.at("user_info").contains("error"))
                                {
                                    QString text = "未知 - 未知发布者";
                                    QString versionStyle = "color: #ff2432;";
                                    if(ptr_latest_detail != nullptr)
                                    {
                                        ptr_latest_detail->setStyleSheet(versionStyle);
                                        ptr_latest_detail->setText(text);
                                    }
                                    ModelWindowPromptWindow::ShowMessageOnModelWindow(11,
                                        QString("获取版本信息失败！"),
                                        3000,
                                        true);
                                    return;
                                }
                            }
                        }
                    }, Qt::QueuedConnection);
            },
            5000);
    }

    return page;
}

// 注册页面
QWidget* Page_Account::CreateRegisterPage()
{
    QWidget* page = new QWidget(this);

    QGroupBox* box = new QGroupBox(page);

    QLabel* label = new QLabel("首先，开发者是个勤快人!");
    label->setAlignment(Qt::AlignCenter);
    QLabel* label2 = new QLabel("(不写这个页面不是因为不想写这个页面！！！)");
    label2->setAlignment(Qt::AlignCenter);
    label2->setStyleSheet(
        "QLabel{"
        "   font-size:10px;"
        "}");
    QPushButton* registerBtn = new QPushButton("点此注册");
    //registerBtn->adjustSize();
    registerBtn->setStyleSheet(
        "QPushButton{"
        "   color: #409eff;"           // 蓝色文字
        "   font-size: 14px;"
        "   text-decoration: underline;"  // 下划线
        "   border: none;"              // 无边框
        "   background: transparent;"   // 透明背景
        "   padding: 2px 4px;"          // 最小内边距
        "}"
        "QPushButton:hover{"
        "   color: #66b1ff;"            // 悬停变浅蓝
        "}"
        "QPushButton:pressed{"
        "   color: #3a8ee6;"            // 按下变深蓝
        "}"
    );
	QHBoxLayout* hLay_registerBtn = new QHBoxLayout();
    hLay_registerBtn->addStretch();
	hLay_registerBtn->addWidget(registerBtn);
    hLay_registerBtn->addStretch();
	connect(registerBtn, &QPushButton::clicked, this, &Page_Account::OpenForgotPasswd_Account); // 点击注册按钮后打开站点可以实现注册功能

    QPushButton* btn_Back = new QPushButton("返回");
    btn_Back->setStyleSheet(
        "QPushButton{"
        "   border:none;"
        "   border-radius:4px;"
        "   font-size:13px;"
        "   padding:6px 10px;"
        "   background:#ffffff;"
        "   color:#333333;"
        "}"
        "QPushButton:hover{"
        "   border-color:#409eff;" // 悬停
        "}"
        "QPushButton:pressed,QPushButton:checked{"
        "   border:2px solid #409eff;"
        "   background:#409eff;"
        "   color:#ffffff;"
        "   font-weight:bold;"
        "}"
    );
    connect(btn_Back, &QPushButton::clicked, this, [this]() {
			ShowLoginPage(UNDEFINED_PAGE);
    });

    

    QHBoxLayout* hlayout_btn = new QHBoxLayout();
	hlayout_btn->addStretch();
	hlayout_btn->addWidget(btn_Back);

	QVBoxLayout* vBox = new QVBoxLayout(box);
    vBox->addStretch();
	vBox->addWidget(label);
	vBox->addWidget(label2);
	vBox->addLayout(hLay_registerBtn);
    vBox->addStretch();
	vBox->addLayout(hlayout_btn);

    QVBoxLayout* pageLayout = new QVBoxLayout(page);
	pageLayout->addWidget(box);
	pageLayout->setContentsMargins(0, 0, 0, 0);

	return page;
}

QWidget* Page_Account::CreateUserInfoPage()
{
    QWidget* page = new QWidget(this);

    // 创建最外层的 GroupBox，设置父对象为 page
    QGroupBox* box = new QGroupBox("用户信息", page);  // 添加标题，设置父对象
    QVBoxLayout* vlayout_box = new QVBoxLayout(box);

    // ========== 用户信息区域 ==========
    QFormLayout* form_UserInfo = new QFormLayout();
    form_UserInfo->setFormAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    form_UserInfo->setLabelAlignment(Qt::AlignRight);

    QLabel* label_UserName = new QLabel("用户名:");
    label_UserName->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QLabel* label_UserName_Value = new QLabel("DisplayName");
    label_UserName_Value->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label_UserName_Value->setObjectName(_PONL::Account_ObjNameList::label_UserName);

    QLabel* label_UID = new QLabel("UID:");
    label_UID->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QLabel* label_UID_Value = new QLabel("UID");
    label_UID_Value->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label_UID_Value->setObjectName(_PONL::Account_ObjNameList::label_UID);

    QLabel* label_Email = new QLabel("邮箱:");
    label_Email->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    QLabel* label_Email_Value = new QLabel("exp@email.com");
    label_Email_Value->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label_Email_Value->setObjectName(_PONL::Account_ObjNameList::label_Email);

    QPushButton* btn_Logout = new QPushButton("退出登录");
    btn_Logout->setStyleSheet(
        "QPushButton{"
        "   border:none;"
        "   border-radius:4px;"
        "   font-size:12px;"
        "   padding:6px 10px;"
        "   background:#ffffff;"
        "   color:#333333;"
        "}"
        "QPushButton:hover{"
        "   border-color:#409eff;"
        "}"
        "QPushButton:pressed,QPushButton:checked{"
        "   border:2px solid #409eff;"
        "   background:#409eff;"
        "   color:#ffffff;"
        "   font-weight:bold;"
        "}"
    );

    // 用户信息分隔线
    QLabel* label_Line = new QLabel();
    label_Line->setFixedSize(2, 20);
    label_Line->setStyleSheet("background:#909090;");

    QLabel* label_Line_2 = new QLabel();
    label_Line_2->setFixedSize(2, 20);
    label_Line_2->setStyleSheet("background:#909090;");

    QHBoxLayout* hlayout_row = new QHBoxLayout();
    hlayout_row->addWidget(label_UserName);
    hlayout_row->addWidget(label_UserName_Value);
    hlayout_row->addWidget(label_Line);
    hlayout_row->addWidget(label_UID);
    hlayout_row->addWidget(label_UID_Value);
    hlayout_row->addWidget(label_Line_2);
    hlayout_row->addWidget(label_Email);
    hlayout_row->addWidget(label_Email_Value);
    hlayout_row->addStretch();
    hlayout_row->addWidget(btn_Logout);
    form_UserInfo->addRow("", hlayout_row);

    // ========== 用户反馈区域 ==========
    QGroupBox* groupBox_Feedback = new QGroupBox("用户反馈");
    QPlainTextEdit* textEdit_UserFeedback = new QPlainTextEdit();
    textEdit_UserFeedback->setObjectName(_PONL::Account_ObjNameList::plainTextEdit_UserFeedback);
    textEdit_UserFeedback->setMinimumHeight(150);

    QPushButton* btn_SendInfo = new QPushButton();
    btn_SendInfo->setFixedWidth(80);
    btn_SendInfo->setStyleSheet(
        "QPushButton{"
        "   border:1px solid #d0d0d0;"
        "   border-radius:4px;"
        "   font-size:16px;"
        "   padding:3px 10px;"
        "   background:#ffffff;"
        "   color:#333333;"
        "}"
        "QPushButton:hover{"
        "   border-color:#409eff;"
        "}"
        "QPushButton:pressed,QPushButton:checked{"
        "   border:2px solid #409eff;"
        "   background:#409eff;"
        "   color:#ffffff;"
        "   font-weight:bold;"
        "}"
    );
    btn_SendInfo->setText("发送");

    QLabel* label_Prompt = new QLabel("提示:点击发送后，您可以将反馈信息发送给开发者（每周可发送一条）");
    label_Prompt->setAlignment(Qt::AlignLeft);
    label_Prompt->setFixedHeight(20);

    QHBoxLayout* hlayout_SendInfo = new QHBoxLayout();
    hlayout_SendInfo->addStretch();
    hlayout_SendInfo->addWidget(label_Prompt);
    hlayout_SendInfo->addWidget(btn_SendInfo);

    QVBoxLayout* vlayout_UserMessage = new QVBoxLayout(groupBox_Feedback);
    vlayout_UserMessage->addWidget(textEdit_UserFeedback);
    vlayout_UserMessage->addLayout(hlayout_SendInfo);

    // ========== 分隔线 ==========
    QFrame* line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);

    // ========== 用户公告区域 ==========
    QGroupBox* groupBox_Announcement = new QGroupBox("公告");
    QPlainTextEdit* textEdit_Announcement = new QPlainTextEdit();
    textEdit_Announcement->setReadOnly(true);
	textEdit_Announcement->setMinimumHeight(180);
    textEdit_Announcement->adjustSize();
    textEdit_Announcement->setObjectName(_PONL::Account_ObjNameList::plainTextEdit_Announcement);
    textEdit_Announcement->setPlainText("欢迎使用本软件！\n\n"
        "1. 本软件目前仍在开发中，可能存在一些问题和不完善的地方，感谢您的理解和支持！\n\n"
        "2. 如果您在使用过程中遇到任何问题或有任何建议，请随时通过反馈功能与我们联系，我们会尽力改进和优化软件。\n\n"
        "3. 请注意保护您的账户信息，避免泄露给他人，以确保您的账户安全。\n\n"
        "4. 本软件尊重用户隐私，不会收集或分享您的个人信息，除非得到您的明确同意。\n\n"
        "5. 感谢您选择使用我们的软件，我们将继续努力为您提供更好的体验和服务！");

    QPushButton* btn_GetNewAnnouncement = new QPushButton("获取最新公告");
    btn_GetNewAnnouncement->setStyleSheet(
        "QPushButton{"
        "   border:1px solid #d0d0d0;"
        "   border-radius:4px;"
        "   font-size:16px;"
        "   padding:3px 10px;"
        "   background:#ffffff;"
        "   color:#333333;"
        "}"
        "QPushButton:hover{"
        "   border-color:#409eff;"
        "}"
        "QPushButton:pressed,QPushButton:checked{"
        "   border:2px solid #409eff;"
        "   background:#409eff;"
        "   color:#ffffff;"
        "   font-weight:bold;"
        "}"
    );

    QVBoxLayout* vlayout_Announcement = new QVBoxLayout(groupBox_Announcement);
    vlayout_Announcement->addWidget(textEdit_Announcement);
    vlayout_Announcement->addWidget(btn_GetNewAnnouncement);

    // ========== 创建内容 Widget ==========
    QWidget* contentWidget = new QWidget();  // 这个 Widget 包含所有内容
    contentWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    QVBoxLayout* mainLayout = new QVBoxLayout(contentWidget);  // 布局设置给 contentWidget
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->addLayout(form_UserInfo);
    mainLayout->addWidget(line2);
    mainLayout->addWidget(groupBox_Feedback);
    mainLayout->addWidget(groupBox_Announcement);
    mainLayout->addStretch();

    // ========== 创建 ScrollArea 并设置内容 ==========
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidget(contentWidget);  // 关键：用 setWidget 而不是 setLayout
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setFrameShape(QFrame::NoFrame);

    // ========== 将 ScrollArea 放入外层 GroupBox ==========
    vlayout_box->addWidget(scrollArea);

    // ========== 将 GroupBox 放入 page ==========
    QVBoxLayout* pageLayout = new QVBoxLayout(page);
    pageLayout->addWidget(box);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    // ========== 信号槽连接 ==========
    connect(btn_Logout, &QPushButton::clicked, this, [this]() {
        this->ShowLoginPage(1);
        });
    connect(btn_SendInfo, &QPushButton::clicked, this, &Page_Account::SendFeedback);
    connect(btn_GetNewAnnouncement, &QPushButton::clicked, this, &Page_Account::GetNewAnnouncement);

    return page;
}

void Page_Account::ShowLoginPage(int nFromPageIndex)
{
    if(nFromPageIndex == 1)
    {
		QMessageBox msgBox;
		msgBox.setWindowTitle("退出登录");
		msgBox.setText("是否确认退出登录？");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        msgBox.button(QMessageBox::Yes)->setText("是");
		msgBox.button(QMessageBox::No)->setText("否");
		
        msgBox.exec();
        if(msgBox.clickedButton() == msgBox.button(QMessageBox::No))
		{
            return;
		}
    }

    if(true)
	{
		this->m_stackedWidget->setCurrentIndex(0);
	}
}

void Page_Account::ShowRegisterPage()
{
    if(true)
	{
		this->m_stackedWidget->setCurrentIndex(2);
	}
}

void Page_Account::DoLogin()
{
    QLineEdit* lineEdit_UserName = this->findChild<QLineEdit*>(_PONL::Account_ObjNameList::lineEdit_UserName);
	QLineEdit* lineEdit_UserPasswd = this->findChild<QLineEdit*>(_PONL::Account_ObjNameList::lineEdit_Password);
    if (lineEdit_UserName == nullptr || lineEdit_UserPasswd == nullptr)
    {
		RuntimeLog_Debug("lineEdit_UserName or lineEdit_UserPasswd not found in json");
		return;
    }

	QString userName = lineEdit_UserName->text();
    QString userPasswd = lineEdit_UserPasswd->text();
    if (userName.isEmpty() || userPasswd.isEmpty())
    {
        ModelWindowPromptWindow::ShowMessageOnModelWindow(11,
            QString("用户名或密码不能为空哦！"),
            3000,
            true);
        return;
    }

    nlohmann::json _requestJson;
    _requestJson["type"] = "auth";
	_requestJson["account"] = userName.toStdString();
	_requestJson["password"] = userPasswd.toStdString();
    std::string caPath;
    UserRequest::Instance().SendRequestAsync(
        _requestJson,
        [this](bool ok, const nlohmann::json& response)
        {
            QMetaObject::invokeMethod(this, [this, ok, response]()
                {
                    if (!ok || response.empty())
                    {
                        ModelWindowPromptWindow::ShowMessageOnModelWindow(11,
                            QString("验证失败：服务器未连接或存在未错误！"),
                            3000,
                            true);
                        RuntimeLog_Debug("_responseJson is empty");
                        return;
                    }
                    else
                    {
                        if (response.contains("user_info") &&
                            response.at("user_info").contains("success") &&
                            response.at("user_info").at("success").get<bool>())
                        {
                            QString _userName = QString::fromStdString(response.at("user_info").at("display_name").get<std::string>());
                            QString _userUID = QString::fromStdString(std::to_string(response.at("user_info").at("user_id").get<int>()));
							QString _userEmail = QString::fromStdString(response.at("user_info").at("user_email").get<std::string>());
                            this->m_stackedWidget->setCurrentIndex(1);
                            QLabel* label;
                            label = this->findChild<QLabel*>(_PONL::Account_ObjNameList::label_UserName);
                            if (label)
                            {
                                label->setText(_userName);
                            }
                            label = this->findChild<QLabel*>(_PONL::Account_ObjNameList::label_UID);
                            if (label)
                            {
                                label->setText(_userUID);
                            }
                            label = this->findChild<QLabel*>(_PONL::Account_ObjNameList::label_Email);
                            if (label)
                            {
                                label->setText(_userEmail);
                            }
                        }
                        else
                        {
                            if (response.contains("user_info") && response.at("user_info").contains("error"))
                            {
                                std::string error = response.at("user_info").at("error").get<std::string>();
                                ModelWindowPromptWindow::ShowMessageOnModelWindow(11,
                                    QString("登陆失败: ") + QString::fromStdString(error),
                                    3000,
                                    true);
                                return;
                            }
                        }
                    }
                }, Qt::QueuedConnection);
        },
        5000);
}

void Page_Account::OpenForgotPasswd_Account()
{
    QDesktopServices::openUrl(QUrl("https://wang-sz.cn"));
}

void Page_Account::SendFeedback()
{
    QPlainTextEdit* textEdit_UserFeedback = this->findChild<QPlainTextEdit*>(_PONL::Account_ObjNameList::plainTextEdit_UserFeedback);

    if (textEdit_UserFeedback)
    {
        QString feedback = textEdit_UserFeedback->toPlainText();
		textEdit_UserFeedback->clear();
        textEdit_UserFeedback->insertPlainText("其实开发者还没有写反馈功能，请关注公告栏。（躺平的颜文字）");
    }
}

void Page_Account::GetNewAnnouncement()
{
    QPlainTextEdit* textEdit_Announcement = this->findChild<QPlainTextEdit*>(_PONL::Account_ObjNameList::plainTextEdit_Announcement);
    if (textEdit_Announcement)
    {
        textEdit_Announcement->clear();
		nlohmann::json _requestJson, _responseJson;
		_requestJson["type"] = "announcement";

        UserRequest::Instance().SendRequestAsync(
            _requestJson,
            [this, textEdit_Announcement](bool ok, const nlohmann::json& response)
            {
                QMetaObject::invokeMethod(this, [this, ok, response, textEdit_Announcement]()
                    {
                        if (!ok || response.empty())
                        {
                            textEdit_Announcement->insertPlainText("暂无公告 或 获取公告失败");
                        }
                        else
                        {
                            nlohmann::json _data = response["data"];
                            std::string announcement = _data["message"].get<std::string>();
                            textEdit_Announcement->clear();
                            textEdit_Announcement->insertPlainText(QString::fromStdString(announcement));

                        }
                    }, Qt::QueuedConnection);
            },
            5000);
        return;
    }
}

void Page_Account::SetPwdDisplayStatus_IconSwitching(QLineEdit* lineEdit_UserPasswd, QAction* QAction_toggleAction, bool isVisible)
{
    if (isVisible)
    {
        lineEdit_UserPasswd->setEchoMode(QLineEdit::Normal);
        QAction_toggleAction->setIcon(QIcon(Config_Account::instance().GetIcon_ShowPassword()));
    }
    else
    {
        lineEdit_UserPasswd->setEchoMode(QLineEdit::Password);
        QAction_toggleAction->setIcon(QIcon(Config_Account::instance().GetIcon_HidePassword()));
    }
}

// ========= end ==========


// ========== class Page_Personalization ==========

Page_Personalization::Page_Personalization(QWidget* parent) :
    QWidget(parent)
{
    QWidget* page = CreatePage_ListWidget();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(page); // 填满整个区域

    setLayout(mainLayout);
}
QWidget* Page_Personalization::CreatePage_ListWidget()
{
    QWidget* page = new QWidget(this);
    QGroupBox* box = new QGroupBox(page);

    QListWidget* listWidget = new QListWidget();
    listWidget->setStyleSheet(R"(
        QListWidget::item {
            background: #ffffff;
            border: 1px solid #d0d0d0;
            border-radius: 6px;
            padding: 2px;
            margin: 2px;          /* 让 item 之间有空隙 */
        }
        QListWidget::item:selected {
            background: #f2f7ff;
            border: 1px solid #1976d2;
            border-left: 4px solid #1976d2;
        }
        QListWidget::item:hover {
            border: 1px solid #64b5f6;
        }
    )");

    for (int i = 0; i < GetPluginsManagerInstance().GetPluginsCount(); ++i)
    {
        ListItemWidget* item_pluginInfo = new ListItemWidget(listWidget);
        item_pluginInfo->SetItemIndex(static_cast<short>(i));
        item_pluginInfo->SetVersion(GetPluginsManagerInstance().GetVersion(i));
        item_pluginInfo->SetPluginName(GetPluginsManagerInstance().GetPluginName(i));
        item_pluginInfo->SetPluginIcon(GetPluginsManagerInstance().GetPluginAbsolutePath_Icon(i));
        item_pluginInfo->SetPluginExecutableFilePath(GetPluginsManagerInstance().GetPluginAbsolutePath_ExecutableFile(i));
        item_pluginInfo->SetDecription(GetPluginsManagerInstance().GetDescription(i));
        item_pluginInfo->SetHotKeyEditInfo(GetPluginsManagerInstance().GetHotKey(i));
        item_pluginInfo->SetAutoRegisterCheckBoxStatus(GetPluginsManagerInstance().GetAutoRegistedStatus(i));
        item_pluginInfo->SetAutoStartCheckBoxStatus(GetPluginsManagerInstance().GetAutoStartStatus(i));
        item_pluginInfo->SetPluginLabelSize_AdjustSize();
        if (GetPluginsManagerInstance().GetAutoRegistedStatus(i) == true)
        {
            item_pluginInfo->InitHotKeyOperationButtonStatus(GetPluginsManagerInstance().GetRegisteredStatus(i));
            // 判断是否已经成功自动注册
            if (GetPluginsManagerInstance().GetRegisteredStatus(i) == true)
            {
				item_pluginInfo->SetHotKeyInfo(GetPluginsManagerInstance().GetHotKey(i));
                item_pluginInfo->SetHotKeyStatus(static_cast<unsigned char>(2));
            }
            else
            {
                item_pluginInfo->SetHotKeyStatus(static_cast<unsigned char>(1));
            }
        }
        else
        {
            item_pluginInfo->SetHotKeyStatus(static_cast<unsigned char>(3));
        }

        QListWidgetItem* listItem_Buffer = new QListWidgetItem(listWidget);
        listItem_Buffer->setSizeHint(QSize(0, 100));
        listWidget->setItemWidget(listItem_Buffer, item_pluginInfo);
    }

    QVBoxLayout* vLayout = new QVBoxLayout(box);
	vLayout->addWidget(listWidget);
	vLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* pageLayout = new QVBoxLayout(page);
    pageLayout->addWidget(box);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    return page;
}
// ========= end ==========

// ========== class Page_FuntionWindows ==========
Page_FuntionWindows::Page_FuntionWindows(QWidget* parent) : 
	QWidget(parent)
{
    QWidget* page = CreatePage();
    if (!page)
    {
		RuntimeLog_Debug("page is nullptr");
        return;
    }

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(page); // 填满整个区域

	setLayout(mainLayout);
}

void Page_FuntionWindows::InitPageFromJson()
{
}

void Page_FuntionWindows::SavePageToJson()
{
}

Config_Function& Page_FuntionWindows::GetPageConfigInstance()
{
    return Configuration::GetIns_FunctionWindowsPageConfig();
}

template <typename _T>
void Page_FuntionWindows::SetJsonDataFromWidget(int index, _T _t_value)
{
    switch (index)
    {
    case 0:     // 错误日志
		this->GetPageConfigInstance().SetUseRuntimeErrorLog(_t_value);
        return;
    case 1:     // 设置是否使用模型唇型同步
        this->GetPageConfigInstance().SetUse_Model_lip_shape(_t_value);
        return;
	case 2:  // 设置是否使用语音助手
        this->GetPageConfigInstance().SetUseVA(_t_value);
        return;
    case 3:  // 设置是否使用快捷键
        this->GetPageConfigInstance().SetUseShortcut_Key_Activate_VA(_t_value);
        return;
    case 4:  // 设置是否使用开发者模式
		this->GetPageConfigInstance().SetUseDeveloperMode(_t_value);
        return;
    case 5:  // 设置是否使用鼠标跟随
        this->GetPageConfigInstance().SetUseMouseMoveFollow(_t_value);
		return;
    case 6:  // 设置是否显示CPU使用率
		this->GetPageConfigInstance().SetShow_CPU_Usage(_t_value);
		return;
	case 7:  // 设置是否使用多线程显示CPU使用率
		this->GetPageConfigInstance().SetUse_MultiThread_CPU_Usage(_t_value);
		return;
	case 8:  // 设置是否显示内存使用率
		this->GetPageConfigInstance().SetShow_Memory_Usage(_t_value);
		return;
	case 9:  // 设置是否使用多线程显示内存使用率
		this->GetPageConfigInstance().SetUse_MultiThread_Memory_Usage(_t_value);
		return;

        // RadioButton
    case 100:
        this->GetPageConfigInstance().SetMouseMoveFollowGlobal(_t_value);
        return;
	case 101:
		this->GetPageConfigInstance().SetMouseMoveFollowLocal(_t_value);
		return;
        // SpinBox
    case 150:
        this->GetPageConfigInstance().SetThreadFlushTime_CPU(_t_value);
		return;
	case 151:
		this->GetPageConfigInstance().SetThreadFlushTime_Memory(_t_value);
		return;

    case 300:
        this->GetPageConfigInstance().SetShowMessageStyle(_t_value);
        return;
    }
}

QWidget* Page_FuntionWindows::CreatePage()
{
    QWidget* mainPage = new QWidget(this);

    QGroupBox* groupBox_main = new QGroupBox(mainPage);

    QToolBox* toolBox_main = new QToolBox(mainPage);
    toolBox_main->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    toolBox_main->setStyleSheet(R"(
        QToolBox {
            background-color: #f5f5f5;
            border: none;
            font-family: "Segoe UI", Arial, sans-serif;
        }
        QToolBox::tab {
            background: #e0e0e0;
            border-radius: 2px;
            padding: 2px;
            font-weight: bold;
            color: #333;
        }
        QToolBox::tab:selected {
            background: #0078d7;
            color: white;
        }
        QToolBox::tab:hover {
            background: #d0d0d0;
        }
        QToolBox::tab:selected:hover {
            background: #005fa3;
        }
        QWidget {
            background-color: #ffffff;
        }
    )");

    // tab - 创建展示信息相关
    try {
        QWidget* widget_show_message = CreatePage_ShowMessage();
        if (!widget_show_message)
        {
			RuntimeLog_Error("CreatePage_Show_CPU_RAM_INFO filed");
        }
        else
        {
            toolBox_main->addItem(widget_show_message, "消息显示相关");
        }
    }
    catch (...)
    {
        RuntimeLog_Error("Error creating display information");
    }
    
    // tab - Live2D相关
    try
    {
        QWidget* widget_About_Live2D = CreatePage_About_Live2D();
        if (!widget_About_Live2D)
        {
			RuntimeLog_Debug("CreatePage_About_Live2D filed");
        }
        else
        {
            toolBox_main->addItem(widget_About_Live2D, "模型动画相关");
        }
    }
    catch (...)
    {
		RuntimeLog_Error("Error creating Live2D related page");
    }

    // tab - 窗口相关
    try {
        QWidget* widget_About_Window = CreatePage_About_Window();
        if (!widget_About_Window)
        {
			RuntimeLog_Debug("CreatePage_About_Window filed");
        }
        else
        {
            toolBox_main->addItem(widget_About_Window, "窗口相关");
        }
    }
    catch (...)
    {
        RuntimeLog_Error("Error creating window related page");
    }

    // tab - AI相关
    try {
        QWidget* widget_About_AI = CreatePage_About_AI();
        if (!widget_About_AI)
        {
			RuntimeLog_Debug("CreatePage_About_AI filed");
        }
        else
        {
            toolBox_main->addItem(widget_About_AI, "AI相关");
        }
    }
    catch (...)
    {
		RuntimeLog_Debug("Error creating AI related page");
    }

    // tab - More
    try {
        QWidget* widget_more = CreatePage_More();
        if (!widget_more)
        {
			RuntimeLog_Debug("CreatePage_More filed");
        }
        else
        {
            toolBox_main->addItem(widget_more, "其它");
        }
    }
    catch (...)
    {
		RuntimeLog_Debug("Error creating other page");
    }
   
    toolBox_main->setCurrentIndex(0);
    // 总布局
    QVBoxLayout* groupLayout = new QVBoxLayout(groupBox_main);
    groupLayout->addWidget(toolBox_main);
    groupLayout->addStretch();
    groupLayout->setContentsMargins(5, 5, 5, 5);

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(groupBox_main);
	scrollArea->setWidgetResizable(true);
	scrollArea->setFrameShape(QFrame::NoFrame);

    return scrollArea;
}

QWidget* Page_FuntionWindows::CreatePage_ShowMessage()
{
    QWidget* widget_show_message = new QWidget(this);
	QVBoxLayout* vLayout_show_message = new QVBoxLayout(widget_show_message);
    QTabWidget* tabWidget_show_message = new QTabWidget();


    // CPU 占用
    QWidget* qwidget_cpu = new QWidget();
    try
    {
        QGroupBox* groupBox_cpu = new QGroupBox();
        // 设置CPU是否展示
        QCheckBox* checkBox_show_cpu = new QCheckBox("显示CPU占用");
        checkBox_show_cpu->setObjectName(_PONL::Funtion_ObjNameList::checkBox_show_cpu);
        checkBox_show_cpu->setChecked(Config_Function::instance().IsShow_CPU_Usage());
        // 竖线分割线
        QFrame* line_cpu = new QFrame();
        line_cpu->setFrameShape(QFrame::VLine);
        line_cpu->setFrameShadow(QFrame::Sunken);
        // CPU多线程展示设置
        QCheckBox* checkBox_show_cpu_thread = new QCheckBox("多线程模式");
        checkBox_show_cpu_thread->setObjectName(_PONL::Funtion_ObjNameList::checkBox_show_cpu_thread);
        checkBox_show_cpu_thread->setChecked(Config_Function::instance().IsUse_MultiThread_CPU_Usage());
        QLabel* label_cpuinfo_flushtime = new QLabel("线程刷新间隔:");
        QSpinBox* spinBox_cpuinfo_flushtime = new QSpinBox();
        spinBox_cpuinfo_flushtime->setObjectName(_PONL::Funtion_ObjNameList::spinBox_flushtime_cpu);
        spinBox_cpuinfo_flushtime->setValue(Config_Function::instance().GetThreadFlushTime_CPU());
        spinBox_cpuinfo_flushtime->setSuffix(" 秒");
        spinBox_cpuinfo_flushtime->setRange(1, 10);
        spinBox_cpuinfo_flushtime->setSingleStep(1);
        spinBox_cpuinfo_flushtime->setFixedWidth(100);
        // 布局
        QHBoxLayout* hLayout_cpu = new QHBoxLayout(groupBox_cpu);
        hLayout_cpu->addWidget(checkBox_show_cpu);
        hLayout_cpu->addWidget(line_cpu);
        hLayout_cpu->addWidget(checkBox_show_cpu_thread);
        hLayout_cpu->addWidget(label_cpuinfo_flushtime);
        hLayout_cpu->addWidget(spinBox_cpuinfo_flushtime);
        hLayout_cpu->addStretch();
        QVBoxLayout* vLayout_cpu = new QVBoxLayout();
        vLayout_cpu->addWidget(groupBox_cpu);
        qwidget_cpu->setLayout(vLayout_cpu);

        // 连接信号
        connect(checkBox_show_cpu, &QCheckBox::checkStateChanged, this, [=]() {
            // 更新CPU多线程设置
            bool _checked = checkBox_show_cpu->isChecked();
            checkBox_show_cpu_thread->setEnabled(_checked);
            if (_checked == false)
            {
                spinBox_cpuinfo_flushtime->setEnabled(_checked);
            }
            else
            {
                spinBox_cpuinfo_flushtime->setEnabled(checkBox_show_cpu_thread->isChecked());
            }
            // 更新配置
            Config_Function::instance().SetShow_CPU_Usage(_checked);
            });

        connect(checkBox_show_cpu_thread, &QCheckBox::checkStateChanged, this, [=]() {
            // 更新状态
            bool _checked = checkBox_show_cpu_thread->isChecked();
            spinBox_cpuinfo_flushtime->setEnabled(_checked);
            // 更新配置
            Config_Function::instance().SetUse_MultiThread_CPU_Usage(_checked);
            });

        connect(spinBox_cpuinfo_flushtime, &QSpinBox::valueChanged, this, [=]() {
            // 更新配置
            Config_Function::instance().SetThreadFlushTime_CPU(spinBox_cpuinfo_flushtime->value());
            });
    }
    catch (std::exception& e)
    {
		RuntimeLog::instance().WriteLogInfo("_T_Page_Config_Subclass& ConfigurationBase::GetConfigInstanceeFromConfiguration()",
			QString::fromStdString(e.what()));

    }
    // RAM 占用
    QWidget* qwidget_ram = new QWidget();
    try
    {
        QGroupBox* groupBox_ram = new QGroupBox();
        // 设置RAM是否展示
        QCheckBox* checkBox_show_ram = new QCheckBox("显示RAM占用");
        checkBox_show_ram->setObjectName(_PONL::Funtion_ObjNameList::checkBox_show_memory);
        checkBox_show_ram->setChecked(Config_Function::instance().IsShow_Memory_Usage());
        // 竖线分割线
        QFrame* line_ram = new QFrame();
		line_ram->setFrameShape(QFrame::VLine);
		line_ram->setFrameShadow(QFrame::Sunken);
        // RAM多线程展示设置
        QCheckBox* checkBox_show_ram_thread = new QCheckBox("多线程模式");
        checkBox_show_ram_thread->setObjectName(_PONL::Funtion_ObjNameList::checkBox_show_memory_thread);
		checkBox_show_ram_thread->setChecked(Config_Function::instance().IsUse_MultiThread_Memory_Usage());
        QLabel* label_flushtime = new QLabel("线程刷新间隔:");
        QSpinBox* spinBox_raminfo_flushtime = new QSpinBox();
        spinBox_raminfo_flushtime->setObjectName(_PONL::Funtion_ObjNameList::spinBox_flushtime_memory);
        spinBox_raminfo_flushtime->setValue(Config_Function::instance().GetThreadFlushTime_Memory());
        spinBox_raminfo_flushtime->setSuffix(" 秒");
        spinBox_raminfo_flushtime->setRange(1, 10);
        spinBox_raminfo_flushtime->setSingleStep(1);
        spinBox_raminfo_flushtime->setFixedWidth(100);

        // 布局
        QHBoxLayout* hLayout_ram = new QHBoxLayout(groupBox_ram);
        hLayout_ram->addWidget(checkBox_show_ram);
		hLayout_ram->addWidget(line_ram);
		hLayout_ram->addWidget(checkBox_show_ram_thread);
		hLayout_ram->addWidget(label_flushtime);
		hLayout_ram->addWidget(spinBox_raminfo_flushtime);
        hLayout_ram->addStretch();

        QVBoxLayout* vLayout_ram = new QVBoxLayout();
        vLayout_ram->addWidget(groupBox_ram);
		qwidget_ram->setLayout(vLayout_ram);

        connect(checkBox_show_ram, &QCheckBox::checkStateChanged, this, [=]() {
            bool _checked = checkBox_show_ram->isChecked();
            checkBox_show_ram_thread->setEnabled(_checked);
            if (_checked == false)
            {
                spinBox_raminfo_flushtime->setEnabled(_checked);
            }
            else
            {
                spinBox_raminfo_flushtime->setEnabled(checkBox_show_ram_thread->isChecked());
            }
            });
        connect(checkBox_show_ram_thread, &QCheckBox::checkStateChanged, this, [=]() {
            bool _checked = checkBox_show_ram_thread->isChecked();
            spinBox_raminfo_flushtime->setEnabled(_checked);
            });
        connect(spinBox_raminfo_flushtime, &QSpinBox::valueChanged, this, [=]() {
            Config_Function::instance().SetThreadFlushTime_Memory(spinBox_raminfo_flushtime->value());
            });
    }
    catch (std::exception& e)
    {
		RuntimeLog::instance().WriteLogInfo("_T_Page_Config_Subclass& ConfigurationBase::GetConfigInstanceeFromConfiguration()",
			QString::fromStdString(e.what()));
    }
	QVBoxLayout* vLayout_ram = nullptr; // 内存 布局
    QWidget* qwidget_message_style = new QWidget();
    QVBoxLayout* vLayout_message_style = nullptr;   // 消息样式
    QWidget* qwidget_text_style = new QWidget();
    QVBoxLayout* vLayout_text_style = nullptr;  // 文本样式

    // 消息显示模型选择
    try
    {
        QLabel* label_message_style = new QLabel("消息显示模式:");
        label_message_style->setObjectName(_PONL::Funtion_ObjNameList::label_show_message_style);
        QComboBox* comboBox_message_style = new QComboBox();
		comboBox_message_style->setObjectName(_PONL::Funtion_ObjNameList::combobox_show_message_style);
		comboBox_message_style->addItem("流式显示消息文本");
		comboBox_message_style->addItem("直接显示消息文本");
		comboBox_message_style->addItem("混合显示消息文本（推荐）");
		comboBox_message_style->addItem("不显示任何消息文本（不推荐）");
        // 防止配置文件中的 Show_Message_Style 值越界，导致 Qt 内部 QList 访问越界
        {
            int styleIndex = Configuration::GetIns_FunctionWindowsPageConfig().GetShow_Message_Style() - 1;
            if (styleIndex < 0 || styleIndex >= comboBox_message_style->count())
            {
                // 配置非法时回退到默认样式（第一个选项）
                styleIndex = 0;
            }
            comboBox_message_style->setCurrentIndex(styleIndex);
        }

		QHBoxLayout* hLayout_message_style = new QHBoxLayout();
		hLayout_message_style->addWidget(label_message_style);
		hLayout_message_style->addWidget(comboBox_message_style);
		hLayout_message_style->addStretch();

        vLayout_message_style = new QVBoxLayout(qwidget_message_style);
		vLayout_message_style->addLayout(hLayout_message_style);

		connect(comboBox_message_style, &QComboBox::currentIndexChanged, this, [this, comboBox_message_style]() {
			this->SetJsonDataFromWidget(300, (comboBox_message_style->currentIndex() + 1));
			});
    }
    catch (std::exception& e)
    {
		RuntimeLog::instance().WriteLogInfo("_T_Page_Config_Subclass& ConfigurationBase::GetConfigInstanceeFromConfiguration()",
			QString::fromStdString(e.what()));
    }

    // 文本样式
    try
    {
        QLabel* label_text_style = new QLabel(" TODO(有时间写) \n(Ciallo～ (∠・ω< )⌒★)");
        QHBoxLayout* hLayout_text_style = new QHBoxLayout();
		hLayout_text_style->addWidget(label_text_style);

		vLayout_text_style = new QVBoxLayout(qwidget_text_style);
		vLayout_text_style->addLayout(hLayout_text_style);
    }
    catch (std::exception& e)
    {
		RuntimeLog::instance().WriteLogInfo("_T_Page_Config_Subclass& ConfigurationBase::GetConfigInstanceeFromConfiguration()",
			QString::fromStdString(e.what()));
    }

    // 文本样式
    tabWidget_show_message->addTab(qwidget_message_style,   "消息显示模式");
    tabWidget_show_message->addTab(qwidget_text_style,      "消息文本样式");
    tabWidget_show_message->addTab(qwidget_cpu,             "CPU信息展示");
	tabWidget_show_message->addTab(qwidget_ram,             "RAM信息展示");

    vLayout_show_message->addWidget(tabWidget_show_message);
	widget_show_message->setLayout(vLayout_show_message);

    return widget_show_message;
}

QWidget* Page_FuntionWindows::CreatePage_About_Live2D()
{
    QWidget* widget_About_Live2D = new QWidget();
    // 创建滚动区域
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(widget_About_Live2D);           // 放入内容widget
    scrollArea->setWidgetResizable(true);                // 关键：允许widget调整大小
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 禁用水平滚动条
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);     // 垂直滚动条按需显示
    scrollArea->setFrameShape(QFrame::NoFrame);          // 无边框
    scrollArea->setMinimumSize(300, 400);
    // 总的布局
    QVBoxLayout* vLayout_About_Live2D = new QVBoxLayout(widget_About_Live2D);
    vLayout_About_Live2D->setSpacing(10);
    vLayout_About_Live2D->setContentsMargins(10, 10, 10, 10);

    // 鼠标跟随相关
    {
        QCheckBox* checkBox_mouse_follow = new QCheckBox("鼠标跟随");
        checkBox_mouse_follow->setObjectName(_PONL::Funtion_ObjNameList::checkBox_mouse_follow);
        vLayout_About_Live2D->addWidget(checkBox_mouse_follow);

        QRadioButton* radioButton_mouse_follow_global = new QRadioButton("全局跟随");
        radioButton_mouse_follow_global->setObjectName(_PONL::Funtion_ObjNameList::radioButton_mouse_follow_global);
        QRadioButton* radioButton_mouse_follow_local = new QRadioButton("局部跟随");
        radioButton_mouse_follow_local->setObjectName(_PONL::Funtion_ObjNameList::radioButton_mouse_follow_local);
        QHBoxLayout* hLayout_mouse_follow = new QHBoxLayout();
        hLayout_mouse_follow->addWidget(radioButton_mouse_follow_global);
        hLayout_mouse_follow->addWidget(radioButton_mouse_follow_local);
		// 连接信号槽
        connect(checkBox_mouse_follow, &QCheckBox::clicked, this, [this, checkBox_mouse_follow]() {
            this->SetJsonDataFromWidget(5, checkBox_mouse_follow->isChecked());
            });
        connect(radioButton_mouse_follow_global, &QRadioButton::clicked, this, [this, radioButton_mouse_follow_global]() {
            this->SetJsonDataFromWidget(6, radioButton_mouse_follow_global->isChecked());
            });
        vLayout_About_Live2D->addLayout(hLayout_mouse_follow);
    }

    // 分割线 - 1
    {
        QFrame* line1 = new QFrame();
        line1->setFrameShape(QFrame::HLine);      // 水平线
        line1->setFrameShadow(QFrame::Sunken);    // 凹陷效果
        line1->setLineWidth(2);                   // 线宽
        vLayout_About_Live2D->addWidget(line1);
    }

	// 模型窗口置顶
    {
        QCheckBox* checkBox_model_window_top = new QCheckBox("模型窗口置顶");
        checkBox_model_window_top->setObjectName(_PONL::Funtion_ObjNameList::checkBox_model_window_top);
        checkBox_model_window_top->setChecked(Configuration::GetIns_FunctionWindowsPageConfig().IsModelWindowAllwaysOnTop());
        connect(checkBox_model_window_top, &QCheckBox::clicked, this, [this, checkBox_model_window_top]() {
            this->GetPageConfigInstance().SetModelWindowAllwaysOnTop(checkBox_model_window_top->isChecked());
            });
        vLayout_About_Live2D->addWidget(checkBox_model_window_top);
    }
    // 分割线 - 2
    {
        QFrame* line2 = new QFrame();
        line2->setFrameShape(QFrame::HLine);      // 水平线
        line2->setFrameShadow(QFrame::Sunken);    // 凹陷效果
        line2->setLineWidth(2);                   // 线宽
        vLayout_About_Live2D->addWidget(line2);
    }
    
    vLayout_About_Live2D->addStretch();
    return scrollArea;
}

QWidget* Page_FuntionWindows::CreatePage_About_Window()
{
	QWidget* widget_about_window = new QWidget(this);
	QVBoxLayout* vLayout_about_window = new QVBoxLayout(widget_about_window);
  
    // 模型窗口的大小
    {
        QCheckBox* checkBox_use_default_model_window_size = new QCheckBox("使用默认模型窗口大小");
        // 分割线 - 3
        QFrame* line3 = new QFrame();
        line3->setFrameShape(QFrame::VLine);      // 垂直线
        line3->setFrameShadow(QFrame::Sunken);    // 凹陷效果
        line3->setLineWidth(2);                   // 线宽
        QPushButton* button_apply_model_window_size = new QPushButton("应用基准大小");
        button_apply_model_window_size->adjustSize();
        QHBoxLayout* hLayout_model_window_size_setting = new QHBoxLayout();
        hLayout_model_window_size_setting->addWidget(checkBox_use_default_model_window_size);
        hLayout_model_window_size_setting->addWidget(line3);
        hLayout_model_window_size_setting->addWidget(button_apply_model_window_size);
        hLayout_model_window_size_setting->addStretch();

        QLabel* label_model_window_size_slider = new QLabel("模型窗口缩放因子");
        // 缩放因子滑块
        QSlider* slider_window_zoom = new QSlider(Qt::Horizontal);
        slider_window_zoom->setRange(1, 200);
        slider_window_zoom->setValue(static_cast<int>(Configuration::GetIns_FunctionWindowsPageConfig().GetWindowZoomFactor() * 100));
        slider_window_zoom->setTickPosition(QSlider::TicksBelow);
        slider_window_zoom->setTickInterval(1);
        QLabel* label_slider_value = new QLabel(QString("窗口缩放因子值: %1")
            .arg(Configuration::GetIns_FunctionWindowsPageConfig().GetWindowZoomFactor(), 0, 'f', 2));
        label_slider_value->setAlignment(Qt::AlignCenter);
        // 样式美化
        slider_window_zoom->setStyleSheet(R"(
            QSlider::groove:horizontal {
                height: 8px;
                background: #e0e0e0;
                border-radius: 4px;
            }
            QSlider::handle:horizontal {
                width: 18px;
                height: 18px;
                margin: -5px 0;
                background: #2196F3;
                border-radius: 9px;
            }
            QSlider::sub-page:horizontal {
                background: #2196F3;
                border-radius: 4px;
            }
        )");
        connect(slider_window_zoom, &QSlider::valueChanged, this, [this, slider_window_zoom, label_slider_value]() {
            float windowZoomFactor = (float)slider_window_zoom->value() / 100.0f;
            this->GetPageConfigInstance().SetWindowZoomFactor(windowZoomFactor);
            label_slider_value->setText(QString("窗口缩放因子值: %1").arg(windowZoomFactor, 0, 'f', 2));

            });
        QHBoxLayout* hLayout_model_window_size_slider = new QHBoxLayout();
        hLayout_model_window_size_slider->addWidget(label_model_window_size_slider);
        hLayout_model_window_size_slider->addWidget(slider_window_zoom);
        hLayout_model_window_size_slider->addWidget(label_slider_value);
        hLayout_model_window_size_slider->addStretch();
        // 缩放因子调整按钮
        QLabel* label_model_window_size_button = new QLabel("模型窗口缩放因子: ");
        QPushButton* button_reset_model_window_size_decrease = new QPushButton("-0.01");
        button_reset_model_window_size_decrease->adjustSize();
        QPushButton* button_reset_model_window_size_increase = new QPushButton("+0.01");
        button_reset_model_window_size_increase->adjustSize();
        connect(button_reset_model_window_size_decrease, &QPushButton::clicked, this, [this, slider_window_zoom, label_slider_value]() {
            int currentValue = slider_window_zoom->value();
            if (currentValue > slider_window_zoom->minimum())
            {
                slider_window_zoom->setValue(currentValue - 1);
                label_slider_value->setText(QString("窗口缩放因子值: %1").arg((float)currentValue / 100.0f, 0, 'f', 2));
            }
            });
        connect(button_reset_model_window_size_increase, &QPushButton::clicked, this, [this, slider_window_zoom, label_slider_value]() {
            int currentValue = slider_window_zoom->value();
            if (currentValue < slider_window_zoom->maximum())
            {
                slider_window_zoom->setValue(currentValue + 1);
                label_slider_value->setText(QString("窗口缩放因子值: %1").arg((float)currentValue / 100.0f, 0, 'f', 2));
            }
            });
        QHBoxLayout* hLayout_model_window_size_button = new QHBoxLayout();
        hLayout_model_window_size_button->addWidget(label_model_window_size_button);
        hLayout_model_window_size_button->addWidget(button_reset_model_window_size_decrease);
        hLayout_model_window_size_button->addWidget(button_reset_model_window_size_increase);
        hLayout_model_window_size_button->addStretch();

        // 窗口高度部分
        QLabel* label_model_window_size_height = new QLabel("窗口基准高度(px): ");
        label_model_window_size_height->adjustSize();
        QLineEdit* lineEdit_model_window_size_height = new QLineEdit();
        lineEdit_model_window_size_height->setValidator(new QIntValidator(0, 900, this));
        lineEdit_model_window_size_height->setText(QString::number(Configuration::GetIns_FunctionWindowsPageConfig().GetDefaultModelWindowHeight()));
        QLabel* label_model_window_size_height_tooltip = _SettingReusablePage::_CreateDoubtPage(
            QString("?"),
            QString("\n设置模型的高度，可以搭配缩放因子使用"),
            Qt::AlignCenter);
        label_model_window_size_height_tooltip->setStyleSheet("QLabel { "
            "border: 1px solid #d0d0d0;"
            "border-radius: 4px;"
            "padding: 1px;"
            "background-color: #f5f5f5;}");
        label_model_window_size_height_tooltip->setFixedHeight(15);
        label_model_window_size_height_tooltip->setFixedWidth(15);
        QHBoxLayout* hLayout_model_window_size_height = new QHBoxLayout();
        hLayout_model_window_size_height->addWidget(label_model_window_size_height);
        hLayout_model_window_size_height->addWidget(lineEdit_model_window_size_height);
        hLayout_model_window_size_height->addWidget(label_model_window_size_height_tooltip);
        connect(lineEdit_model_window_size_height, &QLineEdit::textChanged, this, [this, lineEdit_model_window_size_height]() {
            int value = lineEdit_model_window_size_height->text().toInt();
            if (value > 900)
            {
                lineEdit_model_window_size_height->setText(QString::number(900));
            }
            if (value < 0)
            {
                lineEdit_model_window_size_height->setText(QString::number(300));
            }
            this->GetPageConfigInstance().SetDefaultModelWindowHeight(value);
            });


        // 窗口宽度部分
        QLabel* label_model_window_size_width = new QLabel("窗口基准宽度(px): ");
        label_model_window_size_width->adjustSize();
        QLineEdit* lineEdit_model_window_size_width = new QLineEdit();
        lineEdit_model_window_size_width->setValidator(new QIntValidator(0, 1200, this));
        lineEdit_model_window_size_width->setText(QString::number(Configuration::GetIns_FunctionWindowsPageConfig().GetDefaultModelWindowWidth()));
        QLabel* label_model_window_size_height2_tooltip = _SettingReusablePage::_CreateDoubtPage(
            QString("?"),
            QString("\n设置模型的宽度，可以搭配缩放因子使用"),
            Qt::AlignCenter);
        LAppStyleManager::SetLabelStyle(label_model_window_size_height2_tooltip, 27);
        label_model_window_size_height2_tooltip->setFixedHeight(15);
        label_model_window_size_height2_tooltip->setFixedWidth(15);
        QHBoxLayout* hLayout_model_window_size_width = new QHBoxLayout();
        hLayout_model_window_size_width->addWidget(label_model_window_size_width);
        hLayout_model_window_size_width->addWidget(lineEdit_model_window_size_width);
        hLayout_model_window_size_width->addWidget(label_model_window_size_height2_tooltip);
        connect(lineEdit_model_window_size_width, &QLineEdit::textChanged, this, [this, lineEdit_model_window_size_width]() {
            int value = lineEdit_model_window_size_width->text().toInt();
            if (value > 1200)
            {
                lineEdit_model_window_size_width->setText(QString::number(1200));
            }
            if (value < 0)
            {
                lineEdit_model_window_size_width->setText(QString::number(400));
            }
            this->GetPageConfigInstance().SetDefaultModelWindowWidth(value);
            });

        // 分割线 - 4
        QFrame* line4 = new QFrame();
        line4->setFrameShape(QFrame::VLine);      // 垂直线
        line4->setFrameShadow(QFrame::Sunken);    // 凹陷效果
        line4->setLineWidth(2);                   // 线宽		

        QHBoxLayout* hLayout_model_window_size = new QHBoxLayout();
        hLayout_model_window_size->addLayout(hLayout_model_window_size_height);
        hLayout_model_window_size->addWidget(line3);
        hLayout_model_window_size->addLayout(hLayout_model_window_size_width);
        hLayout_model_window_size->addWidget(button_apply_model_window_size);

        // 局部总布局
        QVBoxLayout* vLayout_model_window_size = new QVBoxLayout();
        vLayout_model_window_size->addLayout(hLayout_model_window_size_setting);
        vLayout_model_window_size->addLayout(hLayout_model_window_size_slider);
        vLayout_model_window_size->addLayout(hLayout_model_window_size_button);
        vLayout_model_window_size->addLayout(hLayout_model_window_size);

        // 处理是否使用默认窗口尺寸
        connect(checkBox_use_default_model_window_size, &QCheckBox::checkStateChanged, this, [=]() {
            slider_window_zoom->setEnabled(!checkBox_use_default_model_window_size->isChecked());
            button_reset_model_window_size_decrease->setEnabled(!checkBox_use_default_model_window_size->isChecked());
            button_reset_model_window_size_increase->setEnabled(!checkBox_use_default_model_window_size->isChecked());
            this->GetPageConfigInstance().SetUseDefaultModelWindowSize(checkBox_use_default_model_window_size->isChecked());
            });
        connect(button_apply_model_window_size, &QPushButton::clicked, this, [=]() {
            bool use_default_model_window_size = checkBox_use_default_model_window_size->isChecked();
            int height = lineEdit_model_window_size_height->text().toInt();
            int width = lineEdit_model_window_size_width->text().toInt();
            this->GetPageConfigInstance().SetUseDefaultModelWindowSize(use_default_model_window_size);
            this->GetPageConfigInstance().SetDefaultModelWindowHeight(height);
            this->GetPageConfigInstance().SetDefaultModelWindowWidth(width);
            float zoom_factor = slider_window_zoom->value() / 100.0f;
            this->GetPageConfigInstance().SetWindowZoomFactor(zoom_factor);
            if (use_default_model_window_size)
            {
                ModelWindowAgent::Instance().UpdateModelWindowSize(width, height);
            }
            else
            {
                width = width * zoom_factor;
                height = height * zoom_factor;
                ModelWindowAgent::Instance().UpdateModelWindowSize(width, height);
            }
            });
        connect(slider_window_zoom, &QSlider::valueChanged, this, [=]() {
            int height = lineEdit_model_window_size_height->text().toInt();
            int width = lineEdit_model_window_size_width->text().toInt();
            float zoom_factor = slider_window_zoom->value() / 100.0f;
            // 不需要判断是否使用默认窗口尺寸
            // 如果使用的话，按钮和滑块都是禁用的，无法调整缩放因子；
            // 如果不使用的话，说明用户想要使用自定义尺寸，这时候就直接按照自定义尺寸和缩放因子计算即可
            height = height * zoom_factor;
            width = width * zoom_factor;
            ModelWindowAgent::Instance().UpdateModelWindowSize(width, height);

            });
        connect(button_reset_model_window_size_decrease, &QPushButton::clicked, this, [=]() {
            int height = lineEdit_model_window_size_height->text().toInt();
            int width = lineEdit_model_window_size_width->text().toInt();
            height = std::round(height * (slider_window_zoom->value() / 100.0f));
            width = std::round(width * (slider_window_zoom->value() / 100.0f));
            ModelWindowAgent::Instance().UpdateModelWindowSize(width, height);
            });
        connect(button_reset_model_window_size_increase, &QPushButton::clicked, this, [=]() {
            int height = lineEdit_model_window_size_height->text().toInt();
            int width = lineEdit_model_window_size_width->text().toInt();
            height = std::round(height * (slider_window_zoom->value() / 100.0f));
            width = std::round(width * (slider_window_zoom->value() / 100.0f));
            ModelWindowAgent::Instance().UpdateModelWindowSize(width, height);
            });
        vLayout_about_window->addLayout(vLayout_model_window_size);
    }
	vLayout_about_window->addStretch();
    return widget_about_window;
}

QWidget* Page_FuntionWindows::CreatePage_About_AI()
{
    QWidget* widget_about_model = new QWidget(this);

    QCheckBox* checkBox_Model_lip_shape = new QCheckBox("模型唇形同步");
    checkBox_Model_lip_shape->setObjectName(_PONL::Funtion_ObjNameList::checkBox_Model_lip_shape);
    QCheckBox* checkBox_Model_VA = new QCheckBox("使用语音助手-已弃用");
    checkBox_Model_VA->setObjectName(_PONL::Funtion_ObjNameList::checkBox_Model_VA);
    QCheckBox* checkBox_Model_USE_HotKey = new QCheckBox("热键启动语音助手-已弃用");
    checkBox_Model_USE_HotKey->setObjectName(_PONL::Funtion_ObjNameList::checkBox_Model_USE_HotKey);
	QVBoxLayout* vLayout_about_model = new QVBoxLayout();
	vLayout_about_model->addWidget(checkBox_Model_VA);
    vLayout_about_model->addWidget(checkBox_Model_USE_HotKey);

    QKeySequenceEdit* ksEdit_hotkey = new QKeySequenceEdit(QKeySequence("Ctrl+F12"));
    ksEdit_hotkey->setObjectName(_PONL::Funtion_ObjNameList::ksEdit_hotkey);
    /* 
        QHotKey: https://github.com/Skycoder42/QHotkey
        QHotKey并不是Qt官方的热键库，但是可以很好的实现热键的功能
    */

    QPushButton* pushButton_RegisterHotkey = new QPushButton("注册热键");
	pushButton_RegisterHotkey->setObjectName(_PONL::Funtion_ObjNameList::pushButton_RegisterHotkey);
    QVBoxLayout* vLayout_hotkey = new QVBoxLayout();
	vLayout_hotkey->addWidget(ksEdit_hotkey);
	vLayout_hotkey->addWidget(pushButton_RegisterHotkey);
    
    QHBoxLayout* hLayout_vioce_assistant = new QHBoxLayout(widget_about_model);
    hLayout_vioce_assistant->addWidget(checkBox_Model_lip_shape);
	hLayout_vioce_assistant->addLayout(vLayout_about_model);
	hLayout_vioce_assistant->addLayout(vLayout_hotkey);

    connect(checkBox_Model_lip_shape, &QCheckBox::clicked, this, [this, checkBox_Model_lip_shape]() {
		this->SetJsonDataFromWidget(1, checkBox_Model_lip_shape->isChecked());
		});
	connect(checkBox_Model_VA, &QCheckBox::clicked, this, [this, checkBox_Model_VA]() {
		this->SetJsonDataFromWidget(2, checkBox_Model_VA->isChecked());
		});
	connect(checkBox_Model_USE_HotKey, &QCheckBox::clicked, this, [this, checkBox_Model_USE_HotKey]() {
		this->SetJsonDataFromWidget(3, checkBox_Model_USE_HotKey->isChecked());
		});
    // 注册热键
    connect(pushButton_RegisterHotkey, &QPushButton::clicked, this, [=]() {
        if (HotKeyManager::instance()->RegisterHotKey(ksEdit_hotkey->keySequence().toString(),
            QString("../../user/plugins/test_plugins/demo.exe")))
        {
            ModelWindowPromptWindow::ShowMessageOnModelWindow(11, QString("注册热键失败了"), 3000, true);
        }
        else
        {
            ModelWindowPromptWindow::ShowMessageOnModelWindow(11, "您已经注册过热键了哦！", 3000, true);
        }
        });

    return widget_about_model;
}

QWidget* Page_FuntionWindows::CreatePage_More()
{
    QWidget* widget_more = new QWidget(this);

    // 错误日志
    QCheckBox* checkBox_errorLog = new QCheckBox("错误日志");
	checkBox_errorLog->setObjectName(_PONL::Funtion_ObjNameList::checkBox_errorLog);
	checkBox_errorLog->setChecked(Config_Function::instance().IsRuntimeErrorLog());
    // 日志级别提示
    QLabel* label_logLevel = new QLabel("日志级别:");
    // 选择日志级别
    QComboBox* comboBox_logLevel = new QComboBox();
	comboBox_logLevel->setObjectName(_PONL::Funtion_ObjNameList::comboBox_logLevel);
    comboBox_logLevel->addItem("Info");
	comboBox_logLevel->addItem("Debug");
	comboBox_logLevel->addItem("Warn");
	comboBox_logLevel->addItem("Error");
    comboBox_logLevel->addItem("Fatal");
    comboBox_logLevel->setCurrentIndex(Config_Function::instance().GetLogLevel());
    // 连接CheckBox
    connect(checkBox_errorLog, &QCheckBox::clicked, this, [=](bool checked) {
		Config_Function::instance().SetUseRuntimeErrorLog(checked);
        });
    // 连接combo的信号
    connect(comboBox_logLevel, &QComboBox::currentIndexChanged, this, [=]() {
		QString logLevel = comboBox_logLevel->currentText();
        Config_Function::instance().SetRuntimeLogLevel(logLevel);
        });

    // 提示
    QLabel* label_errorLog_tooltip = _SettingReusablePage::_CreateDoubtPage(
        QString("?"),
        QString("\n开启可能会需要极少量的性能作为支撑\n \
            但是开发者仍然不建议您关闭，这将帮助开发者改进程序"),
            Qt::AlignCenter);
    LAppStyleManager::SetLabelStyle(label_errorLog_tooltip, 27);
    label_errorLog_tooltip->setFixedHeight(15);
    label_errorLog_tooltip->setFixedWidth(15);
	QHBoxLayout* hLayout_errorLog = new QHBoxLayout();
	hLayout_errorLog->addWidget(checkBox_errorLog);
	hLayout_errorLog->addWidget(label_logLevel);
	hLayout_errorLog->addWidget(comboBox_logLevel);
	hLayout_errorLog->addWidget(label_errorLog_tooltip);
    hLayout_errorLog->addStretch();

    // 开发者选项
	QCheckBox* checkBox_dev = new QCheckBox("开发者选项");
	checkBox_dev->setObjectName(_PONL::Funtion_ObjNameList::checkBox_dev);
	checkBox_dev->setChecked(Config_Function::instance().IsUseDeveloperMode());
    // 提示 - 开发者模式
    QLabel* label_developer_tooltip = _SettingReusablePage::_CreateDoubtPage(
        QString("?"),
        QString("\n开启后会重启程序会展示开发者选项页\n \
如果您是开发者，可以勾选此选项，以供您开发与调试"),
Qt::AlignCenter);
    LAppStyleManager::SetLabelStyle(label_developer_tooltip, 27);
    label_developer_tooltip->setFixedHeight(15);
    label_developer_tooltip->setFixedWidth(15);
    QHBoxLayout* hLayout_developer = new QHBoxLayout();
    hLayout_developer->addWidget(checkBox_dev);
    hLayout_developer->addWidget(label_developer_tooltip);
    hLayout_developer->addStretch();

	QVBoxLayout* vLayout_more = new QVBoxLayout(widget_more);
	vLayout_more->addLayout(hLayout_errorLog);
	vLayout_more->addLayout(hLayout_developer);

    connect(checkBox_dev, &QCheckBox::clicked, this, [=]() {
        Config_Function::instance().SetUseDeveloperMode(checkBox_dev->isChecked());
        });
    return widget_more;
}


// ======== end ==========

// ========== class Page_SetAI ==========
Page_SetAI::Page_SetAI(QWidget* parent) : 
	QWidget(parent)
{
	QWidget* page = CreateSetAIPage();
    if (!page)
    {
		RuntimeLog_Debug("Create SetAI Page failed!");
    }
    // 直接使用堆栈窗口作为主控件
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(page); // 填满整个区域

    setLayout(mainLayout);
    // 初始化页面数据
}

// 创建设置AI页面参数
QWidget* Page_SetAI::CreateSetAIPage()
{
    QWidget* page = new QWidget;

    QGroupBox* groupBox_BaseSettings = CreateBaseSettingPage();

    QGroupBox* groupBox_MoreSettings = new QGroupBox();
    QTabWidget* tabWidget_More = new QTabWidget();
    tabWidget_More->setDocumentMode(true);               // 标签栏更扁平
    QWidgetStyleManager::SetTabWidgetStyle(tabWidget_More, 2);

    QWidget* qwidget_MoreSettings = CreateMoreSettingPage();
    QWidget* qwidget_CustomSetting = CreateCustomSettingPage();
    QWidget* qwidget_AddCustomJson = CreateCustomFieldPage();
    tabWidget_More->addTab(qwidget_MoreSettings, "更多AI设置");
    tabWidget_More->addTab(qwidget_AddCustomJson, "自定义字段");
    tabWidget_More->addTab(qwidget_CustomSetting, "自定义请求");

    QVBoxLayout* vLayout_groupBox = new QVBoxLayout(groupBox_MoreSettings);
    vLayout_groupBox->setContentsMargins(8, 8, 8, 8);   // 边距
    vLayout_groupBox->addWidget(tabWidget_More);

    QVBoxLayout* vLayout_Page = new QVBoxLayout(page);
    vLayout_Page->setSpacing(12);
    vLayout_Page->setContentsMargins(18, 18, 18, 18);
    vLayout_Page->addWidget(groupBox_BaseSettings);
    vLayout_Page->addWidget(groupBox_MoreSettings);
    vLayout_Page->addStretch();               // 让下面内容始终沉底

    QScrollArea* scrollArea_Page = new QScrollArea;
    scrollArea_Page->setWidget(page);
    scrollArea_Page->setWidgetResizable(true);
    scrollArea_Page->setFrameShape(QFrame::NoFrame);
    scrollArea_Page->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget* widget_ConfigurationOperationPage = CreateConfigurationOperationPage();

    QWidget* widget_MainPage = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(widget_MainPage);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(scrollArea_Page);
    mainLayout->addWidget(widget_ConfigurationOperationPage);

    return widget_MainPage;
}

QGroupBox* Page_SetAI::CreateBaseSettingPage()
{
    // 基础设置GroupBox
    QGroupBox* groupBox_BaseSettings = new QGroupBox("基础设置");

    // ===== 基础设置 =====
    // AI服务器地址
    QLabel* label_url = new QLabel("AI服务地址:");
    QLineEdit* lineEdit_url = new QLineEdit();
    lineEdit_url->setObjectName(_PONL::SetAI_ObjNameList::lineEdit_url);
    lineEdit_url->setText(Config_SetAI::instance().Get_Url());
    QHBoxLayout* hLayout_url = new QHBoxLayout();
    hLayout_url->addWidget(label_url);
    hLayout_url->addWidget(lineEdit_url);

    // API 路径 - 聊天
    QLabel* label_urlApiPath = new QLabel("API 路径-聊天:");
    QLineEdit* lineEdit_urlApiPath = new QLineEdit();
	lineEdit_urlApiPath->setObjectName(_PONL::SetAI_ObjNameList::lineEdit_url_extrapath);
	lineEdit_urlApiPath->setText(Config_SetAI::instance().Get_Url_Extra_Path_Chat());
	QHBoxLayout* hLayout_urlApiPath = new QHBoxLayout();
	hLayout_urlApiPath->addWidget(label_urlApiPath);
	hLayout_urlApiPath->addWidget(lineEdit_urlApiPath);

    // API 路径 - 模型列表
    QLabel* label_urlApiPath_model = new QLabel("API 路径-模型:");
	QLineEdit* lineEdit_urlApiPath_model = new QLineEdit();
	lineEdit_urlApiPath_model->setObjectName(_PONL::SetAI_ObjNameList::lineEdit_url_extrapath_model);
	lineEdit_urlApiPath_model->setText(Config_SetAI::instance().Get_Url_Extra_Path_Models());
	QHBoxLayout* hLayout_urlApiPath_model = new QHBoxLayout();
	hLayout_urlApiPath_model->addWidget(label_urlApiPath_model);
	hLayout_urlApiPath_model->addWidget(lineEdit_urlApiPath_model);

    // API Key
    QLabel* label_apiKey = new QLabel("API Key:");
    QLineEdit* lineEdit_ApiKey = new QLineEdit();
	lineEdit_ApiKey->setObjectName(_PONL::SetAI_ObjNameList::lineEdit_api_key);
	lineEdit_ApiKey->setText(Config_SetAI::instance().Get_Api_Key());
    QHBoxLayout* hLayout_apiKey = new QHBoxLayout();
    hLayout_apiKey->addWidget(label_apiKey);
    hLayout_apiKey->addWidget(lineEdit_ApiKey);

    // 模型
    QHBoxLayout* hLayout_model = new QHBoxLayout();
    {
        QLabel* label_model = new QLabel("可用模型列表:");
        QLabel* label_comboBoxStatus = new QLabel("模型获取状态");
        label_comboBoxStatus->setObjectName(_PONL::SetAI_ObjNameList::label_comboBoxStatus);
        QComboBox* comboBox_model = new QComboBox();
        comboBox_model->setObjectName(_PONL::SetAI_ObjNameList::comboBox_ModelList);
        comboBox_model->setMinimumWidth(220);
        comboBox_model->setMaximumWidth(400);
        comboBox_model->setMinimumHeight(30);
        comboBox_model->setMaximumHeight(30);
        comboBox_model->addItems(Config_SetAI::instance().Get_ModelListFromConfigFile());
        comboBox_model->setCurrentIndex(0);
        //分隔竖线
        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        // 刷新获取按钮
        QPushButton* pushbutton_refresh = new QPushButton("刷新模型列表");
        pushbutton_refresh->setObjectName(_PONL::SetAI_ObjNameList::pushbutton_refresh);
        QPushButton* pushbutton_reload_ai = new QPushButton("重载AI配置");
        pushbutton_reload_ai->setObjectName(_PONL::SetAI_ObjNameList::pushbutton_reload_ai);

        hLayout_model->addWidget(label_model);
        hLayout_model->addWidget(comboBox_model);
        hLayout_model->addWidget(label_comboBoxStatus);
        hLayout_model->addWidget(line);
        hLayout_model->addWidget(pushbutton_refresh);
		hLayout_model->addWidget(pushbutton_reload_ai);
        hLayout_model->addStretch();
        // 更新模型ComboBox
        ServerAI::instance().UpdateSetAIPage_ModelsComBoboxInfo(comboBox_model, label_comboBoxStatus);
        // 连接信号
        connect(pushbutton_refresh, &QPushButton::clicked, this, [=]() { 
			qDebug() << "[debug] 刷新模型列表";
            ServerAI::instance().UpdateSetAIPage_ModelsComBoboxInfo(comboBox_model, label_comboBoxStatus); 
            });
        connect(pushbutton_reload_ai, &QPushButton::clicked, this, [=]() {
                qDebug() << "[debug] 重载AI配置";
                QLineEdit* lineEdit_ApiKey = findChild<QLineEdit*>(_PONL::SetAI_ObjNameList::lineEdit_api_key);
                QLineEdit* lineEdit_url = findChild<QLineEdit*>(_PONL::SetAI_ObjNameList::lineEdit_url);
                QLineEdit* lineEdit_url_models = findChild<QLineEdit*>(_PONL::SetAI_ObjNameList::lineEdit_url_extrapath_model);
                if (lineEdit_ApiKey && lineEdit_url && lineEdit_url_models)
                {
					std::string api_key = lineEdit_ApiKey->text().toStdString();
					std::string url = lineEdit_url->text().toStdString();
					std::string url_models = lineEdit_url_models->text().toStdString();
                    qDebug() << "[Debug] url: " << url + url_models << " api_key: " << api_key;
                    if (ServerAI::instance().ReloadAIConfig(url + url_models, api_key))
                    {
                        qDebug() << "[Debug] 重载AI配置成功";
                    }
                    else
                    {
						qDebug() << "[Debug] 重载AI配置失败";
                    }
                    ServerAI::instance().UpdateSetAIPage_ModelsComBoboxInfo(comboBox_model, label_comboBoxStatus);
                }
                
            });
    }
    

    // ==== AI人设 ====
    QCheckBox* checkBox_AIPrompt_Prompt = new QCheckBox("使用此参数 | Prompt:");
    QCheckBox* checkBox_AIPrompt_System = new QCheckBox("使用此参数 | System");
    // prompt
    auto create_page_prompt = [=]() -> QWidget* {
        QWidget* widget_prompt = new QWidget();
        checkBox_AIPrompt_Prompt->setObjectName(_PONL::SetAI_ObjNameList::checkBox_AIPrompt_prompt);
        checkBox_AIPrompt_Prompt->setChecked(Config_SetAI::instance().IsPromptMode());
        QLabel* label_AI_Prompt_tooltip = _SettingReusablePage::_CreateDoubtPage(
            QString("?"),
            QString("\nPrompt、System均可设置AI的人设\n部分模型可能不支持使用prompt，是否使用prompt请根据实际情况进行设置。"),
            Qt::AlignCenter);
        LAppStyleManager::SetLabelStyle(label_AI_Prompt_tooltip, 27);
        label_AI_Prompt_tooltip->setFixedHeight(15);
        label_AI_Prompt_tooltip->setFixedWidth(15);
        QHBoxLayout* hLayout_AI_Prompt_tooltip = new QHBoxLayout();
        hLayout_AI_Prompt_tooltip->addWidget(checkBox_AIPrompt_Prompt);
		hLayout_AI_Prompt_tooltip->addWidget(label_AI_Prompt_tooltip);
        hLayout_AI_Prompt_tooltip->addStretch();

        QPlainTextEdit* textEdit_prompt = new QPlainTextEdit();
        textEdit_prompt->setObjectName(_PONL::SetAI_ObjNameList::plainTextEdit_prompt);
        textEdit_prompt->setPlainText(Config_SetAI::instance().GetPrompt_Prompt());
        textEdit_prompt->setMaximumHeight(300);
        textEdit_prompt->setMinimumHeight(100);
        textEdit_prompt->setEnabled(Config_SetAI::instance().IsPromptMode());
        QVBoxLayout* vLayout_prompt = new QVBoxLayout(widget_prompt);
        vLayout_prompt->addLayout(hLayout_AI_Prompt_tooltip);
        vLayout_prompt->addWidget(textEdit_prompt);
        connect(checkBox_AIPrompt_Prompt, &QPushButton::clicked, this, [this, checkBox_AIPrompt_Prompt, checkBox_AIPrompt_System]() {
            if (checkBox_AIPrompt_Prompt->isChecked() == false && checkBox_AIPrompt_System->isChecked() == false)
            {
                checkBox_AIPrompt_Prompt->setCheckState(Qt::Checked);
                ModelWindowPromptWindow::ShowMessageOnModelWindow(11, QString("请至少选择一个选项"), 3000, true);
                return;
            }
            OnCheckBoxChanged(checkBox_AIPrompt_Prompt, 2);
            });
        return widget_prompt;
        };

    // system
    auto create_page_system = [=]() -> QWidget* {
        QWidget* page_system = new QWidget();
        checkBox_AIPrompt_System->setObjectName(_PONL::SetAI_ObjNameList::checkBox_AIPrompt_system);
        checkBox_AIPrompt_System->setChecked(Config_SetAI::instance().IsSystemMode());
        QLabel* label_AI_System_tooltip = _SettingReusablePage::_CreateDoubtPage(
            QString("?"),
            QString("\nPrompt、System均可设置AI的人设\n部分模型可能不支持使用prompt，是否使用prompt请根据实际情况进行设置。"),
            Qt::AlignCenter);
        LAppStyleManager::SetLabelStyle(label_AI_System_tooltip, 27);
        label_AI_System_tooltip->setFixedHeight(15);
        label_AI_System_tooltip->setFixedWidth(15);
        QHBoxLayout* layout_AI_System_tooltip = new QHBoxLayout();
		layout_AI_System_tooltip->addWidget(checkBox_AIPrompt_System);
		layout_AI_System_tooltip->addWidget(label_AI_System_tooltip);
        layout_AI_System_tooltip->addStretch();

        QPlainTextEdit* textEdit_system = new QPlainTextEdit();
        textEdit_system->setObjectName(_PONL::SetAI_ObjNameList::plainTextEdit_system);
        textEdit_system->setPlainText(Config_SetAI::instance().GetPrompt_System());
        textEdit_system->setMaximumHeight(300);
        textEdit_system->setMinimumHeight(100);
        textEdit_system->setEnabled(Config_SetAI::instance().IsSystemMode());
        QVBoxLayout* vLayout_system = new QVBoxLayout(page_system);
        vLayout_system->addLayout(layout_AI_System_tooltip);
        vLayout_system->addWidget(textEdit_system);

        connect(checkBox_AIPrompt_System, &QPushButton::clicked, this, [this, checkBox_AIPrompt_System, checkBox_AIPrompt_Prompt]() {
            if (checkBox_AIPrompt_System->isChecked() == false && checkBox_AIPrompt_Prompt->isChecked() == false)
            {
                checkBox_AIPrompt_System->setCheckState(Qt::Checked);
                ModelWindowPromptWindow::ShowMessageOnModelWindow(11, QString("请至少选择一个选项"), 3000, true);
                return;
            }
            OnCheckBoxChanged(checkBox_AIPrompt_System, 3);
            });
        return page_system;
        };

    // 布局
    QGroupBox* groupBox_prompt = new QGroupBox("AI人设");
    QVBoxLayout* vLayout_CharacterSetting = new QVBoxLayout(groupBox_prompt);
    QTabWidget* tabWidget_prompt = new QTabWidget();
	QWidget* page_prompt = create_page_prompt();
	QWidget* page_system = create_page_system();
	tabWidget_prompt->addTab(page_prompt, "Prompt");
	tabWidget_prompt->addTab(page_system, "System");
    vLayout_CharacterSetting->addWidget(tabWidget_prompt);

    // ====== 基础设置布局 =====
    QVBoxLayout* vLayout_BaseSettings = new QVBoxLayout(groupBox_BaseSettings);
    vLayout_BaseSettings->addLayout(hLayout_url);
	vLayout_BaseSettings->addLayout(hLayout_urlApiPath);
	vLayout_BaseSettings->addLayout(hLayout_urlApiPath_model);
    vLayout_BaseSettings->addLayout(hLayout_apiKey);
    vLayout_BaseSettings->addLayout(hLayout_model);
    vLayout_BaseSettings->addWidget(groupBox_prompt);

    return groupBox_BaseSettings;
}

QWidget* Page_SetAI::CreateMoreSettingPage()
{
    // 高级设置GroupBox
    QWidget* widget_MoreSettings = new QWidget();

    // ===== 高级设置 =====
    // ==== n ====
    QGroupBox* groupBox_n = new QGroupBox("n值设置");
    {
        bool bNStatus = Config_SetAI::instance().GetUseNStatus();
        QCheckBox* checkBox_n = new QCheckBox("使用自定义的n值:");
        checkBox_n->setObjectName(_PONL::SetAI_ObjNameList::checkBox_n);
        checkBox_n->setChecked(bNStatus);
        QCheckBox* checkBox_append_n = new QCheckBox("追加此字段于请求");
        checkBox_append_n->setObjectName(_PONL::SetAI_ObjNameList::checkBox_append_n);
        checkBox_append_n->setChecked(Config_SetAI::instance().GetUseAppendN());
        DDeskPet_IntegerSlider* dintslider_n = new DDeskPet_IntegerSlider();
        dintslider_n->setObjectName(_PONL::SetAI_ObjNameList::intSlider_n);
        dintslider_n->setEnabled(bNStatus);
        dintslider_n->setRange(1, 100);
        dintslider_n->setValue(Config_SetAI::instance().Get_N());

        QVBoxLayout* vlayout_choice = new QVBoxLayout();
        vlayout_choice->addWidget(checkBox_n);
        vlayout_choice->addWidget(checkBox_append_n);

        // 初始化 N
        QLabel* label_n_tooltip = _SettingReusablePage::_CreateDoubtPage(// 提示
            QString("?"),
            QString("\nn的含义是一次返回多少个回答\n \
                设为3会返回3条choices，会按3倍token计费，所以不建议值过大\n \
                勾选即使用自定义输入值，而不是建议值"),
            Qt::AlignCenter);
        LAppStyleManager::SetLabelStyle(label_n_tooltip, 27);
        label_n_tooltip->setFixedHeight(15);
        label_n_tooltip->setFixedWidth(15);
        QHBoxLayout* hLayout_n = new QHBoxLayout(groupBox_n);  // 布局
        hLayout_n->addLayout(vlayout_choice);
        hLayout_n->addWidget(dintslider_n);
        hLayout_n->addWidget(label_n_tooltip);

        // 连接信号与槽函数
        connect(checkBox_n, &QCheckBox::clicked, this, [=]() {
            dintslider_n->setEnabled(checkBox_n->isChecked());
            });
    }

    // ==== MaxTokens ====
    QGroupBox* groupBox_maxTokens = new QGroupBox("MaxTokens设置");
    {
        bool bMaxTokensStatus = Config_SetAI::instance().GetUseMaxTokensStatus();
        QCheckBox* checkBox_maxTokens = new QCheckBox("MaxTokens:");
        checkBox_maxTokens->setObjectName(_PONL::SetAI_ObjNameList::checkBox_maxtokens);
        checkBox_maxTokens->setChecked(bMaxTokensStatus);
        QCheckBox* checkBox_append_maxTokens = new QCheckBox("追加此字段于请求");
        checkBox_append_maxTokens->setObjectName(_PONL::SetAI_ObjNameList::checkBox_append_maxtokens);
        checkBox_append_maxTokens->setChecked(Config_SetAI::instance().GetUseAppendMaxTokens());
        QVBoxLayout* vLayout_maxTokens_choice = new QVBoxLayout();
        vLayout_maxTokens_choice->addWidget(checkBox_maxTokens);
        vLayout_maxTokens_choice->addWidget(checkBox_append_maxTokens);
        // 滑块
        DDeskPet_IntegerSlider* dintslider_maxTokens = new DDeskPet_IntegerSlider();
        dintslider_maxTokens->setObjectName(_PONL::SetAI_ObjNameList::intSlider_max_tokens);
        dintslider_maxTokens->setRange(1, 1000000);
        dintslider_maxTokens->setValue(Config_SetAI::instance().Get_Max_Tokens());
        // 提示
        QLabel* label_maxTokens_tooltip = _SettingReusablePage::_CreateDoubtPage(
            QString("?"),
            QString("MaxTokens是回答最多用多少token\n \
 值越大,往往回复就会越详细，但是消耗的金额往往也更多，所以不建议值过大\n \
 勾选即使用自定义输入值，而不是建议值"),
            Qt::AlignCenter);
        LAppStyleManager::SetLabelStyle(label_maxTokens_tooltip, 27);
        label_maxTokens_tooltip->setFixedHeight(15);
        label_maxTokens_tooltip->setFixedWidth(15);
        QHBoxLayout* hLayout_maxTokens = new QHBoxLayout(groupBox_maxTokens);    // 布局
        hLayout_maxTokens->addLayout(vLayout_maxTokens_choice);
        hLayout_maxTokens->addWidget(dintslider_maxTokens);
        hLayout_maxTokens->addWidget(label_maxTokens_tooltip);

        // 设置MaxTokens的值
        connect(checkBox_maxTokens, &QCheckBox::clicked, this, [this, checkBox_maxTokens, dintslider_maxTokens]() {
            dintslider_maxTokens->setEnabled(checkBox_maxTokens->isChecked());
            });
    }

    // ==== Randomness - Temperature ====
    QGroupBox* groupBox_Randomness_Temperature = new QGroupBox("Temperature设置");
    {
        bool bTemperatureStatus = Config_SetAI::instance().IsTemperature();
        QCheckBox* checkBox_Temperature = new QCheckBox("使用Temperature");
        checkBox_Temperature->setObjectName(_PONL::SetAI_ObjNameList::checkBox_temperature);
        checkBox_Temperature->setChecked(bTemperatureStatus);
        QCheckBox* checkBox_append_temperature = new QCheckBox("追加此字段于请求");
        checkBox_append_temperature->setObjectName(_PONL::SetAI_ObjNameList::checkBox_append_temperature);
        checkBox_append_temperature->setChecked(Config_SetAI::instance().GetUseAppendTemperature());
        // 布局
        QVBoxLayout* vlayout_Randomness_Temperature = new QVBoxLayout();
        vlayout_Randomness_Temperature->addWidget(checkBox_Temperature);
        vlayout_Randomness_Temperature->addWidget(checkBox_append_temperature);
        QDoubleSpinBox* dspinBox_Temperature = new QDoubleSpinBox();
        dspinBox_Temperature->setObjectName(_PONL::SetAI_ObjNameList::dspinBox_Temperature);
        dspinBox_Temperature->setRange(0.0, 2.0);
        dspinBox_Temperature->setSingleStep(0.1);
        dspinBox_Temperature->setDecimals(1);
        dspinBox_Temperature->setValue(Config_SetAI::instance().Get_Randomness_Temperature());
        dspinBox_Temperature->setEnabled(bTemperatureStatus);

        // 提示
        QLabel* label_Randomness_tooltip = _SettingReusablePage::_CreateDoubtPage(
            QString("?"),
            QString("\n调节模型在选择下一个 token 时的概率分布平滑程度"),
            Qt::AlignCenter);
        LAppStyleManager::SetLabelStyle(label_Randomness_tooltip, 27);
        label_Randomness_tooltip->setFixedHeight(15);
        label_Randomness_tooltip->setFixedWidth(15);
        QHBoxLayout* hLayout_Temperature = new QHBoxLayout(groupBox_Randomness_Temperature);    // 布局
        hLayout_Temperature->addLayout(vlayout_Randomness_Temperature);
        hLayout_Temperature->addWidget(dspinBox_Temperature);
        hLayout_Temperature->addWidget(label_Randomness_tooltip);
        // 设置Temperature的值
        connect(checkBox_Temperature, &QCheckBox::clicked, this, [this, checkBox_Temperature]() {
            OnCheckBoxChanged(checkBox_Temperature, 2);
            });
    }
    // ==== Randomness - Top_P ====
    QGroupBox* groupBox_Randomness_TopP = new QGroupBox("TopP设置");
    {
        bool bTopPStatus = Config_SetAI::instance().IsTop_p();
        QCheckBox* checkBox_TopP = new QCheckBox("使用TopP");
        checkBox_TopP->setObjectName(_PONL::SetAI_ObjNameList::checkBox_top_p);
        checkBox_TopP->setChecked(bTopPStatus);
        QCheckBox* checkBox_append_top_p = new QCheckBox("追加此字段于请求");
        checkBox_append_top_p->setObjectName(_PONL::SetAI_ObjNameList::checkBox_append_top_p);
        checkBox_append_top_p->setChecked(Config_SetAI::instance().GetUseAppendTop_p());
        QVBoxLayout* vlayout_Randomness = new QVBoxLayout();
        vlayout_Randomness->addWidget(checkBox_TopP);
        vlayout_Randomness->addWidget(checkBox_append_top_p);
        QDoubleSpinBox* dspinBox_TopP = new QDoubleSpinBox();
        dspinBox_TopP->setObjectName(_PONL::SetAI_ObjNameList::dspinBox_top_p);
        dspinBox_TopP->setRange(0.0, 1.0);
        dspinBox_TopP->setSingleStep(0.1);
        dspinBox_TopP->setDecimals(1);
        dspinBox_TopP->setValue(Config_SetAI::instance().Get_Randomness_Top_p());
        dspinBox_TopP->setEnabled(bTopPStatus);

        // 提示
        QLabel* label_Randomness_tooltip = _SettingReusablePage::_CreateDoubtPage(
            QString("?"),
            QString("\n通过动态截断候选 token 集合来控制随机性"),
            Qt::AlignCenter);
        LAppStyleManager::SetLabelStyle(label_Randomness_tooltip, 27);
        label_Randomness_tooltip->setFixedHeight(15);
        label_Randomness_tooltip->setFixedWidth(15);
        QHBoxLayout* hLayout_TopP = new QHBoxLayout(groupBox_Randomness_TopP);    // 布局
        hLayout_TopP->addLayout(vlayout_Randomness);
        hLayout_TopP->addWidget(dspinBox_TopP);
        hLayout_TopP->addWidget(label_Randomness_tooltip);

        // 设置TopP的值
        connect(checkBox_TopP, &QCheckBox::clicked, this, [this, checkBox_TopP]() {
            OnCheckBoxChanged(checkBox_TopP, 3);
            });
    }

    // ==== frequency_penalty & presence_penalty ====
    // frequency_penalty
    QGroupBox* groupBox_frequency_penalty = new QGroupBox("frequency_penalty设置");
    {
        bool bFrequencyPenaltyStatus = Config_SetAI::instance().Use_Frequency_Penalty();
        QCheckBox* checkBox_frequency_penalty = new QCheckBox("使用frequency_penalty:");
        checkBox_frequency_penalty->setObjectName(_PONL::SetAI_ObjNameList::checkBox_frequency_penalty);
        checkBox_frequency_penalty->setChecked(bFrequencyPenaltyStatus);
        QCheckBox* checkBox_append_frequency_penalty = new QCheckBox("追加此字段于请求");
		checkBox_append_frequency_penalty->setObjectName(_PONL::SetAI_ObjNameList::checkBox_append_frequency_penalty);
		checkBox_append_frequency_penalty->setChecked(Config_SetAI::instance().Use_Append_Frequency_Penalty());
        QVBoxLayout* vlayout_frequency_penalty = new QVBoxLayout();
		vlayout_frequency_penalty->addWidget(checkBox_frequency_penalty);
		vlayout_frequency_penalty->addWidget(checkBox_append_frequency_penalty);

        QDoubleSpinBox* dspinBox_frequency_penalty = new QDoubleSpinBox();
        dspinBox_frequency_penalty->setObjectName(_PONL::SetAI_ObjNameList::dspinBox_frequency_penalty);
        dspinBox_frequency_penalty->setRange(-2.0, 2.0);
        dspinBox_frequency_penalty->setSingleStep(0.1);
        dspinBox_frequency_penalty->setDecimals(1);
        dspinBox_frequency_penalty->setValue((double)Config_SetAI::instance().Get_Frequency_Penalty()); 

        QLabel* label_frequency_penalty_tooltip = _SettingReusablePage::_CreateDoubtPage(
			QString("?"),
			QString("\nfrequency_penalty: 抑制重复词, 正值让模型更少用已经出现过的词"),
			Qt::AlignCenter);
		LAppStyleManager::SetLabelStyle(label_frequency_penalty_tooltip, 27);
		label_frequency_penalty_tooltip->setFixedHeight(15);
		label_frequency_penalty_tooltip->setFixedWidth(15);

        QHBoxLayout* hLayout_frequency_penalty = new QHBoxLayout(groupBox_frequency_penalty);
		hLayout_frequency_penalty->addLayout(vlayout_frequency_penalty);
		hLayout_frequency_penalty->addWidget(dspinBox_frequency_penalty);
		hLayout_frequency_penalty->addWidget(label_frequency_penalty_tooltip);
    }
    // presence_penalty
    QGroupBox* groupBox_presence_penalty = new QGroupBox("presence_penalty设置");
    {
        bool bPresencePenaltyStatus = Config_SetAI::instance().Use_Presence_Penalty();
        QCheckBox* checkBox_presence_penalty = new QCheckBox("使用presence_penalty:");
        checkBox_presence_penalty->setObjectName(_PONL::SetAI_ObjNameList::checkBox_presence_penalty);
        checkBox_presence_penalty->setChecked(bPresencePenaltyStatus);
        QCheckBox* checkBox_append_presence_penalty = new QCheckBox("追加此字段于请求");
		checkBox_append_presence_penalty->setObjectName(_PONL::SetAI_ObjNameList::checkBox_append_presence_penalty);
		checkBox_append_presence_penalty->setChecked(Config_SetAI::instance().Use_Append_Presence_Penalty());
        QVBoxLayout* vlayout_presence_penalty = new QVBoxLayout();
		vlayout_presence_penalty->addWidget(checkBox_presence_penalty);
		vlayout_presence_penalty->addWidget(checkBox_append_presence_penalty);

        QDoubleSpinBox* dspinBox_presence_penalty = new QDoubleSpinBox();
        dspinBox_presence_penalty->setObjectName(_PONL::SetAI_ObjNameList::dspinBox_presence_penalty);
        dspinBox_presence_penalty->setRange(-2.0, 2.0);
        dspinBox_presence_penalty->setSingleStep(0.1);
        dspinBox_presence_penalty->setDecimals(1);
        dspinBox_presence_penalty->setValue((double)Config_SetAI::instance().Get_Presence_Penalty());
        // 提示
        QLabel* label_presence_penalty_tooltip = _SettingReusablePage::_CreateDoubtPage(
            QString("?"),
            QString("\npresence_penalty: 抑制重复词, 正值让模型更少用已经出现过的词"),
            Qt::AlignCenter);
        LAppStyleManager::SetLabelStyle(label_presence_penalty_tooltip, 27);    // 设置label样式
        label_presence_penalty_tooltip->setFixedHeight(15);
        label_presence_penalty_tooltip->setFixedWidth(15);

		// 布局
		QHBoxLayout* hLayout_presence_penalty = new QHBoxLayout(groupBox_presence_penalty);
		hLayout_presence_penalty->addLayout(vlayout_presence_penalty);
		hLayout_presence_penalty->addWidget(dspinBox_presence_penalty);
		hLayout_presence_penalty->addWidget(label_presence_penalty_tooltip);
    }
    
    // ==== Stop ====
    QGroupBox* groupBox_Stop = new QGroupBox("Stop");
    // table
    QTableWidget* tableWidget_Stop = new QTableWidget();
    tableWidget_Stop->setObjectName(_PONL::SetAI_ObjNameList::qtableWidget_Stop);
    tableWidget_Stop->setColumnCount(1);
    tableWidget_Stop->setHorizontalHeaderLabels(QStringList() << "内容");
    tableWidget_Stop->horizontalHeader()->setStretchLastSection(true);
    tableWidget_Stop->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget_Stop->insertRow(0);
    tableWidget_Stop->setStyleSheet(
        "QTableWidget { "
        "   background-color: #f5f5f5; }"
        "QHeaderView::section { "
        "   background - color: #e0e0e0; }"
        "QTableWidgetItem { "
        "   padding: 5px; }");
    QSet<QString> qset_Stop = Config_SetAI::instance().Get_Stop();
    tableWidget_Stop->setRowCount(0);                  // 清空旧数据
    for (const QString& s : qset_Stop)
    {
        int row = tableWidget_Stop->rowCount();
        tableWidget_Stop->insertRow(row); // 新增一行
        QTableWidgetItem* item = new QTableWidgetItem(s);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        tableWidget_Stop->setItem(row, 0, item);
    }

    QPushButton* pushButton_Stop_Add = new QPushButton("末尾添加行");
    QPushButton* pushButton_Stop_Del = new QPushButton("删除选中行");
    QPushButton* pushButton_Stop_Clear = new QPushButton("清除所有项");
    QPushButton* pushButton_Stop_ClearNullString = new QPushButton("清除空行");
    QCheckBox* checkBox_append_stop = new QCheckBox("追加此字段于请求:");
	checkBox_append_stop->setObjectName(_PONL::SetAI_ObjNameList::checkBox_append_stop);
    checkBox_append_stop->setChecked(Config_SetAI::instance().Use_Append_Stop());
    QLabel* label_stop_tooltip = _SettingReusablePage::_CreateDoubtPage(
        QString("?"),
        QString("\nStop:  提前终止生成的字符串或数组\n \
一旦生成了Stop表中的内容，立即结束。"),
Qt::AlignCenter);
	LAppStyleManager::SetLabelStyle(label_stop_tooltip, 27);
    label_stop_tooltip->setFixedHeight(15);
    label_stop_tooltip->setFixedWidth(15);
    QVBoxLayout* vLayout_Stop_func = new QVBoxLayout();
	vLayout_Stop_func->addWidget(pushButton_Stop_Add);
	vLayout_Stop_func->addWidget(pushButton_Stop_Del);
    vLayout_Stop_func->addWidget(pushButton_Stop_Clear);
    vLayout_Stop_func->addWidget(pushButton_Stop_ClearNullString);
    vLayout_Stop_func->addWidget(checkBox_append_stop);
    vLayout_Stop_func->addWidget(label_stop_tooltip);
	// 布局
	QHBoxLayout* vLayout_Stop = new QHBoxLayout(groupBox_Stop);
	vLayout_Stop->addWidget(tableWidget_Stop);
	vLayout_Stop->addLayout(vLayout_Stop_func);
	vLayout_Stop->setStretch(0, 4);
	vLayout_Stop->setStretch(1, 1);

    // ===== 高级设置布局 =====
    // 设置比例
    // 第一层
    QHBoxLayout* hLayout_MoreSettings_layer_1 = new QHBoxLayout();
    hLayout_MoreSettings_layer_1->addWidget(groupBox_n);
    // 第二层
	QHBoxLayout* hLayout_MoreSettings_layer_2 = new QHBoxLayout();
    hLayout_MoreSettings_layer_2->addWidget(groupBox_maxTokens);
    // 第三层
    QHBoxLayout* hLayout_MoreSettings_layer_3 = new QHBoxLayout();
	hLayout_MoreSettings_layer_3->addWidget(groupBox_Randomness_Temperature);
    hLayout_MoreSettings_layer_3->addWidget(groupBox_Randomness_TopP);
    hLayout_MoreSettings_layer_3->setStretch(0, 1);
	hLayout_MoreSettings_layer_3->setStretch(1, 1);
    // 第四层
	QHBoxLayout* hLayout_MoreSettings_layer_4 = new QHBoxLayout();
	hLayout_MoreSettings_layer_4->addWidget(groupBox_presence_penalty);
	hLayout_MoreSettings_layer_4->addWidget(groupBox_frequency_penalty);
	hLayout_MoreSettings_layer_4->setStretch(0, 1);
	hLayout_MoreSettings_layer_4->setStretch(1, 1);
    // 第五层
	QHBoxLayout* hLayout_MoreSettings_layer_5 = new QHBoxLayout();
	hLayout_MoreSettings_layer_5->addWidget(groupBox_Stop);
	hLayout_MoreSettings_layer_5->setStretch(0, 1);

	QVBoxLayout* vLayout_MoreSettings = new QVBoxLayout(widget_MoreSettings);
	vLayout_MoreSettings->addLayout(hLayout_MoreSettings_layer_1);
	vLayout_MoreSettings->addLayout(hLayout_MoreSettings_layer_2);
	vLayout_MoreSettings->addLayout(hLayout_MoreSettings_layer_3);
	vLayout_MoreSettings->addLayout(hLayout_MoreSettings_layer_4);
	vLayout_MoreSettings->addLayout(hLayout_MoreSettings_layer_5);

	// ====== 连接信号槽 ======
    // Stop
    connect(pushButton_Stop_Add, &QPushButton::clicked, this, [this, pushButton_Stop_Add]() {
			OnPushButtonClicked_StopTable(pushButton_Stop_Add, 1);
        });
	connect(pushButton_Stop_Del, &QPushButton::clicked, this, [this, pushButton_Stop_Del]() {
			OnPushButtonClicked_StopTable(pushButton_Stop_Del, 2);
		});
    connect(pushButton_Stop_Clear, &QPushButton::clicked, this, [this, pushButton_Stop_Clear]() {
            OnPushButtonClicked_StopTable(pushButton_Stop_Clear, 3);
        });
    connect(pushButton_Stop_ClearNullString, &QPushButton::clicked, this, [this, pushButton_Stop_ClearNullString]() {
            OnPushButtonClicked_StopTable(pushButton_Stop_ClearNullString, 4);
        });

    return widget_MoreSettings;
}

QWidget* Page_SetAI::CreateCustomSettingPage()
{
    QWidget* page = new QWidget(this);
    QCheckBox* qcheckbox_CusJson = new QCheckBox("自定义json");
    qcheckbox_CusJson->setObjectName(_PONL::SetAI_ObjNameList::checkBox_useCustomJson);
    qcheckbox_CusJson->setChecked(Config_SetAI::instance().GetUseCustomJson());
    QLabel* qLabel_CusJson = new QLabel("(你可以在此自定义json格式,请勿填写message数组！)");
    QHBoxLayout* hLayout_CusJson = new QHBoxLayout();
    hLayout_CusJson->addWidget(qcheckbox_CusJson);
	hLayout_CusJson->addWidget(qLabel_CusJson);
    hLayout_CusJson->addStretch();

    QPlainTextEdit* qplaintextEdit_CusJson = new QPlainTextEdit();
	qplaintextEdit_CusJson->setObjectName(_PONL::SetAI_ObjNameList::plainTextEdit_customJson);
    qplaintextEdit_CusJson->setPlainText(QString::fromStdString(Config_SetAI::instance().GetCustomJson().dump(4)));

    QVBoxLayout* vLayout_CusJson = new QVBoxLayout(page);
	vLayout_CusJson->addLayout(hLayout_CusJson);
	vLayout_CusJson->addWidget(qplaintextEdit_CusJson);
    return page;
}

QWidget* Page_SetAI::CreateCustomFieldPage()
{
    QWidget* page = new QWidget(this);
    QCheckBox* checkbox_AddCustomField = new QCheckBox("添加自定义字段");
    checkbox_AddCustomField->setObjectName(_PONL::SetAI_ObjNameList::checkBox_AddCustomField);
	checkbox_AddCustomField->setChecked(Config_SetAI::instance().GetUseAppendCustomJson());
    QLabel* label_add_custom_field = _SettingReusablePage::_CreateDoubtPage(
        QString("?"),
        QString("\n在此编辑框中，你可以添加一些自定义的字段。\n格式为标准json格式，请勿填写message数组！\n这些自定义的字段会被追加到请求的json中"),
        Qt::AlignCenter);
    label_add_custom_field->setFixedHeight(15);
	label_add_custom_field->setFixedWidth(15);
    LAppStyleManager::SetLabelStyle(label_add_custom_field, 27);
    
    QHBoxLayout* hLayout_add_custom_field = new QHBoxLayout();
	hLayout_add_custom_field->addWidget(checkbox_AddCustomField);
	hLayout_add_custom_field->addWidget(label_add_custom_field);
	hLayout_add_custom_field->addStretch();

    QPlainTextEdit* plainTextEdit_add_custom_field = new QPlainTextEdit();
	plainTextEdit_add_custom_field->setObjectName(_PONL::SetAI_ObjNameList::plainTextEdit_addCustomField);
    plainTextEdit_add_custom_field->setMinimumHeight(200);
    plainTextEdit_add_custom_field->setPlainText(QString::fromStdString(Config_SetAI::instance().GetAppendCustomJson().dump(4)));

	QVBoxLayout* vLayout_add_custom_field = new QVBoxLayout(page);
	vLayout_add_custom_field->addLayout(hLayout_add_custom_field);
	vLayout_add_custom_field->addWidget(plainTextEdit_add_custom_field);
    return page;
}

QWidget* Page_SetAI::CreateConfigurationOperationPage()
{
    QWidget* page = new QWidget();
    // 保存配置
    QPushButton* pushButton_SaveConfig = new QPushButton();
    pushButton_SaveConfig->setText(QString("保存配置"));
    pushButton_SaveConfig->setObjectName(_PONL::SetAI_ObjNameList::pushButton_SaveConfig);
    // 恢复默认配置
    QPushButton* pushButton_RestoreDefaultConfig = new QPushButton();
    pushButton_RestoreDefaultConfig->setText(QString("恢复默认配置"));
    pushButton_RestoreDefaultConfig->setObjectName(_PONL::SetAI_ObjNameList::pushButton_RestoreDefaultConfig);
    // 保存配置到服务器
    QPushButton* pushButton_SaveConfigToServer = new QPushButton();
    pushButton_SaveConfigToServer->setText(QString("保存配置到服务器"));
    pushButton_SaveConfigToServer->setObjectName(_PONL::SetAI_ObjNameList::pushButton_SaveConfigToServer);
    // 布局
    QHBoxLayout* mainLayout = new QHBoxLayout(page);
    mainLayout->addWidget(pushButton_SaveConfig);
    mainLayout->addWidget(pushButton_RestoreDefaultConfig);
    mainLayout->addWidget(pushButton_SaveConfigToServer);

    // 保存配置信号
    connect(pushButton_SaveConfig, &QPushButton::clicked, this, &Page_SetAI::OnSaveBtnClicked);
    // 恢复默认信号
    connect(pushButton_RestoreDefaultConfig, &QPushButton::clicked, this, &Page_SetAI::OnRestoreDefaultBtnClicked);
    return page;
}

void Page_SetAI::OnPushButtonClicked_StopTable(QPushButton* button, int nPushButtonID)
{
    QTableWidget* tableWidget_Stop = this->findChild<QTableWidget*>(_PONL::SetAI_ObjNameList::qtableWidget_Stop);
    if(tableWidget_Stop == nullptr)
	{
        RuntimeLog::instance().WriteLogInfo(QString("Page_SetAI::OnPushButtonClicked_StopTable"), 
            QString("tableWidget_Stop is nullptr"));
		return;
	}
    switch (nPushButtonID)
    {
        // 添加
        case 1:
        {
            int rowCount = tableWidget_Stop->rowCount();
            for (int row = 0; row < rowCount; ++row)
            {
                QTableWidgetItem* item = tableWidget_Stop->item(row, 0); // 获取第一列的项目
                RuntimeLog::instance().WriteLogInfo(QString("Page_SetAI::OnPushButtonClicked_StopTable"),
                    QString("At case1: item is nullptr"));
                if (item != nullptr && item->text().isEmpty())
                {
                    QString qstr_Prompt = QString("还有空白行噢，请先把空白行填写了吧！\n") + QString("空白行：%1").arg(row + 1);
                    ModelWindowPromptWindow::ShowMessageOnModelWindow(11, qstr_Prompt, 3000, true);
                    return;
                }
            }
            rowCount = tableWidget_Stop->rowCount();
            tableWidget_Stop->insertRow(rowCount); // Add new row

            QTableWidgetItem* item = new QTableWidgetItem();
            tableWidget_Stop->setItem(rowCount, 0, item); // 添加到单元格
            item->setFlags(item->flags() | Qt::ItemIsEditable); // 允许用户编辑

            return;
        }
        // 删除
        case 2:
        {
            QList<QTableWidgetItem*> selectedItems = tableWidget_Stop->selectedItems();
            if (!selectedItems.isEmpty()) 
            {
                int row = selectedItems.first()->row();
                tableWidget_Stop->removeRow(row); // Remove selected row
            }
            // 始终保留一行
            if(tableWidget_Stop->rowCount() == 0)
			{
                tableWidget_Stop->insertRow(0);
			}
            return;
        }
        // 清空
        case 3:
        {
            tableWidget_Stop->clear(); // 清除所有项
            tableWidget_Stop->setRowCount(1); // 除去行
            tableWidget_Stop->setHorizontalHeaderLabels(QStringList() << "内容");
            return;
        }
        case 4:
        {
            int rowCount = tableWidget_Stop->rowCount();
            for (int row = rowCount - 1; row >= 0; --row)
            {
                QTableWidgetItem* item = tableWidget_Stop->item(row, 0); // 获取第一列的项目
                RuntimeLog::instance().WriteLogInfo(QString("Page_SetAI::OnPushButtonClicked_StopTable"),
                    QString("At case4: item is nullptr"));
                if (item != nullptr && item->text().isEmpty())
                {
                    tableWidget_Stop->removeRow(row); // 删除空行
                }
            }
            return;
        }
        default:
        {
            return;
        }
    }
}

void Page_SetAI::OnSaveBtnClicked()
{
    qDebug() << "Page_SetAI::OnSaveBtnClicked()";
    QLineEdit* lineEdit_Buffer = nullptr;
    QPlainTextEdit* plainTextEdit_Buffer = nullptr;
    QCheckBox* checkBox_Buffer = nullptr;
    QCheckBox* checkBox_Buffer_2 = nullptr;
    QRadioButton* radioButton_Buffer = nullptr;
    QSpinBox* spinBox_Buffer = nullptr;
    QDoubleSpinBox* doubleSpinBox_Buffer = nullptr;
    QTableWidget* tableWidget_Buffer = nullptr;
    QComboBox* comboBox_Buffer = nullptr;
    DDeskPet_IntegerSlider* integerSlider_Buffer = nullptr;

    // url
    {
        lineEdit_Buffer = this->findChild<QLineEdit*>(_PONL::SetAI_ObjNameList::lineEdit_url);
        if (lineEdit_Buffer)
        {
            Config_SetAI::instance().Set_Url(lineEdit_Buffer->text());
        }
        else
        {
			RuntimeLog_Debug("lineEdit_URL not found in json");
        }
    }
    
    // extra-path
    {
        lineEdit_Buffer = this->findChild<QLineEdit*>(_PONL::SetAI_ObjNameList::lineEdit_url_extrapath);
        if (lineEdit_Buffer)
        {
            Config_SetAI::instance().Set_URL_APIPath(lineEdit_Buffer->text());
        }
        else
        {
			RuntimeLog_Debug("lineEdit_Url_ApiPath not found in json");
        }
    }
	
    // extra-path-models
    {
        lineEdit_Buffer = this->findChild<QLineEdit*>(_PONL::SetAI_ObjNameList::lineEdit_url_extrapath_model);
        if (lineEdit_Buffer)
        {
            Config_SetAI::instance().Set_URL_ModelsPath(lineEdit_Buffer->text());
        }
        else
        {
			RuntimeLog_Debug("lineEdit_Url_ApiPath_Models not found in json");
        }
    }
    
    // api-key
    {
        lineEdit_Buffer = this->findChild<QLineEdit*>(_PONL::SetAI_ObjNameList::lineEdit_api_key);
        if (lineEdit_Buffer)
        {
            Config_SetAI::instance().Set_Api_Key(lineEdit_Buffer->text());
        }
        else
        {
            RuntimeLog_Debug("lineEdit_API-Key not found in json");
        }
    }

    // n
    {
        checkBox_Buffer = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_n);
        checkBox_Buffer_2 = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_append_n);
        integerSlider_Buffer = this->findChild<DDeskPet_IntegerSlider*>(_PONL::SetAI_ObjNameList::intSlider_n);
        int n = integerSlider_Buffer->getValue();
        if (checkBox_Buffer && checkBox_Buffer_2)
        {
            bool bNStatus = checkBox_Buffer->isChecked();
            bool bAppendN = checkBox_Buffer_2->isChecked();
            // 选中时使用LineEdit 否则使用SpinBox的数据
            if (checkBox_Buffer->isChecked())
            {
                Config_SetAI::instance().Set_N(n, true, bAppendN);
            }
            else
            {
                Config_SetAI::instance().Set_N(1, false, bAppendN);
            }
        }
        else
        {
            Config_SetAI::instance().Set_N(1, false);
			RuntimeLog_Debug("checkBox_N not found in json");
        }
    }

    // max tokens
    {
        checkBox_Buffer = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_maxtokens);
        checkBox_Buffer_2 = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_append_maxtokens);
        integerSlider_Buffer = this->findChild<DDeskPet_IntegerSlider*>(_PONL::SetAI_ObjNameList::intSlider_max_tokens);
        int n = integerSlider_Buffer->getValue();
        if (checkBox_Buffer)
        {
            // 如果选中就不使用默认值
            if (checkBox_Buffer->isChecked())
            {
                Config_SetAI::instance().Set_Max_Tokens(n, true, checkBox_Buffer_2->isChecked());
            }
            else
            {
				Config_SetAI::instance().Set_Max_Tokens(1000, false, checkBox_Buffer_2->isChecked());
            }
        }
        else
        {
            Config_SetAI::instance().Set_Max_Tokens(1000, false, false);
			RuntimeLog_Debug("checkBox_Max_Tokens not found in json");
        }
    }
    

    // temperature
    {
        doubleSpinBox_Buffer = this->findChild<QDoubleSpinBox*>(_PONL::SetAI_ObjNameList::dspinBox_Temperature);
        checkBox_Buffer = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_temperature);
        checkBox_Buffer_2 = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_append_temperature);
        if (doubleSpinBox_Buffer && checkBox_Buffer && checkBox_Buffer_2)
        {
            if(checkBox_Buffer->isChecked())
            {
                Config_SetAI::instance().Set_Randomness_Temperature(doubleSpinBox_Buffer->value(), true, checkBox_Buffer_2->isChecked());
            }
            else
            {
                Config_SetAI::instance().Set_Randomness_Temperature(0.0f, false, checkBox_Buffer_2->isChecked());
			}
        }
        else
        {
            RuntimeLog_Debug("doubleSpinBox_Temperature not found in json");
            Config_SetAI::instance().Set_Randomness_Temperature(0.0f, false, false);
        }
    }

    // top_p
    {
        doubleSpinBox_Buffer = this->findChild<QDoubleSpinBox*>(_PONL::SetAI_ObjNameList::dspinBox_top_p);
        checkBox_Buffer = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_top_p);
		checkBox_Buffer_2 = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_append_top_p);
        if (doubleSpinBox_Buffer && checkBox_Buffer && checkBox_Buffer_2)
        {
            if(checkBox_Buffer->isChecked())
            {
                Config_SetAI::instance().Set_Randomness_Top_p(doubleSpinBox_Buffer->value(), true, checkBox_Buffer_2->isChecked());
            }
            else
            {
                Config_SetAI::instance().Set_Randomness_Top_p(0.0f, false, checkBox_Buffer_2->isChecked());
            }
        }
        else
        {
			RuntimeLog_Debug("doubleSpinBox_Top_P not found in json");
            Config_SetAI::instance().Set_Randomness_Top_p(0.0f, false, false);
        }
    }

    // Prompt - Prompt
    {
        plainTextEdit_Buffer = this->findChild<QPlainTextEdit*>(_PONL::SetAI_ObjNameList::plainTextEdit_prompt);
        if (plainTextEdit_Buffer)
        {
            Config_SetAI::instance().Set_Prompt_Prompt(plainTextEdit_Buffer->toPlainText());
        }
        else
        {
			RuntimeLog_Debug("plainTextEdit_Prompt not found in json");
        }
    }
    
    // Prompt - System
    {
        plainTextEdit_Buffer = this->findChild<QPlainTextEdit*>(_PONL::SetAI_ObjNameList::plainTextEdit_system);
        if (plainTextEdit_Buffer)
        {
            Config_SetAI::instance().Set_Prompt_System(plainTextEdit_Buffer->toPlainText());
        }
        else
        {
			RuntimeLog_Debug("plainTextEdit_System not found in json");
        }
    }

    // frequency_penalty
    {
        checkBox_Buffer = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_frequency_penalty);
		checkBox_Buffer_2 = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_append_frequency_penalty);
        doubleSpinBox_Buffer = this->findChild<QDoubleSpinBox*>(_PONL::SetAI_ObjNameList::dspinBox_frequency_penalty);
        if (checkBox_Buffer && checkBox_Buffer_2 && doubleSpinBox_Buffer)   // 选中时使用LineEdit 否则使用SpinBox的数据
        {
            bool bIsChecked = checkBox_Buffer->isChecked();
            if (doubleSpinBox_Buffer)
            {
                Config_SetAI::instance().Set_frequency_penalty(doubleSpinBox_Buffer->value(), bIsChecked, checkBox_Buffer_2->isChecked());
            }
            else
            {
                Config_SetAI::instance().Set_frequency_penalty(0.0f, bIsChecked, checkBox_Buffer_2->isChecked());
				RuntimeLog_Debug("doubleSpinBox_Frequency_Penalty not found in widget");
            }
        }
        else
        {
            Config_SetAI::instance().Set_frequency_penalty(0.0f, false, checkBox_Buffer_2->isChecked());
			RuntimeLog_Debug("checkBox_Frequency_Penalty not found in widget");
        }
    }

    // presence_penalty
    {
        checkBox_Buffer = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_presence_penalty);
        checkBox_Buffer_2 = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_append_presence_penalty);
        doubleSpinBox_Buffer = this->findChild<QDoubleSpinBox*>(_PONL::SetAI_ObjNameList::dspinBox_presence_penalty);
        if (checkBox_Buffer && checkBox_Buffer_2 && doubleSpinBox_Buffer)   // 选中时使用LineEdit 否则使用SpinBox的数据
        {
            bool bIsChecked = checkBox_Buffer->isChecked();
            if (doubleSpinBox_Buffer)
            {
                Config_SetAI::instance().Set_presence_penalty(doubleSpinBox_Buffer->value(), bIsChecked, checkBox_Buffer_2->isChecked());
            }
            else
            {
                Config_SetAI::instance().Set_presence_penalty(0.0f, bIsChecked, checkBox_Buffer_2->isChecked());
				RuntimeLog_Debug("doubleSpinBox_Presence_Penalty not found in widget");
            }
        }
        else
        {
			RuntimeLog_Debug("checkBox_Presence_Penalty not found in widget");
            Config_SetAI::instance().Set_presence_penalty(0.0f, false, checkBox_Buffer_2->isChecked());
        }
    }

    // Model
    {
        comboBox_Buffer = this->findChild<QComboBox*>(_PONL::SetAI_ObjNameList::comboBox_ModelList);
        if (comboBox_Buffer)
        {
            Config_SetAI::instance().Set_Model(comboBox_Buffer->currentText());
        }
        else
        {
			RuntimeLog_Debug("comboBox_ModelList not found in widget");
        }
    }

    // Stop
    {
        tableWidget_Buffer = this->findChild<QTableWidget*>(_PONL::SetAI_ObjNameList::qtableWidget_Stop);
        checkBox_Buffer = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_append_stop);
        if (tableWidget_Buffer && checkBox_Buffer)
        {
            QSet<QString> vals;
            for (int r = 0; r < tableWidget_Buffer->rowCount(); ++r)
            {
                QTableWidgetItem* it = tableWidget_Buffer->item(r, 0);
                if (it)
                    vals.insert(it->text().trimmed());
            }
            Config_SetAI::instance().Set_Stop(vals, checkBox_Buffer->isChecked());
        }
        else
        {
            RuntimeLog_Debug("qtableWidget_Stop not found in widget");
        }
    }
	
    // 使用自定义设置
    {
        checkBox_Buffer = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_useCustomJson);
        if (checkBox_Buffer)
        {
            Config_SetAI::instance().Set_UseCustomJson(checkBox_Buffer->isChecked());
        }
        else
        {
			RuntimeLog_Debug("checkBox_CustomSetting not found in widget");
            Config_SetAI::instance().Set_UseCustomJson(false);
        }
        // 保存配置文件
        plainTextEdit_Buffer = this->findChild<QPlainTextEdit*>(_PONL::SetAI_ObjNameList::plainTextEdit_customJson);
        if (plainTextEdit_Buffer)
        {
            std::string jsonText = plainTextEdit_Buffer->toPlainText().toStdString();
            Config_SetAI::instance().Set_CustomJson(jsonText);
        }
        else
        {
			RuntimeLog_Debug("plainTextEdit_CustomSetting not found in widget");
            Config_SetAI::instance().Set_CustomJson("{}");
        }
    }
	
    // 追加自定义的JSON
    {
		checkBox_Buffer = this->findChild<QCheckBox*>(_PONL::SetAI_ObjNameList::checkBox_AddCustomField);
        plainTextEdit_Buffer = this->findChild<QPlainTextEdit*>(_PONL::SetAI_ObjNameList::plainTextEdit_addCustomField);
        if(checkBox_Buffer && plainTextEdit_Buffer)
		{
			std::string jsonText = plainTextEdit_Buffer->toPlainText().toStdString();
			Config_SetAI::instance().Set_UseAppendCustomJson(checkBox_Buffer->isChecked());
			Config_SetAI::instance().Set_AppendCustomJson(jsonText);
		}
		else
		{
			RuntimeLog_Debug("plainTextEdit_addCustomField not found in widget");
			Config_SetAI::instance().Set_UseAppendCustomJson(false);
			Config_SetAI::instance().Set_AppendCustomJson("{}");
		}
    }

// 保存设置
	{
        Config_SetAI::instance().WirteConfigToFile();
	}
}

void Page_SetAI::OnRestoreDefaultBtnClicked()
{
}

void Page_SetAI::OnCheckBoxChanged(QCheckBox* checkBox, int index)
{
    switch (index)
    {
    case 1:  // maxtokens
    {
        QLineEdit* lineEdit_maxtokens = this->findChild<QLineEdit*>(_PONL::SetAI_ObjNameList::lineEdit_maxtokens);
        QSpinBox* spinBox_maxtokens = this->findChild<QSpinBox*>(_PONL::SetAI_ObjNameList::spinBox_maxtokens);
        if (lineEdit_maxtokens && spinBox_maxtokens)
        {
            if (checkBox->isChecked())
            {
                spinBox_maxtokens->setEnabled(false);
                lineEdit_maxtokens->setEnabled(true);
            }
            else
            {
                spinBox_maxtokens->setEnabled(true);
                lineEdit_maxtokens->setEnabled(false);
            }
        }
        return;
    }
    case 2:  // Prompt
    {
        QPlainTextEdit* plainTextEdit_promptPrompt = this->findChild<QPlainTextEdit*>(_PONL::SetAI_ObjNameList::plainTextEdit_prompt);
       
        if (checkBox->isChecked())
        {
            plainTextEdit_promptPrompt->setEnabled(true);
            Config_SetAI::instance().Set_UsePrompt(true);
        }
        else
        {
            plainTextEdit_promptPrompt->setEnabled(false);
            Config_SetAI::instance().Set_UsePrompt(false);
        }
        return;
    }
    
    case 3:  // System
    {
        QPlainTextEdit* plainTextEdit_promptSystem = this->findChild<QPlainTextEdit*>(_PONL::SetAI_ObjNameList::plainTextEdit_system);
        if (checkBox->isChecked())
        {
            plainTextEdit_promptSystem->setEnabled(true);
            Config_SetAI::instance().Set_UseSystem(true);
        }
        else
        {
            plainTextEdit_promptSystem->setEnabled(false);
            Config_SetAI::instance().Set_UseSystem(false);
        }
        return;
    }
    
    case 4:

        break;
    }
}
// ========== End ==========

// ========= class Page_Chat ==========
Page_Chat::Page_Chat(QWidget* parent) : 
	QWidget(parent)
{
    QWidget* page = CreatePage();
    
	if (!page)
	{
        RuntimeLog_Debug("page is nullptr");
	}
	QVBoxLayout * mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(page); // 填满整个区域
    
	setLayout(mainLayout);
}

QWidget* Page_Chat::CreatePage()
{
    QWidget* page = new QWidget(this);
    QGroupBox* box = new QGroupBox(page);

    QWidget* page_record = CreateChatRecordWidget();
    if (!page_record)
    {
        RuntimeLog_Debug("page_record is nullptr");
    }

    QWidget* page_select = CreateDateSelectWidget();
    if(!page_select)
	{
        RuntimeLog_Debug("page_select is nullptr");
	}
    
    // 布局
    QVBoxLayout* vLayout = new QVBoxLayout(box);
    vLayout->addWidget(page_select);
	vLayout->addWidget(page_record);
    vLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* pageLayout = new QVBoxLayout(page);
    pageLayout->addWidget(box);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    return page;
}

QWidget* Page_Chat::CreateDateSelectWidget()
{
    // 外层容器
    QWidget* page = new QWidget;
    QHBoxLayout* mainLayout = new QHBoxLayout(page);
    mainLayout->setContentsMargins(6, 6, 6, 6);
    mainLayout->setSpacing(0);

    // 内层有视觉样式的容器
    QFrame* container = new QFrame(page);
    container->setObjectName("dateContainer");
    container->setFrameShape(QFrame::NoFrame);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout* hl = new QHBoxLayout(container);
    hl->setContentsMargins(2, 2, 2, 2); // 内容较紧凑,留点内边距
    hl->setSpacing(8);

    // 左侧提示文字（稍微加粗）
    QLabel* lblPrompt = new QLabel(tr("请选择日期："), container);
    QFont f = lblPrompt->font();
    f.setPointSize(11);
    f.setBold(true);
    lblPrompt->setFont(f);
    lblPrompt->setStyleSheet("color: #2b2b2b;");
    lblPrompt->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    hl->addWidget(lblPrompt);

    // 日期选择（单个日期）
    QDateEdit* dateEdit = new QDateEdit(QDate::currentDate(), container);
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("yyyy-MM-dd");
    dateEdit->setToolTip(tr("选择要查看聊天记录的日期"));

    // 关键：给 dateEdit 足够的最小宽度，并允许在需要时扩展
    dateEdit->setMinimumWidth(140);             // 保证能完整显示 yyyy-MM-dd
    dateEdit->setMaximumWidth(220);             // 上限防止太宽
    dateEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    // 统一高度（后面会应用到按钮与分隔符）
    int targetHeight = 28;
    dateEdit->setFixedHeight(targetHeight);

    // dateEdit 简洁样式
    dateEdit->setStyleSheet(
        "QDateEdit {"
        "  border: 1px solid #d6e3f5;"
        "  border-radius: 4px;"
        "  padding-left: 8px;"
        "  padding-right: 6px;"
        "  background: #ffffff;"
        "  font-size: 12px;"
        "}"
        "QDateEdit::drop-down {"
        "  subcontrol-origin: padding;"
        "  subcontrol-position: right center;"
        "  width: 20px;"
        "}"
        // 给下拉日历里的年份 spinbox 足宽
        "QDateEdit QAbstractSpinBox {"
        "  min-width: 60px;"
        "}"
        "QCalendarWidget QSpinBox#qt_calendar_yearspinbox {"
        "  min-width: 80px;"
        "}"
    );

    hl->addWidget(dateEdit);

    // 小分割竖线
    QFrame* vline = new QFrame(container);
    vline->setFrameShape(QFrame::VLine);
    vline->setFrameShadow(QFrame::Plain);
    vline->setLineWidth(1);
    vline->setFixedHeight(targetHeight);
    vline->setStyleSheet("color: #e6eefc; margin-left:4px; margin-right:4px;");
    hl->addWidget(vline);

    // 说明文字：设置为 Fixed，避免占用过多宽度
    QLabel* lblHint = new QLabel(tr("查看该日期的聊天记录"), container);
    QFont fh = lblHint->font();
    fh.setPointSize(11);
    lblHint->setFont(fh);
    lblHint->setStyleSheet("color: #6b6b6b;");
    lblHint->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    lblHint->setMaximumWidth(260); // 限制最大宽度，超出时可以改为省略显示
    hl->addWidget(lblHint);

    // 伸缩确保按钮靠右（注意放在说明后面）
    hl->addStretch(1);

    // 右侧按钮
    QPushButton* btnView = new QPushButton(tr("查看"), container);
    btnView->setCursor(Qt::PointingHandCursor);
    btnView->setToolTip(tr("查看选择日期的聊天记录"));
    btnView->setFixedHeight(targetHeight);
    btnView->setMinimumWidth(80);
    btnView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnView->setStyleSheet(
        "QPushButton {"
        "  color: #ffffff;"
        "  background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4b9bff, stop:1 #1f6feb);"
        "  border: none;"
        "  border-radius: 4px;"
        "  padding-left: 10px;"
        "  padding-right: 10px;"
        "  font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "  background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #66aefe, stop:1 #2a7bf0);"
        "}"
        "QPushButton:pressed {"
        "  background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #1d66d9, stop:1 #155bb8);"
        "}"
        "QPushButton:disabled {"
        "  background: #d3dbe8; color: #9aa7bf;"
        "}"
    );
    hl->addWidget(btnView);

    // 把 container 加入 mainLayout
    mainLayout->addWidget(container);

    // container 的样式
    container->setStyleSheet(
        "#dateContainer {"
        "  background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #fbfdff, stop:1 #f6f9ff);"
        "  border: 1px solid #e6eefc;"
        "  border-radius: 8px;"
        "}"
    );

    // 页面高度尽量低
    page->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    page->setMaximumHeight(targetHeight + mainLayout->contentsMargins().top() + mainLayout->contentsMargins().bottom());

    // 槽（留空）
    connect(btnView, &QPushButton::clicked, this, [this, dateEdit]() {
        this->m_displayChatHistoryWidget->Update(GetChatHistoryManagerInstance().LoadChatHistory(dateEdit->date()));
        });

    return page;
}

QWidget* Page_Chat::CreateChatRecordWidget()
{
    QWidget* page = new QWidget;
    QGroupBox* box = new QGroupBox(QString("聊天记录"), page);

    this->m_displayChatHistoryWidget = new DisplayChatHistoryWidget();

    QJsonObject today;
    today["date"] = "2025-10-25";
    QJsonArray arr;
    arr.push_back(QJsonObject{ {"type","text"},{"body","你好"},{"sender","User"},{"ts",QDateTime::currentMSecsSinceEpoch()} });
    arr.push_back(QJsonObject{ {"type","text"},{"body","欢迎您使用糯米AI桌宠"},{"sender","AI"},{"ts",QDateTime::currentMSecsSinceEpoch() + 1} });
    today["messages"] = arr;
    this->m_displayChatHistoryWidget->Update(today);

    QVBoxLayout* vlayout_box = new QVBoxLayout(box);
    vlayout_box->addWidget(this->m_displayChatHistoryWidget);

    QVBoxLayout* vlayout_main = new QVBoxLayout(page);
    vlayout_main->addWidget(box);
    return page;
}
// =========== end ==========

// =========== class Page_Mpdel ==========
Page_Model::Page_Model(QWidget* parent) : 
	QWidget(parent)
{
    QWidget* page = CreateDisplayModelInfoPage();
    if (!page)
    {
		RuntimeLog_Debug("page is nullptr");
    }
    QVBoxLayout * mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(page); // 填满整个区域

    setLayout(mainLayout);
}
QWidget* Page_Model::CreateDisplayModelInfoPage()
{
    QWidget* page = new QWidget(this);
    QGroupBox* box = new QGroupBox(page);

    QListWidget* listWidget = new QListWidget();
    listWidget->setStyleSheet(R"(
        QListWidget::item {
            background: #ffffff;
            border: 1px solid #d0d0d0;
            border-radius: 6px;
            padding: 2px;
            margin: 2px;
        }
        QListWidget::item:selected {
            background: #f2f7ff;
            border: 1px solid #1976d2;
            border-left: 4px solid #1976d2;
        }
        QListWidget::item:hover {
            border: 1px solid #64b5f6;
        }
    )");

    for (int i = 0; i < GetLModelManagerInstance().GetLive2DModelCount(); ++i)
    {
        ListItemWidget_Live2DModel* listItem_Live2D_Buffer = new ListItemWidget_Live2DModel(
            GetLModelManagerInstance().GetLive2DModelFileName(i),
            GetLModelManagerInstance().GetLive2DModelFolderName(i));
        listItem_Live2D_Buffer->SetIndex(i);
        QListWidgetItem* listItem_Buffer = new QListWidgetItem(listWidget);
        listItem_Buffer->setSizeHint(QSize(0, 50));
        listWidget->setItemWidget(listItem_Buffer, listItem_Live2D_Buffer);
    }

    QVBoxLayout* vLayout = new QVBoxLayout(box);
    vLayout->addWidget(listWidget);
    vLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* pageLayout = new QVBoxLayout(page);
    pageLayout->addWidget(box);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    return page;
}
void Page_Model::ScanfModels()
{

}
// ========== end ===========

// ========== class PageDeveloper ===========
Page_Developer::Page_Developer(QWidget* parent) : 
	QWidget(parent)
{
    this->m_developer_Lips_Shape = new Developer_Lips_Shape();
    this->m_developer_Debug_AI_Page = new Developer_Debug_AI_Page();
    this->m_developer_Debug_Plugins = new Developer_Debug_Plugins();
    this->m_developer_Debug_Models = new Developer_Debug_Models();
    this->m_developer_Debug_ChatHistory = new Developer_Debug_ChatHistory();
    this->m_developer_Debug_NetService = new Developer_Debug_NetService();
    this->m_developer_Debug_ModelMessage = new Developer_Debug_ModelMessage();


    QWidget* contentWidget = new QWidget;
    QVBoxLayout* vlayout_developer = new QVBoxLayout(contentWidget);
    vlayout_developer->addWidget(this->m_developer_Lips_Shape);
    vlayout_developer->addWidget(this->m_developer_Debug_AI_Page);
    vlayout_developer->addWidget(this->m_developer_Debug_Plugins);
    vlayout_developer->addWidget(this->m_developer_Debug_Models);
    vlayout_developer->addWidget(this->m_developer_Debug_ChatHistory);
    vlayout_developer->addWidget(this->m_developer_Debug_NetService);
	vlayout_developer->addWidget(this->m_developer_Debug_ModelMessage);
    vlayout_developer->addStretch();

    // 处理滚动区
    this->m_scrollArea = new QScrollArea(this);
    this->m_scrollArea->setWidget(contentWidget);
    this->m_scrollArea->setWidgetResizable(true);
    this->m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // 布局
    this->m_groupbox_main = new QGroupBox();
    QVBoxLayout* vlayout_debug = new QVBoxLayout(this->m_groupbox_main);
    vlayout_debug->addWidget(this->m_scrollArea);

    QVBoxLayout* vlayout_main = new QVBoxLayout(this);   // this 是你的主窗口/对话框
    vlayout_main->addWidget(this->m_groupbox_main);
}
// ========== end ==========

// =========== class Page_Factory ===========
Page_Factory::Page_Factory()
{
}

Page_Factory& Page_Factory::Instance()
{
	static Page_Factory instance; 
	return instance;
}

// ========== end ==========