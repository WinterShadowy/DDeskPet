/*
 ============================================================================
 Header: LAppPluginsManager.h
 Description: 插件管理
 Start time: 2025.10.04
 End time: 2026.01.07
 Maintainer: WangSZ
 ============================================================================
*/
#pragma once

#include "FileOperator.h"
#include "qhotkey_Manager.h"
#include "Configuration.h"

#include <QObject>
#include <QString>
#include <QMap>
#include <QVector>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <utility>
#include <QProcess>
#include <algorithm>


struct PluginInfo
{
    QString author;     // 作者
    QString pluginName;     // 插件名称
    QString executableFilePath;   // 相对插件根目录
    QString executableFileName;   // 执行文件名
    QString stop_executableFilePath;    // 停止插件的文件路径
    QString stop_executableFileName;    // 停止插件的文件名
    QString hotKey;     // 热键
    QString version;    // 插件版本
    QString description;    // 插件描述

    QString absolute_path_icon;
    QString absolute_path_executable_file;
    QString absolute_path_stop_executable_file;
    
    int startup_priority = 10; // 启动优先级，数值越小优先启动

    bool    autoRegisterHotKey = false; // 是否在启动时自动注册热键
    bool    registered = false;  // 是否已注册
    bool    activate_upon_registration = false;     // 是否在注册时立即启动
    
    // 工具-返回完整可执行文件绝对路径
    QString AbsoluteExecutable() const;

    // 工具-返回完整停止可执行文件绝对路径
    QString AbsoluteStopExecutable() const;
};

class PluginsManager
{
public:
    // 单例入口：第一次调用必须给汇总文件夹路径
    static PluginsManager& instance();
    void InitPluginsInfo(const QString& pluginPath);
    
    // 只读接口
    QVector<std::pair<PluginInfo, QString> > GetAllPluginsInfo() const;  // 返回所有插件信息
    PluginInfo GetPluginInfo(const int index) const;  // 根据插件名返回插件信息
    QString GetVersion(const int index) const;
    QString GetPluginName(const int index) const;
    QString GetDescription(const int index) const;
    size_t GetPluginsCount() const;
    QString GetHotKey(const int index) const;
    // 获取插件图标绝对路径
    QString GetPluginAbsolutePath_Icon(const int index) const;
    // 获取插件的可执行文件绝对路径
    QString GetPluginAbsolutePath_ExecutableFile(const int index) const;
    // 获取插件的自动注册状态
    bool GetAutoRegistedStatus(const int index) const;
    // 获取插件的自动启动状态
    bool GetAutoStartStatus(const int index) const;
    // 获取插件的注册状态
    bool GetRegisteredStatus(const int index) const;
    // 自动启动插件
    int AutomaticallyStartPlugins();
    // 停止所有的插件
    int StopAllPlugins();
    // 修改接口（仅内存）
    bool SetHotKey(int index, QString newHotKey);
    bool SetAutoRegister(const int index, bool bStatus);
    bool SetAutoStartStatus(const int index, bool bStatus);
private:
    PluginsManager() = default;
    ~PluginsManager();

    void ScanPlugins();  // 扫描子目录并加载config.json
    bool LoadConfig(const QString& dirPath, PluginInfo& _out_pluginInfo);
    bool SaveConfig(QString& json_path, PluginInfo& info) const; // 单插件回写
    void SaveAllConfig();                         // 全量回写（析构用）

    // 自动注册所有已经设置好的热键
    void AutoRegisterAllHotKey();

    bool RegisterHotKey(const QString& qstr_hotkey, const QString& qstr_filePath);

    QString m_root;
    QVector<std::pair<PluginInfo, QString> > m_pluginsInfo;    // first : PluginInfo  second : file Path Info
};

// 获取实例
#define GetPluginsManagerInstance() PluginsManager::instance()