#pragma once

#pragma disable : 4996

#include <QLabel>
#include <QTimer>
#include <QPainter>

class MarqueeLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MarqueeLabel(QWidget* parent = nullptr, int speed = 2);
    void setSpeed(int px);

protected:
    void paintEvent(QPaintEvent* e) override;
    void showEvent(QShowEvent* e) override;
    void hideEvent(QHideEvent* e) override;

private:
    void startTimer();
    int  m_speed;
    int  m_pos;
    bool m_needScroll;
    QTimer* m_timer;
};