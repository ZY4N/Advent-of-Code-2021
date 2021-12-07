#include <util.hpp>
#include <Logger.hpp>
#include <fstream>
#include <stdexcept>

std::string extractBetween(
	const std::string& startPhrase,
	const std::string& endPhrase,
	const std::string& text
) {
	size_t start = text.find(startPhrase, 0) + startPhrase.length();
	size_t end = text.find(endPhrase, start);

	if (start == std::string::npos || end == std::string::npos) {
		throw std::runtime_error("Cannot find start or endphrase");
	}

	return text.substr(start, end);
}

int main(int numArgs, char* argc[]) {
	if (numArgs < 5) {
		Logger::error("Not enough arguments\n");
		return -1;
	}

	Logger::debug("input:  |P\"%s\"\n", argc[1]);
	Logger::debug("output: |p\"%s\"\n", argc[2]);
	Logger::debug("start:  |p\"%s\"\n", argc[3]);
	Logger::debug("end:    |p\"%s\"\n", argc[4]);

	const std::string output = extractBetween(argc[3], argc[3], util::loadString(argc[1]));

	std::ofstream outputFile;
	outputFile.open(argc[2]);

	outputFile << output;

	if (output.back() != '\n')
		outputFile << '\n';

	outputFile.flush();
	outputFile.close();

	Logger::log("|Gfinished!\n");
}

/*
"article class=\"day-desc\"", "**</p>"
*/