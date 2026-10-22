#include "SettingVTabPageBar.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QScrollArea>
#include <QStackedWidget>
#include <QResizeEvent>
#include <QStyle>
#include <QGroupBox>

VTabPageBar::VTabPageBar(QWidget* parent)
    : QWidget(parent)
{
    // 左侧按钮区域
    this->m_btnContainer = new QWidget;
    this->m_btnLayout = new QVBoxLayout(m_btnContainer);
    this->m_btnLayout->setContentsMargins(0, 0, 0, 0);
    this->m_btnLayout->setSpacing(0);
    //this->m_btnContainer->setFixedWidth(150);

    this->m_btnGroup = new QButtonGroup(this);
    this->m_btnGroup->setExclusive(true);

    // 按钮过多时滚动
    this->m_scrollArea = new QScrollArea(this);
    this->m_scrollArea->setWidget(m_btnContainer);
    this->m_scrollArea->setWidgetResizable(true);
    this->m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->m_scrollArea->setFrameShape(QFrame::NoFrame);
    this->m_scrollArea->setMinimumWidth(120);
    this->m_scrollArea->setMaximumWidth(160);

    // 右侧页面栈
    this->m_pageStack = new QStackedWidget(this);

    // 主布局
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_scrollArea);
    mainLayout->addWidget(m_pageStack);

    this->m_nPreviousPageID = 0;
}

QVector<QWidget*> VTabPageBar::GetAllWidget() const
{
    return std::move(this->m_widgetInfoPool);
}

void VTabPageBar::AddItem(const QString& text, QWidget* page)
{
    if (!page) // 防止空页面
        return;

    // 创建按钮
    QPushButton* btn = new QPushButton(text, this);
    btn->setCheckable(true);
    btn->setFixedHeight(70);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_btnLayout->addWidget(btn);
    m_btnGroup->addButton(btn, m_pages.size());

    // 注册页面
    m_pages.append(page);
    m_pageStack->addWidget(page);

    // 默认选中第一个
    if (m_pages.size() == 1)
    {
        btn->setChecked(true);
        m_pageStack->setCurrentWidget(page);
    }

    // 点击事件直接转换页面id
    connect(m_btnGroup, QOverload<int>::of(&QButtonGroup::idClicked), this, &VTabPageBar::OnBtnClicked);
    int h = m_btnLayout->count() * btn->height();
    m_btnContainer->setFixedHeight(h);
    // 将页面信息添加到页面信息池
    AddItemInfoToWidgetPool(page);
}

void VTabPageBar::AddItem(const QString& text, QWidget* page, QString iconPath)
{
    if (!page) // 防止空页面
        return;

    // 创建按钮
    QPushButton* btn = new QPushButton(text, this);
    if(iconPath.isEmpty() == false)
        btn->setIcon(QIcon(iconPath));
    btn->setCheckable(true);
    btn->setFixedHeight(50);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_btnLayout->addWidget(btn);
    m_btnGroup->addButton(btn, m_pages.size());

    // 注册页面
    m_pages.append(page);
    m_pageStack->addWidget(page);

    // 默认选中第一个
    if (m_pages.size() == 1)
    {
        btn->setChecked(true);
        m_pageStack->setCurrentWidget(page);
    }

    // 点击事件直接转换页面id
    connect(m_btnGroup, QOverload<int>::of(&QButtonGroup::idClicked), this, &VTabPageBar::OnBtnClicked);
    int h = m_btnLayout->count() * btn->height();
    m_btnContainer->setFixedHeight(h);
    // 将页面信息添加到页面信息池
    AddItemInfoToWidgetPool(page);
}

void VTabPageBar::SetSideBarBorder(int width)
{
    // 右侧画线
    QString style = QString("QScrollArea { border-right: %1px solid #d0d0d0; }").arg(width);
    m_scrollArea->setStyleSheet(style);
}

void VTabPageBar::SetButtonSpacing(int nSpacing)
{
    m_btnLayout->setSpacing(nSpacing);
}

int VTabPageBar::GetClientHeight(const QWidget* w) const
{
    if (!w) 
        return 0;
    return w->height() - w->style()->pixelMetric(QStyle::PM_LayoutTopMargin) - w->style()->pixelMetric(QStyle::PM_LayoutBottomMargin);
}

void VTabPageBar::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
}

void VTabPageBar::UpdateGeometryToParent()
{
    if (!parentWidget()) 
        return;
    // 让侧边栏高度与父窗口一致
    setFixedHeight(parentWidget()->height());
}

void VTabPageBar::AddItemInfoToWidgetPool(QWidget* page)
{
    this->m_widgetInfoPool.push_back(page);
    return;
}

void VTabPageBar::OnBtnClicked(int id)
{
    if (id >= 0 && id < this->m_pages.size() && id != this->m_nPreviousPageID)
    {
        this->m_nPreviousPageID = id;
        this->m_pageStack->setCurrentWidget(this->m_pages.at(id));
    }
}

void VTabPageBar::SetAllButtonStyle(CALLBACK_SetToolButtonStyle cb_Func_SetToolButtonStyle)
{
    for (int i = 0; i < m_btnLayout->count(); i++)
	{
		QPushButton* btn = (QPushButton*)m_btnLayout->itemAt(i)->widget();
		cb_Func_SetToolButtonStyle(btn);
	}
}

void VTabPageBar::SetSideHeight(int nHeight)
{
    setFixedHeight(nHeight);
}

bool VTabPageBar::eventFilter(QObject* obj, QEvent* ev)
{
    return QWidget::eventFilter(obj, ev);
}