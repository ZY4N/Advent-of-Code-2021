#include <util.hpp>
#include <Logger.hpp>
#include <string>

struct line{
	uint32_t startX, startY, endX, endY;
};


int64_t fistChallenge(const std::vector<std::string>& input) {
	
	int width, height;
	width = height = 0;

	std::vector<line> lines;
	for (const auto& l : input) {
		size_t offset, pos = 0;
		int startX = std::stoi(&l[pos], &offset);
		pos += offset + 1;
		int startY = std::stoi(&l[pos], &offset);
		pos += offset + 4;
		int endX = std::stoi(&l[pos], &offset);
		pos += offset + 1;
		int endY = std::stoi(&l[pos], &offset);

		width = std::max(std::max(startX, endX), width);
		height = std::max(std::max(startY, endY), height);

		if (startX == endX || startY == endY) {
			lines.emplace_back(
				std::min(startX, endX),
				std::min(startY, endY),
				std::max(startX, endX),
				std::max(startY, endY)
			);
		}
	}

	width++;
	height++;

	uint8_t* board = (uint8_t*)calloc(1, width * height);

	for (const auto& line : lines) {
		for (int x = line.startX; x <= line.endX; x++) {
			for (int y = line.startY; y <= line.endY; y++) {
				board[x + y * width]++;
			}
		}
	}

	uint32_t sum = 0;
	for (uint32_t i = 0; i < width * height; i++) {
		if (board[i] > 1) {
			sum++;
		}
	}

	free(board);
	return sum;
}

int64_t secondChallenge(const std::vector<std::string>& input) {
	int width, height;
	width = height = 0;

	std::vector<line> lines;
	for (const auto& l : input) {
		size_t offset, pos = 0;
		int startX = std::stoi(&l[pos], &offset);
		pos += offset + 1;
		int startY = std::stoi(&l[pos], &offset);
		pos += offset + 4;
		int endX = std::stoi(&l[pos], &offset);
		pos += offset + 1;
		int endY = std::stoi(&l[pos], &offset);

		width = std::max(std::max(startX, endX), width);
		height = std::max(std::max(startY, endY), height);

		lines.emplace_back(startX, startY, endX, endY);
	}

	width++;
	height++;

	uint8_t* board = (uint8_t*)calloc(width * height, 1);
	
	for (const auto& line : lines) {
		int dx = line.endX - line.startX;
		int dy = line.endY - line.startY;
		int stepX = dx == 0 ? 0 : dx / std::abs(dx);
		int stepY = dy == 0 ? 0 : dy / std::abs(dy);
		int numSteps = std::max(std::abs(dx), std::abs(dy));

		int x = line.startX, y = line.startY; 
		for (int i = 0; i <= numSteps; i++) {
			board[x + y * width]++;
			x += stepX;
			y += stepY;	
		}
	}

	uint32_t sum = 0;
	for (uint32_t i = 0; i < width * height; i++) {
		if (board[i] > 1) {
			sum++;
		}
	}
	
	free(board);

	return sum;
}

int main() {
	util::test({ fistChallenge, secondChallenge }, { 5, 12 });
}
