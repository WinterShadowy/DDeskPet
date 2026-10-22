#include "SettingReusablePage.h"

// ========== NotificationWidget ==========
QDialog* NotificationWidget::s_dialog = nullptr;

NotificationWidget::NotificationWidget(const QString& text, unsigned int ms, unsigned int Width, unsigned int Height)
{
	setAttribute(Qt::WA_DeleteOnClose);   // 动画结束自毁
	setWindowFlags(Qt::FramelessWindowHint |
		Qt::WindowStaysOnTopHint |
		Qt::Tool);             // 不抢任务栏
	setFixedSize(QSize(Width, Height)); // 设置尺寸(w, h);

	auto* lab = new QLabel(text, this);
	lab->setAlignment(Qt::AlignCenter);
	lab->setStyleSheet("color:white;font:14px;");
	auto* lay = new QHBoxLayout(this);
	lay->addWidget(lab);
	lay->setContentsMargins(12, 0, 12, 0);

	setStyleSheet("background:#409eff;border-radius:6px;");
}

void NotificationWidget::execute()
{
	// 1. 屏幕坐标（保持不变）
	QRect scr = qApp->primaryScreen()->availableGeometry();
	int x0 = scr.right() + 1;
	int y0 = scr.top() + 60;
	move(x0, y0);
	show();

	auto* in = new QPropertyAnimation(this, "pos");
	in->setDuration(250);
	in->setStartValue(QPoint(x0, y0));
	in->setEndValue(QPoint(scr.right() - width() - 10, y0));
	in->start(QAbstractAnimation::DeleteWhenStopped);

	QTimer::singleShot(m_duration, this, [this]() {
		QRect r = qApp->primaryScreen()->availableGeometry();
		auto* out = new QPropertyAnimation(this, "pos");
		out->setDuration(250);
		out->setStartValue(pos());
		out->setEndValue(QPoint(r.right() + 1, y()));

		// 关键：动画结束立即销毁对象
		connect(out, &QPropertyAnimation::finished,
			this, &QObject::deleteLater);

		out->start(QAbstractAnimation::DeleteWhenStopped);
		});
}

void NotificationWidget::ShowNotification(const QString& qstr_content, unsigned int Width, unsigned int Height, int nTimeout)
{
	if(s_dialog == nullptr)
		return;
	 NotificationWidget* notifi = new NotificationWidget(qstr_content, nTimeout, Width, Height);
	 notifi->execute();
}

void NotificationWidget::installDialog(QDialog* dlg)
{
	s_dialog = dlg;
	return;
}

// ========== end ==========

namespace _SettingReusablePage {
	QLabel* _CreateDoubtPage(
		const QString qstr_title,
		const QString qstr_ToolTip,
		Qt::Alignment alignment,
		QWidget* parent)
	{
		QLabel* label = new QLabel(parent);
		label->setAlignment(alignment);
		label->setToolTip(qstr_ToolTip);
		if (qstr_title.isEmpty())
		{
			label->setText("label");
		}
		else
		{
			label->setText(qstr_title);
		}
		return label;
	}

}
// ========= end =========

// ========== class ReusablePage ==========

void ReusablePage::CreateNotificationWidget(const QString& text, unsigned int Width, unsigned int Height, int nTimeout)
{
	NotificationWidget::ShowNotification(text, Width, Height, nTimeout);
	return;
}

// ========= end ==========