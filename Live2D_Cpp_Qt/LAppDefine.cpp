/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */



#include "LAppDefine.hpp"
#include <CubismFramework.hpp>

namespace LAppDefine {

    using namespace Csm;

    // 画面
    const csmFloat32 ViewScale = 1.0f;
    const csmFloat32 ViewMaxScale = 2.0f;
    const csmFloat32 ViewMinScale = 0.8f;

    const csmFloat32 ViewLogicalLeft = -1.0f;
    const csmFloat32 ViewLogicalRight = 1.0f;
    const csmFloat32 ViewLogicalBottom = -1.0f;
    const csmFloat32 ViewLogicalTop = -1.0f;

    const csmFloat32 ViewLogicalMaxLeft = -2.0f;
    const csmFloat32 ViewLogicalMaxRight = 2.0f;
    const csmFloat32 ViewLogicalMaxBottom = -2.0f;
    const csmFloat32 ViewLogicalMaxTop = 2.0f;

    // 相対パス
    const csmChar* ResourcesPath = "Resources/";

    // モデルの後ろにある背景の画像ファイル
    const csmChar* BackImageName = "back_class_normal.png";
    // 歯車
    const csmChar* GearImageName = "icon_gear.png";
    // 終了ボタン
    const csmChar* PowerImageName = "close.png";

    // シェーダー相対パス
    const csmChar* ShaderPath = "SampleShaders/";
    // シェーダー本体
    const csmChar* ShaderName = "SpriteEffect.fx";

    // モデル定義------------------------------------------  
    // 外部定義ファイル(json)と合わせる 
    const csmChar* MotionGroupIdle = "Idle"; // アイドリング
    const csmChar* MotionGroupTapBody = "TapBody"; // 体をタップしたとき

    // 外部定義ファイル(json)と合わせる
    const csmChar* HitAreaNameHead = "Head";
    const csmChar* HitAreaNameBody = "Body";

    // モーションの優先度定数
    const csmInt32 PriorityNone = 0;
    const csmInt32 PriorityIdle = 1;
    const csmInt32 PriorityNormal = 2;
    const csmInt32 PriorityForce = 3;

    // MOC3の整合性検証オプション
    const csmBool MocConsistencyValidationEnable = true;

    // デバッグ用ログの表示オプション
    const csmBool DebugLogEnable = true;
    const csmBool DebugTouchLogEnable = false;

    // Frameworkから出力するログのレベル設定
    const CubismFramework::Option::LogLevel CubismLoggingLevel = CubismFramework::Option::LogLevel_Verbose;

    //SDD
    //const csmInt32 RenderTargetWidth = 1900;
    //const csmInt32 RenderTargetHeight = 1000;
    //SDE

    //SDA
    csmInt32 RenderTargetWidth = 1900;
    csmInt32 RenderTargetHeight = 1000;
    //SDE

    std::pair <QString, int> Mouse_View_TouchesMoved_Localized_Scale = \
        std::make_pair<QString, int>(\
            QString(MOUSE_VIEW_TOUCHESMOVED_LOCALIZED_SCALE_QSTR),   \
            MOUSE_VIEW_TOUCHESMOVED_LOCALIZED_SCALE);
    std::pair <QString, int> Mouse_View_TouchesMoved_Global_Scale = \
        std::make_pair<QString, int>(\
            QString(MOUSE_VIEW_TOUCHESMOVED_GLOBAL_SCALE_QSTR), \
			MOUSE_VIEW_TOUCHESMOVED_GLOBAL_SCALE);
}



// ----------------------------------------------
// ----------- class RuntimeLog -----------------
// ----------------------------------------------

RuntimeLog::RuntimeLog()
{
    this->m_currentDateTime = new QDateTime();
}

RuntimeLog::~RuntimeLog()
{
}

RuntimeLog& RuntimeLog::instance()
{
	// 单例模式
	static RuntimeLog ins;
	return ins;
}

void RuntimeLog::InitializeRuntimeLog()
{
    RuntimeLog::WriteLog(
        QString("Function=[") + QString("RuntimeLog::InitializeRuntimeLog") + QString("] -- "),
        QString("Success=[") + QString("Use RuntimeLog") + QString("]"),
        RuntimeLog::GetLogFilePathInfo());
    return;
}

void RuntimeLog::Destory()
{
    delete this->m_currentDateTime;
    this->m_currentDateTime = nullptr;
}

QString RuntimeLog::GetLogFilePath() const
{
    return this->m_logFilePath;
}

void RuntimeLog::PrintErrorLogs_QDebug(const QStringList& qsl_ErrorLogs)
{
    for(int i = 0; i < qsl_ErrorLogs.size(); i++)
	{
        qWarning() << qsl_ErrorLogs.at(i) << Qt::endl;
	}
    return;
}

void RuntimeLog::WriteLogInfo(const QString qstr_RuntimeErrorInfo, const QString qstr_RuntimeErrorFunction)
{
    // 确保线程安全
    std::lock_guard<std::mutex> lock(this->m_mutex_runtimelog_);
    if (qstr_RuntimeErrorFunction.isEmpty() || qstr_RuntimeErrorInfo.isEmpty())
    {
        RuntimeLog::WriteLog(
            QString("Function=[RuntimeLog::WriteLogInfo] -- "),
            QString("Error=[Function parameter: The string is an empty string.]"),
            RuntimeLog::GetLogFilePathInfo());
    }
    else
    {
        RuntimeLog::WriteLog(
            QString("Function=[") + qstr_RuntimeErrorFunction + QString("] -- "),
            QString("Error=[") + qstr_RuntimeErrorInfo + QString("]"),
            RuntimeLog::GetLogFilePathInfo());
    }
}

void RuntimeLog::WriteLogMessage(const LogLevel logLevel, const QString& qstr_ErrorInfo, const QString& qstr_ErrorFunc)
{
    // 确保线程安全
    std::lock_guard<std::mutex> lock(this->m_mutex_runtimelog_);
    if (qstr_ErrorFunc.isEmpty() || qstr_ErrorInfo.isEmpty())
    {
        RuntimeLog::WriteLog(
            QString("Function=[RuntimeLog::WriteLogInfo] -- "),
            QString("Error=[Function parameter: The string is an empty string.]"),
            RuntimeLog::GetLogFilePathInfo());
    }
    else
    {
        RuntimeLog::WriteLog(
            QString("Function=[") + qstr_ErrorFunc + QString("] -- "),
            QString("Error=[") + qstr_ErrorInfo + QString("]"),
            RuntimeLog::GetLogFilePathInfo());
    }
}

void RuntimeLog::SetLogFilePath(QString qstr_FilePath)
{
    this->m_logFilePath = qstr_FilePath;
    return;
}

// 创建日志文件名称
QString RuntimeLog::BuildFileName()
{
    QString qstr_Buffer = GetCurrentDate() + QString(".ulog");
    return qstr_Buffer;
}

QString RuntimeLog::BuildLogInfo(QString& qstr_ErrorFunc, QString& qstr_ErrorInfo)
{
    QString qstr_InfoBuffer = GetCurrentDateTime() + qstr_ErrorFunc + qstr_ErrorInfo;
    return qstr_InfoBuffer;
}

QString RuntimeLog::GetCurrentDateTime()
{
    this->m_currentDateTime->setDate(QDate::currentDate());
    this->m_currentDateTime->setTime(QTime::currentTime());
    QString qstr_Buffer = QString("[") + this->m_currentDateTime->toString("yyyy-MM-dd hh:mm:ss") + QString("] -- ");
    return qstr_Buffer;
}

QString RuntimeLog::GetCurrentDate()
{
    this->m_currentDateTime->setDate(QDate::currentDate());
    this->m_currentDateTime->setTime(QTime::currentTime());
    QDate qdate = this->m_currentDateTime->date();
	QString qstr_Buffer = QString("%1-%2-%3").arg(qdate.year()).arg(qdate.month(), 2, 10, QChar('0')).arg(qdate.day(), 2, 10, QChar('0'));
    return qstr_Buffer;
}

QStringList RuntimeLog::ReadLog(const QString qstr_date)
{
    QStringList logList;
    // 处理路径
    QString qstr_FilePathBuffer = QDir::toNativeSeparators(this->m_logFilePath + qstr_date + QString(".ulog"));
	QFile file(qstr_FilePathBuffer);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine().trimmed();

            // 处理注释和空行
            if (line.isEmpty() || line.startsWith('#'))
                continue;
			logList.append(line);
        }
        file.close();
    }
    return logList;
}

QStringList RuntimeLog::ReadLog(const QDate qdate)
{
    QString qstr_date = QString("%1-%2-%3").arg(qdate.year()).arg(qdate.month(), 2, 10, QChar('0')).arg(qdate.day(), 2, 10, QChar('0'));
    QStringList logList;
    // 处理路径
    QString qstr_FilePathBuffer = QDir::toNativeSeparators(this->m_logFilePath + qstr_date + QString(".ulog"));
    QFile file(qstr_FilePathBuffer);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine().trimmed();

            // 处理注释和空行
            if (line.isEmpty() || line.startsWith('#'))
                continue;
            if (this->m_b_PrintInCmd)
            {
                qDebug() << line << Qt::endl;
            }
            logList.append(line);
        }
        file.close();
    }
    return logList;
}

bool RuntimeLog::InitializeLogFile(const QString& qstr_FilePath)
{
    QString qstr_FilePathBuffer = QDir::toNativeSeparators(qstr_FilePath);
    // 构建目录
    QDir dir;
    if (!dir.mkpath(QFileInfo(qstr_FilePathBuffer).absolutePath()))
    {
        return false;
    }
    QFile file(qstr_FilePathBuffer);
	if (!file.open(QIODevice::Append | QIODevice::Text))
	{
		return false;
	}
    else
    {
		file.close();
    }
	return true;
}

QString RuntimeLog::GetLogFilePathInfo()
{
    QString qstr_FileName = RuntimeLog::GetCurrentDate();
    QString qstr_FilePath = this->m_logFilePath + qstr_FileName + QString(".ulog");
    return qstr_FilePath;
}

void RuntimeLog::WriteLog(const QString& qstr_ErrorFunc, const QString& qstr_ErrorInfo, const QString& qstr_logFileName)
{
    // 如果创建不了文件夹，则不进行写入。
    if (InitializeLogFile(qstr_logFileName) == false)
    {
        return;
    }
    QFile file(qstr_logFileName);
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        QString qstr_LogInfo = RuntimeLog::GetCurrentDateTime() + qstr_ErrorFunc + qstr_ErrorInfo;
        out << qstr_LogInfo << Qt::endl;
    }
    file.close();
    return;
}

void RuntimeLog::ClearLogFile()
{

}