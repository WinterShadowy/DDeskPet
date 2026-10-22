#pragma once
#include <QObject>
#include <QPoint>
#include <windows.h>
#include <QThread>

class GlobalMouseHook : public QObject
{
    Q_OBJECT
public:
    static GlobalMouseHook& instance();
    bool install();       // 安装钩子
    void uninstall();     // 卸载钩子

signals:
    void mouseMove(const QPoint& globalPos);
    void mousePress(Qt::MouseButton btn, const QPoint& globalPos);
    void mouseRelease(Qt::MouseButton btn, const QPoint& globalPos);
    void wheel(int delta, const QPoint& globalPos);

private:
    GlobalMouseHook() = default;
    ~GlobalMouseHook();
    static LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam);

private:
    QThread* m_thread = nullptr;
};