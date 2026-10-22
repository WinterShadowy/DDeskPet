#pragma once

#include <QWidget>
#include <QOpenGLWidget>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QPoint>
#include <QMenu>
#include <QMouseEvent>
#include <QAction>
#include <cmath>

#include "Live2D_Cpp_Qt.h"
#include "MouseHook.h"
#include "Configuration.h"
#include "CubismWindowStyle.h"
#include "LAppModelMessage.h"

class GLCore : public QOpenGLWidget
{
	Q_OBJECT
public:
	GLCore(QWidget* parent = nullptr);
	~GLCore();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void closeEvent(QCloseEvent* event) override;
	void updateWindow();
	void moveEvent(QMoveEvent* event) override;

	// 处理鼠标事件
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	// 判断鼠标是否在窗口内
	bool underMouse_WinAPI();

private:
	// 检测透明鼠标 设置穿透
	void checkTransparencyMousePos_V4(const QPoint& screenPos);
	void updateGaze();             // 定时器槽：让注视点平滑追踪目标
	void updateBubbleAnchor();     // 气泡锚点平滑跟随

private slots:
	// 全局鼠标钩子
	void onMouseMove(const QPoint& globalPos);
	void onWindowMoved(int x, int y);
	
private:
	// 刷新相关
	QTimer* timer_flush_window;
	QMouseEvent* m_event;
	
	// 鼠标按钮判断
	bool b_IsLeftButton = false;
	bool b_IsRightButton = false;
	bool b_IsUnderMouse = false;
	QPoint point_MouseCurButton;
	QPoint m_lastMousePos;  // 添加鼠标位置缓存

	// 窗口移动 - 此部分与文本窗口相关
	QPointF m_anchorTarget;             // 锚点目标（全局坐标）
	QPointF m_anchorCurrent;            // 锚点当前值
	bool    m_anchorInit = false;
	double  m_anchorFollow = 0.18;      // 跟随系数：越小惯性越强、拖尾越明显

	QPointF m_gazeTarget;          // 注视目标点（由鼠标事件更新）
	QPointF m_gazeCurrent;         // 注视当前点（定时器缓动）
	bool    m_gazeInit = false;
	QTimer* m_gazeTimer = nullptr;

	// 可调参数
	double m_gazeGain = 0.8;       // 整体跟随强度，0.5 = 转一半，1.0 = 完全跟随
	double m_gazeSmooth = 0.12;    // 每帧走剩余距离的比例，越小越"肉"
};

class ModelWindowAgent
{
public:
	// 设置托盘实例
	void SetGLCoreInstance(GLCore* p_gl_core);

	// 修改窗口的大小
	void UpdateModelWindowSize(int width, int height);

	void LoadLive2DModel();

	bool IsVisible();

	void SetVisible(bool isVisible);

	static ModelWindowAgent& Instance();

private:
	ModelWindowAgent();
	~ModelWindowAgent();

	ModelWindowAgent(const ModelWindowAgent&) = delete;
	ModelWindowAgent& operator=(const ModelWindowAgent&) = delete;

private:

	GLCore* m_gl_core = nullptr;
	std::vector <GLCore*> m_gl_core_vec;
};