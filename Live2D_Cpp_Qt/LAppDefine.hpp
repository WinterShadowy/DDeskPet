/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */
#pragma once

 // 日志头文件
#include "LAppDDeskPetLog.h"

#include <CubismFramework.hpp>
#include <utility>
#include <QString>
#include <algorithm>
#include <QDir>
#include <mutex>
/**
* @brief  Sample Appで使用する定数
*
*/

#define MOUSE_VIEW_TOUCHESMOVED_LOCALIZED_SCALE 1        // 鼠标指针在模型周围移动时有效
#define MOUSE_VIEW_TOUCHESMOVED_GLOBAL_SCALE    2        // 鼠标指针全局有效

// 显示消息的样式
#define SHOW_MESSAGE_STYLE_DISPLAY_TEXT_STYLE       0x01
#define SHOW_MESSAGE_STYLE_DISPLAY_TEXT_DIRECTLY    0x02
#define SHOW_MESSAGE_STYLE_MIXED_DISPLAY_TEXT       0x03
#define SHOW_MESSAGE_STYLE_NOT_DISPLAY_TEXT         0x04

namespace LAppDefine {

    using namespace Csm;

    extern const csmFloat32 ViewScale;              ///< 拡大縮小率
    extern const csmFloat32 ViewMaxScale;           ///< 拡大縮小率の最大値
    extern const csmFloat32 ViewMinScale;           ///< 拡大縮小率の最小値

    extern const csmFloat32 ViewLogicalLeft;        ///< 論理的なビュー座標系の左端の値
    extern const csmFloat32 ViewLogicalRight;       ///< 論理的なビュー座標系の右端の値
    extern const csmFloat32 ViewLogicalBottom;      ///< 論理的なビュー座標系の下端の値
    extern const csmFloat32 ViewLogicalTop;         ///< 論理的なビュー座標系の上端の値

    extern const csmFloat32 ViewLogicalMaxLeft;     ///< 論理的なビュー座標系の左端の最大値
    extern const csmFloat32 ViewLogicalMaxRight;    ///< 論理的なビュー座標系の右端の最大値
    extern const csmFloat32 ViewLogicalMaxBottom;   ///< 論理的なビュー座標系の下端の最大値
    extern const csmFloat32 ViewLogicalMaxTop;      ///< 論理的なビュー座標系の上端の最大値

    extern const csmChar* ResourcesPath;            ///< 素材パス
    extern const csmChar* BackImageName;         ///< 背景画像ファイル
    extern const csmChar* GearImageName;         ///< 歯車画像ファイル
    extern const csmChar* PowerImageName;        ///< 終了ボタン画像ファイル

    // 改
    extern const csmChar* ShaderPath;               ///< シェーダーパス
    extern const csmChar* ShaderName;               ///< シェーダー本体

    // モデル定義--------------------------------------------
                                                    // 外部定義ファイル(json)と合わせる
    extern const csmChar* MotionGroupIdle;          ///< アイドリング時に再生するモーションのリスト
    extern const csmChar* MotionGroupTapBody;       ///< 体をタップした時に再生するモーションのリスト

                                                    // 外部定義ファイル(json)と合わせる
    extern const csmChar* HitAreaNameHead;          ///< 当たり判定の[Head]タグ
    extern const csmChar* HitAreaNameBody;          ///< 当たり判定の[Body]タグ

                                                    // モーションの優先度定数
    extern const csmInt32 PriorityNone;             ///< モーションの優先度定数: 0
    extern const csmInt32 PriorityIdle;             ///< モーションの優先度定数: 1
    extern const csmInt32 PriorityNormal;           ///< モーションの優先度定数: 2
    extern const csmInt32 PriorityForce;            ///< モーションの優先度定数: 3

    extern const csmBool MocConsistencyValidationEnable; ///< MOC3の整合性検証機能の有効・無効

                                                    // デバッグ用ログの表示
    extern const csmBool DebugLogEnable;            ///< デバッグ用ログ表示の有効・無効
    extern const csmBool DebugTouchLogEnable;       ///< タッチ処理のデバッグ用ログ表示の有効・無効

    // Frameworkから出力するログのレベル設定
    extern const CubismFramework::Option::LogLevel CubismLoggingLevel;
    
    // デフォルトのレンダーターゲットサイズ
    //SDD
    //extern const csmInt32 RenderTargetWidth;
    //extern const csmInt32 RenderTargetHeight;
    //SDE

    //SDA
    extern csmInt32 RenderTargetWidth;
    extern csmInt32 RenderTargetHeight;
    //SDE

    /*
    服务于main.cpp - RunApplication函数
    当窗口退出时检测是重启还是退出
    */
    enum WINDOW_EXITED_RETURN {
        WINDOW_EXITED_RETURN_RESTART = 1,       // 当窗口关闭时，返回此值可以重启窗口
        WINDOW_EXITED_RETURN_EXIT = 2           // 当窗口退出时，返回此值会退出程序
    };

    // 控制模型眼睛看向的方向的模式，是小范围有效还是全局有效
    

    extern std::pair <QString, int> Mouse_View_TouchesMoved_Localized_Scale;
	extern std::pair <QString, int> Mouse_View_TouchesMoved_Global_Scale;

}
// 定义字符串常量
#define MOUSE_VIEW_TOUCHESMOVED_LOCALIZED_SCALE_QSTR    "MOUSE_VIEW_TOUCHESMOVED_LOCALIZED_SCALE"
#define MOUSE_VIEW_TOUCHESMOVED_GLOBAL_SCALE_QSTR       "MOUSE_VIEW_TOUCHESMOVED_GLOBAL_SCALE"


#include <QDateTime>
class RuntimeLog {
public:
    enum LogLevel {
		LOG_LEVEL_INFO      = 0,
        LOG_LEVEL_DEBUG     = 1,
		LOG_LEVEL_WARNING   = 2,
		LOG_LEVEL_ERROR     = 3,
        LOG_LEVEL_FATAL     = 4
    };

    RuntimeLog();
    ~RuntimeLog();

    static RuntimeLog& instance();

    void InitializeRuntimeLog();
    void Destory();

    QString GetLogFilePath() const;
    
    void PrintErrorLogs_QDebug(const QStringList& qsl_ErrorLogs);

    // 写日志信息
    void WriteLogInfo(const QString qstr_RuntimeErrorInfo, const QString qstr_RuntimeErrorFunction);

    void WriteLogMessage(const LogLevel logLevel, const QString& qstr_ErrorInfo, const QString& qstr_ErrorFunc);
    
    void SetLogFilePath(QString qstr_FilePath);

private:

    // 构建日志文件名
    __declspec(deprecated("This function(BuildFileName) has been deprecated."))
    QString BuildFileName();
    // 构建日志信息
    __declspec(deprecated("This function(BuildLogInfo) has been deprecated."))
    QString BuildLogInfo(QString& qstr_ErrorFunc, QString& qstr_ErrorInfo);
    // 获取当前时间
    QString GetCurrentDateTime();
    // 获取日期
	QString GetCurrentDate();

    QStringList ReadLog(const QString qstr_date);
    QStringList ReadLog(const QDate qdate);

    bool InitializeLogFile(const QString& qstr_FilePath);  // 初始化日志文件

    QString GetLogFilePathInfo();
    void WriteLog(const QString& qstr_ErrorFunc, const QString& qstr_logInfo, const QString& qstr_logFileName);  // 写入日志

    void ClearLogFile();  // 清除日志内容
    RuntimeLog(const RuntimeLog&) = delete;
    RuntimeLog& operator=(const RuntimeLog&) = delete;

private:

    std::mutex m_mutex_runtimelog_;
	QString m_logFilePath;  // 日志文件路径
	QString m_logFileName;  // 日志文件名
    QDateTime* m_currentDateTime;  // 当前时间
    bool m_b_PrintInCmd = false;
};

#define LOG_LEVEL_INFO      (RuntimeLog::LogLevel::LOG_LEVEL_INFO)
#define LOG_LEVEL_DEBUG     (RuntimeLog::LogLevel::LOG_LEVEL_DEBUG)
#define LOG_LEVEL_WARNING   (RuntimeLog::LogLevel::LOG_LEVEL_WARNING)
#define LOG_LEVEL_ERROR     (RuntimeLog::LogLevel::LOG_LEVEL_ERROR)
#define LOG_LEVEL_FATIAL    (RuntimeLog::LogLevel::LOG_LEVEL_FATAL)

//#define RuntimeLog_WriteLogInfo(qstr_RuntimeErrorInfo, qstr_RuntimeErrorFunction) \
//    RuntimeLog::instance().WriteLogInfo(qstr_RuntimeErrorInfo, qstr_RuntimeErrorFunction)

#define RuntimeLog_Info(msg)    (LOG_INFO(msg))
#define RuntimeLog_Debug(msg)   (LOG_DEBUG(msg))
#define RuntimeLog_Warn(msg)    (LOG_WARN(msg))
#define RuntimeLog_Error(msg)   (LOG_ERROR(msg))
#define RuntimeLog_Fatal(msg)   (LOG_FATAL(msg))