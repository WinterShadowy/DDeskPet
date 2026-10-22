#include "LAppLive2DModelsManager.h"
#include "LAppLive2DManager.hpp"

LModelManager::LModelManager(const QString& modelsfilePath)
{
	ScanLive2DModels(modelsfilePath);
}

LModelManager::~LModelManager()
{
}

LModelManager& LModelManager::Instance()
{
    static LModelManager ins(BaseConfig::instance().GetModelFolderPath());
    return ins;
}

int LModelManager::ReReadLive2DModelFiles()
{
    this->qvec_LModelInfo.clear();
    ScanLive2DModels(this->qstr_modelsfilePath);
	return this->qvec_LModelInfo.size();
}

const QVector<ModelInfo_Live2D>& LModelManager::GetLive2DModelInfo()
{
    return this->qvec_LModelInfo;
}

QString LModelManager::GetLive2DModelFolderName(const int& index)
{
    if (index >= 0 && index < this->qvec_LModelInfo.size())
    {
        return this->qvec_LModelInfo.at(index).ModelFolderName;
    }
    else
    {
        RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
    }
	return QString("");
}

QString LModelManager::GetLive2DModelFileName(const int& index)
{
	if (index >= 0 && index < this->qvec_LModelInfo.size())
		return this->qvec_LModelInfo.at(index).ModelFileName;
    return QString("");
}

int LModelManager::GetLive2DModelCount()
{
	return this->qvec_LModelInfo.size();
}

bool LModelManager::SetDisplayLive2DModel(int index)
{
	if (index >= 0 && index < this->qvec_LModelInfo.size())
	{
        if(HasChinese(GetLive2DModelFolderName(index)) == true || HasChinese(GetLive2DModelFileName(index)) == true)
		{
			RuntimeLog_Error(QString("FolderName or FileName has Chinese").toStdString().c_str());
            ModelWindowPromptWindow::ShowMessageOnModelWindow(11, QString("模型文件名存在中文/日文/韩文等，无法进行设置哦！"), 4000, true);
			return false;
		}
        Config_Model::instance().SetDisplayModelInfo(
            GetLive2DModelFolderName(index),
            GetLive2DModelFileName(index));

        // 热重载当前 Live2D 模型
        LAppLive2DManager::GetInstance()->LoadModelFromPath(
            Config_Model::instance().GetDisplayModelPath().toStdString(),
            Config_Model::instance().GetDisplayModelName().toStdString());
		return true;
	}
    else
    {
        RuntimeLog_Error(QString("Index out of bounds:%1").arg(index).toStdString().c_str());
    }
    return false;
}

void LModelManager::PrintLModelInfo()
{
    for (int i = 0; i < this->qvec_LModelInfo.size(); i++)
    {
        qDebug() << "FolderName:" << this->qvec_LModelInfo.at(i).ModelFolderName 
            << "FileName:" << this->qvec_LModelInfo.at(i).ModelFileName
            << "Version:" << this->qvec_LModelInfo.at(i).Version
            << "Name:" << this->qvec_LModelInfo.at(i).ModelName << Qt::endl;
    }
}

void LModelManager::SetLive2DShowModel(const QString& model_folder_name, const QString& model_name)
{
    if (HasChinese(model_folder_name) == true || HasChinese(model_name) == true)
    {
        RuntimeLog_Error(QString("FolderName or FileName has Chinese").toStdString().c_str());
        return;
    }
    Config_Model::instance().SetDisplayModelInfo(model_folder_name, model_name);
    return;
}

void LModelManager::ScanLive2DModels(const QString& modelsfilePath)
{
    if (Config_Developer::instance().GetUseModels() == false || 
        Config_Developer::instance().GetLoadModels() == false)
    {
        return;
    }
    if(modelsfilePath.isEmpty())
		return;
    // 保存路径，供 ReRead 使用
    this->qstr_modelsfilePath = modelsfilePath;

    QDir rootDir(modelsfilePath);
    if (!rootDir.exists())
        return;
    QFileInfoList folderList = rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (int i = 0; i < folderList.size(); ++i)
    {
        QString folderPath = folderList.at(i).absoluteFilePath();
        QString folderName = folderList.at(i).fileName();
        LoadLive2DModelInfo_V2(folderPath, folderName);
    }
    return;
}

void LModelManager::LoadLive2DModelInfo(QString& folderPath, QString& folderName)
{
    QDir dir(folderPath);
    // 找 *.model3.json
    QStringList nameFilters;
    nameFilters.append("*.model3.json");
    QFileInfoList fileList = dir.entryInfoList(nameFilters, QDir::Files);

    if (fileList.isEmpty())
        return;

    // 每个文件夹只拿第一个匹配的 json
    QFileInfo jsonFile = fileList.first();
    QFile file(jsonFile.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray raw = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(raw);
    if (!doc.isObject())
        return;

    QJsonObject root = doc.object();
    ModelInfo_Live2D info;
    info.ModelFileName = jsonFile.fileName();

    //Version
    if (root.contains("Version"))
        info.Version = root.value("Version").toString();
    //Name
    if (root.contains("Name"))
        info.ModelName = root.value("Name").toString();
    else
        info.ModelName = jsonFile.baseName(); //  fallback
    info.ModelFolderName = folderName;
    info.ModelPath = folderPath;
    this->qvec_LModelInfo.push_back(info);
}

void LModelManager::LoadLive2DModelInfo_V2(QString& folderPath, QString& folderName)
{
    QDir dir(folderPath);

    // 先过滤两种可能文件名
    QStringList filters = { "*.model3.json", "model0.json" };   // 按优先级
    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files);

    /* 2. 把结果按后缀排序，保证 *.model3.json 排在最前 */
    std::sort(fileList.begin(), fileList.end(), [](const QFileInfo& a, const QFileInfo& b) {
            // model3.json 优先，其余按字典序
            if (a.fileName().endsWith("model3.json") &&
                !b.fileName().endsWith("model3.json"))
            {
                return true;
            }
            return a.fileName() < b.fileName();
        });

    if (fileList.isEmpty())
        return;

    // 取第一个（优先 model3.json，其次 model.json）
    QFileInfo jsonFile = fileList.first();
    QFile file(jsonFile.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray raw = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(raw);
    if (!doc.isObject())
        return;

    QJsonObject root = doc.object();
    ModelInfo_Live2D info;
    info.ModelFileName = jsonFile.fileName();

    if (root.contains("Version"))
        info.Version = root.value("Version").toString();
    if (root.contains("Name"))
        info.ModelName = root.value("Name").toString();
    else
        info.ModelName = jsonFile.baseName();   // 兜底
    info.ModelFolderName = folderName;
    info.ModelPath = folderPath;
    qvec_LModelInfo.push_back(info);
}
bool LModelManager::HasChinese(const QString& text)
{
    static QRegularExpression re(QStringLiteral("[一-鿿㐀-䶵]"));
    return re.match(text).hasMatch();
}
