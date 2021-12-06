#include <util.hpp>
#include <Logger.hpp>
#include <string>
#include <cstring>

//this is stupid but i think it might just work
uint64_t popCnt(int initialOffset, int numDays) {
	std::vector<int8_t> fish;
	fish.push_back(initialOffset);
	for (int i = 0; i < numDays; i++) {
		size_t numFish = fish.size();
		for (size_t j = 0; j < numFish; j++) {
			if(--fish[j] < 0) {
				fish[j] = 6;
				fish.push_back(8);
			} 
		}
	}
	return fish.size();
}

int64_t fistChallenge(const std::vector<std::string>& lines) {
	const auto& line = lines[0];
	uint32_t numFishOffsets[7];
	std::memset(numFishOffsets, 0, sizeof(numFishOffsets));

	for (size_t i = 0; i < line.length(); i += 2) {
		numFishOffsets[line[i] - '0' - 1]++;
	}
	uint64_t totalFish = 0;
	for (size_t i = 0; i < 7; i++) {
		if (numFishOffsets[i] > 0) {
			totalFish += numFishOffsets[i] * popCnt(i + 1, 80);
		}
	}

	return totalFish;
}

//again not very elegant bot enough to get this done
void popCnt(int initialOffset, int numDays, uint64_t* allOffset, uint64_t scale) {
	std::vector<int8_t> fish;
	fish.push_back(initialOffset);
	for (int i = 0; i < numDays; i++) {
		size_t numFish = fish.size();
		for (size_t j = 0; j < numFish; j++) {
			if(--fish[j] < 0) {
				fish[j] = 6;
				fish.push_back(8);
			} 
		}
	}
	for (int8_t offset : fish) {
		allOffset[offset] += scale;
	}
}

int64_t secondChallenge(const std::vector<std::string>& lines) {

	const int maxOffset = 9;
	const int batchSize = 64;
	const int iterations = 4;

	uint64_t numFishOffsets[9];
	std::memset(numFishOffsets, 0, sizeof(numFishOffsets));

	const auto& line = lines[0];
	for (size_t i = 0; i < line.length(); i += 2) {
		numFishOffsets[line[i] - '0']++;
	}

	for (int i = 0; i < iterations; i++) {
		uint64_t nextOffsets[maxOffset];
		std::memset(nextOffsets, 0, sizeof(nextOffsets));
		for (int j = 0; j < maxOffset; j++) {
			if (numFishOffsets[j] > 0) {
				popCnt(j, batchSize, nextOffsets, numFishOffsets[j]);
			}
		}
		std::memcpy(numFishOffsets, nextOffsets,  sizeof(nextOffsets));
	}

	uint64_t sum = 0;
	for (size_t i = 0; i < maxOffset; i++) {
		sum += numFishOffsets[i];
	}

	return sum;
}

int main() {
	util::test({ fistChallenge, secondChallenge }, { 5934, 26984457539 });
}
