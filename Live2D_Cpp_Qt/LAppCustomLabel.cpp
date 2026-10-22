#include "LAppCustomLabel.h"
#include <QFontMetrics>

MarqueeLabel::MarqueeLabel(QWidget* parent, int speed)
    : QLabel(parent), m_speed(speed), m_pos(0), m_needScroll(false)
{
    setWordWrap(false);
    setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, [this] {
        if (m_needScroll) 
        {
            m_pos -= m_speed;
            update();
        }
    });
}

void MarqueeLabel::setSpeed(int px) 
{ 
    m_speed = px; 
}

void MarqueeLabel::paintEvent(QPaintEvent* e)
{
    const int textW = fontMetrics().horizontalAdvance(text());
    const int labW = width();
    m_needScroll = (textW > labW);

    if (!m_needScroll) // 正常显示
    {                       
        QLabel::paintEvent(e);
        return;
    }
    QPainter p(this);
    p.setClipRect(rect());  // 防止画出边界
    p.drawText(m_pos, (height() - fontMetrics().height()) / 2 + fontMetrics().ascent(), text());
    if (m_pos + textW < 0) m_pos = labW;       // 循环
}

void MarqueeLabel::showEvent(QShowEvent* e)
{
    QLabel::showEvent(e);
    if (!m_timer->isActive()) 
        m_timer->start(25);
}

void MarqueeLabel::hideEvent(QHideEvent* e)
{
    QLabel::hideEvent(e);
    m_timer->stop();
}