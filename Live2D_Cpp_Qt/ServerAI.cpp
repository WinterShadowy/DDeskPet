#include "ServerAI.h"

ServerAI::ServerAI()
{
}

ServerAI::~ServerAI()
{
}

ServerAI& ServerAI::instance()
{
    static ServerAI ins;
	return ins;
}

// UpdateSetAIPage_ModelsComBoboxInfo 的实现
void ServerAI::UpdateSetAIPage_ModelsComBoboxInfo(QComboBox* comBobox_Models, QLabel* qlabel_updatestatus)
{
    // 使用 QPointer 以在回调时检测控件是否已被销毁
    QPointer<QComboBox> comboPtr(comBobox_Models);
    QPointer<QLabel> labelPtr(qlabel_updatestatus);

    // 在调用方线程（通常是 GUI 线程）做一些立即反馈
    if (labelPtr) 
    {
        labelPtr->setText("正在加载模型...");
    }
    else
    {
        RuntimeLog_Warn("qlabel_updatestatus is nullptr");
        return;
    }
    if (comboPtr) 
    {
        comboPtr->setEnabled(false);
    }
    else
    {
		RuntimeLog_Warn("comBobox_Models is nullptr");
		return;
    }

    // 在后台线程运行 阻塞函数获取模型表
    QFuture<QStringList> future = QtConcurrent::run([this]() -> QStringList {
        return this->GetModelsFromServer();
        });

    // 用一个 heap 分配的 watcher 来观察 future，保证它的信号在创建它的线程（GUI）上发出
    QFutureWatcher<QStringList>* watcher = new QFutureWatcher<QStringList>();
    // 当任务完成时，finished 信号将在 watcher 所在线程被触发
    QObject::connect(watcher, &QFutureWatcher<QStringList>::finished, watcher, [watcher, comboPtr, labelPtr]() {
        // 获取结果
        QStringList models;
        try {
            models = watcher->result();
        }
        catch (...) {
            models.clear();            // 如果 GetModelsFromServer 抛异常，这里捕获并留下空列表
        }
        // 更新 UI（先检查指针仍然有效）
        if (labelPtr) 
        {
            if (!models.isEmpty()) 
            {
                labelPtr->setText(QStringLiteral("已加载 %1 个模型").arg(models.size()));
            }
            else 
            {
                labelPtr->setText(QStringLiteral("未获取到模型或加载失败"));
            }
        }
        if (comboPtr) 
        {
            comboPtr->clear();
            if (!models.isEmpty()) 
            {
                comboPtr->addItems(models);
                comboPtr->setCurrentIndex(models.size() - 1);
            }
            comboPtr->setEnabled(true);
        }
        // 清理 watcher，lambda 绑定在 watcher 上，因此这里使用 deleteLater
        watcher->deleteLater();
    });
    // 启动 watcher 与 future 关联（必须在 connect 之后）
    watcher->setFuture(future);
    return;
}

bool ServerAI::ReloadAIConfig(std::string str_url_path, std::string str_api_key)
{
    return AIManager::instance().ReinitAIManager(str_url_path, str_api_key);;
}

QStringList ServerAI::GetModelsFromServer()
{
    /*AIManager::instance().ResetURL(Config_SetAI::instance().GetUrl_FullPath_Models());
    AIManager::instance().ResetAPIKey(Config_SetAI::instance().Get_Api_Key());*/
    std::string _url = Config_SetAI::instance().GetUrl_FullPath_Models().toStdString();
	std::string _api_key = Config_SetAI::instance().Get_Api_Key().toStdString();
    if (AIManager::instance().ReinitAIManager(_url, _api_key))
    {
        qDebug() << "[debug] AIManager::instance().ReinitAIManager success";
    }
    else
    {
		qDebug() << "[debug] AIManager::instance().ReinitAIManager fail";
    }

    QStringList qsl_modelsist = AIManager::instance().GetModels();

    // 如果服务端没有返回任何模型列表，则直接退回到配置文件中的默认模型列表，
    // 避免对空 QStringList 调用 takeAt(0) 导致 QList 越界断言。
    if (qsl_modelsist.isEmpty())
    {
        RuntimeLog_Warn("AIManager::GetModels() return empty");
        return Config_SetAI::instance().Get_ModelListFromConfigFile();
    }

    // 从配置文件中获取首选模型名称（保证至少有一个，但仍做安全检查）。
    QStringList configModels = Config_SetAI::instance().Get_ModelListFromConfigFile();
    QString model;
    if (!configModels.isEmpty())
    {
        model = configModels.at(0);
    }

    // 将首选模型移动到列表末尾，如果它存在于服务端返回的列表中。
    int index = qsl_modelsist.indexOf(model);
    if (index >= 0 && index < qsl_modelsist.size())
    {
        QString movedModel = qsl_modelsist.takeAt(index);
        qsl_modelsist.append(movedModel);
    }
    return qsl_modelsist;
}
