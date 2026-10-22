#include "qhotkey_Manager.h"
#include <QMap>

HotKeyManager* HotKeyManager::instance() 
{
    static HotKeyManager ins;
    return &ins;
}

HotKeyManager::HotKeyManager(QObject* parent)
    : QObject(parent)
{
}

HotKeyManager::~HotKeyManager()
{
    m_hotkeys.clear();
    m_filePath.clear();
}

bool HotKeyManager::RegisterHotKey(const QString& hotKey, const QString& filePath) 
{
    QString pathBuffer = QDir::toNativeSeparators(filePath);
    // 检查路径是否有效
    if (pathBuffer.isEmpty() || !QFileInfo::exists(pathBuffer))
    {
        RuntimeLog_Warn(QString("Invalid file path:%1").arg(pathBuffer).toStdString().c_str());
        return false;
    }

    // 检查热键是否已注册
    if (m_hotkeys.contains(hotKey))
    {
        RuntimeLog_Error(QString("hotkey: %1 already registered").arg(hotKey).toStdString().c_str());
        return false;
    }

    // 创建并注册热键
    QHotkey* qhk = new QHotkey(QKeySequence(hotKey), true);
    if (!qhk->isRegistered())
    {
		RuntimeLog_Error(QString("Failed to register hotkey: %1").arg(hotKey).toStdString().c_str());
        delete qhk;
        return false;
    }
    m_hotkeys.insert(hotKey, qhk); // 保存热键
    m_filePath.insert(hotKey, filePath);
    // 连接激活信号
    connect(qhk, &QHotkey::activated, [pathBuffer]() {
        QStringList args;// 使用空标题参数 ""，并传入绝对路径（QProcess 会把每个参数单独传给 cmd）
        args << "/c" << "start" << "" << pathBuffer;
        bool ok = QProcess::startDetached("cmd", args);
        if (!ok) 
        {
			RuntimeLog_Error(QString("Failed to start process: %1").arg(pathBuffer).toStdString().c_str());
        }
    });
    return true;
}

bool HotKeyManager::UnregisterHotKey(const QString& hotKey)
{
    if (!m_hotkeys.contains(hotKey))
    {
		RuntimeLog_Error(QString("Failed to unregister hotkey: %1 not found").arg(hotKey).toStdString().c_str());
        return false;
    }

    QHotkey* qhk = m_hotkeys.take(hotKey);
    if (qhk)
    {
        qhk->setRegistered(false); // 注销热键
        delete qhk; // 清理资源
        m_filePath.remove(hotKey);
        return true;
    }
    else
    {
		RuntimeLog_Warn(QString("Failed to unregister hotkey: %1 not found").arg(hotKey).toStdString().c_str());
        return false;
    }
}

bool HotKeyManager::IsRegistered(const QString& hotKey) const 
{
    return m_hotkeys.contains(hotKey);
}

bool HotKeyManager::ChangeHotKey(const QString& oldHotKey, const QString& newHotKey)
{
    if (IsRegistered(newHotKey))
    {
		RuntimeLog_Info(QString("The new hotkey has been registered:%1").arg(newHotKey).toStdString().c_str());
        return false;
    }
    QHotkey* qhk = m_hotkeys.take(oldHotKey);
    QString filePath = m_filePath.take(oldHotKey);
    if (qhk)
    {
        qhk->setRegistered(false); // 注销热键
        delete qhk; // 清理资源
        qhk = nullptr;
    }
    else
    {
        RuntimeLog_Warn(QString("Failed to unregister hotkey: %1 not found").arg(oldHotKey).toStdString().c_str());
        return false;
    }
    // 创建并注册热键
    qhk = new QHotkey(QKeySequence(newHotKey), true);   
    m_hotkeys.insert(newHotKey, qhk);
    m_filePath.insert(newHotKey, filePath);
    // 连接激活信号
    connect(qhk, &QHotkey::activated, [filePath]() {
        QStringList args;// 使用空标题参数 ""，并传入绝对路径（QProcess 会把每个参数单独传给 cmd）
        args << "/c" << "start" << "" << filePath;
        bool ok = QProcess::startDetached("cmd", args);
        if (!ok)
        {
			RuntimeLog_Error(QString("Failed to start process: %1").arg(filePath).toStdString().c_str());
        }
    });
    return true;
}
