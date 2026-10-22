#include "LAppUserInterface.h"
#include "LAppDefine.hpp"
#include <QApplication>
#include <QWidget>
#include <QWindow>
#include <QThread>

UserWindowInterface::UserWindowInterface(QObject* parent) : QObject(parent) {}

quint32 UserWindowInterface::GetProcessId() 
{
    return QCoreApplication::applicationPid();
}

QString UserWindowInterface::GetMainThreadId() 
{
    Qt::HANDLE mainThreadId = QThread::currentThreadId();
    return QString::number(reinterpret_cast<quintptr>(mainThreadId), 16);
}

QString UserWindowInterface::GetWindowTitle() 
{
    // 获取主窗口的标题
    QWidget* mainWindow = QApplication::activeWindow();
    return mainWindow ? mainWindow->windowTitle() : QString();
}

QString UserWindowInterface::GetWindowClass() 
{
    // 可以被用 toIdent 为目标集（窗口类名就是类名）
    QWidget* mainWindow = QApplication::activeWindow();
    return mainWindow ? mainWindow->metaObject()->className() : QString();
}

void UserWindowInterface::CollectAndSaveInfo(const QString& filePath)
{
    QJsonObject jsonObj;
    jsonObj["processId"] = static_cast<int>(GetProcessId());
    jsonObj["mainThreadId"] = GetMainThreadId();
    /*
    jsonObj["windowTitle"] = GetWindowTitle();
    jsonObj["windowClass"] = GetWindowClass();
    */

    QJsonDocument jsonDoc(jsonObj);

    // 提取文件目录
    QFileInfo fileInfo(filePath);
    QString dirPath = fileInfo.absolutePath(); // 获取文件的父目录

    // 创建目录（如果不存在）
    QDir dir(dirPath);
    if (!dir.exists()) 
    {
        if (!dir.mkpath(".")) 
        { // mkpath 用于递归创建路径
			RuntimeLog_Warn(QString("Directory creation failed:%1").arg(dirPath).toStdString().c_str());
            return; // 目录创建失败
        }
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        RuntimeLog_Error(QString("Could not open file for writing: %1").arg(file.errorString()).toStdString().c_str());
        return;
    }

    file.write(jsonDoc.toJson());
    file.close();
    return;
}

quint32 UserWindowInterface::GetPID()
{
    return GetProcessId();
}


void InitilizeUserWindowInterface(const QString& filePath)
{
    UserWindowInterface* userWindowInterface = new UserWindowInterface;
    if (userWindowInterface == nullptr)
    {
		RuntimeLog_Error("userWindowInterface is nullptr");
        return;
    }
    userWindowInterface->CollectAndSaveInfo(filePath);
    delete userWindowInterface;
    userWindowInterface = nullptr;
}

quint32 UserInterface_GetProcessID()
{
    UserWindowInterface* userWindowInterface = new UserWindowInterface;
    quint32 pid = userWindowInterface->GetPID();
    delete userWindowInterface;
    return pid;
}
