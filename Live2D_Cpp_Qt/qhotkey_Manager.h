#pragma once

#include <QObject>
#include <QScopedPointer>
#include <QDebug>
#include <QFileInfo>
#include <QProcess>
#include "qhotkey.h"
#include "LAppDefine.hpp"

class HotKeyManager : public QObject
{
    Q_OBJECT
public:
    static HotKeyManager* instance();
    // 注册热键
    bool RegisterHotKey(const QString& hotKey, const QString& filePath);
    // 注销热键
    bool UnregisterHotKey(const QString& hotKey);
    // 判断热键是否已注册
    bool IsRegistered(const QString& hotKey) const;
    // 改变注册的热键
    bool ChangeHotKey(const QString& oldHotKey, const QString& newHotKey);

private:
    explicit HotKeyManager(QObject* parent = nullptr);
    ~HotKeyManager();

    QMap<QString, QHotkey*> m_hotkeys; // 存储注册的热键 key: 热键
    QMap<QString, QString> m_filePath; // 可执行文件路径
};

// 注册热键
#define HKM_Register(_hotkey, _filePath) \
    HotKeyManager::instance()->RegisterHotKey(_hotkey, _filePath)
// 注销热键
#define HKM_Unregister(_hotkey) \
    HotKeyManager::instance()->UnregisterHotKey(_hotkey)
// 检查热键是否注册
#define HKM_Isregistered(_hotkey) \
    HotKeyManager::instance()->IsRegistered(_hotkey);
// 修改已经注册的热键
#define HKM_ChangeHotkey(_old_hotkey, _new_hotkey) \
    HotKeyManager::instance()->ChangeHotKey(_old_hotkey, _new_hotkey)
