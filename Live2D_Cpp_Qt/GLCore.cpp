// 请勿修改顺序

#include "LAppDelegate.hpp"
#include "LAppView.hpp"
#include "LAppPal.hpp"
#include "LAppLive2DManager.hpp"
#include "LAppDefine.hpp"
#include "GLCore.h"
#include "Configuration.h"

#include <QScreen>
#include <QGuiApplication>

GLCore::GLCore(QWidget* parent)
	: QOpenGLWidget(parent)
{
	//this->setAttribute(Qt::WA_DeleteOnClose);			// 自动释放内存
	this->setAttribute(Qt::WA_TranslucentBackground);   // 设置窗口透明
	this->setWindowFlag(Qt::FramelessWindowHint);    // 设置窗口为无边框窗口
    if (Configuration::GetIns_FunctionWindowsPageConfig().IsModelWindowAllwaysOnTop())
    {
        this->setWindowFlag(Qt::WindowStaysOnTopHint);    // 设置窗口置顶
    }
	this->setWindowFlag(Qt::Tool);    // 设置窗口为工具

	this->timer_flush_window = new QTimer();
	connect(this->timer_flush_window, &QTimer::timeout, this, &GLCore::updateWindow);   // 更新窗口
    connect(this->timer_flush_window, &QTimer::timeout, this, &GLCore::updateGaze);     // 更新头部
    connect(this->timer_flush_window, &QTimer::timeout, this, &GLCore::updateBubbleAnchor);  // 更新泡泡

	this->timer_flush_window->start(static_cast<int>((1.0 / 60) * 1000));
    this->setMouseTracking(true); // 启用鼠标追踪
    
    //SetMousePenetration_GlobalWindow();
    if (Configuration::GetIns_FunctionWindowsPageConfig().GetMouseMoveFollowMode() == MOUSE_VIEW_TOUCHESMOVED_GLOBAL_SCALE)
    {
        // 连接鼠标移动信号到窗口的槽函数
        connect(&GlobalMouseHook::instance(), &GlobalMouseHook::mouseMove, this, &GLCore::onMouseMove);
    }

	// 读取上次窗口位置并移动窗口
    QPoint lastWindowPos = BaseConfig::instance().GetWindowPosition();
    if(lastWindowPos.x() != -1000000 && lastWindowPos.y() != -1000000)
    {
        this->move(lastWindowPos);
        //ModelWindowPromptWindow::SetWindowNowPosition(this->pos().x(), this->pos().y(), this->width(), this->height());
    }
}

GLCore::~GLCore()
{
	this->timer_flush_window->stop();
    LAppDelegate::ReleaseInstance();
}

void GLCore::initializeGL()
{
	// 初始化
	LAppDelegate::GetInstance()->Initialize(this);
}

void GLCore::resizeGL(int w, int h)
{
	LAppDelegate::GetInstance()->ReSize(w, h);

    // 偏移按新尺寸重算
    onWindowMoved(this->pos().x(), this->pos().y());
}

void GLCore::paintGL()
{
	// 渲染Live2D模型
	LAppDelegate::GetInstance()->Update();
}

void GLCore::updateWindow()
{
    this->update();
    this->b_IsUnderMouse = this->underMouse_WinAPI();
    // 替换为安全的位置检测
    if (b_IsUnderMouse) 
    {
        QPoint currentPos = QCursor::pos();
        // 仅当鼠标位置变化时检测
        if (currentPos != m_lastMousePos)
        {
            checkTransparencyMousePos_V4(currentPos);
            m_lastMousePos = currentPos;
        }
    }
}

void GLCore::moveEvent(QMoveEvent* event)
{
    onWindowMoved(event->pos().x(), event->pos().y());
    QOpenGLWidget::moveEvent(event);
}

void GLCore::closeEvent(QCloseEvent* event)
{
    QApplication::quit();  // 显式退出事件循环
    event->accept();  // 确保关闭事件被接受
}

void GLCore::checkTransparencyMousePos_V4(const QPoint& screenPos)
{
    // 将屏幕坐标转换为窗口客户区坐标
    QPoint windowPos = this->mapFromGlobal(screenPos);

    // 检查坐标是否在窗口内
    if (!this->rect().contains(windowPos)) 
    {
        return;
    }

    // 考虑设备像素比并转换为OpenGL物理坐标
    makeCurrent(); // 确保激活了OpenGL上下文
    qreal dpr = this->devicePixelRatioF();
    int physX = windowPos.x() * dpr;
    int physY = windowPos.y() * dpr;

    // 转换为OpenGL坐标系（Y轴翻转）
    int physHeight = this->height() * dpr;
    int glY = physHeight - physY - 1;

    // 读取鼠标位置像素的RGBA值
    unsigned char rgba[4];
    glReadPixels(physX, glY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

    HWND hwnd = (HWND)winId();
    LONG ex = GetWindowLong(hwnd, GWL_EXSTYLE);

    // Alpha值大于0 不透明
    if (rgba[3] > 0) 
    {
        ex &= ~WS_EX_TRANSPARENT; // 禁用穿透
    }
    else
    {
        ex |= WS_EX_TRANSPARENT; // 启用穿透
    }

    SetWindowLong(hwnd, GWL_EXSTYLE, ex);
    doneCurrent();
}

void GLCore::updateGaze()
{
    if (!this->m_gazeInit)
        return;

    const QPointF before = this->m_gazeCurrent;

    // 每帧走剩余距离的 m_gazeSmooth 比例
    // 数学上是无超调的指数收敛：快而不抖
    this->m_gazeCurrent += (this->m_gazeTarget - this->m_gazeCurrent) * this->m_gazeSmooth;

    // 变化小于 0.5 像素不再调用，防止模型微颤
    if (std::hypot(this->m_gazeCurrent.x() - before.x(),
        this->m_gazeCurrent.y() - before.y()) < 0.5)
        return;

    LAppDelegate::GetInstance()->GetView()->OnTouchesMoved(
        static_cast<float>(this->m_gazeCurrent.x()),
        static_cast<float>(this->m_gazeCurrent.y()));
}

void GLCore::updateBubbleAnchor()
{
    if (!this->m_anchorInit)
        return;

    const QPointF diff = this->m_anchorTarget - this->m_anchorCurrent;

    // 已到位：对齐一次后不再打扰管理器，避免每帧无效布局
    if (diff.manhattanLength() < 0.5) 
    {
        if (this->m_anchorCurrent != this->m_anchorTarget)
        {
            this->m_anchorCurrent = this->m_anchorTarget;
            BubbleManager::instance().SetAnchor(this->m_anchorCurrent.toPoint());
        }
        return;
    }

    // 指数趋近：快速启动、平稳减速，拖动越快跟得越快
    this->m_anchorCurrent += diff * this->m_anchorFollow;
    BubbleManager::instance().SetAnchor(this->m_anchorCurrent.toPoint());
    return;
}

void GLCore::onWindowMoved(int x, int y)
{
    const QPointF mouthLocal(width() * 1.0, height() * 0.35);

    this->m_anchorTarget = QPointF(x, y) + mouthLocal;
#if defined(_DEBUG) || defined(DEBUG)
    qDebug() << "x: " << this->m_anchorTarget.x() << ", y: " << this->m_anchorTarget.y();
#endif
    // 第一次直接对齐，避免启动时气泡从屏幕角落飞过来
    if (!this->m_anchorInit)
    {
        this->m_anchorInit = true;
        this->m_anchorCurrent = this->m_anchorTarget;
        BubbleManager::instance().SetAnchor(this->m_anchorCurrent.toPoint());
    }
    return;
}

void GLCore::mousePressEvent(QMouseEvent* event)
{
    // 调用Live2D SDK的触摸开始事件处理
    int x = event->position().x();
    int y = event->position().y();
    LAppDelegate::GetInstance()->GetView()->OnTouchesMoved(x, y);

    if (event->button() == Qt::LeftButton)
    {
        this->b_IsLeftButton = true;
        this->point_MouseCurButton = event->pos(); // 记录拖拽起始点
    }
    if (event->button() == Qt::RightButton)
    {
        // 处理右键等其他按钮
        this->b_IsRightButton = true;
    }
    // 如果事件未被接受，调用基类实现
    if (!event->isAccepted())
        QOpenGLWidget::mousePressEvent(event);
}

void GLCore::mouseReleaseEvent(QMouseEvent* event)
{
	int x = event->position().x();
	int y = event->position().y();
	LAppDelegate::GetInstance()->GetView()->OnTouchesMoved(x, y);

	if(event->button() == Qt::LeftButton)
	{
		this->b_IsLeftButton = false;
	}
	if(event->button() == Qt::RightButton)
	{
		this->b_IsRightButton = false;
	}
}

void GLCore::mouseMoveEvent(QMouseEvent* event)
{
    if(Configuration::GetIns_FunctionWindowsPageConfig().GetMouseMoveFollowMode() == MOUSE_VIEW_TOUCHESMOVED_LOCALIZED_SCALE)
    {
        int x = event->position().x();
        int y = event->position().y();
        LAppDelegate::GetInstance()->GetView()->OnTouchesMoved(x, y);
    }
    if(this->b_IsUnderMouse)
    {
        int x = event->position().x();
        int y = event->position().y();
        LAppDelegate::GetInstance()->GetView()->OnTouchesMoved(x, y);
    }

	if (this->b_IsLeftButton)
	{
        ModelWindowPromptWindow::SetWindowNowPosition(this->pos().x(), this->pos().y(), this->width(), this->height());
		this->move(event->pos() - this->point_MouseCurButton + this->pos());
        BaseConfig::instance().SetWindowPosition(event->pos() - this->point_MouseCurButton + this->pos());
	}
}

bool GLCore::underMouse_WinAPI()
{
    // 窗口不可见或最小化
    if (!isVisible() || window()->isMinimized())
    {
        return false;
    }

    HWND hwnd = reinterpret_cast<HWND>(winId());
    RECT winRect;
    if (!GetWindowRect(hwnd, &winRect))
    {
        return false;
    }

    POINT cursorPos;
    if (!GetCursorPos(&cursorPos))
    {
        return false;
    }

    // 鼠标是否在窗口矩形内
    const bool inWindow = (cursorPos.x >= winRect.left &&
        cursorPos.x < winRect.right &&
        cursorPos.y >= winRect.top &&
        cursorPos.y < winRect.bottom);

    // 不在窗口矩形内
    if (!inWindow)
    {
        return false;
    }

    // 获取窗口样式
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    bool isTransparent = (exStyle & WS_EX_TRANSPARENT);

    // 透明穿透窗口
    if (isTransparent)
    {
        return true;
    }

    // 非穿透窗口，确保窗口未被其他窗口遮挡
    HWND hUnderMouse = WindowFromPoint(cursorPos);
    return (hUnderMouse == hwnd) || IsChild(hwnd, hUnderMouse);
}

void GLCore::onMouseMove(const QPoint& globalPos)
{
    if (this->b_IsUnderMouse)
    {
        return;
    }

    const QPointF center = QRectF(rect()).center();   // 窗口中心（本地坐标）
    const QPointF local = mapFromGlobal(globalPos);  // 鼠标 → 本地坐标

    QPointF offset = local - center;

    // ---- 幅度压缩：软上限曲线 ----
    // 近处 ≈ 线性跟随；远处趋近 maxR 后不再增加
    // 效果：鼠标甩到屏幕角落，头部也只是"看向那边"，不会转到极限
    const double dist = std::hypot(offset.x(), offset.y());
    const double maxR = qMin(width(), height()) * 0.9;   // 最大响应半径
    if (dist > 1e-3) {
        const double soft = maxR * (1.0 - std::exp(-dist / maxR));
        offset *= (soft / dist);
    }

    m_gazeTarget = center + offset * m_gazeGain;

    // 第一次调用时对齐，避免模型启动瞬间从 (0,0) 弹过去
    if (!m_gazeInit) 
    {
        m_gazeCurrent = m_gazeTarget;
        m_gazeInit = true;
    }
    return;
}

void ModelWindowAgent::SetGLCoreInstance(GLCore* p_gl_core)
{
    this->m_gl_core = p_gl_core;
    return;
}

void ModelWindowAgent::UpdateModelWindowSize(int width, int height)
{
    // 判断是否有系统托盘实例，如果没有则直接返回
    if (this->m_gl_core == nullptr)
    {
        return;
    }
    this->m_gl_core->resize(width, height);
    return;
}

void ModelWindowAgent::LoadLive2DModel()
{
    if(this->m_gl_core != nullptr)
	{
        this->m_gl_core->hide();
	}
    this->m_gl_core = new GLCore();
    this->m_gl_core->show();
    this->m_gl_core->resize(Configuration::GetIns_FunctionWindowsPageConfig().GetDefaultModelWindowHeight(),
        Configuration::GetIns_FunctionWindowsPageConfig().GetDefaultModelWindowWidth());
    QPoint currentPos = this->m_gl_core->pos();
    ModelWindowPromptWindow::SetWindowNowPosition(currentPos.x(), currentPos.y(), this->m_gl_core->width(), this->m_gl_core->height());
    this->m_gl_core_vec.push_back(this->m_gl_core);
}

bool ModelWindowAgent::IsVisible()
{
    return this->m_gl_core->isVisible();
}

void ModelWindowAgent::SetVisible(bool isVisible)
{
    if(isVisible)
    {
        this->m_gl_core->show();
    }
    else
    {
        this->m_gl_core->hide();
	}
    return;
}

ModelWindowAgent& ModelWindowAgent::Instance()
{
    static ModelWindowAgent ins;
    return ins;
}

ModelWindowAgent::ModelWindowAgent()
{
}

ModelWindowAgent::~ModelWindowAgent()
{
    for (auto it : this->m_gl_core_vec)
    {
        if(it != nullptr)
        {
            delete it;
			it = nullptr;
		}
    }
}
