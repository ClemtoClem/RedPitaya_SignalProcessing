#include "Logger.hpp"
#include <time.h>
#include <sys/time.h>
#include <cstdarg>

Logger &Logger::GetInstance() {
	static Logger instance;
	return instance;
}

template<typename T>
static void AppendToStream(std::stringstream& ss, T&& arg) {
    ss << std::forward<T>(arg);
}

template<typename T, typename... Args>
static void AppendToStream(std::stringstream& ss, T&& arg, Args&&... args) {
    ss << std::forward<T>(arg) << " ";
    AppendToStream(ss, std::forward<Args>(args)...);
}

template<typename... Args>
void Logger::Log(const char *file, int line, LogLevel level, CodeError code, Args&&... args) {
	_lastLogLevel = level;
	_lastCodeError = code;
    std::stringstream ss;
    ss << "[" << file << ":" << line << "] " << GetLogLevelString(level) << ": " << GetCodeErrorString(code) << ": ";
	AppendToStream(ss, std::forward<Args>(args)...);

    PrintLog(level, ss.str());
    WriteLog(ss.str());
}

template<typename... Args>
void Logger::Log(const char *file, int line, LogLevel level, Args&&... args) {
	_lastLogLevel = level;
	_lastCodeError = CodeError::UNKNOWN;
    std::stringstream ss;
    ss << "[" << file << ":" << line << "] " << GetLogLevelString(level) << ": ";
	AppendToStream(ss, std::forward<Args>(args)...);

    PrintLog(level, ss.str());
    WriteLog(ss.str());
}

void Logger::Separator(char sig) {
	std::string sep(60, sig);
	PrintLog(LogLevel::Info, sep);
	WriteLog(sep);
}

void Logger::BreakLine() {
	std::cout << std::endl;
	WriteLog("");
}

std::string Logger::GetLogLevelString(LogLevel level) {
	std::string levelString;
	switch(level) {
		case LogLevel::Debug:
			levelString = "DEBUG";
			break;
		case LogLevel::Info:
			levelString = "INFO";
			break;
		case LogLevel::Warning:
			levelString = "WARN";
			break;
		case LogLevel::Error:
			levelString = "ERROR";
			break;
		case LogLevel::Fatal:
			levelString = "FATAL";
			break;
		default:
			levelString = "UNKNOWN";
			break;
	}
	return levelString;
}

std::string Logger::GetCodeErrorString(CodeError code) {
	std::string codeString;
	switch (code) {
		case CodeError::OK:
			codeString = "Success";
			break;
		case CodeError::EOED:
			codeString = "Failed to Open EEPROM Device";
			break;
		case CodeError::EOMD:
			codeString = "Failed to Open Memory Device";
			break;
		case CodeError::ECMD:
			codeString = "Failed to Close Memory Device";
			break;
		case CodeError::EMMD:
			codeString = "Failed to Map Memory Device";
			break;
		case CodeError::EUMD:
			codeString = "Failed to Unmap Memory Device";
			break;
		case CodeError::EOOR:
			codeString = "Value Out Of Range";
			break;
		case CodeError::ELID:
			codeString = "LED Input Direction is not valid";
			break;
		case CodeError::EMRO:
			codeString = "Modifying Read Only field";
			break;
		case CodeError::EWIP:
			codeString = "Writing to Input Pin is not valid";
			break;
		case CodeError::EPN:
			codeString = "Invalid Pin number";
			break;
		case CodeError::UIA:
			codeString = "Uninitialized Input Argument";
			break;
		case CodeError::FCA:
			codeString = "Failed to Find Calibration Parameters";
			break;
		case CodeError::RCA:
			codeString = "Failed to Read Calibration Parameters";
			break;
		case CodeError::BTS:
			codeString = "Buffer too small";
			break;
		case CodeError::EIPV:
			codeString = "Invalid parameter value";
			break;
		case CodeError::EUF:
			codeString = "Unsupported Feature";
			break;
		case CodeError::ENN:
			codeString = "Data not normalized";
			break;
		case CodeError::EFOB:
			codeString = "Failed to open bus";
			break;
		case CodeError::EFCB:
			codeString = "Failed to close bus";
			break;
		case CodeError::EABA:
			codeString = "Failed to acquire bus access";
			break;
		case CodeError::EFRB:
			codeString = "Failed to read from the bus";
			break;
		case CodeError::EFWB:
			codeString = "Failed to write to the bus";
			break;
		case CodeError::EMNC:
			codeString = "Extension module not connected";
			break;
		case CodeError::NOTS:
			codeString = "Command not supported";
			break;
		case CodeError::EFOF:
			codeString = "Failed to open file";
			break;
		case CodeError::EFCF:
			codeString = "Failed to close file";
			break;
		case CodeError::EFRF:
			codeString = "Failed to read from the file";
			break;
		case CodeError::EFWF:
			codeString = "Failed to write to the file";
			break;
		case CodeError::UNKNOWN:
		default:
			codeString = "Unknown";
			break;
	}
	return codeString;
}

std::string Logger::GetTimeString() {
	std::stringstream ss;
	time_t now = time(nullptr);
	struct tm* timeinfo = localtime(&now);
	char date[20];
	strftime(date, sizeof(date), "%y-%m-%d %H:%M:%S", timeinfo);
	ss << "[" << date << "]";
	return ss.str();
}

std::string Logger::GetEscapeCodeColor(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:   return "\033[36m"; // Cyan
        case LogLevel::Info:    return "\033[32m"; // Green
        case LogLevel::Warning: return "\033[33m"; // Yellow
        case LogLevel::Error:   return "\033[31m"; // Red
        case LogLevel::Fatal:   return "\033[41m"; // Red background
        default:                return "\033[0m";  // Reset
    }
}

void Logger::PrintLog(LogLevel level, const std::string &message)
{
	std::cout << GetEscapeCodeColor(level) << message << std::endl;
}

void Logger::WriteLog(const std::string &message) {
	std::ofstream file(DEGUG_FILENAME, std::ios::app);
	if (file.is_open()) {
		file << GetTimeString() << " " << message << std::endl;
		file.close();
	}
}
