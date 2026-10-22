// LAppModelMessage.h

// 模拟模型发出的消息

#pragma once
#pragma disable : 4996

#include "LAppDefine.hpp"
#include "LAppSystemInfo.h"
#include "ConvenientTool.hpp"
#include "nlohmann/json.hpp"

#include <QString>
#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QPainter>
#include <QProgressBar>
#include <QDialog>
#include <QTextDocument>
#include <QPoint>
#include <QPainterPath>
#include <QMouseEvent>
#include <QObject>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QParallelAnimationGroup>
#include <QEasingCurve>
#include <QFont>
// ModelWindowPromptWindow效果实现
class ScanLineWidget : public QWidget
{
	Q_OBJECT
public:
	/* 构造后自动开始动画，动画结束自动销毁 */
	explicit ScanLineWidget(QLabel* label, QWidget* parent = nullptr);
private:
	void paintEvent(QPaintEvent*) override;
	QTimer m_timer;
	int m_y = 0;          // 当前扫线 y 坐标
	int m_h = 0;          // 每次刷新的矩形高度
	QColor m_bgColor;     // 父窗口底色，用来“盖住”文字
};

class ModelWindowPromptWindow : QWidget {
public:
	enum PopWindowStyle {
		Style_V_1_0 = 0,
		Style_V_1_1 = 1,
		Style_V_1_2 = 2,
		Style_V_1_3 = 3,

		Style_V_2_1 = 21,
		Style_V_2_2 = 22,
		Style_V_2_3 = 23,

		Style_V_3_0 = 30
	};
	explicit ModelWindowPromptWindow() = default;
	explicit ModelWindowPromptWindow(QWidget* parent = nullptr);
	~ModelWindowPromptWindow() = default;
	
	static void SetWindowNowPosition(int x, int y, int width, int height);

	// 自定义的提示页面
	static void ShowMessageOnModelWindow(const int style_index, const QString& Message, const unsigned int timeout_ms, bool stream);
	static void ShowMessageOnModelWindow(PopWindowStyle popWindowStyle, const QString& qstr_Text, const QJsonObject& qjson_Style);
	static void ShowMessageOnModelWindow_CustomizedPage(QWidget* pCustomizedPageWidegt, QPushButton* pCloseButton, unsigned int timeout_ms);

private:
	static void ShowMessageOnModelWindow_V_1_0(QString qstr_Messages, unsigned int timeout_ms);

	// style-v1.1-已经放弃使用
	__declspec(deprecated("This function(ShowMessageOnModelWindow_V_1_1) has been deprecated."))
	static void ShowMessageOnModelWindow_V_1_1(QString qstr_Messages, unsigned int timeout_ms);

	// style-v1.2-已经放弃使用
	__declspec(deprecated("This function(ShowMessageOnModelWindow_V_1_2) has been deprecated."))
		static void ShowMessageOnModelWindow_V_1_2(const QString& qstr_Messages, unsigned int timeout_ms);

	static void ShowMessageOnModelWindow_V_1_3(const QString& qstr_Messages, unsigned int timeout_ms);

	static void ShowMessageOnModelWindow_V_2_0(const QString& qstr_Messages, unsigned int timeout_ms, bool stream);
	static void ShowMessageOnModelWindow_V_2_1(const QString& qstr_Messages, unsigned int timeout_ms, bool stream);
	__declspec(deprecated("This function(ShowMessageOnModelWindow_V_2_2) has been deprecated."))
		static void ShowMessageOnModelWindow_V_2_2(const QString& qstr_Messages, unsigned int timeout_ms, bool stream);

	static void ShowMessageOnModelWindow_V_2_3(const QString& qstr_Messages, unsigned int timeout_ms, bool stream);

	static void ShowMessageOnModelWindow_V_3_0(const QString& qstr_Messages, QJsonObject qjson_style);
	static void ComputePosition();
	static void MoveMessageWindow();   // 当窗口移动时，重新计算窗口位置

private:
	static int s_x;      // 主窗口左上角
	static int s_y;
	static int s_width;  // 主窗口尺寸
	static int s_height;

	static QWidget* s_popup; // 当前正在显示的提示框
};

// ======================== 单个漫画气泡 ========================
class MessageBubble : public QWidget
{
    Q_OBJECT
public:
    enum class TextType {
        Plain = 0,  // 纯文本
        Markdown,   // Markdown
        Html        // HTML
    };

    // 尾巴附着在哪条边上
    enum class TailSide {
        Auto = 0,   // 自动：根据锚点方位决定（默认，保持现有行为）
        Top,        // 顶边，尖朝上
        Bottom,     // 底边，尖朝下
        Left,       // 左边，尖朝左
        Right       // 右边，尖朝右
    };

    // 设置尾巴：side = 哪条边；ratio = 沿边位置 0.0~1.0（0=左/上端，1=右/下端）
    // ratio 传 -1 表示沿边位置也自动对准锚点
    void SetTail(TailSide side, double ratio = -1.0);

    explicit MessageBubble(QWidget* parent = nullptr);

    // 直接显示全部内容
    void SetContent(TextType type, const QString& text);

    // 流式：打字机效果显示 content；之后可用 AppendContent 追加新分片
    void StartStream(TextType type, const QString& content);
    void AppendContent(const QString& chunk);
    bool IsStreaming() const;

    // 气泡尾巴指向的全局坐标（Live2D 角色嘴边）
    void SetTailTarget(const QPoint& globalPos);

    // 显示结束后 ms 毫秒自动请求关闭；< 0 表示不自动关闭（默认）
    void SetAutoDismiss(int ms);

    // 带入场动画的显示（替代 show()）
    void ShowAnimated();
    // 平滑移动到目标位置（堆叠补位用）
    void SlideTo(const QRect& target);

signals:
    void clicked(MessageBubble* self);             // 用户点击了气泡
    void geometryChanged();                        // 尺寸变化，通知管理者重新布局
    void displayFinished(MessageBubble* self);     // 内容完整显示完毕
    void dismissRequested(MessageBubble* self);    // 倒计时结束，请求管理者销毁自己

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void RebuildDocument();     // 按 m_type / 当前可见文本重建 QTextDocument
    void UpdateSize();          // 按文档大小调整窗口尺寸
    QRect BodyRect() const;     // 圆角主体区域（不含尾巴）
    void MaybeStartDismiss();   // 若已设置自动关闭，则启动倒计时

    TailSide ResolvedSide() const;
private:
    TailSide m_tailSide = TailSide::Auto;
    double   m_tailRatio = -1.0;   // -1 = 自动对准锚点

    

    int    m_autoDismissMs = -1;    // 自动关闭毫秒数，-1 = 不启用
    QTimer m_dismissTimer;

    TextType m_type = TextType::Plain;
    QString  m_fullText;            // 完整文本
    int      m_visibleLen = 0;      // 当前可见字符数（流式用）
    bool     m_streaming = false;
    bool     m_entered = false;  // 入场动画是否已播放过

    QTextDocument m_doc;
    QTimer m_typeTimer;
    QPoint m_tailGlobal;            // 尾巴指向（全局坐标）

    int m_maxWidth = 300;         // 文本最大宽度
    int m_tailHeight = 18;          // 尾巴高度
    int m_hPad = 14, m_vPad = 10;   // 文字内边距
};

// ======================== 气泡管理器（单例） ========================
class BubbleManager : public QObject
{
    Q_OBJECT
public:
    static BubbleManager& instance();

    void SetMaxBubbles(int n);      // 同屏最多弹窗数，默认 3
    int  MaxBubbles() const;

    // 以锚点为基准向上堆叠；animate = true 时气泡平滑滑动到位
    void LayoutBubbles(bool animate = false);

public slots:
    // 设置弹窗锚点（角色嘴边，全局屏幕坐标）——供外部信号连接
    void SetAnchor(const QPoint& pos);
    void SetAnchor(int x, int y);

    // 消息入口：nlohmann::json 字符串 或 QJsonObject（内部统一转 nlohmann 解析）
    void ShowMessage(const QString& jsonText);
    void ShowMessage(const QJsonObject& jsonObj);

private:
    explicit BubbleManager(QObject* parent = nullptr);
    ~BubbleManager() override;

    void HandleJson(const nlohmann::json& j);
    void Dismiss(MessageBubble* b);    // 淡出并销毁
    void RemoveOldest();

private:

    QVector<MessageBubble*> m_bubbles;
    QPoint m_anchor;
    bool   m_anchorValid = false;
    int    m_max = 3;
};