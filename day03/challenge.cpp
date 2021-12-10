#include <util.hpp>
#include <Logger.hpp>
#include <string>
#include <iostream>

int64_t fistChallenge(const std::vector<std::string>& lines) {
	uint32_t numBits = lines[0].length();
	uint32_t* numOnes = (uint32_t*)calloc(numBits, sizeof(uint32_t));

	for (const std::string& line : lines) {
		for (uint32_t i = 0; i < line.length(); i++) {
			if(line[i] == '1') numOnes[i]++;
		}
	}

	uint32_t halNumNumbers = lines.size() / 2;
	uint32_t gamma = 0, epsilon = 0;
	for (uint32_t i = 0; i < numBits; i++) {
		bool isOne = numOnes[numBits - 1 - i] > halNumNumbers; 
		gamma |= ((uint32_t)isOne) << i;
		epsilon |= ((uint32_t)!isOne) << i;
	}

	free(numOnes);

	return gamma * epsilon;
}

void removeMajority(std::vector<std::string>& numbers, bool majority) {

	size_t index = 0;
	while(numbers.size() > 1) {
		uint32_t numOnes = 0;
		for (const auto& line : numbers) {
			if (line[index] == '1') {
				numOnes++;
			}
		}
		
		bool removeValue;
		if (numOnes == numbers.size() - numOnes) {
			removeValue = majority;
		} else {
			bool majorityValue = numOnes > numbers.size() / 2;
			removeValue = majority ? majorityValue : !majorityValue;
		}

		numbers.erase(std::remove_if(numbers.begin(), numbers.end(), [&](const auto& number){
			return number[index] == removeValue + '0';
		}), numbers.end());

		index++;
	}
}

int64_t secondChallenge(const std::vector<std::string>& lines) {
	std::vector<std::string> majority = lines;
	if(majority[majority.size() - 1].length() == 0) {
		majority.erase(majority.end()--);
	}
	std::vector<std::string> minority = majority;
	removeMajority(majority, true);
	removeMajority(minority, false);

	return std::stoi(majority[0], 0, 2) * std::stoi(minority[0], 0, 2);
}

int main() {
	util::test({ fistChallenge, secondChallenge }, { 198, 230 });
}
