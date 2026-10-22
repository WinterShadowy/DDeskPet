#pragma once
#include <QWidget>
#include <QVector>

QT_BEGIN_NAMESPACE
class QPushButton;
class QVBoxLayout;
class QButtonGroup;
class QScrollArea;
class QStackedWidget;
QT_END_NAMESPACE

typedef void(*CALLBACK_SetToolButtonStyle)(QPushButton* btn);

class VTabPageBar : public QWidget
{
    Q_OBJECT
public:
    explicit VTabPageBar(QWidget* parent = nullptr);
    
    QVector<QWidget*> GetAllWidget() const;
    
    void AddItem(const QString& text, QWidget* page);   // 左侧按钮

    void AddItem(const QString& text, QWidget* page, QString iconPath);
    void SetSideBarBorder(int width = 1);               // 右侧边框宽度
    void SetButtonSpacing(int nSpacing);                 // 按钮间距
    int  GetClientHeight(const QWidget* w) const;       // 取可用高度
    
    // 设置Button样式
    void SetAllButtonStyle(CALLBACK_SetToolButtonStyle cb_Func_SetToolButtonStyle);
	void SetSideHeight(int nHeight);   // 设置侧边栏高度
	bool eventFilter(QObject* obj, QEvent* ev) override;  // 监视父窗口大小变化
protected:
    void resizeEvent(QResizeEvent* event) override;
    
private:
    void UpdateGeometryToParent();   // 窗口高度变化
    void AddItemInfoToWidgetPool(QWidget* page);

private slots:
    void OnBtnClicked(int id);

private:
    QVBoxLayout* m_btnLayout = nullptr;
    QButtonGroup* m_btnGroup = nullptr;
    QScrollArea* m_scrollArea = nullptr;
    QWidget* m_btnContainer = nullptr;
    QStackedWidget* m_pageStack = nullptr;
    QVector<QWidget*> m_pages;

    int m_nPreviousPageID;
    QVector<QWidget*> m_widgetInfoPool;
};