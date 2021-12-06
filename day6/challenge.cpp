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

int64_t secondChallenge(const std::vector<std::string>& lines) {

	const size_t maxOffset = 8;

	uint64_t fishOffsets[maxOffset + 1];
	std::memset(fishOffsets, 0, sizeof(fishOffsets));

	const auto& line = lines[0];
	for (size_t i = 0; i < line.length(); i += 2) {
		fishOffsets[line[i] - '0']++;
	}

	for (size_t i = 0; i < 256; i++) {
		uint64_t numDuplicatingFish = fishOffsets[0];
		for (size_t j = 1; j <= maxOffset; j++) { //sub 1
			fishOffsets[j - 1] = fishOffsets[j];
		}
		fishOffsets[maxOffset] = numDuplicatingFish; //add new fish
		fishOffsets[6] += numDuplicatingFish; //reset old fish
	}

	uint64_t sum = 0;
	for (size_t i = 0; i <= maxOffset; i++) {
		sum += fishOffsets[i];
	}

	return sum;
}

int main() {
	util::test({ fistChallenge, secondChallenge }, { 5934, 26984457539 });
}
