#include "LAppModelMessage.h"


// 扫描线
ScanLineWidget::ScanLineWidget(QLabel* label, QWidget* parent)
    : QWidget(label), m_bgColor(parent ? parent->palette().color(QPalette::Window)
        : Qt::white)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    resize(label->size());
    move(0, 0);
    show();
    raise();                       // 确保盖在最上面

    connect(&m_timer, &QTimer::timeout, this, [&] {
        m_y += 2;                  // 每步 2 px，可调
        if (m_y >= height()) {
            m_timer.stop();
            deleteLater();         // 揭开完成，自我销毁
            return;
        }
        update();
        });
    m_timer.start(20);
}

void ScanLineWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.fillRect(0, 0, width(), m_y, m_bgColor);   // 上方未扫到区域
    p.setPen(QPen(m_bgColor, 2));                // 扫线本身
    p.drawLine(0, m_y, width(), m_y);
}

// static 成员定义
int ModelWindowPromptWindow::s_x = 0;
int ModelWindowPromptWindow::s_y = 0;
int ModelWindowPromptWindow::s_width = 0;
int ModelWindowPromptWindow::s_height = 0;
QWidget* ModelWindowPromptWindow::s_popup = nullptr;

ModelWindowPromptWindow::ModelWindowPromptWindow(QWidget* parent) :
	QWidget(parent)
{

}

void ModelWindowPromptWindow::SetWindowNowPosition(int x, int y, int width, int height)
{
    s_x = x;
    s_y = y;
    s_width = width;
    s_height = height;
    MoveMessageWindow(); // 若提示框存在则同步移动
}

void ModelWindowPromptWindow::ShowMessageOnModelWindow(const int style_index, const QString& Message, const unsigned int timeout_ms, bool stream)
{
    if (style_index == 11)
    {
		ShowMessageOnModelWindow_V_2_1(Message, timeout_ms, stream);
        return;
    }
    switch (style_index)
    {
    case 0:
        ShowMessageOnModelWindow_V_1_0(Message, timeout_ms);
        return;
    case 3:
		ShowMessageOnModelWindow_V_1_3(Message, timeout_ms);
		return;
    case 10:
		ShowMessageOnModelWindow_V_2_0(Message, timeout_ms, stream);
        return;
    case 13:
		ShowMessageOnModelWindow_V_2_3(Message, timeout_ms, stream);
		return;
    }
}

void ModelWindowPromptWindow::ShowMessageOnModelWindow(PopWindowStyle popWindowStyle, const QString& qstr_Text, const QJsonObject& qjson_Style)
{
    if (popWindowStyle == ModelWindowPromptWindow::Style_V_3_0)
    {
        ShowMessageOnModelWindow_V_3_0(qstr_Text, qjson_Style);
        return;
    }
    switch (popWindowStyle)
    {
        default:
        {
            return;
        }
    }
}

void ModelWindowPromptWindow::ShowMessageOnModelWindow_CustomizedPage(QWidget* pCustomizedPageWidegt, QPushButton* pCloseButton, unsigned int timeout_ms)
{
    if (s_popup)
    {
		s_popup->deleteLater();
		s_popup = nullptr;
    }

    s_popup = pCustomizedPageWidegt;
    // 计算并移动
    ComputePosition();
    pCustomizedPageWidegt->show();

    // 自动关闭计时
    QTimer::singleShot(timeout_ms, pCustomizedPageWidegt, [pCustomizedPageWidegt]() {
        if (s_popup == pCustomizedPageWidegt)
            s_popup = nullptr;
        pCustomizedPageWidegt->deleteLater();
        });

    // 手动关闭
    QObject::connect(pCloseButton, &QPushButton::clicked, pCustomizedPageWidegt, [pCustomizedPageWidegt]() {
        if (s_popup == pCustomizedPageWidegt)
            s_popup = nullptr;
        pCustomizedPageWidegt->deleteLater();
        });
}

void ModelWindowPromptWindow::ShowMessageOnModelWindow_V_1_0(QString qstr_Messages, unsigned int timeout_ms)
{
    // 如果已有提示框，先关闭
    if (s_popup)
    {
        s_popup->deleteLater();
        s_popup = nullptr;
    }

    // 创建无边框窗口
    QWidget* pop = new QWidget(nullptr, 
        Qt::FramelessWindowHint | 
        Qt::WindowStaysOnTopHint | 
		Qt::Tool);
    pop->setAttribute(Qt::WA_DeleteOnClose);
    pop->setStyleSheet("background:#222;color:#eee;border:1px solid #555;border-radius:4px;");

    QLabel* label = new QLabel(qstr_Messages, pop);
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("border:none;padding:10px;");

    QPushButton* btn = new QPushButton("关闭", pop);
    btn->setFixedHeight(24);
    btn->setStyleSheet("padding:0 12px;");

    QVBoxLayout* lay = new QVBoxLayout(pop);
    lay->addWidget(label);
    lay->addWidget(btn);
    lay->setContentsMargins(8, 8, 8, 8);

    pop->adjustSize();
    s_popup = pop;

    // 计算并移动
    ComputePosition();
    pop->show();

    // 自动关闭计时
    QTimer::singleShot(timeout_ms, pop, [pop]() {
        if (s_popup == pop)
            s_popup = nullptr;
        pop->deleteLater();
    });

    // 手动关闭
    QObject::connect(btn, &QPushButton::clicked, pop, [pop]() {
        if (s_popup == pop) s_popup = nullptr;
        pop->deleteLater();
    });
}

void ModelWindowPromptWindow::ShowMessageOnModelWindow_V_1_1(QString qstr_Messages, unsigned int timeout_ms)
{
    // 若已有弹窗，先关掉
    if (s_popup)
    {
        s_popup->deleteLater();
        s_popup = nullptr;
    }

    // 创建透明无边框窗口
    QWidget* pop = new QWidget(nullptr,
        Qt::FramelessWindowHint
        | Qt::WindowStaysOnTopHint
        | Qt::Tool);
    pop->setAttribute(Qt::WA_TranslucentBackground);
    pop->setAttribute(Qt::WA_DeleteOnClose);

    // 控件
    QLabel* label = new QLabel(qstr_Messages, pop);
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);

    QPushButton* btn = new QPushButton(pop);
    btn->setFixedSize(12, 12);                 // 尽可能小
    QFont f = btn->font();
    f.setPointSize(9);
    btn->setFont(f);

    // 布局
    QVBoxLayout* lay = new QVBoxLayout(pop);
    lay->addWidget(label);
    lay->addWidget(btn, 0, Qt::AlignRight | Qt::AlignVCenter);
    lay->setContentsMargins(8, 6, 8, 6);
    lay->setSpacing(4);

    // QSS：窗口透明 + 1 px 边框；按钮透明
    pop->setStyleSheet(
        "QWidget#popup {"
        "    font-color: red;"
        "    background:transparent;"
        "    border:3px solid #555;"
        "    border-radius:3px;"
        "}"
        "QLabel {"
        "    color:#eee; "
        "    background:transparent;"
        "    border:none; "
        "}"
        "QPushButton{"
        "    font-size:9px;"
        "    color:#eee;"
        "    background:transparent;"
        "    border: 2px solid #555;"
        "    text-align:left;"
        "}"
        "QPushButton:hover {"
        "    color:#fff; "
        "}"
    );
    pop->setObjectName("popup");

    pop->adjustSize();
    s_popup = pop;
    ComputePosition();
    pop->show();

    // 倒计时逻辑
    unsigned int leftSec = (timeout_ms + 999) / 1000; // 向上取整
    auto updateBtnText = [=]() mutable {
        btn->setText(QString("关闭 %1").arg(leftSec));
        };
    updateBtnText();

    QTimer* countDown = new QTimer(pop);
    QObject::connect(countDown, &QTimer::timeout, pop, [=]() mutable {
        if (--leftSec == 0) {
            if (s_popup == pop) s_popup = nullptr;
            pop->deleteLater();
            return;
        }
        updateBtnText();
        });
    countDown->start(500); // 每 500 ms 刷新一次

    // 手动关闭
    QObject::connect(btn, &QPushButton::clicked, pop, [pop]() {
        if (s_popup == pop) s_popup = nullptr;
        pop->deleteLater();
        });
}

void ModelWindowPromptWindow::ShowMessageOnModelWindow_V_1_2(const QString& qstr_Messages, unsigned int timeout_ms)
{
    // 若已有弹窗，先关掉
    if (s_popup) {
        s_popup->deleteLater();
        s_popup = nullptr;
    }

    // 创建毛玻璃背景无边框窗口
    QWidget* pop = new QWidget(nullptr,
        Qt::FramelessWindowHint
        | Qt::WindowStaysOnTopHint
        | Qt::Tool);
    pop->setAttribute(Qt::WA_TranslucentBackground);   // 允许透明
    pop->setAttribute(Qt::WA_DeleteOnClose);

    // 控件：标签 + 分割线 + 按钮
    QLabel* label = new QLabel(qstr_Messages, pop);
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);

    // 浅分割线
    QFrame* line = new QFrame(pop);
    line->setFrameShape(QFrame::HLine);
    line->setFixedHeight(1);
    line->setStyleSheet("background:rgba(255,255,255,60);"); // 24% 白

    QPushButton* btn = new QPushButton("关闭", pop);
    btn->setFixedSize(28, 16);
    QFont f = btn->font();
    f.setPixelSize(10);
    btn->setFont(f);

    // 布局：垂直主布局
    QVBoxLayout* mainLay = new QVBoxLayout(pop);
    mainLay->addWidget(label);
    mainLay->addWidget(line);
    mainLay->addWidget(btn);
    mainLay->setContentsMargins(10, 8, 10, 0); // 下边距 0，让按钮贴边
    mainLay->setSpacing(4);

    // 把按钮强行压到左下角并与下边框重叠 2 px
    btn->move(6, pop->height() - 2); // 先临时调，show 后再精调
    QObject::connect(pop, &QWidget::resizeEvent, pop, [=](QResizeEvent*) {
        btn->move(6, pop->height() - 2);
        });

    // QSS：窗口毛玻璃 + 3 px 边框
    pop->setStyleSheet(
        "QWidget#popup {"
        "  background-color:rgba(30,30,30,200); /* 78% 黑 */"
        "  border:3px solid #555;"
        "  border-radius:5px;"
        "}"
        "QLabel{"
        "  color:#eee;"
        "  background:transparent;"
        "  border:none;"
        "  padding:2px;"
        "}"
        "QPushButton{"
        "  font-size:10px;"
        "  color:#eee;"
        "  background:transparent;"
        "  border:none;"
        "  text-align:left;"
        "}"
        "QPushButton:hover{ color:#fff; }"
    );
    pop->setObjectName("popup");

    pop->adjustSize();
    s_popup = pop;
    ComputePosition();
    pop->show();
    btn->raise(); // 确保按钮在最上层

    // 倒计时逻辑
    unsigned int leftSec = (timeout_ms + 999) / 1000;
    auto updateBtn = [=]() mutable {
        btn->setText(QString("关闭 %1").arg(leftSec));
        };
    updateBtn();

    QTimer* countDown = new QTimer(pop);
    QObject::connect(countDown, &QTimer::timeout, pop, [=]() mutable {
        if (--leftSec == 0) {
            if (s_popup == pop) s_popup = nullptr;
            pop->deleteLater();
            return;
        }
        updateBtn();
        });
    countDown->start(500);

    // 手动关闭
    QObject::connect(btn, &QPushButton::clicked, pop, [pop]() {
        if (s_popup == pop) s_popup = nullptr;
        pop->deleteLater();
        });
}

void ModelWindowPromptWindow::ShowMessageOnModelWindow_V_1_3(const QString& qstr_Messages, unsigned int timeout_ms)
{
    if (s_popup)
    {
        s_popup->deleteLater();
        s_popup = nullptr;
    }
    int* sec = new int((timeout_ms + 999) / 1000);

    // 创建无边框窗口
    QWidget* pop = new QWidget(nullptr,
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint |
        Qt::Tool);

    pop->setAttribute(Qt::WA_DeleteOnClose);
    pop->setAttribute(Qt::WA_TranslucentBackground); // 透明背景
    pop->setWindowOpacity(0.9); // 设置窗口透明度

    QLabel* label_msg = new QLabel(qstr_Messages, pop);
    label_msg->setWordWrap(true);
    label_msg->setAlignment(Qt::AlignCenter);
    // 1 px 灰色实线边框 + 圆角
    label_msg->setStyleSheet(
        "border:1px solid #888;"   // 颜色可换成 #555 / #aaa 等
        "border-radius:4px;"
        "padding:5px;");

    // 创建
    QPushButton* pushButton_close = new QPushButton(pop);
    pushButton_close->setText(QString("关闭(%1)").arg(*sec));
    pushButton_close->setFixedHeight(24);

    // 完全扁平 + 去掉系统内边距
    pushButton_close->setFlat(true);
    pushButton_close->setStyleSheet(
        "QPushButton{"
        "   padding:0px 4px;"             // 左右 4 px 即可
        "   font-size:10px;"
        "   color:#00aaffa;"                  // 正常文字色
        "   text-align:left;"             // 左对齐
        "}"
        "QPushButton:hover{"
        "   color:#33caff;"                   // 悬停高亮
        "}");

    pushButton_close->adjustSize();   // 宽度 = 文字 + 8 px

    QHBoxLayout* hlayout_tips = new QHBoxLayout();
    hlayout_tips->addWidget(pushButton_close);
    hlayout_tips->addStretch();

    QVBoxLayout* vlayout_main= new QVBoxLayout(pop);
    vlayout_main->addWidget(label_msg);
    //vlayout_main->addWidget(separator);
    vlayout_main->addLayout(hlayout_tips);
    vlayout_main->setContentsMargins(5, 5, 5, 5); // 设置边框距lay->setContentsMargins(8, 8, 8, 8);

    pop->adjustSize();
    s_popup = pop;
    ComputePosition();
    pop->show();

    // 自动关闭计时
    QTimer::singleShot((*sec * 1000), pop, [pop]() {
        if (s_popup == pop)
            s_popup = nullptr;
        pop->deleteLater();
        });

    --(*sec);
    // 创建倒计时更新计时器
    QTimer* timer = new QTimer(pop);
    connect(timer, &QTimer::timeout, [pop, pushButton_close, sec, timer]() {
        pushButton_close->setText(QString("关闭(%1)").arg(*sec));
        pushButton_close->adjustSize();
        if (*sec > 0) 
        {
            --(*sec);  // 递减 sec
        }
        else 
        {
            if (s_popup == pop)
            {
                s_popup = nullptr;
                pop->deleteLater();
            }
        }
    });
    timer->start(1000); // 每秒更新 

    // 手动关闭
    QObject::connect(pushButton_close, &QPushButton::clicked, pop, [pop]() {
        if (s_popup == pop) 
            s_popup = nullptr;
        pop->deleteLater();
    });
    QObject::connect(pop, &QObject::destroyed, [sec]() { 
        delete sec; 
    });
}

void ModelWindowPromptWindow::ShowMessageOnModelWindow_V_2_0(const QString& qstr_Messages, unsigned int timeout_ms, bool stream)
{
    if (s_popup) 
    {
        s_popup->deleteLater();
        s_popup = nullptr;
    }

    int* sec = new int((timeout_ms + 999) / 1000);

    QWidget* pop = new QWidget(nullptr,
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint |
        Qt::Tool);
    pop->setAttribute(Qt::WA_DeleteOnClose);
    pop->setAttribute(Qt::WA_TranslucentBackground);
    pop->setWindowOpacity(0.9);

    QLabel* label_msg = new QLabel(pop);
    label_msg->setWordWrap(true);
    label_msg->setAlignment(Qt::AlignLeft);
    label_msg->setStyleSheet(
        "border:1px solid #888;"
        "border-radius:4px;"
        "padding:5px;");

    QPushButton* pushButton_close = new QPushButton(pop);
    pushButton_close->setText(QString("关闭(%1)").arg(*sec));
    pushButton_close->setFixedHeight(24);
    pushButton_close->setFlat(true);
    pushButton_close->setStyleSheet(
        "QPushButton{"
        "   padding:0px 4px;"
        "   font-size:10px;"
        "   color:#00aaff;"
        "   text-align:left;"
        "}"
        "QPushButton:hover{"
        "   color:#33caff;"
        "}");
    pushButton_close->adjustSize();

    QHBoxLayout* hlayout_tips = new QHBoxLayout();
    hlayout_tips->addWidget(pushButton_close);
    hlayout_tips->addStretch();

    QVBoxLayout* vlayout_main = new QVBoxLayout(pop);
    vlayout_main->addWidget(label_msg);
    vlayout_main->addLayout(hlayout_tips);
    vlayout_main->setContentsMargins(5, 5, 5, 5);

    pop->adjustSize();
    s_popup = pop;
    ComputePosition();
    pop->show();

    // 打字机效果
    if (stream && !qstr_Messages.isEmpty()) 
    {
        QString buffer;                // 已显示部分
        int idx = 0;
        const int step = 50;           // 每 50 ms 出一个字符
        QTimer* typeTimer = new QTimer(pop);
        connect(typeTimer, &QTimer::timeout, pop, [=]() mutable {
            buffer.append(qstr_Messages.at(idx++));
            label_msg->setText(buffer);
            if (idx == qstr_Messages.size()) {
                typeTimer->stop();
                typeTimer->deleteLater();
            }
        });
        typeTimer->start(step);
    }
    else 
    {
        label_msg->setText(qstr_Messages);
    }

    // 自动关闭
    QTimer::singleShot(*sec * 1000, pop, [pop]() {
            if (s_popup == pop) 
                s_popup = nullptr;
            pop->deleteLater();
        });

    // 倒计时刷新
    --(*sec);
    QTimer* timer = new QTimer(pop);
    connect(timer, &QTimer::timeout, pop, [=]() mutable {
        pushButton_close->setText(QString("关闭(%1)").arg(*sec));
        pushButton_close->adjustSize();
        if (*sec > 0) 
        {
            --(*sec);
        }
        else 
        {
            if (s_popup == pop) 
            {
                s_popup = nullptr;
                pop->deleteLater();
            }
        }
    });
    timer->start(1000);

    // 手动关闭
    connect(pushButton_close, &QPushButton::clicked, pop, [pop]() {
        if (s_popup == pop) 
            s_popup = nullptr;
        pop->deleteLater();
    });

    // 清理 sec
    connect(pop, &QObject::destroyed, [sec]() { 
            delete sec; 
    });
}

void ModelWindowPromptWindow::ShowMessageOnModelWindow_V_2_1(const QString& qstr_Messages, unsigned int timeout_ms, bool stream)
{
    if (s_popup)
    {
        s_popup->deleteLater();
        s_popup = nullptr;
    }

    int* sec = new int((timeout_ms + 999) / 1000);

    // 创建无边框窗口
    QWidget* pop = new QWidget(nullptr,
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint |
        Qt::Tool);

    pop->setAttribute(Qt::WA_DeleteOnClose);
    pop->setAttribute(Qt::WA_TranslucentBackground);
    pop->setWindowOpacity(0.95);

    // 创建消息标签
    QLabel* label_msg = new QLabel(pop);
    label_msg->setWordWrap(true);
    label_msg->setAlignment(Qt::AlignLeft);
    QFont font("Microsoft YaHei");
    label_msg->setFont(font);
    label_msg->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    label_msg->setStyleSheet(
        "QLabel {"
        "   border: 1px solid #aaa;"
        "   border-radius: 8px;"
        "   padding: 10px;"
        "   background-color: white;"
        "   color: #333;"
        "   font-size: 12px;"
        "}");

    // 创建关闭按钮
    QPushButton* pushButton_close = new QPushButton(pop);
    pushButton_close->setText(QString("关闭(%1)").arg(*sec));
    pushButton_close->setFixedHeight(30);
    pushButton_close->setFlat(true);
    pushButton_close->setStyleSheet(
        "QPushButton {"
        "   padding: 5px 10px;"
        "   font-size: 12px;"
        "   color: #005f87;"
        "   background-color: transparent;"
        "   border: 1px solid transparent;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e0f5ff;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #c2e1ec;"
        "}");
    pushButton_close->adjustSize();
    QLabel* qlabel_SystemInfo = new QLabel(pop);
    qlabel_SystemInfo->setText(QString("CPU:%1% - RAM:%2%")
        .arg(QString::number(SystemInfo::cpuLoad(), 'f', 2))
        .arg(QString::number(SystemInfo::memPercent(), 'f', 2)));
    // 创建水平布局
    QHBoxLayout* hlayout_tips = new QHBoxLayout();
    hlayout_tips->addWidget(pushButton_close);
    hlayout_tips->addWidget(qlabel_SystemInfo);
    hlayout_tips->addStretch();

    // 创建垂直布局
    QVBoxLayout* vlayout_main = new QVBoxLayout(pop);
    vlayout_main->addWidget(label_msg);
    vlayout_main->addLayout(hlayout_tips);
    vlayout_main->setContentsMargins(10, 10, 10, 10);
    vlayout_main->setSizeConstraint(QLayout::SetFixedSize); // 让高度随内容，但宽度不再膨胀
    pop->adjustSize();
    s_popup = pop;
    ComputePosition();
    pop->show();

    // 打字机效果
    if (stream && !qstr_Messages.isEmpty())
    {
        QString buffer;               // 已显示部分
        int idx = 0;
        const int step = 50;          // 每50 ms出一个字符
        QTimer* typeTimer = new QTimer(pop);
        connect(typeTimer, &QTimer::timeout, pop, [=]() mutable {
            // 在打字机效果中添加淡入淡出效果
            label_msg->setText(buffer + qstr_Messages.at(idx));
            buffer.append(qstr_Messages.at(idx++));
            if (idx == qstr_Messages.size()) 
            {
                typeTimer->stop();
                typeTimer->deleteLater();
            }
        });
        typeTimer->start(step);
    }
    else
    {
        label_msg->setText(qstr_Messages);
    }

    // 自动关闭
    QTimer::singleShot(*sec * 1000, pop, [pop]() {
        if (s_popup == pop)
            s_popup = nullptr;
        pop->deleteLater();
    });

    // 倒计时刷新
    --(*sec);
    QTimer* timer = new QTimer(pop);
    connect(timer, &QTimer::timeout, pop, [=]() mutable {
        pushButton_close->setText(QString("关闭(%1)").arg(*sec));
        qlabel_SystemInfo->setText(QString("CPU:%1% - RAM:%2%")
            .arg(QString::number(SystemInfo::cpuLoad(), 'f', 2))
            .arg(QString::number(SystemInfo::memPercent(), 'f', 2)));
        pushButton_close->adjustSize();
        if (*sec > 0)
        {
            --(*sec);
        }
        else
        {
            if (s_popup == pop)
            {
                s_popup = nullptr;
                pop->deleteLater();
            }
        }
        });
    timer->start(1000);

    // 手动关闭
    connect(pushButton_close, &QPushButton::clicked, pop, [pop]() {
        if (s_popup == pop)
            s_popup = nullptr;
        pop->deleteLater();
        });

    // 清理 sec
    connect(pop, &QObject::destroyed, [sec]() {
        delete sec;
        });
}

void ModelWindowPromptWindow::ShowMessageOnModelWindow_V_2_2(const QString& qstr_Messages, unsigned int timeout_ms, bool stream)
{
    if (s_popup)
    {
        s_popup->deleteLater();
        s_popup = nullptr;
    }

    int* sec = new int((timeout_ms + 999) / 1000);

    // 创建无边框窗口
    QWidget* pop = new QWidget(nullptr,
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint |
        Qt::Tool);

    pop->setAttribute(Qt::WA_DeleteOnClose);
    pop->setAttribute(Qt::WA_TranslucentBackground);
    pop->setWindowOpacity(0.95);

    // 创建消息标签
    QLabel* label_msg = new QLabel(pop);
    label_msg->setWordWrap(true);
    label_msg->setAlignment(Qt::AlignLeft);
    label_msg->setStyleSheet(
        "QLabel {"
        "   border: 1px solid #aaa;"
        "   border-radius: 8px;"
        "   padding: 10px;"
        "   background-color: white;"
        "   color: #333;"
        "   font-size: 12px;"
        "}");

    // 创建关闭按钮
    QPushButton* pushButton_close = new QPushButton(pop);
    pushButton_close->setText(QString("关闭(%1)").arg(*sec));
    pushButton_close->setFixedHeight(30);
    pushButton_close->setFlat(true);
    pushButton_close->setStyleSheet(
        "QPushButton {"
        "   padding: 5px 10px;"
        "   font-size: 12px;"
        "   color: #005f87;"
        "   background-color: transparent;"
        "   border: 1px solid transparent;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e0f5ff;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #c2e1ec;"
        "}");

    pushButton_close->adjustSize();

    // 创建水平布局
    QHBoxLayout* hlayout_tips = new QHBoxLayout();
    hlayout_tips->addWidget(pushButton_close);
    hlayout_tips->addStretch();

    // 创建垂直布局
    QVBoxLayout* vlayout_main = new QVBoxLayout(pop);
    vlayout_main->addWidget(label_msg);
    vlayout_main->addLayout(hlayout_tips);
    vlayout_main->setContentsMargins(10, 10, 10, 10);

    pop->adjustSize();
    s_popup = pop;
    ComputePosition();
    pop->show();

    // 打字机效果
    if (stream && !qstr_Messages.isEmpty())
    {
        QString buffer;               // 已显示部分
        int idx = 0;
        const int step = 50;          // 每50 ms出一个字符
        QTimer* typeTimer = new QTimer(pop);
        connect(typeTimer, &QTimer::timeout, pop, [=]() mutable {
            // 在打字机效果中添加淡入淡出效果
            label_msg->setText(buffer + qstr_Messages.at(idx));
            if (idx == 0) 
                new ScanLineWidget(label_msg, pop);   // 第一次输出时套动画
            buffer.append(qstr_Messages.at(idx++));
            if (idx == qstr_Messages.size()) {
                typeTimer->stop();
                typeTimer->deleteLater();
            }
            });
        typeTimer->start(step);
    }
    else
    {
        label_msg->setText(qstr_Messages);
    }

    // 自动关闭
    QTimer::singleShot(*sec * 1000, pop, [pop]() {
        if (s_popup == pop)
            s_popup = nullptr;
        pop->deleteLater();
        });

    // 倒计时刷新
    --(*sec);
    QTimer* timer = new QTimer(pop);
    connect(timer, &QTimer::timeout, pop, [=]() mutable {
        pushButton_close->setText(QString("关闭(%1)").arg(*sec));
        pushButton_close->adjustSize();
        if (*sec > 0)
        {
            --(*sec);
        }
        else
        {
            if (s_popup == pop)
            {
                s_popup = nullptr;
                pop->deleteLater();
            }
        }
        });
    timer->start(1000);

    // 手动关闭
    connect(pushButton_close, &QPushButton::clicked, pop, [pop]() {
        if (s_popup == pop)
            s_popup = nullptr;
        pop->deleteLater();
        });

    // 清理 sec
    connect(pop, &QObject::destroyed, [sec]() {
        delete sec;
        });
}

void ModelWindowPromptWindow::ShowMessageOnModelWindow_V_2_3(const QString& qstr_Messages, unsigned int timeout_ms, bool stream)
{
    if (s_popup) 
    {
        s_popup->deleteLater();
        s_popup = nullptr;
    }

    int* sec = new int((timeout_ms + 999) / 1000);

    QWidget* pop = new QWidget(nullptr,
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint |
        Qt::Tool);
    pop->setAttribute(Qt::WA_DeleteOnClose);
    pop->setAttribute(Qt::WA_TranslucentBackground);
    pop->setWindowOpacity(0.95);

    // 消息标签
    QLabel* label_msg = new QLabel(pop);
    label_msg->setWordWrap(true);
    label_msg->setAlignment(Qt::AlignLeft);
    label_msg->setStyleSheet(
        "QLabel {"
        "   border: 1px solid #aaa;"
        "   border-radius: 8px;"
        "   padding: 10px;"
        "   background-color: white;"
        "   color: #333;"
        "   font-size: 12px;"
        "}");

    // 超细进度条
    QProgressBar* progress = new QProgressBar(pop);
    progress->setRange(0, 100);
    progress->setTextVisible(false);          // 条内不显示文字
    progress->setFixedHeight(2);              // 2 像素高
    progress->setStyleSheet(
        "QProgressBar {"
        "   border: none;"
        "   background-color: #e6e6e6;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #05B8CC;"
        "}");

    // 独立百分比文本
    QLabel* percentLabel = new QLabel("0%", pop);
    percentLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    percentLabel->setStyleSheet(
        "QLabel {"
        "   font-size: 8px;"
        "   color: #666;"
        "   background: transparent;"
        "   padding: 0 2px 0 0;"
        "}");

    // 关闭按钮 
    QPushButton* pushButton_close = new QPushButton(pop);
    pushButton_close->setText(QString("关闭(%1)").arg(*sec));
    pushButton_close->setFixedHeight(30);
    pushButton_close->setFlat(true);
    pushButton_close->setStyleSheet(
        "QPushButton {"
        "   padding: 5px 10px;"
        "   font-size: 12px;"
        "   color: #005f87;"
        "   background-color: transparent;"
        "   border: 1px solid transparent;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e0f5ff;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #c2e1ec;"
        "}");

    pushButton_close->adjustSize();

    // 布局
    QHBoxLayout* hlayout_tips = new QHBoxLayout();
    hlayout_tips->addWidget(pushButton_close);
    hlayout_tips->addStretch();

    // - 进度条 + 百分比
    QHBoxLayout* hlayout_progress = new QHBoxLayout();
    hlayout_progress->setContentsMargins(0, 0, 0, 0);
    hlayout_progress->setSpacing(0);
    hlayout_progress->addWidget(progress);
    hlayout_progress->addWidget(percentLabel);

    QVBoxLayout* vlayout_main = new QVBoxLayout(pop);
    vlayout_main->addWidget(label_msg);
    vlayout_main->addLayout(hlayout_progress);   // 紧贴消息框下方
    vlayout_main->addLayout(hlayout_tips);
    vlayout_main->setContentsMargins(10, 10, 10, 10);

    pop->adjustSize();
    s_popup = pop;
    ComputePosition();
    pop->show();

    //打字机 / 进度更新
    auto updateProgress = [=](int percent) {
        if (progress && percentLabel) 
        {  // 确保这些指针依旧有效
            progress->setValue(percent);
            percentLabel->setText(percent == 100 ? QString("完成") : QString("%1%").arg(percent));
        }
    };


    if (stream && !qstr_Messages.isEmpty()) 
    {
        const int total = qstr_Messages.size();
        QString buffer;
        int idx = 0;
        const int step = 50;
        QTimer* typeTimer = new QTimer(pop);
        connect(typeTimer, &QTimer::timeout, pop, [=]() mutable {
            if (idx < total) 
            {
                buffer.append(qstr_Messages.at(idx));
                label_msg->setText(buffer);
                updateProgress((idx + 1) * 100 / total); // 请求更新进度
                ++idx;
            }
            if (idx == total) 
            {
                typeTimer->stop();
                typeTimer->deleteLater();
            }
            });
        typeTimer->start(step);
    }
    else 
    {
        label_msg->setText(qstr_Messages);
        updateProgress(100); // 如果直接展示文字
    }

    // 关闭逻辑
    QTimer::singleShot(*sec * 1000, pop, [pop]() {
        if (s_popup == pop) 
            s_popup = nullptr;
        pop->deleteLater();
    });

    --(*sec);
    QTimer* timer = new QTimer(pop);
    connect(timer, &QTimer::timeout, pop, [=]() mutable {
        pushButton_close->setText(QString("关闭(%1)").arg(*sec));
        pushButton_close->adjustSize();
        if (*sec > 0) 
        {
            --(*sec);
        }
        else 
        {
            if (s_popup == pop) 
            {
                s_popup = nullptr;
                pop->deleteLater();
            }
        }
    });
    timer->start(1000);

    connect(pushButton_close, &QPushButton::clicked, pop, [pop]() {
        if (s_popup == pop) 
            s_popup = nullptr;
        pop->deleteLater();
    });

    connect(pop, &QObject::destroyed, [sec]() { 
        delete sec; 
    });
}

void ModelWindowPromptWindow::ShowMessageOnModelWindow_V_3_0(const QString& qstr_Messages, QJsonObject qjson_style)
{
    bool useStream = ConvenientTool::GetValueFromQJson<bool>(qjson_style, "stream", "use");
    // 获取显示文本的方式
    int count = 0;
    bool bFixedSpeed = ConvenientTool::GetValueFromQJson<bool>(qjson_style, "fixed_speed", "use");  // 是否使用固定速度
    if (bFixedSpeed)
    {
        ++count;
    }
    bool bRandomSpeed = ConvenientTool::GetValueFromQJson<bool>(qjson_style, "random_speed", "use");    // 是否使用随机速度
    if(bRandomSpeed)
	{
		++count;
	}
    // 固定速度的变种
    bool bFixedTime = ConvenientTool::GetValueFromQJson<bool>(qjson_style, "fixed_time", "use");    // 是否使用固定时间
    if (bFixedTime)
    {
		++count;
    }
    bool bAudioDuration = ConvenientTool::GetValueFromQJson<bool>(qjson_style, "audio_duration", "use");    // 是否使用音频时长
    if (bAudioDuration)
    {
		++count;
    }
    // 防止启用多个显示方式
    if (count > 1)
    {
        bFixedSpeed = true;
        bRandomSpeed = false;
        bFixedTime = false;
        bAudioDuration = false;
    }
    int interval = 0, range_min = 0, range_max = 0;
    if (bFixedSpeed)
    {
        interval = ConvenientTool::GetValueFromQJson<int>(qjson_style, "fixed_speed", "interval");
    }
    if (bRandomSpeed)
    {
        range_min = ConvenientTool::GetValueFromQJson<int>(qjson_style, "random_speed", "range_min");
        range_max = ConvenientTool::GetValueFromQJson<int>(qjson_style, "random_speed", "range_max");
    }
    // 固定速度的变种
    if (bFixedTime)
	{
		 int time_ms= ConvenientTool::GetValueFromQJson<int>(qjson_style, "fixed_time", "timeout");
         if(time_ms <= 0)
			 time_ms = 3000;
         interval = time_ms * 1.0 / qstr_Messages.size() + 1;
         bFixedSpeed = true;
	}
	if (bAudioDuration)
	{
        int wav_duration_ms = ConvenientTool::GetValueFromQJson<int>(qjson_style, "audio_duration", "duration");
        if(wav_duration_ms <= 0)
			wav_duration_ms = 3000;
        interval = wav_duration_ms * 1.0 / qstr_Messages.size() - 5;
        bFixedSpeed = true;
	}

    // 读取自动关闭秒数（优先从 qjson_style 中取值，否则默认 5 秒）
    int defaultAutoCloseSec = 5;
    int initialAutoCloseSec = defaultAutoCloseSec;

    if (s_popup)
    {
        s_popup->deleteLater();
        s_popup = nullptr;
    }

    // 分配倒计时整数（使用 new 以便多个 lambda 共享并在窗口销毁时释放）
    int* sec = new int(initialAutoCloseSec);

    // 创建无边框窗口
    QWidget* pop = new QWidget(nullptr,
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint |
        Qt::Tool);

    pop->setAttribute(Qt::WA_DeleteOnClose);
    pop->setAttribute(Qt::WA_TranslucentBackground);
    pop->setWindowOpacity(0.95);

    // 创建消息标签
    QLabel* label_msg = new QLabel(pop);
    label_msg->setWordWrap(true);
    label_msg->setAlignment(Qt::AlignLeft);
    QFont font("Microsoft YaHei");
    label_msg->setFont(font);
    label_msg->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    label_msg->setStyleSheet(
        "QLabel {"
        "   border: 1px solid #aaa;"
        "   border-radius: 8px;"
        "   padding: 10px;"
        "   background-color: white;"
        "   color: #333;"
        "   font-size: 12px;"
        "}");

    // 创建关闭按钮（初始禁用，等待文本显示完成后启用）
    QPushButton* pushButton_close = new QPushButton(pop);
    pushButton_close->setText(QString("关闭")); // 不显示倒计时，直到启用后显示
    pushButton_close->setFixedHeight(30);
    pushButton_close->setFlat(true);
    //pushButton_close->setEnabled(false); // 初始禁用
    pushButton_close->setStyleSheet(
        "QPushButton {"
        "   padding: 5px 10px;"
        "   font-size: 12px;"
        "   color: #005f87;"
        "   background-color: transparent;"
        "   border: 1px solid transparent;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e0f5ff;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #c2e1ec;"
        "}");
    pushButton_close->adjustSize();

    QLabel* qlabel_SystemInfo = new QLabel(pop);
    qlabel_SystemInfo->setText(QString("CPU:%1% - RAM:%2%")
        .arg(QString::number(SystemInfo::cpuLoad(), 'f', 2))
        .arg(QString::number(SystemInfo::memPercent(), 'f', 2)));

    // 创建水平布局
    QHBoxLayout* hlayout_tips = new QHBoxLayout();
    hlayout_tips->addWidget(qlabel_SystemInfo);
    hlayout_tips->addWidget(pushButton_close);
    hlayout_tips->addStretch();

    // 创建垂直布局
    QVBoxLayout* vlayout_main = new QVBoxLayout(pop);
    vlayout_main->addWidget(label_msg);
    vlayout_main->addLayout(hlayout_tips);
    vlayout_main->setContentsMargins(10, 10, 10, 10);
    vlayout_main->setSizeConstraint(QLayout::SetFixedSize); // 让高度随内容，但宽度不再膨胀
    pop->adjustSize();
    s_popup = pop;
    ComputePosition();
    pop->show();

    // 倒计时与系统信息定时器指针（稍后创建）
    QTimer* countdownTimer = nullptr;

    // 打字机效果
    if (useStream && !qstr_Messages.isEmpty())
    {
        QString buffer;               // 已显示部分
        int idx = 0;
        QTimer* typeTimer = new QTimer(pop);
        typeTimer->setSingleShot(true);   // 单次触发，后续的字符由 nextChar lambda 触发

        // 此函数的作用是显示下一个字符
        std::function<void()> nextChar = [=]() mutable {
            if (idx >= qstr_Messages.size())
            {
                // 显示完成，启动倒计时并启用关闭按钮
                typeTimer->deleteLater();

                // 关闭按钮显示倒计时值
                pushButton_close->setText(QString("关闭(%1)").arg(*sec));
                pushButton_close->adjustSize();

                // 创建倒计时 timer（每秒触发）
                countdownTimer = new QTimer(pop);
                countdownTimer->setInterval(1000);
                connect(countdownTimer, &QTimer::timeout, pop, [=]() mutable {
                    // 更新系统信息
                    qlabel_SystemInfo->setText(QString("CPU:%1% - RAM:%2%")
                        .arg(QString::number(SystemInfo::cpuLoad(), 'f', 2))
                        .arg(QString::number(SystemInfo::memPercent(), 'f', 2)));

                    if (*sec > 0)
                    {
                        --(*sec);
                        pushButton_close->setText(QString("关闭(%1)").arg(*sec));
                        pushButton_close->adjustSize();
                    }
                    else
                    {
                        // 时间到，关闭窗口
                        if (s_popup == pop)
                        {
                            s_popup = nullptr;
                        }
                        pop->deleteLater();
                    }
                    });
                countdownTimer->start();

                return;
            }

            buffer.append(qstr_Messages.at(idx));
            label_msg->setText(buffer);
            ++idx;

            // 计算下一次间隔
            int delay = 0;
            if (bFixedSpeed)
            {
                delay = interval;
            }
            else if (bRandomSpeed)
            {
                delay = QRandomGenerator::global()->bounded(range_min, range_max + 1);
            }

            typeTimer->start(delay);
            };

        connect(typeTimer, &QTimer::timeout, pop, nextChar);

        // 启动第一个字符
        nextChar();
    }
    else
    {
        // 非流式：直接显示文本并立即启用关闭按钮与倒计时
        label_msg->setText(qstr_Messages);

        pushButton_close->setEnabled(true);
        pushButton_close->setText(QString("关闭(%1)").arg(*sec));
        pushButton_close->adjustSize();

        countdownTimer = new QTimer(pop);
        countdownTimer->setInterval(1000);
        connect(countdownTimer, &QTimer::timeout, pop, [=]() mutable {
            qlabel_SystemInfo->setText(QString("CPU:%1% - RAM:%2%")
                .arg(QString::number(SystemInfo::cpuLoad(), 'f', 2))
                .arg(QString::number(SystemInfo::memPercent(), 'f', 2)));

            if (*sec > 0)
            {
                --(*sec);
                pushButton_close->setText(QString("关闭(%1)").arg(*sec));
                pushButton_close->adjustSize();
            }
            else
            {
                if (s_popup == pop)
                    s_popup = nullptr;
                pop->deleteLater();
            }
            });
        countdownTimer->start();
    }

    // 手动关闭（立即关闭）
    connect(pushButton_close, &QPushButton::clicked, pop, [pop, sec]() {
        if (s_popup == pop)
            s_popup = nullptr;
        pop->deleteLater();
        });

    // 窗口销毁时清理 sec 指针 和 停止计时器等
    connect(pop, &QObject::destroyed, [sec]() {
        delete sec;
        });
}

void ModelWindowPromptWindow::ComputePosition()
{
    if (!s_popup) 
        return;
    QSize sz = s_popup->size();
    int px = s_x + s_width / 2 + 20;
    int py = s_y + (s_height - sz.height()) / 3 - 20; // 偏上
    s_popup->move(px, py);
}

void ModelWindowPromptWindow::MoveMessageWindow()
{
    if (s_popup) 
        ComputePosition();
}

// -------------------- MessageBubble --------------------

void MessageBubble::SetTail(TailSide side, double ratio)
{
    this->m_tailSide = side;
    this->m_tailRatio = qBound(-1.0, ratio, 1.0);
    UpdateSize();   // 左右方向时尾巴占的是宽度，需要重算尺寸
    update();
}


MessageBubble::MessageBubble(QWidget* parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint
        | Qt::Tool                    // 不出现在任务栏
        | Qt::WindowStaysOnTopHint);  // 置顶，保证在桌面上可见
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating); // 不抢焦点
    setFocusPolicy(Qt::NoFocus);

    this->m_doc.setDocumentMargin(0);
    this->m_doc.setDefaultFont(QFont(QStringLiteral("Microsoft YaHei"), 10));
    // 让 markdown / html 渲染更顺眼（QTextDocument 支持的 CSS 子集）
    this->m_doc.setDefaultStyleSheet(QStringLiteral(
        "p{margin:0;} "
        "code{background:#eef0f2; border-radius:3px; padding:0 3px;} "
        "pre{background:#f5f6f8; padding:6px; border-radius:6px;} "
        "blockquote{color:#777; padding-left:8px;}"));

    // 打字机定时器
    this->m_typeTimer.setInterval(24);
    connect(&this->m_typeTimer, &QTimer::timeout, this, [this] {
        this->m_visibleLen = qMin(this->m_visibleLen + 2, int(this->m_fullText.size()));
        RebuildDocument();
        UpdateSize();
        if (this->m_visibleLen >= this->m_fullText.size())
        {
            this->m_typeTimer.stop();
            this->m_streaming = false;
            emit displayFinished(this);   // 流式显示结束
            MaybeStartDismiss();          // 此刻才开始自动关闭倒计时
        }
        });

    // 自动关闭倒计时（单次触发）
    this->m_dismissTimer.setSingleShot(true);
    connect(&this->m_dismissTimer, &QTimer::timeout, this, [this] {
        emit dismissRequested(this);   // 不自杀，通知管理者走统一销毁流程
        });
}

void MessageBubble::SetContent(TextType type, const QString& text)
{
    this->m_typeTimer.stop();
    this->m_streaming = false;
    this->m_type = type;
    this->m_fullText = text;
    this->m_visibleLen = int(text.size());
    RebuildDocument();
    UpdateSize();
    emit displayFinished(this);
    MaybeStartDismiss();
}

void MessageBubble::StartStream(TextType type, const QString& content)
{
    this->m_type = type;
    this->m_fullText = content;
    this->m_visibleLen = 0;
    this->m_streaming = true;
    RebuildDocument();
    UpdateSize();
    this->m_typeTimer.start();
}

void MessageBubble::AppendContent(const QString& chunk)
{
    this->m_dismissTimer.stop();          // 新分片到达，打断自动关闭倒计时
    this->m_fullText += chunk;
    // 若当前可见长度 < 完整长度，启动定时器把新增片段"打"出来
    if (this->m_visibleLen < this->m_fullText.size() && !this->m_typeTimer.isActive())
    {
        this->m_streaming = true;
        this->m_typeTimer.start();
    }
}

bool MessageBubble::IsStreaming() const
{
    return this->m_streaming;
}

void MessageBubble::SetTailTarget(const QPoint& globalPos)
{
    this->m_tailGlobal = globalPos;
    update();   // 尾巴尖端改变进行重绘
}

void MessageBubble::SetAutoDismiss(int ms)
{
    this->m_autoDismissMs = ms;
    // 如果设置时已经显示完毕（direct 模式），立即开始倒计时
    if (!this->m_streaming)
        MaybeStartDismiss();
}

void MessageBubble::ShowAnimated()
{
    m_entered = true;
    const QRect target = geometry();   // 管理器已把最终位置布局好

    // 起始矩形：缩小到 30%，锚在底边中心（尾巴一侧）——气泡从嘴边"长"出来
    QRect start = target;
    start.setSize(target.size() * 0.3);
    start.moveCenter(QPoint(target.center().x(),
        target.bottom() - this->m_tailHeight / 2));

    setGeometry(start);
    setWindowOpacity(0.0);
    show();

    auto* group = new QParallelAnimationGroup(this);

    auto* fade = new QPropertyAnimation(this, "windowOpacity");
    fade->setDuration(200);
    fade->setStartValue(0.0);
    fade->setEndValue(1.0);
    fade->setEasingCurve(QEasingCurve::OutCubic);

    auto* pop = new QPropertyAnimation(this, "geometry");
    pop->setDuration(320);
    pop->setStartValue(start);
    pop->setEndValue(target);
    pop->setEasingCurve(QEasingCurve::OutBack);   // 回弹，漫画感来源

    group->addAnimation(fade);
    group->addAnimation(pop);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void MessageBubble::SlideTo(const QRect& target)
{
    // 入场动画没播完、或位置没变：直接设置，不播动画
    if (!this->m_entered || isHidden() || geometry() == target)
    {
        setGeometry(target);
        return;
    }
    auto* anim = new QPropertyAnimation(this, "geometry");
    anim->setDuration(160);
    anim->setStartValue(geometry());
    anim->setEndValue(target);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    update();   // 位置变了，重算尾巴
}

void MessageBubble::RebuildDocument()
{
    const QString text = m_fullText.left(m_visibleLen);
    switch (m_type)
    {
    case TextType::Markdown: 
        this->m_doc.setMarkdown(text);  
        break;   // Qt6 原生支持
    case TextType::Html:     
        this->m_doc.setHtml(text);      
        break;
    default:                 
        this->m_doc.setPlainText(text); 
        break;
    }
    this->m_doc.setTextWidth(this->m_maxWidth);
}

void MessageBubble::UpdateSize()
{
    const QSizeF ds = this->m_doc.size();
    const TailSide side = ResolvedSide();
    const bool horizontalTail = (side == TailSide::Left || side == TailSide::Right);

    const int w = qMax(70, int(ds.width()) + this->m_hPad * 2 + (horizontalTail ? this->m_tailHeight : 0));
    const int h = qMax(46, int(ds.height()) + this->m_vPad * 2 + (horizontalTail ? 0 : this->m_tailHeight));
    if (size() != QSize(w, h)) 
    {
        resize(w, h);
        emit geometryChanged();
    }
    update();
}

QRect MessageBubble::BodyRect() const
{
    switch (ResolvedSide()) 
    {
    case TailSide::Bottom: 
        return rect().adjusted(0, 0, 0, -this->m_tailHeight);
    case TailSide::Top:    
        return rect().adjusted(0, this->m_tailHeight, 0, 0);
    case TailSide::Left:   
        return rect().adjusted(this->m_tailHeight, 0, 0, 0);
    case TailSide::Right:  
        return rect().adjusted(0, 0, -this->m_tailHeight, 0);
    default:               
        return rect();
    }
}

void MessageBubble::MaybeStartDismiss()
{
    if (m_autoDismissMs >= 0 && !m_streaming)
        m_dismissTimer.start(m_autoDismissMs);
}

MessageBubble::TailSide MessageBubble::ResolvedSide() const
{
    if (m_tailSide != TailSide::Auto)
        return m_tailSide;
    // 自动：锚点在气泡下方
    // 尾巴在底边，反之在顶边（保持原行为）
    return (mapFromGlobal(m_tailGlobal).y() >= height() / 2) ? TailSide::Bottom : TailSide::Top;
}

void MessageBubble::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const QRect body = BodyRect();
    const TailSide side = ResolvedSide();
    const QPoint anchorLocal = mapFromGlobal(this->m_tailGlobal);

    // ---- 计算尖端与根部 ----
    QPoint tip, baseA, baseB;
    const int halfBase = 14;   // 尾巴根部半宽

    auto ratioOn = [&](int lo, int hi, int autoAim) {
        // ratio >= 0 用指定比例；< 0 自动对准锚点（钳制在主体范围内）
        if (this->m_tailRatio >= 0.0)
            return lo + int(this->m_tailRatio * (hi - lo));
        return qBound(lo + 24, autoAim, hi - 24);
        };

    switch (side) 
    {
    case TailSide::Bottom: 
    {
        const int bx = ratioOn(body.left(), body.right(), anchorLocal.x());
        tip = QPoint(bx, body.bottom() + m_tailHeight);
        baseA = QPoint(bx - halfBase, body.bottom());
        baseB = QPoint(bx + halfBase, body.bottom());
        break;
    }
    case TailSide::Top: 
    {
        const int bx = ratioOn(body.left(), body.right(), anchorLocal.x());
        tip = QPoint(bx, body.top() - m_tailHeight);
        baseA = QPoint(bx - halfBase, body.top());
        baseB = QPoint(bx + halfBase, body.top());
        break;
    }
    case TailSide::Left: 
    {
        const int by = ratioOn(body.top(), body.bottom(), anchorLocal.y());
        tip = QPoint(body.left() - m_tailHeight, by);
        baseA = QPoint(body.left(), by - halfBase);
        baseB = QPoint(body.left(), by + halfBase);
        break;
    }
    case TailSide::Right: 
    {
        const int by = ratioOn(body.top(), body.bottom(), anchorLocal.y());
        tip = QPoint(body.right() + m_tailHeight, by);
        baseA = QPoint(body.right(), by - halfBase);
        baseB = QPoint(body.right(), by + halfBase);
        break;
    }
    default: break;
    }

    QPainterPath bubblePath;
    bubblePath.addRoundedRect(body, 14, 14);

    QPainterPath tailPath;
    tailPath.moveTo(baseA);
    tailPath.lineTo(tip);
    tailPath.lineTo(baseB);
    tailPath.closeSubpath();

    const QPainterPath shape = bubblePath.united(tailPath);

    p.fillPath(shape, QColor(255, 255, 255, 245));
    p.setPen(QPen(QColor(60, 60, 60), 2));
    p.drawPath(shape);

    p.save();
    p.translate(body.adjusted(this->m_hPad, this->m_vPad, -this->m_hPad, -this->m_vPad).topLeft());
    m_doc.drawContents(&p);
    p.restore();
    return;
}

void MessageBubble::mousePressEvent(QMouseEvent* event)
{
    emit clicked(this);
    QWidget::mousePressEvent(event);
}

// -------------------- BubbleManager --------------------
BubbleManager& BubbleManager::instance()
{
    static BubbleManager s;   // C++11 起局部静态变量初始化线程安全
    return s;
}

BubbleManager::BubbleManager(QObject* parent) : QObject(parent) {}

BubbleManager::~BubbleManager()
{
    qDeleteAll(m_bubbles);
    m_bubbles.clear();
}

void BubbleManager::SetMaxBubbles(int n)
{
    m_max = qMax(1, n);
    while (m_bubbles.size() > m_max)
        RemoveOldest();
}

int BubbleManager::MaxBubbles() const
{
    return m_max;
}

void BubbleManager::LayoutBubbles(bool animate)
{
    if (!m_anchorValid || m_bubbles.isEmpty())
        return;

    QScreen* screen = QGuiApplication::screenAt(m_anchor);
    if (!screen)
        screen = QGuiApplication::primaryScreen();
    const QRect avail = screen->availableGeometry();

    int x = m_anchor.x() + 28;   // 气泡默认在锚点右上方
    //int y = m_anchor.y() - 30;
    int y = m_anchor.y();

    // 最新的气泡最靠近角色，依次向上堆叠
    for (int i = m_bubbles.size() - 1; i >= 0; --i)
    {
        MessageBubble* b = m_bubbles[i];
        y -= b->height();

        //int bx = x;
        int bx = m_anchor.x() - b->width() / 2;
        if (bx + b->width() > avail.right())          // 右边放不下 → 放左边
            bx = m_anchor.x() - b->width() - 28;
        bx = qBound(avail.left(), bx, int(avail.right() - b->width()));

        if (animate)
            b->SlideTo(QRect(bx, qMax(int(avail.top()), y), b->width(), b->height()));
        else
            b->move(bx, qMax(int(avail.top()), y));
        b->update();                                  // 移动后重算尾巴朝向
        y -= 12;                                      // 气泡间距
    }
}

void BubbleManager::SetAnchor(int x, int y)
{
    SetAnchor(QPoint(x, y));
}

void BubbleManager::SetAnchor(const QPoint& pos)
{
    m_anchor = pos;
    m_anchorValid = true;
    for (auto* b : m_bubbles)
        b->SetTailTarget(pos);
    LayoutBubbles(false);   // 跟随移动：60Hz 小步推进本身已平滑，不再叠加动画
}

void BubbleManager::ShowMessage(const QString& jsonText)
{
    // 用 nlohmann::json 解析；UTF-8 传入，中文不会乱码
    const nlohmann::json j =
        nlohmann::json::parse(jsonText.toUtf8().constData(), nullptr, false);
    if (j.is_discarded() || !j.is_object())
    {
        qWarning() << "[BubbleManager] JSON 解析失败，消息已忽略:" << jsonText;
        return;
    }
    HandleJson(j);
}

void BubbleManager::ShowMessage(const QJsonObject& jsonObj)
{
    // QJsonObject → 字符串 → nlohmann::json，保证解析路径统一
    ShowMessage(QString::fromUtf8(
        QJsonDocument(jsonObj).toJson(QJsonDocument::Compact)));
}

void BubbleManager::HandleJson(const nlohmann::json& j)
{
    // ---- 1. 三个必填字段校验，缺任意一个直接忽略 ----
    if (!j.contains("type") ||      // 文本类型: markdown / html / text
        !j.contains("mode") ||      // 显示模式: stream / direct / none
        !j.contains("content"))     // 文本内容
        return;
    if (!j["type"].is_string() || !j["mode"].is_string() || !j["content"].is_string())
        return;

    const std::string typeS = j["type"].get<std::string>();
    const std::string modeS = j["mode"].get<std::string>();
    const QString     content = QString::fromStdString(j["content"].get<std::string>());

    // ---- 2. 显示模式 ----
    if (modeS == "none")            // 不显示：即使收到信号也丢弃
        return;

    // ---- 3. 文本类型 ----
    MessageBubble::TextType tt;
    if (typeS == "markdown" || typeS == "md")                    
        tt = MessageBubble::TextType::Markdown;
    else if (typeS == "html")                                         
        tt = MessageBubble::TextType::Html;
    else if (typeS == "text" || typeS == "plain" || typeS == "string") 
        tt = MessageBubble::TextType::Plain;
    else  
        return;                   // 未知类型，忽略

    // ---- 4. 流式追加：append=true 时合并到最新气泡 ----
    const bool append = (j.contains("append") && j["append"].is_boolean())
        ? j["append"].get<bool>() : false;
    if (append && modeS == "stream" && !m_bubbles.isEmpty())
    {
        m_bubbles.last()->AppendContent(content);
        return;
    }

    // ---- 5. 确定锚点：消息自带 x/y 优先，否则用全局锚点 ----
    QPoint anchor = m_anchor;
    if (j.contains("x") && j.contains("y")
        && j["x"].is_number() && j["y"].is_number())
    {
        anchor = QPoint(j["x"].get<int>(), j["y"].get<int>());
    }
    else if (!m_anchorValid)
    {
        anchor = QGuiApplication::primaryScreen()->availableGeometry().center();
    }

    // ---- 6. 超出上限，移除最旧 ----
    while (m_bubbles.size() >= m_max)
        RemoveOldest();

    // ---- 7. 创建气泡 ----
    auto* bubble = new MessageBubble;   // 顶层窗口，无 parent
    bubble->SetTailTarget(anchor);
    if (modeS == "stream")
        bubble->StartStream(tt, content);
    else
        bubble->SetContent(tt, content);

    // 点击关闭 / 倒计时结束 → 统一走 Dismiss（淡出 + 从列表移除）
    connect(bubble, &MessageBubble::clicked, this,
        [this](MessageBubble* b) { Dismiss(b); });
    connect(bubble, &MessageBubble::dismissRequested, this,
        [this](MessageBubble* b) { Dismiss(b); });
    // 内容高度变化 → 重新布局（lambda 形式，规避重载歧义与默认参数问题）
    connect(bubble, &MessageBubble::geometryChanged, this,
        [this] { LayoutBubbles(false); });

    // ---- 8. 显示结束之后自动关闭（毫秒）----
    if (j.contains("auto_dismiss") && j["auto_dismiss"].is_number())
    {
        int ms = -1;
        try { ms = j["auto_dismiss"].get<int>(); }
        catch (const std::exception& e) { RuntimeLog_Error(e.what()); }

        if (ms >= 0)
            bubble->SetAutoDismiss(ms);
    }

    // ---- 可选：尾巴方向与位置 ----
    // "tail_side": "auto|top|bottom|left|right"，"tail_ratio": 0.0~1.0（可省，省=-1自动对准锚点）
    if (j.contains("tail_side") && j["tail_side"].is_string())
    {
        const std::string s = j["tail_side"].get<std::string>();
        MessageBubble::TailSide ts = MessageBubble::TailSide::Auto;
        if (s == "top")
            ts = MessageBubble::TailSide::Top;
        else if (s == "bottom")
            ts = MessageBubble::TailSide::Bottom;
        else if (s == "left")
            ts = MessageBubble::TailSide::Left;
        else if (s == "right")
            ts = MessageBubble::TailSide::Right;

        double ratio = -1.0;
        if (j.contains("tail_ratio") && j["tail_ratio"].is_number())
            ratio = j["tail_ratio"].get<double>();

        bubble->SetTail(ts, ratio);
    }

    m_bubbles.append(bubble);
    LayoutBubbles(false);
    bubble->ShowAnimated();   // 带入场动画显示；不想要动画就换回 bubble->show();

    // ---- 9. 可选 lifetime：从创建时算起，超时强制关闭 ----
    if (j.contains("lifetime") && j["lifetime"].is_number())
    {
        const int lifetime = j["lifetime"].get<int>();
        if (lifetime > 0)
            QTimer::singleShot(lifetime, this, [this, bubble] { Dismiss(bubble); });
    }
}

void BubbleManager::Dismiss(MessageBubble* b)
{
    if (!b || !m_bubbles.contains(b))
        return;
    m_bubbles.removeAll(b);

    auto* anim = new QPropertyAnimation(b, "windowOpacity");
    anim->setDuration(160);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    connect(anim, &QPropertyAnimation::finished, b, &QWidget::deleteLater);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    LayoutBubbles(true);   // 其余气泡平滑补位
    return;
}

void BubbleManager::RemoveOldest()
{
    if (!m_bubbles.isEmpty())
        Dismiss(m_bubbles.first());
}