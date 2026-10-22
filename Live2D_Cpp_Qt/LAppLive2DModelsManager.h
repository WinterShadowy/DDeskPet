/*
 ============================================================================
 Header: LAppLive2DModelsManager.h
 Description: Live2D模型管理
 Start time: 2025.10.20
 End time: 2026.01.07
 Maintainer: WangSZ
 ============================================================================
*/
#pragma once

#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QVector>
#include <QString>
#include <QByteArray>
#include <QFile>
#include <QRegularExpression>

#include "Configuration.h"
#include "LAppDefine.hpp"
#include "LAppModelMessage.h"
#include "GLCore.h"

struct ModelInfo_Live2D
{
    QString Version;
    QString ModelName;
    QString ModelFileName;
    QString ModelFolderName;
    QString ModelFileSize;
    QString ModelPath;
};

class LModelManager {
public:
    LModelManager() = default;
    explicit LModelManager(const QString& modelsfilePath);
    ~LModelManager();

    // 获取单例
    static LModelManager& Instance();

public:
    // 重新读取模型文件
    int ReReadLive2DModelFiles();
    // 获取模型信息
    const QVector<ModelInfo_Live2D>& GetLive2DModelInfo();
    // 获取模型文件路径
    QString GetLive2DModelFolderName(const int& index);
    // 获取模型文件名
    QString GetLive2DModelFileName(const int& index);
    // 获取模型个数
    int GetLive2DModelCount();
    // 设置显示模型
    bool SetDisplayLive2DModel(int index);

    // 打印所有的模型信息
    void PrintLModelInfo();
private:
    // 设置显示模型
    void SetLive2DShowModel(const QString& model_folder_name, const QString& model_name);
    // 扫描模型
	void ScanLive2DModels(const QString& modelsfilePath);
    // 加载模型
    void LoadLive2DModelInfo(QString& folderPath, QString& folderName);
    void LoadLive2DModelInfo_V2(QString& folderPath, QString& folderName);
    bool HasChinese(const QString& text);

private:
    QVector<ModelInfo_Live2D> qvec_LModelInfo;
    QString qstr_modelsfilePath;
};

#define GetLModelManagerInstance() \
    LModelManager::Instance()