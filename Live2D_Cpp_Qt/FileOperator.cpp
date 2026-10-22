#include "FileOperator.h"

nlohmann::json FileOperator::ReadFilleFromJson(const QString& filePath)
{
	QString qstr_FilePathBuffer = QDir(QDir::toNativeSeparators(filePath)).absolutePath();
	QFile file(qstr_FilePathBuffer);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		nlohmann::json _json;
		_json["Error"] = std::string("Failed to open the target configuration file. Function: ReadFilleFromJson");
		return _json;
	}
	nlohmann::json _json;
	try {
		QByteArray byteArray = file.readAll();
		file.close();
		std::string str_Json = byteArray.toStdString();
		_json = nlohmann::json::parse(str_Json);
	}
	catch (nlohmann::json::parse_error& e)
	{
		nlohmann::json _json;
		_json["Error"] = std::string(e.what());
		return _json;
	}
    return _json;
}

bool FileOperator::WriteFilleToJson(const QString& filePath, const nlohmann::json& _json)
{
	QString qstr_FilePathBuffer = QDir::toNativeSeparators(filePath);
	QFile file(qstr_FilePathBuffer);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		nlohmann::json _json;
		_json["Error"] = std::string("Failed to open the target configuration file. Function: WriteFilleToJson");
		return _json;
		return false;
	}

	// 将 JSON 对象转为字符串
	QByteArray jsonString = QString::fromStdString(_json.dump(4)).toUtf8(); // 4表示缩进空格数

	// 写入文件
	file.write(jsonString);
	file.close();
	return true;
}



bool FileOperator::IFileOperator::existsFile(const std::string& path)
{
	return std::filesystem::exists(path);
}

bool FileOperator::IFileOperator::createDirectories(const std::string& path)
{
	try
	{
		std::filesystem::create_directories(path);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

std::size_t FileOperator::IFileOperator::fileSize(const std::string& path)
{
	try 
	{
		return std::filesystem::file_size(path);
	}
	catch (...) 
	{
		return 0;
	}
}

// ImmediateFileOperator
FileOperator::ImmediateFileOperator::ImmediateFileOperator()
{
}

FileOperator::ImmediateFileOperator::ImmediateFileOperator(const std::string& path, FileMode mode) :
	m_mode(mode),
	m_path(path)
{
}

FileOperator::ImmediateFileOperator::~ImmediateFileOperator()
{
}

bool FileOperator::ImmediateFileOperator::writeToFile(const std::string& data)
{
	std::ofstream _file(this->m_path, toStdMode(this->m_mode));
	if (_file.is_open())
	{
		_file << data;
		_file.close();
		return true;
	}
	return false;
}

bool FileOperator::ImmediateFileOperator::writeLine(const std::string& line)
{
	std::ofstream _file(m_path, toStdMode(FileMode::Append));
	if (_file.is_open())
	{
		_file << line << '\n';
	}
	return _file.good();
}

bool FileOperator::ImmediateFileOperator::readAll(std::string& out)
{
	std::ifstream _file(m_path, std::ios::in | std::ios::binary);
	if (!_file.is_open()) 
		return false;
	out.assign((std::istreambuf_iterator<char>(_file)),
		std::istreambuf_iterator<char>());
	return _file.good();
}

std::string FileOperator::ImmediateFileOperator::readAll()
{
	std::ifstream _file(m_path, std::ios::in | std::ios::binary);
	if (!_file.is_open())
		return std::string();
	std::stringstream _stream;
	_stream << _file.rdbuf();
	std::string _file_str = _stream.str();
	return _file_str;
}

bool FileOperator::ImmediateFileOperator::readLine(std::string& out)
{
	std::ifstream _file(m_path, std::ios::in);
	if (!_file.is_open()) 
		return false;
	return static_cast<bool>(std::getline(_file, out));
}

std::string FileOperator::ImmediateFileOperator::readLine()
{
	std::ifstream _file(m_path, std::ios::in);
	if (!_file.is_open())
		return std::string();
	std::string _line;
	std::getline(_file, _line);
	return _line;
}

bool FileOperator::ImmediateFileOperator::flushFileBuffer()
{
	std::ofstream _file(m_path, std::ios::out | std::ios::trunc);
	if (_file.is_open())
	{
		_file.close();
		return true;
	}
	return false;
}

std::ios::openmode FileOperator::ImmediateFileOperator::toStdMode(FileMode mode) const
{
	switch (mode)
	{
	case FileMode::ReadOnly:
		return std::ios::in;
	case FileMode::WriteOnly:
		return std::ios::out | std::ios::trunc;
	case FileMode::Append:
		return std::ios::out | std::ios::app;
	case FileMode::ReadWrite:
		return std::ios::in | std::ios::out;
	default:
		return std::ios::out | std::ios::app;
	}
}

FileOperator::PersistentFileOperator::PersistentFileOperator()
{
}

FileOperator::PersistentFileOperator::PersistentFileOperator(const std::string& path, FileMode mode) : 
	m_mode(mode), 
	m_path(path)
{
	_ensureOpen();
}

FileOperator::PersistentFileOperator::~PersistentFileOperator()
{
	closeFile();
	return;
}

FileOperator::PersistentFileOperator::PersistentFileOperator(PersistentFileOperator&& other) noexcept : 
	m_path(std::move(other.m_path)), 
	m_mode(other.m_mode), 
	m_file(std::move(other.m_file)), 
	m_isOpen(other.m_isOpen) 
{
		other.m_isOpen = false;
}

FileOperator::PersistentFileOperator& FileOperator::PersistentFileOperator::operator=(PersistentFileOperator&& other) noexcept
{
	if (this != &other) 
	{
		closeFile();
		m_path = std::move(other.m_path);
		m_mode = other.m_mode;
		m_file = std::move(other.m_file);
		m_isOpen = other.m_isOpen;
		other.m_isOpen = false;
	}
	return *this;
}

bool FileOperator::PersistentFileOperator::writeToFile(const std::string& data)
{
	if (!_ensureOpen()) 
		return false;
	this->m_file << data;
	return this->m_file.good();
}

bool FileOperator::PersistentFileOperator::writeLine(const std::string& line)
{
	if (!_ensureOpen()) 
		return false;
	this->m_file << line << '\n';
	return this->m_file.good();
}

bool FileOperator::PersistentFileOperator::readAll(std::string& out)
{
	if (!_ensureOpen()) 
		return false;

	// 先 flush 写缓冲区（如果是 fstream，确保写入的数据落盘）
    m_file.flush();

    // 从文件头开始测大小、读取
    m_file.seekg(0, std::ios::end);
    auto size = m_file.tellg();
    m_file.seekg(0, std::ios::beg);

	if (size > 0) 
	{
		out.resize(static_cast<size_t>(size));
		m_file.read(out.data(), size);
	}
	return m_file.good();
}

std::string FileOperator::PersistentFileOperator::readAll()
{
	return std::string();
}

bool FileOperator::PersistentFileOperator::readLine(std::string& out)
{
	if (!_ensureOpen())
		return false;
	return static_cast<bool>(std::getline(m_file, out));
}

std::string FileOperator::PersistentFileOperator::readLine()
{
	if(!_ensureOpen())
		return std::string();
	std::string _line;
	std::getline(m_file, _line);
	return _line;
}

bool FileOperator::PersistentFileOperator::flushFileBuffer()
{
	if (!m_isOpen) 
		return false;
	m_file.flush();
	return m_file.good();
}

void FileOperator::PersistentFileOperator::closeFile()
{
	if (this->m_file.is_open())
	{
		this->m_file.close();
		this->m_isOpen = false;
	}
}

bool FileOperator::PersistentFileOperator::reopenFile()
{
	if (this->m_file.is_open())
	{
		this->m_file.close();
		this->m_isOpen = false;
	}
	this->m_file.open(this->m_path, toStdMode(this->m_mode));
	if (this->m_file.is_open())
	{
		this->m_isOpen = true;
		return true;
	}
	return false;
}

bool FileOperator::PersistentFileOperator::isOpen() const
{
	return this->m_isOpen;
}

void FileOperator::ImmediateFileOperator::setPath(const std::string& path)
{
	this->m_path = path;
	return;
}

void FileOperator::ImmediateFileOperator::setMode(FileMode mode)
{
	this->m_mode = mode;
	return;
}

std::ios::openmode FileOperator::PersistentFileOperator::toStdMode(FileMode mode) const
{
	switch (mode) 
	{
		case FileMode::ReadOnly:       
			return std::ios::in;
		case FileMode::WriteOnly:      
			return std::ios::out | std::ios::trunc;
		case FileMode::Append:     
			return std::ios::out | std::ios::app;
		case FileMode::ReadWrite:  
			return std::ios::in | std::ios::out;
		default:                   
			return std::ios::out | std::ios::app;
	}
}

bool FileOperator::PersistentFileOperator::_ensureOpen()
{
	if (m_isOpen)
		return true;

	// 确保目录存在
	std::filesystem::path parentPath = std::filesystem::path(m_path).parent_path();
	if (!parentPath.empty()) 
	{
		createDirectories(parentPath.string());
	}

	m_file.open(m_path, toStdMode(m_mode));
	m_isOpen = m_file.is_open();
	return m_isOpen;
}
