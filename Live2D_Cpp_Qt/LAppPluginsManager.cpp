#include "LAppPluginsManager.h"

PluginsManager& PluginsManager::instance()
{
    static PluginsManager ins;
    return ins;
}

// 初始化插件文件夹信息
void PluginsManager::InitPluginsInfo(const QString& pluginPath)
{
    m_root = QDir(pluginPath).absolutePath();
    ScanPlugins();
    AutoRegisterAllHotKey();
    //AutomaticallyStartPlugins();
}

PluginsManager::~PluginsManager()
{
    SaveAllConfig();
    StopAllPlugins();
}

void PluginsManager::ScanPlugins()
{
    if (Config_Developer::instance().GetLoadPlugins())
    {
        return;
    }
    this->m_pluginsInfo.clear();
    QDir dir(m_root);
    if (!dir.exists()) 
        return;

    const QFileInfoList folders = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

    for (const QFileInfo& folder : folders)
    {
        // 拼接完整的文件路径
        QString configPath = folder.absoluteFilePath().trimmed(); // 假设配置文件名为 config.json
        PluginInfo _plugin_info;

        // 使用组合的路径和 plugin 信息调用 LoadConfig
        if (LoadConfig(configPath, _plugin_info))
        {
            this->m_pluginsInfo.push_back(std::make_pair(_plugin_info, configPath)); // 存储插件信息
        }
    }

    // 按照插件名排序
	std::sort(this->m_pluginsInfo.begin(), 
        this->m_pluginsInfo.end(), 
        [](const std::pair<PluginInfo, QString>& a, const std::pair<PluginInfo, QString>& b) {
            // 优先启动
            if(a.first.activate_upon_registration != b.first.activate_upon_registration)
                return  (a.first.activate_upon_registration && !b.first.activate_upon_registration);
			else if (a.first.startup_priority != b.first.startup_priority)   // 优先级数值越小优先启动
                return a.first.startup_priority < b.first.startup_priority;

            if(a.first.pluginName != b.first.pluginName)
		        return a.first.pluginName < b.first.pluginName;
            else if(a.first.version != b.first.version)
				return a.first.version > b.first.version;
            else if(a.first.AbsoluteExecutable().length() != b.first.AbsoluteExecutable().length())
				return a.first.AbsoluteExecutable().length() < b.first.AbsoluteExecutable().length();
            else  if(a.first.author != b.first.author)
                return a.first.author < b.first.author;
            else 
                return a.first.description < b.first.description;
	    });

    return;
}

bool PluginsManager::LoadConfig(const QString& dirPath, PluginInfo& _out_pluginInfo)
{
    QDir dir(dirPath);
    QString jsonFile = QDir::toNativeSeparators(dir.absoluteFilePath("config.json"));
    QFile file(jsonFile);
    if (!file.open(QIODevice::ReadOnly)) 
    {
		RuntimeLog_Error(QString("Open File Failed:%1").arg(jsonFile).toStdString().c_str());
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) 
    {
        RuntimeLog_Error(QString("File Format Error:%1").arg(jsonFile).toStdString().c_str());
        return false;
    }
	file.close();

    QJsonObject _json_obj = doc.object();

    // 检验相关字段
    auto req = [&](const char* key) -> QString {
            return _json_obj.value(key).toString();
        };
    _out_pluginInfo.author = req("author");     // 作者
    _out_pluginInfo.pluginName = req("plugin_name");    // 插件名称
    _out_pluginInfo.executableFilePath = req("executable_file_path");   // 可执行文件路径
    _out_pluginInfo.executableFileName = req("executable_file_name");   // 可执行文件名
    _out_pluginInfo.hotKey = req("hot_key");        // 热键
    _out_pluginInfo.version = req("version");       // 版本
    _out_pluginInfo.description = req("description");   // 插件描述
    _out_pluginInfo.stop_executableFilePath = req("stop_executable_file_path"); // 停止插件的文件路径
	_out_pluginInfo.stop_executableFileName = req("stop_executable_file_name"); // 停止插件的文件名
    _out_pluginInfo.activate_upon_registration = _json_obj.value("activate_upon_registration").toBool();    // 激活插件
    _out_pluginInfo.autoRegisterHotKey = _json_obj.value("auto_register_hotkey").toBool();  // 自动注册热键
	_out_pluginInfo.startup_priority = _json_obj.value("startup_priority").toInt();  // 启动优先级

    // 处理可执行文件路径
    QString fullPath = QDir::cleanPath(dir.absoluteFilePath(
        _out_pluginInfo.executableFilePath + _out_pluginInfo.executableFileName));
    _out_pluginInfo.absolute_path_executable_file = QDir::toNativeSeparators(fullPath);

    // 处理停止文件路径
    if(!_out_pluginInfo.stop_executableFilePath.isEmpty() && !_out_pluginInfo.stop_executableFileName.isEmpty())
    {
        QString fullPath_stop = QDir::cleanPath(dir.absoluteFilePath(
            _out_pluginInfo.stop_executableFilePath + _out_pluginInfo.stop_executableFileName));
        _out_pluginInfo.absolute_path_stop_executable_file = QDir::toNativeSeparators(fullPath_stop);
    }

    // 处理图标路径
    _out_pluginInfo.absolute_path_icon = QDir::toNativeSeparators(dir.absoluteFilePath(req("icon_path")));
    // 如果没有找到图标就使用默认图标
    if (_out_pluginInfo.absolute_path_icon.isEmpty())
    {
        _out_pluginInfo.absolute_path_icon = QDir::toNativeSeparators(QDir(ConfigurationBase::instance().GetDefaultIconPath()).absolutePath());
    }

    if (_out_pluginInfo.author.isEmpty() ||
        _out_pluginInfo.pluginName.isEmpty() ||
        _out_pluginInfo.executableFileName.isEmpty() ||
        _out_pluginInfo.hotKey.isEmpty() ||
        _out_pluginInfo.executableFilePath.isEmpty())
    {
		RuntimeLog_Debug(QString("config.json missing required fields in %1").arg(dirPath).toStdString().c_str());
        return false;
    }
    return true;
}

// 全量回写
void PluginsManager::SaveAllConfig()
{
    for (auto it = m_pluginsInfo.begin(); it != m_pluginsInfo.end(); ++it)
    {
		SaveConfig(it->second, it->first);
    }
    return;
}

// 单插件回写
bool PluginsManager::SaveConfig(QString& json_path, PluginInfo& info) const
{
    // 拼目录
    QDir pluginDir(json_path);
    if(pluginDir.exists() == false)
		return false;

    QString configPath = QDir::toNativeSeparators(pluginDir.absoluteFilePath("config.json"));
    QFile file(configPath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
		RuntimeLog_Error(QString("Open File Failed: %1").arg(configPath).toStdString().c_str());
        return false;
    }

    // 读原始 json
    QByteArray old = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(old);
    QJsonObject obj = doc.object();

    // 覆盖字段
    obj["hot_key"] = info.hotKey;
    obj["auto_register_hotkey"] = info.autoRegisterHotKey;
    obj["activate_upon_registration"] = info.activate_upon_registration;

    // 写回文件
    file.resize(0);                 // 清空
    doc.setObject(obj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

QVector<std::pair<PluginInfo, QString> > PluginsManager::GetAllPluginsInfo() const
{
    return this->m_pluginsInfo;
}

PluginInfo PluginsManager::GetPluginInfo(const int index) const
{
    if(index < 0 || index >= this->m_pluginsInfo.size())
	{
        RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
		return PluginInfo();
	}
    return this->m_pluginsInfo.at(index).first;
}

// 获取插件版本
QString PluginsManager::GetVersion(const int index) const
{
    if(index < 0 || index >= this->m_pluginsInfo.size()) 
	{
		RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
		return QString();
	}
	auto it = this->m_pluginsInfo.at(index);
	if(it.first.version.length() > 0)
		return it.first.version;
    return QString();
}
QString PluginsManager::GetPluginName(const int index) const
{
    return this->m_pluginsInfo.at(index).first.pluginName;
}
QString PluginsManager::GetDescription(const int index) const
{
    return this->m_pluginsInfo.at(index).first.description;
}
// 获取插件数量
size_t PluginsManager::GetPluginsCount() const
{
    return this->m_pluginsInfo.size();
}
// 获取热键
QString PluginsManager::GetHotKey(const int index) const
{
    if (index < 0 || index >= this->m_pluginsInfo.size())
    {
		RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
		return QString();
    }
    PluginInfo info = this->m_pluginsInfo.at(index).first;
	if (info.hotKey.length() > 0)
		return info.hotKey;
    return QString();
}
// 获取插件图标绝对路径
QString PluginsManager::GetPluginAbsolutePath_Icon(const int index) const
{
	if (index < 0 || index >= this->m_pluginsInfo.size())
	{
		RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
		return QString();
	}
	PluginInfo info = this->m_pluginsInfo.at(index).first;
    if (info.absolute_path_icon.size() > 0)
        return info.absolute_path_icon;
    return QDir(ConfigurationBase::instance().GetDefaultIconPath()).absolutePath();
}
// 获取插件绝对路径
QString PluginsManager::GetPluginAbsolutePath_ExecutableFile(const int index) const
{
	if (index < 0 || index >= this->m_pluginsInfo.size())
	{
		RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
		return QString();
	}
	PluginInfo info = this->m_pluginsInfo.at(index).first;
    if (info.absolute_path_executable_file.size() > 0)
        return info.absolute_path_executable_file;
    return QString();
}

bool PluginsManager::GetAutoRegistedStatus(const int index) const
{
	if (index < 0 || index >= this->m_pluginsInfo.size()) 
	{
		RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
		return false;
	}
    return this->m_pluginsInfo.at(index).first.autoRegisterHotKey;
}

bool PluginsManager::GetAutoStartStatus(const int index) const
{
	if (index < 0 || index >= this->m_pluginsInfo.size()) 
	{
		RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
		return false;
	}
    return this->m_pluginsInfo.at(index).first.activate_upon_registration;
}

bool PluginsManager::GetRegisteredStatus(const int index) const
{
#pragma message("Compiling GetAutoRegistedStatus")  // MSVC
	if (index < 0 || index >= this->m_pluginsInfo.size()) 
	{
        RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
		return false;
	}
    return this->m_pluginsInfo.at(index).first.registered;
}

int PluginsManager::AutomaticallyStartPlugins()
{
    int count = 0, priority = 1;
    // 优先级
    for (priority; priority <= 10; ++priority)
    {
		// 遍历
        for (auto it = m_pluginsInfo.begin(); it != m_pluginsInfo.end(); ++it)
        {
			// 如果是自动启动，且优先级相同，则启动插件
            if (it->first.activate_upon_registration == true && it->first.startup_priority == priority)
            {
                QStringList args;// 使用空标题参数 ""，并传入绝对路径（QProcess 会把每个参数单独传给 cmd）
                args << "/c" << "start" << "" << it->first.absolute_path_executable_file;
                bool ok = QProcess::startDetached("cmd", args);
                if (!ok)
                {
					RuntimeLog_Warn(QString("fild to start：%1 failed").arg(it->first.absolute_path_stop_executable_file).toStdString().c_str());
                }
                else
                {
                    ++count;
                }
            }
        }
    }
    return count;
}


int PluginsManager::StopAllPlugins()
{
    int count = 0;
	for (auto it = m_pluginsInfo.begin(); it != m_pluginsInfo.end(); ++it)
	{
		if (it->first.activate_upon_registration == true)
		{
			// 如果停止插件的绝对路径为空，则跳过
            if(!it->first.absolute_path_stop_executable_file.isEmpty())
			{
                qDebug() << it->first.absolute_path_stop_executable_file << Qt::endl;
                QStringList args;// 使用空标题参数 ""，并传入绝对路径（QProcess 会把每个参数单独传给 cmd）
                args << "/c" << "start" << "" << it->first.absolute_path_stop_executable_file;
                bool ok = QProcess::startDetached("cmd", args);
                if (!ok)
                {
					RuntimeLog_Warn(QString("fild to start:%1 failed").arg(it->first.absolute_path_stop_executable_file).toStdString().c_str());
                }
                else
                {
                    ++count;
                }
			}
		}
	}
    return count;
}

bool PluginsManager::SetHotKey(int index, QString newHotKey)
{
    if (index < 0 || index >= this->m_pluginsInfo.size()) 
    {
		RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
        return false;
    }
    this->m_pluginsInfo[index].first.hotKey = newHotKey;
    return true;
}

bool PluginsManager::SetAutoRegister(const int index, bool bStatus)
{
	if (index < 0 || index >= this->m_pluginsInfo.size())
	{
        RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
		return false;
	}
    this->m_pluginsInfo[index].first.autoRegisterHotKey = bStatus;
    return true;
}

bool PluginsManager::SetAutoStartStatus(const int index, bool bStatus)
{
    if (index < 0 || index >= this->m_pluginsInfo.size())
    {
		RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
        return false;
    }
    this->m_pluginsInfo[index].first.activate_upon_registration = bStatus;
    return true;
}

void PluginsManager::AutoRegisterAllHotKey()
{
    // 判断是否开启自动启动插件 - true为开启 - 反之亦然
    if (Config_Developer::instance().GetAutoStartPlugins())
    {
        return;
    }
    for(auto it = m_pluginsInfo.begin(); it != m_pluginsInfo.end(); ++it)
	{
        // 自动注册热键
        if (it->first.autoRegisterHotKey)
        {
            it->first.registered = RegisterHotKey(it->first.hotKey, it->first.absolute_path_executable_file);
        }
	}
}

bool PluginsManager::RegisterHotKey(const QString& qstr_hotkey, const QString& qstr_filePath)
{
    return HKM_Register(qstr_hotkey, qstr_filePath);
}

QString PluginInfo::AbsoluteExecutable() const
{
    return QDir::toNativeSeparators(this->executableFilePath + this->executableFileName);
}

QString PluginInfo::AbsoluteStopExecutable() const
{
    return QDir::toNativeSeparators(this->stop_executableFilePath + this->stop_executableFileName);
}