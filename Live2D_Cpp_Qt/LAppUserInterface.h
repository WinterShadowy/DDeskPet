#pragma once
// 用户文档接口

#include "LAppDefine.hpp"
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QJsonValue>
#include <QDebug>
#include <QApplication>

class UserWindowInterface : public QObject
{
    Q_OBJECT

public:
    explicit UserWindowInterface(QObject* parent = nullptr);
    ~UserWindowInterface() = default;

    // 收集并保存信息
    void CollectAndSaveInfo(const QString& filePath);

    // 获取PID
    quint32 GetPID();
private:
    quint32 GetProcessId();
    QString GetMainThreadId();
    QString GetWindowTitle();
    QString GetWindowClass();
};

void InitilizeUserWindowInterface(const QString& filePath);
quint32 UserInterface_GetProcessID();
