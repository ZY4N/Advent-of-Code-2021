#include <Logger.hpp>

#include <algorithm>
#include <stdarg.h>

uint8_t Logger::logLevel = 6;

bool Logger::logType = true;
bool Logger::logColors = true;
char Logger::prefix = '|';

void Logger::setLogLevel(const int _logLevel) {
	logLevel = (uint8_t)_logLevel;
}

void Logger::setPrefix(const char _prefix) {
	prefix = _prefix;
}

void Logger::showType(bool show) {
	logType = show;
}

void Logger::showColor(bool show) {
	logColors = show;
}

void Logger::printColored(const char* str, size_t length, const uint8_t level) {
	
	const auto printColor = [](char colorChar) constexpr {
		int index = -1;
		for (int i = 0; i < sizeof(tokens) - 1; i++) {
			if (tokens[i] == colorChar) {
				index = i;
				break;
			}
		}
		if (index >= 0) {
			char color[] = { 27, 91, 51, (char)(48 + index), 59, 49, 109 };
			size_t colorLen = sizeof(color);
			if (index < sizeof(tokens) / 2) {
				color[4] = 109;
				colorLen -= 2;
			} else {
				color[3] -= sizeof(tokens) / 2;
			}
			fwrite(color , sizeof(char), colorLen, stdout);
		}
	};

	size_t firstToken = 0, lastIndex = 0;
	while(firstToken < length && str[firstToken] != prefix)
		firstToken++;

	if (firstToken == length) {
		printColor(defaultColors[level]);
		fwrite(str, sizeof(char), length, stdout);
	} else {
		for (size_t i = firstToken; i < length; i++) {
			if (str[i] == prefix && i + 1 < length) {
				fwrite(&str[lastIndex] , sizeof(char), i++ - lastIndex, stdout);
				printColor(str[i]);
				lastIndex = i + 1;
			}
		}
		fwrite(&str[lastIndex] , sizeof(char), length - lastIndex, stdout);
	}
	fwrite("\x001B[0m" , sizeof(char), 5, stdout);
}

void Logger::debug(const char* fmt, ...) {
	if (logLevel >= 4) {
		va_list args, copyArg;

		va_start(args, fmt);
		va_copy(copyArg, args);
		size_t strLen = std::vsnprintf(NULL, 0, fmt, copyArg);
		va_end(copyArg);

		char* str = new char[strLen];
		std::vsnprintf(str, strLen, fmt, args);
		va_end(args);
		
		if (logType)
			fputs("[debug] " , stdout);

		printColored(str, strLen, 3);
	
		delete[] str;
	}
}

void Logger::log(const char* fmt, ...) {
	if (logLevel >= 4) {
		va_list args, copyArg;

		va_start(args, fmt);
		va_copy(copyArg, args);
		size_t strLen = std::vsnprintf(NULL, 0, fmt, copyArg);
		va_end(copyArg);

		char* str = new char[strLen + 1];
		std::vsnprintf(str, strLen + 1, fmt, args);
		va_end(args);
		
		if (logType)
			fputs("[log] " , stdout);

		printColored(str, strLen, 2);
	
		delete[] str;
	}
}

void Logger::warn(const char* fmt, ...) {
	if (logLevel >= 4) {
		va_list args, copyArg;

		va_start(args, fmt);
		va_copy(copyArg, args);
		size_t strLen = std::vsnprintf(NULL, 0, fmt, copyArg);
		va_end(copyArg);

		char* str = new char[strLen];
		std::vsnprintf(str, strLen, fmt, args);
		va_end(args);
		
		if (logType)
			fputs("[warn] " , stdout);

		printColored(str, strLen, 1);
	
		delete[] str;
	}
}
