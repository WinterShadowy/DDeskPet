#pragma once

#include <QWidget>
#include <QLabel>

#include <QPropertyAnimation>
#include <QTimer>
#include <QApplication>
#include <QScreen>
#include <QString>
#include <QPushButton>

#include <QHBoxLayout>
#include <QVBoxLayout>

// 提示窗口
class NotificationWidget : public QWidget
{
	Q_OBJECT
public:
	//static NotificationWidget& Instance();
    static void ShowNotification(const QString& qstr_content, unsigned int Width = 150, unsigned int Height = 40, int nTimeout = 3000);
	static void installDialog(QDialog* dlg);
private:
	NotificationWidget() = default;
	NotificationWidget(const QString& text, unsigned int ms, unsigned int Width, unsigned int Height);
	~NotificationWidget() = default;
	// 阻止拷贝构造和赋值操作
	NotificationWidget(const NotificationWidget&) = delete;
	NotificationWidget& operator=(const NotificationWidget&) = delete;
	void execute();
private:
	int m_duration;
	static QDialog* s_dialog;
};

class ReusablePage
{
public:
	explicit ReusablePage() = default;
	~ReusablePage() = default;

	// 提示窗口
	static void  CreateNotificationWidget(const QString& text, unsigned int Width = 150, unsigned int Height = 40, int nTimeout = 3000);
	// 配置操作

};

namespace _SettingReusablePage {

	// 创建疑问提示页面
	QLabel* _CreateDoubtPage(
		const QString qstr_title,
		const QString qstr_ToolTip,
		Qt::Alignment alignment,
		QWidget* parent = nullptr);
}
