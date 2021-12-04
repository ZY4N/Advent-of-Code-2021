#include <util.hpp>
#include <Logger.hpp>
#include <string>
#include <cstring>
#include <array>

#define WIDTH 5
#define HEIGHT 5 

int64_t fistChallenge(const std::vector<std::string>& lines) {

	std::vector<std::string> parts = util::split(lines[0], ",");
	std::vector<uint8_t> numbers;
	numbers.reserve(parts.size());
	for (const std::string& part : parts) {
		numbers.push_back(std::stoi(part));
	}
	
	const size_t numBoards = (lines.size() - 1) / (HEIGHT + 1);
	std::vector<uint8_t[WIDTH * HEIGHT]> boards(numBoards);

	for (size_t i = 0; i < numBoards; i++) {
		uint8_t* board = boards[i];
		for (uint32_t y = 0; y < HEIGHT; y++) {
			const char* line = &lines[i * (HEIGHT + 1) + 2 + y][0];
			for (uint32_t x = 0; x < WIDTH; x++) {
				uint8_t& cell = board[x + y * WIDTH];
				cell = 0;
				if (line[x * 3] != ' ')
					cell += 10 * (line[x * 3] - '0');
				cell += line[x * 3 + 1] - '0';
			}
		}
	}

	uint32_t finalNumber = -1;
	uint8_t* finalBoard = nullptr;


	for (uint8_t number : numbers) {
		for (uint8_t* board : boards) {
			uint32_t columnSums[WIDTH];
			std::memset(columnSums, 0, sizeof(columnSums));

			for (uint32_t y = 0; y < HEIGHT; y++) {		
				uint32_t rowSum = 0;
				for (uint32_t x = 0; x < WIDTH; x++) {
					uint32_t i = x + WIDTH * y;
					board[i] |= (board[i] == number) << 7;
					if (board[i] >= 128) {
						rowSum++;
						columnSums[x]++;
					}
				}
				if (rowSum == WIDTH) {
					finalNumber = number; 
					finalBoard = board;
					goto foundResult;
				}
			}
			for (uint32_t i = 0; i < sizeof(columnSums); i++) {
				if (columnSums[i] == HEIGHT) {
					finalNumber = number; 
					finalBoard = board;
					goto foundResult;
				}
			}
		}
	}

foundResult:
	if (finalNumber == -1) {
		Logger::error("No bingo!");
	}

	uint32_t boardSum = 0;
	for (uint32_t i = 0; i < WIDTH * HEIGHT; i++) {
		if (finalBoard[i] < 128) {
			boardSum += finalBoard[i];
		}
	}

	return boardSum * finalNumber;
}

int64_t secondChallenge(const std::vector<std::string>& lines) {
	std::vector<std::string> parts = util::split(lines[0], ",");
	std::vector<uint8_t> numbers;
	numbers.reserve(parts.size());
	for (const std::string& part : parts) {
		numbers.push_back(std::stoi(part));
	}
	
	const size_t numBoards = (lines.size() - 1) / (HEIGHT + 1);
	std::vector<std::array<uint8_t, WIDTH * HEIGHT>> boards(numBoards);

	for (size_t i = 0; i < numBoards; i++) {
		uint8_t* board = &boards[i][0];
		for (uint32_t y = 0; y < HEIGHT; y++) {
			const char* line = &lines[i * (HEIGHT + 1) + 2 + y][0];
			for (uint32_t x = 0; x < WIDTH; x++) {
				uint8_t& cell = board[x + y * WIDTH];
				cell = 0;
				if (line[x * 3] != ' ')
					cell += 10 * (line[x * 3] - '0');
				cell += line[x * 3 + 1] - '0';
			}
		}
	}

	uint32_t finalNumber = -1;
	std::array<uint8_t, WIDTH * HEIGHT> finalBoard;

	for (uint8_t number : numbers) {
		for (auto it = boards.begin(); it != boards.end();) {
			uint32_t columnSums[WIDTH];
			std::memset(columnSums, 0, sizeof(columnSums));

			bool foundResult = false;
			for (uint32_t y = 0; y < HEIGHT; y++) {		
				uint32_t rowSum = 0;
				for (uint32_t x = 0; x < WIDTH; x++) {
					uint32_t i = x + WIDTH * y;
					(*it)[i] |= ((*it)[i] == number) << 7;
					if ((*it)[i] >= 128) {
						rowSum++;
						columnSums[x]++;
					}
				}
				if (rowSum == WIDTH) {
					foundResult = true;
					break;
				}
			}

			for (uint32_t i = 0; i < sizeof(columnSums); i++) {
				if (columnSums[i] == HEIGHT) {
					foundResult = true;
					break;
				}
			}

			if (foundResult) {
				finalBoard = *it;
				finalNumber = number;
				it = boards.erase(it);
			} else {
				it++;
			}
		}
	}

	if (finalNumber == -1) {
		Logger::error("No bingo!");
	}

	uint32_t boardSum = 0;
	for (uint32_t i = 0; i < WIDTH * HEIGHT; i++) {
		if (finalBoard[i] < 128) {
			boardSum += finalBoard[i];
		}
	}

	return boardSum * finalNumber;
	return 0;
}


int main() {
	util::test({ fistChallenge, secondChallenge }, { 4512, 1924 });
}
