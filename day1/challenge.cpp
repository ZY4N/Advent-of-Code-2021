#include <util.hpp>
#include <Logger.hpp>
#include <string>

int64_t fistChallenge(const std::vector<std::string>& lines) {
	uint32_t counter = 0;
	uint32_t lastNumber = UINT32_MAX;
	
	for (const auto& line : lines) {
		uint32_t number = std::stoi(line);
		if (number > lastNumber) {
			counter++;
		}
		lastNumber = number;
	}

	return counter;
}

int64_t secondChallenge(const std::vector<std::string>& lines) {
	static const constexpr uint32_t filterSize = 3;

	uint32_t counter = 0;
	uint32_t lastSum = UINT32_MAX;

	for (size_t i = filterSize - 1; i < lines.size(); i++) {
		uint32_t sum = 0;
		for (size_t j = 0; j < filterSize; j++) {
			sum += std::stoi(lines[i - j]);
		}
		if (sum > lastSum) {
			counter++;
		}
		lastSum = sum;
	}

	return counter;
}

int main() {
	util::test({ fistChallenge, secondChallenge }, { 7, 5 });
}
