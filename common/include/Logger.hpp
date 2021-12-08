#pragma once
#include <string>
#include <bitset>
#include <stdarg.h>

class Logger {
private:
	static uint8_t logLevel;
	static char prefix;
	static bool newline;
	static uint8_t lastLogType;
	
	static constexpr char tokens[] = "krgybpcwKRGYBPCW";
	static constexpr char defaultColors[] = "RYwK";

	static void printColored(const char* str, size_t length, const uint8_t level);

public:
	static void setLogLevel(const int logLevel);
	static void setPrefix(const char prefix);

	static void debug(const char* fmt, ...);
	static void log(const char* fmt, ...);
	static void warn(const char* fmt, ...);
	static void error(const char funcName[], int line, const char* fmt, ...) {
		if (logLevel >= 1) {
			va_list args, copyArg;

			va_start(args, fmt);
			va_copy(copyArg, args);
			size_t strLen = std::vsnprintf(NULL, 0, fmt, copyArg);
			va_end(copyArg);

			char* str = new char[strLen];
			std::vsnprintf(str, strLen, fmt, args);
			va_end(args);

			printf("[error][%s][%d]", funcName, line);

			printColored(str, strLen , 0);
		
			delete[] str;
		}
	}
	#define error(input) error(__FUNCTION__, __LINE__, input);
};
