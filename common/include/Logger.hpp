#pragma once
#include <bitset>

enum class logType : uint8_t {
	MUTE = 0,
	ERROR = 1,
	WARN = 2,
	LOG = 3,
	INFO = 4,
	DEBUG = 5
};

class Logger {
private:
	using enum logType;

	static FILE* os; 
	static logType logLevel;
	static char prefix;
	static bool newline;
	
	static const constexpr char tokens[] = "krgybpcwKRGYBPCW";
	static const constexpr char logTypeColors[] = "kRYwyK";
	static const constexpr char* logTypeNames[] = {
		"mute", "error", "warn", "log", "info", "debug"
	};

	static char* logTypePrefix[]; 

	static char* createTypePrefix(logType type);
	static int getColorIndex(char colorChar);
	static void printColor(char colorChar);
	static void printColored(const char* str, size_t length, const logType type);
	
public:
	static void setLogLevel(logType type);
	static void setPrefix(const char prefix);
	static void setStream(FILE* newStream);

	static void print(const char* str, logType type, const char* fmt, ...);

	#define error(...)	print(__FUNCTION__, logType::ERROR, __VA_ARGS__)
	#define warn(...) print(__FUNCTION__, logType::WARN, __VA_ARGS__)
	#define log(...) print(__FUNCTION__, logType::LOG, __VA_ARGS__)
	#define debug(...) print(__FUNCTION__, logType::DEBUG, __VA_ARGS__)
	#define info(...) print(__FUNCTION__, logType::INFO, __VA_ARGS__)

};
