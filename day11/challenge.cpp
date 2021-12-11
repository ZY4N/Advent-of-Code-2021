#include <util.hpp>
#include <Logger.hpp>
#include <string>

static const constexpr uint8_t hasFlashed = (uint8_t)(1 << 7);
static const constexpr uint8_t ignoreFlash = (uint8_t)(~hasFlashed);
static const constexpr int flashRadius = 1;

void tryFlash(std::vector<std::string>& octpusField, int x, int y, uint64_t& numFlashes) {
	if (
		(octpusField[y][x] & ignoreFlash) > 9 && 
		!(octpusField[y][x] & hasFlashed)
	) {
		octpusField[y][x] = hasFlashed;
		numFlashes++;
		for (int i = -flashRadius; i <= flashRadius; i++) {
			for (int j = -flashRadius; j <= flashRadius; j++) {
				int px = x + j, py = y + i;
				if (
					(i != 0 || j != 0) &&
					px >= 0 && px < octpusField[0].length() &&
					py >= 0 && py < octpusField.size()
				) {
					octpusField[py][px]++;
					tryFlash(octpusField, px, py, numFlashes);
				}
			}
		}
	}
}

int64_t fistChallenge(const std::vector<std::string>& lines) {
	std::vector<std::string> octpusField = lines;

	const size_t height = octpusField.size();
	const size_t width = octpusField[0].length();

	for (auto& octopusLine : octpusField) {
		for (auto& octupus : octopusLine) {
			octupus = octupus - '0' + 1;
		}
	}

	uint64_t numFlashes = 0;
	for (int i = 0; i < 100; i++) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				tryFlash(octpusField, x, y, numFlashes);
			}
		}

		for (auto& octopusLine : octpusField) {
			for (auto& octupus : octopusLine) {
				if (octupus & hasFlashed) {
					octupus = 0;	
				}
				octupus++;
			}
		}
	}

	return numFlashes;
}

int64_t secondChallenge(const std::vector<std::string>& lines) {
	std::vector<std::string> octpusField = lines;

	const size_t height = octpusField.size();
	const size_t width = octpusField[0].length();

	uint64_t maxFlashes = width * height;

	for (auto& octopusLine : octpusField) {
		for (auto& octupus : octopusLine) {
			octupus = octupus - '0' + 1;
		}
	}

	uint64_t numFlashes, index = 0;
	do {
		numFlashes = 0; 
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				tryFlash(octpusField, x, y, numFlashes);
			}
		}
		for (auto& octopusLine : octpusField) {
			for (auto& octupus : octopusLine) {
				if (octupus & hasFlashed) {
					octupus = 0;
				}
				octupus++;
			}
		}
		index++;
	} while(numFlashes != maxFlashes);

	return index;
}

int main() {
	util::test({ fistChallenge, secondChallenge }, { 1656, 195 });
}
