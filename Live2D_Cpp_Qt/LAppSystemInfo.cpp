#include "LAppSystemInfo.h"
 
#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QAtomicInt>
#include <QReadWriteLock>

#ifdef Q_OS_WIN
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#else
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#endif

namespace {

    double calcCpuLoad()
    {
#ifdef Q_OS_WIN
        static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
        static int init = []() {
            FILETIME sysIdle, sysKernel, sysUser;
            GetSystemTimes(&sysIdle, &sysKernel, &sysUser);
            lastCPU.QuadPart = sysIdle.dwLowDateTime | (uint64_t)sysIdle.dwHighDateTime << 32;
            lastSysCPU.QuadPart = sysKernel.dwLowDateTime | (uint64_t)sysKernel.dwHighDateTime << 32;
            lastUserCPU.QuadPart = sysUser.dwLowDateTime | (uint64_t)sysUser.dwHighDateTime << 32;
            return 1;
        }();

        (void)init;

        FILETIME sysIdle, sysKernel, sysUser;
        GetSystemTimes(&sysIdle, &sysKernel, &sysUser);
        ULARGE_INTEGER sysIdle_new, sysKernel_new, sysUser_new;
        sysIdle_new.QuadPart = sysIdle.dwLowDateTime | (uint64_t)sysIdle.dwHighDateTime << 32;
        sysKernel_new.QuadPart = sysKernel.dwLowDateTime | (uint64_t)sysKernel.dwHighDateTime << 32;
        sysUser_new.QuadPart = sysUser.dwLowDateTime | (uint64_t)sysUser.dwHighDateTime << 32;

        uint64_t deltaIdle = sysIdle_new.QuadPart - lastCPU.QuadPart;
        uint64_t deltaKernel = sysKernel_new.QuadPart - lastSysCPU.QuadPart;
        uint64_t deltaUser = sysUser_new.QuadPart - lastUserCPU.QuadPart;
        lastCPU = sysIdle_new;
        lastSysCPU = sysKernel_new;
        lastUserCPU = sysUser_new;

        uint64_t deltaTotal = deltaKernel + deltaUser;
        return deltaTotal ? qBound(0.0, 100.0 * (1.0 - (double)deltaIdle / deltaTotal), 100.0) : 0.0;
#else
        // Linux / macOS 用 /proc/stat 第一行 
        static QVector<qulonglong> last(10, 0);
        QVector<qulonglong> cur(10, 0);
        int fd = ::open("/proc/stat", O_RDONLY);
        if (fd < 0) return 0.0;
        char buf[512] = { 0 };
        ssize_t n = ::read(fd, buf, sizeof(buf) - 1);
        ::close(fd);
        if (n <= 0) return 0.0;

        qulonglong user, nice, sys, idle, iowait, irq, softirq, steal;
        if (std::sscanf(buf, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
            &user, &nice, &sys, &idle, &iowait, &irq, &softirq, &steal) < 4)
            return 0.0;
        cur[0] = user;   cur[1] = nice; cur[2] = sys;  cur[3] = idle;
        cur[4] = iowait; cur[5] = irq;  cur[6] = softirq; cur[7] = steal;

        qulonglong prevIdle = last[3] + last[4];
        qulonglong idleNew = cur[3] + cur[4];
        qulonglong prevNonIdle = last[0] + last[1] + last[2] + last[5] + last[6] + last[7];
        qulonglong nonIdleNew = cur[0] + cur[1] + cur[2] + cur[5] + cur[6] + cur[7];

        qulonglong prevTotal = prevIdle + prevNonIdle;
        qulonglong totalNew = idleNew + nonIdleNew;
        last = cur;

        if (prevTotal == 0) return 0.0;
        return qBound(0.0, 100.0 * (1.0 - (double)(idleNew - prevIdle) / (totalNew - prevTotal)), 100.0);
#endif
    }

    // 后台线程&共享数据
    class CpuThread : public QThread
    {
        Q_OBJECT
    public:
        static CpuThread& instance()
        {
            static CpuThread t;
            return t;
        }
        double value() const 
        { 
            return m_load; 
        }
        void startUpdate(int sec) 
        { 
            QMutexLocker l(&m_mutex); m_interval = sec; 
            if (!isRunning()) 
                start(); 
        }

    protected:
        void run() override
        {
            QTimer timer;
            timer.setInterval(m_interval * 1000);
            connect(&timer, &QTimer::timeout, this, [&] { 
                m_load = calcCpuLoad(); 
            });
            timer.start();
            exec();
        }

    private:
        CpuThread() = default;
        QAtomicInt m_interval{ 1 };
        mutable QMutex m_mutex;
        double m_load = 0.0;
    };

} 

// ========== 获取当前进程物理内存占用百分比 ==========
static double calcSystemMemPercent()
{
#ifdef Q_OS_WIN
    MEMORYSTATUSEX ms{ sizeof(ms) };
    if (!GlobalMemoryStatusEx(&ms)) 
        return 0.0;
    qulonglong total = ms.ullTotalPhys;
    qulonglong avail = ms.ullAvailPhys;
    return total ? 100.0 * (1.0 - double(avail) / double(total)) : 0.0;
#else
    /* Linux 用 /proc/meminfo 第一屏即可 */
    auto readKB = [](const char* key) -> qint64 {
        int fd = ::open("/proc/meminfo", O_RDONLY);
        if (fd < 0) 
            return -1;
        char buf[1024] = { 0 };
        ssize_t n = ::read(fd, buf, sizeof(buf) - 1);
        ::close(fd);
        if (n <= 0) 
            return -1;
        char* p = std::strstr(buf, key);
        if (!p) 
            return -1;
        return std::atoll(p + std::strlen(key)); // kb
    };

    qint64 total = readKB("MemTotal:");
    qint64 avail = readKB("MemAvailable:"); // 内核 3.14+ 才有
    if (avail == -1) 
    { // 老内核回退计算
        qint64 free = readKB("MemFree:");
        qint64 buffers = readKB("Buffers:");
        qint64 cached = readKB("Cached:");
        avail = free + buffers + cached;
    }
    return total > 0 ? 100.0 * (1.0 - double(avail) / double(total)) : 0.0;
#endif
}

/* --------- 后台线程 --------- */
class SysMemThread : public QThread
{
    Q_OBJECT
public:
    static SysMemThread& instance()
    {
        static SysMemThread t;
        return t;
    }
    double value() const
    {
        QReadLocker lk(&m_lock);
        return m_value;
    }
    void startUpdate(int sec)
    {
        QWriteLocker lk(&m_lock);
        m_interval = sec;
        if (!isRunning())
            start();
        else
            m_timer->start(m_interval * 1000);
    }
protected:
    void run() override
    {
        m_timer = new QTimer;
        m_timer->setInterval(m_interval * 1000);
        connect(m_timer, &QTimer::timeout, this, [&] {
            double v = calcSystemMemPercent();
            QWriteLocker lk(&m_lock);
            m_value = v;
        });
        m_timer->start();
        (void)calcSystemMemPercent(); // 立即采一次
        exec();
    }
private:
    SysMemThread() = default;
    mutable QReadWriteLock m_lock;
    QTimer* m_timer = nullptr;
    double m_value = 0.0;
    int m_interval = 1;
};

#include "LAppSystemInfo.moc"
// anonymous namespace

// ========== SystemInfo ==========

SystemInfo::SystemInfo()
{
}

SystemInfo::~SystemInfo()
{
}

// ========== 接口 =========
double SystemInfo::cpuLoad(bool threaded, int intervalSec)
{
     if (threaded) 
     {
         CpuThread::instance().startUpdate(intervalSec);
         return CpuThread::instance().value();
     }
     return calcCpuLoad();
}

double SystemInfo::memPercent(bool threaded, int intervalSec)
{
    if (threaded) 
    {
        SysMemThread::instance().startUpdate(intervalSec);
        return SysMemThread::instance().value();
    }
    return calcSystemMemPercent();
}