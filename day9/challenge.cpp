#include <util.hpp>
#include <Logger.hpp>
#include <string>
#include <numeric>
#include <algorithm>

std::vector<uint32_t> findLowSpots(uint8_t* map, uint32_t width, uint32_t height) {
	std::vector<uint32_t> lowSpots;
	for (uint32_t y = 0; y < height; y++) {
		for (uint32_t x = 0; x < width; x++) {
			uint32_t center = map[y * width + x];
			if (
				(y == 0          || map[(y - 1) * width + x] > center) &&
				(y == height - 1 || map[(y + 1) * width + x] > center) &&
				(x == 0          || map[y * width + x - 1]   > center) &&
				(x == width - 1  || map[y * width + x + 1]   > center)
			) {
				lowSpots.push_back(y * width + x);
			}
		}
	}
	return lowSpots;
}

int64_t fistChallenge(const std::vector<std::string>& lines) {
	static bool testing = true;
	uint32_t height = lines.size();
	uint32_t width = lines[0].length();
	uint32_t mapSize = width * height;
	uint8_t* map = new uint8_t[mapSize];
	
	uint32_t index = 0;
	for (const auto& line : lines) {
		for (char point : line) {
			map[index++] = point - '0';
		}
	}

	const auto lowSpots = findLowSpots(map, width, height);
	return std::accumulate(lowSpots.begin(), lowSpots.end(), 0, [&map](uint32_t sum, uint32_t index) {
		return sum + map[index] + 1;
	});
}

void findBasinPoints(
	uint8_t* map,
	uint32_t x, uint32_t y,
	uint32_t width, uint32_t height,
	std::vector<uint32_t>& points
) {	
	size_t index = y * width + x;
	uint8_t z = map[index];
	if (z != 9 && std::find(points.begin(), points.end(), index) == points.end()) {
		points.push_back(index);

		if (y > 0 && map[(y - 1) * width + x] > z) {
			findBasinPoints(map, x, y - 1, width, height, points);
		} 
		if (y < height - 1 && map[(y + 1) * width + x] > z) {
			findBasinPoints(map, x, y + 1, width, height, points);
		}
		if (x > 0 && map[index - 1] > z) {
			findBasinPoints(map, x - 1, y, width, height, points);
		}
		if (x < width - 1 && map[index + 1] > z) {
			findBasinPoints(map, x + 1, y, width, height, points);
		}
	
	}
}

int64_t secondChallenge(const std::vector<std::string>& lines) {
	uint32_t height = lines.size();
	uint32_t width = lines[0].length();
	uint32_t mapSize = width * height;
	uint8_t* map = new uint8_t[mapSize];
	
	uint32_t index = 0;
	for (const auto& line : lines) {
		for (char point : line) {
			map[index++] = point - '0';
		}
	}

	const auto lowSpots = findLowSpots(map, width, height);
	std::vector<uint32_t> basinSizes;
	std::vector<uint32_t> basinPoints;
	for (const auto lowSpot : lowSpots) {
		basinPoints.clear();
		findBasinPoints(map, lowSpot % width, lowSpot / width, width, height, basinPoints);
		basinSizes.push_back(basinPoints.size());
	}

	std::sort(basinSizes.begin(), basinSizes.end(), std::greater<uint32_t>());

	return basinSizes[0] * basinSizes[1] * basinSizes[2];
}

int main() {
	util::test({ fistChallenge, secondChallenge }, { 15, 1134 });
}
