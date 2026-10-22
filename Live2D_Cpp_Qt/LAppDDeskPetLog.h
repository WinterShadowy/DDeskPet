// ===== Logger.h =====
#define _CRT_SECURE_NO_WARNINGS

#pragma once

#include "FileOperator.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <chrono>

enum class LogLevel {
    Debug = 0, Info = 1, Warning = 2, Error = 3, Fatal = 4
};

struct LogEntry {
    std::chrono::system_clock::time_point timestamp;
    LogLevel level;
    int line;
    std::thread::id threadId;
    std::string message;
    std::string file;
    std::string function;
};

// 日志输出接口（策略模式）
class ILogAppender {
public:
    virtual ~ILogAppender() = default;
    virtual void append(const LogEntry& entry) = 0;
    virtual void flush() {};
};

// 文件日志输出器 - 使用 PersistentFileOperator（高性能）
class FileLogAppender : public ILogAppender {
public:
    explicit FileLogAppender(const std::string& rootDir);
    ~FileLogAppender() override;

    void append(const LogEntry& entry) override;
    void flush() override;

private:
    std::string m_rootDir;
    std::unique_ptr<FileOperator::PersistentFileOperator> m_fileOp;
    std::string m_currentFilePath;
    std::chrono::system_clock::time_point m_lastCheck;

    std::string generateFilePath(const std::chrono::system_clock::time_point& tp) const;
    void rotateFileIfNeeded(const std::chrono::system_clock::time_point& tp);
};

// 控制台输出器
class ConsoleLogAppender : public ILogAppender {
public:
    void append(const LogEntry& entry) override;
};

// 日志核心类
class Logger {
public:
    static Logger& instance();

    static LogLevel valueToLogLevel(int val);

    // 初始化
    static void init(const char* rootDir, LogLevel minLevel = LogLevel::Debug);
    static void shutdown();
    static void setLevel(LogLevel minLevel);

    void log(LogLevel level, const std::string& msg,
        const char* file, int line, const char* func);

    void addAppender(std::unique_ptr<ILogAppender> appender);

private:
    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger* s_instance;
    static std::mutex s_instanceMutex;

    // 工作线程
    void workerLoop();
    void enqueue(LogEntry entry);

    std::queue<LogEntry> m_queue;
    std::mutex m_queueMutex;
    std::condition_variable m_condition;
    std::thread m_worker;
    std::atomic<bool> m_running{ true };
    std::atomic<bool> m_workerStarted{ false };

    LogLevel m_minLevel = LogLevel::Debug;
    std::vector<std::unique_ptr<ILogAppender>> m_appenders;
    std::mutex m_appendersMutex;
};

// 便捷宏
#define LOG_DEBUG(msg)  Logger::instance().log(LogLevel::Debug, (msg), __FILE__, __LINE__, __func__)
#define LOG_INFO(msg)   Logger::instance().log(LogLevel::Info, (msg), __FILE__, __LINE__, __func__)
#define LOG_WARN(msg)   Logger::instance().log(LogLevel::Warning, (msg), __FILE__, __LINE__, __func__)
#define LOG_ERROR(msg)  Logger::instance().log(LogLevel::Error, (msg), __FILE__, __LINE__, __func__)
#define LOG_FATAL(msg)  Logger::instance().log(LogLevel::Fatal, (msg), __FILE__, __LINE__, __func__)