#pragma once

#include <vector>
#include <string>

using challengeFunction = int64_t(*)(const std::vector<std::string>& input);

namespace util {

	std::string loadString(const std::string& filename);

	std::vector<std::string> loadStrings(const std::string& filename);

	std::vector<std::string> split(const std::string& str, const std::string& pattern);

	void test(const std::vector<challengeFunction>& challenges, const std::vector<int64_t>& testSolutions);

}
