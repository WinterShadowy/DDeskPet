

#include "LAppDDeskPetLog.h"
#include <iostream>
#include <sstream>
#include <iomanip>


// ---------- ¹¤¾ßº¯Êý ----------

static std::string levelToString(LogLevel level) 
{
    switch (level) 
    {
        case LogLevel::Debug:   
            return "DEBUG";
        case LogLevel::Info:    
            return "INFO";
        case LogLevel::Warning: 
            return "WARN";
        case LogLevel::Error:   
            return "ERROR";
        case LogLevel::Fatal:   
            return "FATAL";
    }
    return "UNKNOWN";
}

static std::string timePointToString(const std::chrono::system_clock::time_point& tp) 
{
    time_t time = std::chrono::system_clock::to_time_t(tp);
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        tp.time_since_epoch()) % 1000;

    std::stringstream _ss;
    _ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    _ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return _ss.str();
}

// ---------- FileLogAppender ----------

FileLogAppender::FileLogAppender(const std::string& rootDir) : 
    m_rootDir(rootDir) 
{
}

FileLogAppender::~FileLogAppender() 
{
    flush();
}

std::string FileLogAppender::generateFilePath(
    const std::chrono::system_clock::time_point& tp) const 
{

    time_t time = std::chrono::system_clock::to_time_t(tp);
    tm tm = *std::localtime(&time);

    std::stringstream _ss;
    _ss << m_rootDir << '/'
        << std::put_time(&tm, "%Y") << '/'
        << std::put_time(&tm, "%m") << '/'
        << std::put_time(&tm, "%Y-%m-%d") << ".log";
    return _ss.str();
}

void FileLogAppender::rotateFileIfNeeded(
    const std::chrono::system_clock::time_point& tp) 
{

    auto newPath = generateFilePath(tp);
    if (newPath != m_currentFilePath) 
    {
        this->m_fileOp = std::make_unique<FileOperator::PersistentFileOperator>(newPath, FileOperator::FileMode::Append);
        this->m_currentFilePath = newPath;
    }
}

void FileLogAppender::append(const LogEntry& entry) 
{
    rotateFileIfNeeded(entry.timestamp);

    std::stringstream _ss;
    _ss << '[' << timePointToString(entry.timestamp) << "] "
        << '[' << std::left << std::setw(5) << levelToString(entry.level) << "] "
        << "[Thread_ID:" << entry.threadId << "] "
        << entry.file << ':' << entry.line << ' '
        << entry.function << " | "
        << entry.message << '\n';

    this->m_fileOp->writeToFile(_ss.str());
}

void FileLogAppender::flush() 
{
    if (this->m_fileOp)
    {
        this->m_fileOp->flushFileBuffer();
    }
}

// ---------- ConsoleLogAppender ----------

void ConsoleLogAppender::append(const LogEntry& entry) 
{
    std::stringstream _ss;
    _ss << '[' << timePointToString(entry.timestamp) << "] "
        << '[' << levelToString(entry.level) << "] "
        << entry.message << '\n';

    std::cout << _ss.str();
}

// ---------- Logger ----------

Logger* Logger::s_instance = nullptr;
std::mutex Logger::s_instanceMutex;

Logger& Logger::instance() 
{
    std::lock_guard<std::mutex> lock(s_instanceMutex);
    if (!s_instance) 
    {
        s_instance = new Logger();
    }
    return *s_instance;
}

LogLevel Logger::valueToLogLevel(int val)
{
    switch (val)
    {
    case 0:
        return LogLevel::Info;
    case 1:
        return LogLevel::Debug;
    case 2:
		return LogLevel::Warning;
    case 3:
		return LogLevel::Error;
	case 4:
		return LogLevel::Fatal;
    }
    return LogLevel::Info;
}

void Logger::init(const char* rootDir, LogLevel minLevel) 
{
    Logger& _logger = instance();
    _logger.m_minLevel = minLevel;
    _logger.addAppender(std::make_unique<FileLogAppender>(rootDir));
}

void Logger::shutdown() 
{
    std::lock_guard<std::mutex> lock(s_instanceMutex);
    delete s_instance;
    s_instance = nullptr;
}

void Logger::setLevel(LogLevel minLevel) 
{
    instance().m_minLevel = minLevel;
}

Logger::Logger() = default;

Logger::~Logger() 
{
    this->m_running = false;
    this->m_condition.notify_all();
    if (this->m_worker.joinable())
    {
        this->m_worker.join();
    }
}

void Logger::addAppender(std::unique_ptr<ILogAppender> appender) 
{
    std::lock_guard<std::mutex> lock(m_appendersMutex);
    this->m_appenders.push_back(std::move(appender));
}

void Logger::enqueue(LogEntry entry) 
{
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        this->m_queue.push(std::move(entry));
    }
    this->m_condition.notify_one();
}

void Logger::workerLoop() 
{
    while (m_running) 
    {
        std::queue<LogEntry> localQueue;

        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_condition.wait(lock, [this] {
                return !m_running || !m_queue.empty();
                });

            if (!m_running && m_queue.empty()) 
                break;

            localQueue.swap(m_queue);
        }

        std::lock_guard<std::mutex> lock(m_appendersMutex);
        while (!localQueue.empty()) 
        {
            const auto& entry = localQueue.front();
            for (auto& appender : m_appenders) 
            {
                appender->append(entry);
            }
            localQueue.pop();
        }

        for (auto& appender : m_appenders) 
        {
            appender->flush();
        }
    }
}

void Logger::log(LogLevel level, const std::string& msg,
    const char* file, int line, const char* func) 
{
    if (level < m_minLevel) 
        return;

    if (!m_workerStarted.exchange(true)) 
    {
        m_worker = std::thread(&Logger::workerLoop, this);
    }

    LogEntry entry;
    entry.timestamp = std::chrono::system_clock::now();
    entry.level = level;
    entry.message = msg;
    entry.file = file;
    entry.line = line;
    entry.function = func;
    entry.threadId = std::this_thread::get_id();

    enqueue(std::move(entry));
}