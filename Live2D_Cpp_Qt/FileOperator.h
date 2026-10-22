
/*
 ============================================================================
 Header: FileOperator.h
 Description: 文件操作
 Start time: 2025.10.04
 End time: 2026.01.07
 Maintainer: WangSZ
 ============================================================================
*/

#pragma once

#include "json.hpp"

#include <QString>
#include <QDir>
#include <QFile>

#include <fstream>
#include <sstream>
#include <string>

namespace FileOperator
{
	// ========== 读操作 ==========
	nlohmann::json ReadFilleFromJson(const QString& filePath);


	// ========== 写操作 ==========
	bool WriteFilleToJson(const QString& filePath, const nlohmann::json& _json);

	// 文件打开模式
	enum class FileMode {
		ReadOnly,      // 只读
		WriteOnly,     // 只写（覆盖）
		Append,        // 追加
		ReadWrite      // 读写
	};

	// 文件操作器接口
	class IFileOperator
	{
	public:

		virtual ~IFileOperator() = default;
		// 文件操作
		// 写入文件
		virtual bool writeToFile(const std::string& data) = 0;
		virtual bool writeLine(const std::string& data) = 0;
		// 读取文件
		virtual bool readAll(std::string& out) = 0;
		virtual std::string readAll() = 0;
		virtual bool readLine(std::string& out) = 0;
		virtual std::string readLine() = 0;

		// 刷新文件缓冲区
		virtual bool flushFileBuffer() = 0;

		// 文件是否存在
		static bool existsFile(const std::string& path);

		// 创建目录（递归）
		static bool createDirectories(const std::string& path);

		// 获取文件大小
		static std::size_t fileSize(const std::string& path);
	};

	// ========== 即时打开关闭（RAII单次操作）模式==========

	class ImmediateFileOperator : public IFileOperator {
	public:
		explicit ImmediateFileOperator();
		explicit ImmediateFileOperator(const std::string& path, FileMode mode = FileMode::Append);
		
		~ImmediateFileOperator() override;

		// 写文件
		virtual bool writeToFile(const std::string& data) override;
		virtual bool writeLine(const std::string& line) override;
		// 读文件
		virtual bool readAll(std::string& out) override;
		virtual std::string readAll() override;
		virtual bool readLine(std::string& out) override;
		virtual std::string readLine() override;
		virtual bool flushFileBuffer() override;

		void setPath(const std::string& path);
		void setMode(FileMode mode);

	private:
		std::ios::openmode toStdMode(FileMode mode) const;

	private:

		std::string m_path;
		FileMode m_mode;
	};

	// ========== 持久打开（构造打开，析构关闭）模式==========

	class PersistentFileOperator : public IFileOperator {
	public:
		explicit PersistentFileOperator();
		explicit PersistentFileOperator(const std::string& path, FileMode mode = FileMode::Append);
		~PersistentFileOperator() override;

		// 禁止拷贝，允许移动
		PersistentFileOperator(const PersistentFileOperator&) = delete;
		PersistentFileOperator& operator=(const PersistentFileOperator&) = delete;
		PersistentFileOperator(PersistentFileOperator&& other) noexcept;
		PersistentFileOperator& operator=(PersistentFileOperator&& other) noexcept;

		virtual bool writeToFile(const std::string& data) override;
		virtual bool writeLine(const std::string& line) override;
		virtual bool readAll(std::string& out) override;
		virtual std::string readAll() override;
		virtual bool readLine(std::string& out) override;
		virtual std::string readLine() override;
		virtual bool flushFileBuffer() override;

		// 显式关闭（可提前关闭，析构时不再操作）
		void closeFile ();

		// 重新打开（如果已关闭）
		bool reopenFile();

		// 是否已打开
		bool isOpen() const;

		void setPath(const std::string& path);
		void setMode(FileMode mode);

	private:

		std::ios::openmode toStdMode(FileMode mode) const;
		bool _ensureOpen();

	private:
		std::string m_path;
		FileMode m_mode;
		std::fstream m_file;
		bool m_isOpen = false;
	};
};

namespace _FIO = FileOperator;