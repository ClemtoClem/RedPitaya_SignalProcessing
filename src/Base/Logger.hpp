/**
 * @brief Red Pitaya simple logger.
 * @author @ClemtoClem (https://github.com/ClemtoClem/RedPitaya)
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <ctime>


enum class LogLevel {
	Debug,
	Info,
	Warning,
	Error,
	Fatal
};

enum class CodeError {
	/** Success */
	OK   = 0,
	/** Failed to Open EEPROM Device */
	EOED = 1,
	/** Failed to Open Memory Device */
	EOMD = 2,
	/** Failed to Close Memory Device*/
	ECMD = 3,
	/** Failed to Map Memory Device */
	EMMD = 4,
	/** Failed to Unmap Memory Device */
	EUMD = 5,
	/** Value Out Of Range */
	EOOR = 6,
	/** LED Input Direction is not valid */
	ELID = 7,
	/** Modifying Read Only field */
	EMRO = 8,
	/** Writing to Input Pin is not valid */
	EWIP = 9,
	/** Invalid Pin number */
	EPN  = 10,
	/** Uninitialized Input Argument */
	UIA  = 11,
	/** Failed to Find Calibration Parameters */
	FCA  = 12,
	/** Failed to Read Calibration Parameters */
	RCA  = 13,
	/** Buffer too small */
	BTS  = 14,
	/** Invalid parameter value */
	EIPV = 15,
	/** Unsupported Feature */
	EUF  = 16,
	/** Data not normalized */
	ENN  = 17,
	/** Failed to open bus */
	EFOB = 18,
	/** Failed to close bus */
	EFCB = 19,
	/** Failed to acquire bus access */
	EABA = 20,
	/** Failed to read from the bus */
	EFRB = 21,
	/** Failed to write to the bus */
	EFWB = 22,
	/** Extension module not connected */
	EMNC = 23,
	/** Command not supported */
	NOTS = 24,
	/** Failed to open file */
	EFOF = 25,
	/** Failed to close file */
	EFCF = 26,
	/** Failed to read from file */
	EFRF = 27,
	/** Failed to write to file */
	EFWF = 28,


	UNKNOWN = 99
};

const std::string DEGUG_FILENAME = "debug.log";

class Logger {
private:
	Logger() {
		// delete file
		std::remove(DEGUG_FILENAME.c_str());
	}
	~Logger() {
		std::cerr << "\033[0m" << std::endl;
	}
public:
	static Logger& GetInstance();

	template<typename... Args>
	void Log(const char *file, int line, LogLevel level, CodeError code, Args&&... args);
	template<typename... Args>
	void Log(const char *file, int line, LogLevel level, Args&&... args);
	void Separator(char sig = '-');
	void BreakLine();

	LogLevel GetLastLogLevel() const { return _lastLogLevel; }
	CodeError GetLastCodeError() const { return _lastCodeError; }

private:
	std::string GetLogLevelString(LogLevel level);
	std::string GetCodeErrorString(CodeError code);
	std::string GetTimeString();
	std::string GetEscapeCodeColor(LogLevel level);

	void PrintLog(LogLevel level, const std::string& message);
	void WriteLog(const std::string& message);

	LogLevel _lastLogLevel;
	CodeError _lastCodeError;

};

#define LOG(level, ...) Logger::GetInstance().Log(__FILE__, __LINE__, level, __VA_ARGS__);
#define LOG_CODE(level, code, ...) Logger::GetInstance().Log(__FILE__, __LINE__, level, code, __VA_ARGS__);
#define LOG_SEPARATOR(sig) Logger::GetInstance().Separator(sig);
#define LOG_BREAK_LINE() Logger::GetInstance().BreakLine();

#endif /* LOGGER_HPP */