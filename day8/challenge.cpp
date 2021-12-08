#include <util.hpp>
#include <Logger.hpp>
#include <string>
#include <bit>
#include <functional>
#include <algorithm>
#include <strings.h>
#include <bitset>

#define NUM_INPUTS 10
#define NUM_OUTPUTS 4
using displayArray = uint8_t[NUM_INPUTS + NUM_OUTPUTS];

void parseBits(const std::vector<std::string>& lines, std::function<void(displayArray&)> f) {
	for (const auto& line : lines) {
		uint8_t displays[NUM_INPUTS + NUM_OUTPUTS];
		size_t offset = 0;
		for (size_t i = 0; i < sizeof(displays); i++) {
			displays[i] = 0;
			do {
				displays[i] |= ((uint8_t)1) << (line[offset] - 'a');
			} while (++offset < line.length() && line[offset] != ' ');
			offset += i == NUM_INPUTS - 1 ? 3 : 1;
		}
		f(displays);
	}
}

int64_t fistChallenge(const std::vector<std::string>& lines) {
	uint32_t num1478 = 0; 
	parseBits(lines, [&num1478](displayArray& displays) {
		for (size_t i = NUM_INPUTS; i < sizeof(displays); i++) {
			uint8_t setBits = std::popcount(displays[i]);
			if (setBits == 2 || setBits == 3 || setBits == 4 || setBits == 7) {
				num1478++;
			}
		}
	});

	return num1478;
}

//runs in 127341ns
int64_t secondChallenge(const std::vector<std::string>& lines) {
	uint64_t sum = 0; 
	parseBits(lines, [&sum](displayArray& displays) {
		std::sort(displays, displays + NUM_INPUTS, [](uint8_t a, uint8_t b){
			return std::popcount(a) < std::popcount(b);
		});

		uint8_t numbers[10];
		numbers[1] = displays[0]; // 2 segments
		numbers[7] = displays[1]; // 3 segments
		numbers[4] = displays[2]; // 4 segments
		numbers[8] = displays[9]; // 7 segments

		uint8_t notSevenAndFour = ~(numbers[7] | numbers[4]);
		uint8_t gANDe[3] {
			(uint8_t)(displays[3] & notSevenAndFour),
			(uint8_t)(displays[4] & notSevenAndFour),
			(uint8_t)(displays[5] & notSevenAndFour)
		};
		numbers[2] = displays[ 
			std::popcount(gANDe[0]) == 2 ? 3 :
			std::popcount(gANDe[1]) == 2 ? 4 : 5
		];

		uint8_t notE = ~((gANDe[0] ^ gANDe[1]) | (gANDe[1] ^ gANDe[2]));
		uint8_t notC = ~(numbers[1] & numbers[2]);

		numbers[9] = numbers[8] & notE;
		numbers[6] = numbers[8] & notC;
		numbers[5] = numbers[6] & numbers[9];
		numbers[3] = (numbers[1] | numbers[2]) & notE;
		
		for (int i = 6; i <= 8; i++) {
			if (displays[i] != numbers[6] && displays[i] != numbers[9]) {
				numbers[0] = displays[i];
				break;
			}
		}

		uint32_t scale = 1;
		for (int i = 1; i <= NUM_OUTPUTS; i++) {
			uint32_t number = -1;
			uint8_t display = displays[sizeof(displays) - i];
			for (int j = 0; j < sizeof(numbers); j++){
				if (display == numbers[j]) {
					number = j;
					break;
				}
			}
			sum += number * scale;
			scale *= 10;
		}
	});
	return sum; 
}

int main() {
	util::test({ fistChallenge, secondChallenge }, { 26, 61229 });
}
