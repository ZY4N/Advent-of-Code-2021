#include <Logger.hpp>

#include <stdarg.h>

logType Logger::logLevel = logType::DEBUG;

char Logger::prefix = '|';

bool Logger::newline = true;

FILE* Logger::os = stdout;

char* Logger::logTypePrefix[]{
	createTypePrefix(logType::MUTE),
	createTypePrefix(logType::ERROR),
	createTypePrefix(logType::WARN),
	createTypePrefix(logType::LOG),
	createTypePrefix(logType::INFO),
	createTypePrefix(logType::DEBUG)
};


void Logger::setLogLevel(logType newLogLevel) {
	logLevel = newLogLevel;
}

void Logger::setPrefix(const char newPrefix) {
	prefix = newPrefix;
}

void Logger::setStream(FILE* newStream) {
	os = newStream;
}

int Logger::getColorIndex(char colorChar) {
	for (int i = 0; i < sizeof(tokens) - 1; i++) {
		if (tokens[i] == colorChar) {
			return i;
		}
	}
	return -1;
}

char* Logger::createTypePrefix(logType type) {
	int colorIndex = getColorIndex(logTypeColors[(size_t)type]);
	bool dark = colorIndex < sizeof(tokens) / 2;
	colorIndex += dark ? 30 : 22;
	const char fmt[] = "[\x001B[%d%sm%s\x001B[0m][\u001b[36;1m";

	size_t strLen = std::snprintf(nullptr, 0, fmt, 
		colorIndex,
		dark ? "" : ";1",
		logTypeNames[(size_t)type]
	);

	char* str = new char[strLen + 1];
	std::snprintf(str, strLen + 1, fmt,
		colorIndex,
		dark ? "" : ";1",
		logTypeNames[(size_t)type]
	);

	return str;
}


void Logger::printColor(char colorChar) {
	int index = getColorIndex(colorChar);
	if (index >= 0) {
		char color[] = { 27, 91, 51, (char)(48 + index), 59, 49, 109 };
		size_t colorLen = sizeof(color);
		if (index < sizeof(tokens) / 2) {
			color[4] = 109;
			colorLen -= 2;
		} else {
			color[3] -= sizeof(tokens) / 2;
		}
		fwrite(color, sizeof(char), colorLen, os);
	}
}

void Logger::printColored(const char* str, size_t length, const logType type) {
	size_t firstToken = 0, lastIndex = 0;
	while(firstToken < length && str[firstToken] != prefix)
		firstToken++;

	if (firstToken == length || getColorIndex(str[firstToken + 1]) == -1) {
		printColor(logTypeColors[(size_t)type]);
		fwrite(str, sizeof(char), length, os);
		newline = str[length - 1] == '\n';
	} else {
		for (size_t i = firstToken; i < length; i++) {
			if (str[i] == prefix && i + 1 < length) {
				newline = i > 0 && str[i - 1] == '\n';
				fwrite(&str[lastIndex], sizeof(char), i++ - lastIndex, os);
				printColor(str[i]);
				lastIndex = i + 1;	
			}
		}
		fwrite(&str[lastIndex] , sizeof(char), length - lastIndex, os);
		newline |= str[length - 1] == '\n';
	}
	fwrite("\x001B[0m", sizeof(char), 5, os);
	fflush(os);
}

void Logger::print(const char* functionName, logType type, const char* fmt, ...) {
	static logType pType = MUTE;

	if (type <= logLevel) {
		if (pType != type && !newline) {
			fputc('\n', os);
			newline = true;
		}
		pType = type;

		if (newline) {
			fputs(logTypePrefix[(size_t)type], os);
			fputs(functionName, os);
			fwrite("\x001B[0m] ", sizeof(char), 7, os);
		}

		va_list args, copyArg;

		va_start(args, fmt);
		va_copy(copyArg, args);
		size_t strLen = std::vsnprintf(nullptr, 0, fmt, copyArg);
		va_end(copyArg);

		char* str = new char[strLen + 1];
		std::vsnprintf(str, strLen + 1, fmt, args);
		va_end(args);

		printColored(str, strLen, type);
	
		delete[] str;
	}
}
