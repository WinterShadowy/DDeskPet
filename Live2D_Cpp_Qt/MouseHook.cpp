#include "MouseHook.h"
#include <QCursor>
#include <QDebug>

static HHOOK g_hook = nullptr;
GlobalMouseHook& GlobalMouseHook::instance()
{
    static GlobalMouseHook inst;
    return inst;
}

bool GlobalMouseHook::install()
{
    if (g_hook) 
        return true;

    // 启动专用线程
    m_thread = new QThread;
    m_thread->start();

    // 把单例挪到工作线程：之后 mouseProc 里 emit 的信号
    // 自动以队列方式投递给 GUI 线程的槽
    this->moveToThread(m_thread);

    // 在工作线程里安装钩子（关键：钩子属于调用 SetWindowsHookEx 的线程）
    bool ok = false;
    QMetaObject::invokeMethod(this, [&ok] {
        g_hook = SetWindowsHookExW(WH_MOUSE_LL, mouseProc,
            GetModuleHandle(nullptr), 0);
        ok = (g_hook != nullptr);
        }, Qt::BlockingQueuedConnection);

    return ok;
}

void GlobalMouseHook::uninstall()
{
    if (!g_hook) 
        return;
    QMetaObject::invokeMethod(this, [] {
        if (g_hook) 
        { 
            UnhookWindowsHookEx(g_hook); 
            g_hook = nullptr; 
        }
        }, Qt::BlockingQueuedConnection);
}

GlobalMouseHook::~GlobalMouseHook()
{
    uninstall();
    if (m_thread) 
    {
        m_thread->quit();
        m_thread->wait();
        m_thread->deleteLater();
    }
}

LRESULT CALLBACK GlobalMouseHook::mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION) 
    {
        MSLLHOOKSTRUCT* ms = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
        QPoint pt(ms->pt.x, ms->pt.y);

        // 消息选择器
        switch (static_cast<ULONG_PTR>(wParam))
        {
        case WM_MOUSEMOVE:
            {
                static DWORD lastTick = 0;
                const DWORD now = GetTickCount();
                // 约 60 次/秒，人眼完全够用，如果太快会导致消息丢失/积压
                if (now - lastTick < 16)    
                    break;
                lastTick = now;
                emit instance().mouseMove(pt);
            }
            break;
        case WM_LBUTTONDOWN:
            emit instance().mousePress(Qt::LeftButton, pt);
            break;
        case WM_LBUTTONUP:
            emit instance().mouseRelease(Qt::LeftButton, pt);
            break;
        case WM_RBUTTONDOWN:
            emit instance().mousePress(Qt::RightButton, pt);
            break;
        case WM_RBUTTONUP:
            emit instance().mouseRelease(Qt::RightButton, pt);
            break;
        case WM_MOUSEWHEEL:
            {
                const int delta = GET_WHEEL_DELTA_WPARAM(ms->mouseData);
                emit instance().wheel(delta, pt);
                break;
            }
        }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}