#include <util.hpp>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <string>
#include <stdexcept>
#include <cstring>
#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>
#include <Logger.hpp>

std::string util::loadString(const std::string& filename)  {
	FILE* file = fopen(filename.c_str(), "rb");
	if (file == NULL) {
		return "";
	}

	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	fseek(file, 0L, SEEK_SET);

	std::string str;
	str.reserve(fileSize);
	for (size_t i = 0; i < fileSize; i++) {
		str += fgetc(file);
	}
	fclose(file);
	
	return str;
}

std::vector<std::string> util::loadStrings(const std::string& filename) {
	std::string raw = loadString(filename);
	
	std::vector<std::string> lines;
	size_t prevIndex = 0;
	for (size_t i = 0; (i = raw.find("\n", i)) != std::string::npos; i++) {
		lines.emplace_back(&raw[prevIndex], &raw[i]);
		prevIndex = i + 1;
	}
	return lines;
}

std::vector<std::string> util::split(const std::string& str, const std::string& pattern) {
	std::vector<std::string> parts;
	size_t prevIndex = 0;
	for (size_t i = 0; (i = str.find(pattern, i)) != std::string::npos; i++) {
		parts.emplace_back(&str[prevIndex], &str[i]);
		prevIndex = i + pattern.length();
	}
	if (prevIndex < str.length()) {
		parts.emplace_back(&str[prevIndex], &str[str.length()]);
	}
	return parts;
}


void util::test(const std::vector<challengeFunction>& challenges, const std::vector<int64_t>& testSolutions) {
	Logger::showType(false);
	
	const std::vector<std::string> test = loadStrings("testInput.txt");
	const std::vector<std::string> input = loadStrings("input.txt");

	for (size_t i = 0; i < challenges.size(); i++) {
		Logger::log("|Y--------------------\n");
		int64_t testOutput = challenges[i](test);
		if (testSolutions.size() > i) {
			Logger::log("test %s (%d)\n", (testOutput == testSolutions[i] ? "|Gpassed" : "|Rfailed"), testOutput);
		} else {
			Logger::log("test-output: %lld\n", testOutput);
		}

		const auto start = std::chrono::high_resolution_clock::now();
		int64_t output = challenges[i](input);
		const auto end = std::chrono::high_resolution_clock::now();
		
		Logger::log("real-output: |P%lld\n", output);
		Logger::log("Time: |B%lldns\n", (end - start).count());
	}
	Logger::error("|Y--------------------\n");
}
