#include <util.hpp>
#include <Logger.hpp>
#include <string>
#include <cmath>


int64_t fistChallenge(const std::vector<std::string>& lines) {
	std::vector<std::pair<uint32_t, uint32_t>> positionCounts;

	uint32_t  minPos = UINT32_MAX, maxPos = 0;
	const std::string& line = lines[0];
	size_t index = 0;
	while (index < line.length()) {
		size_t offset;
		uint32_t num = std::stoi(&line[index], &offset);
		index += offset + 1;
		minPos = std::min(num, minPos);
		maxPos = std::max(num, maxPos);

		bool found = false;
		for (size_t i = 0; i < positionCounts.size(); i++) {
			if (positionCounts[i].first == num) {
				positionCounts[i].second++;
				found = true;
				break;
			}
		}
		if (!found) {
			positionCounts.push_back({ num, 1 });
		}
	}

	uint32_t lowestFuelConsumption = UINT32_MAX, bestPosition = -1;
	for (uint32_t pos = minPos; pos <= maxPos; pos++) {
		uint32_t fuelConsumption = 0;
		for (const auto& position : positionCounts) {
			fuelConsumption += abs(position.first - pos) * position.second;
		}
		if (fuelConsumption < lowestFuelConsumption) {
			lowestFuelConsumption = fuelConsumption;
			bestPosition = pos;
		}
	}

	return lowestFuelConsumption;
}

int64_t secondChallenge(const std::vector<std::string>& lines) {
		std::vector<std::pair<uint32_t, uint32_t>> positionCounts;

	uint32_t  minPos = UINT32_MAX, maxPos = 0;
	const std::string& line = lines[0];
	size_t index = 0;
	while (index < line.length()) {
		size_t offset;
		uint32_t num = std::stoi(&line[index], &offset);
		index += offset + 1;
		minPos = std::min(num, minPos);
		maxPos = std::max(num, maxPos);

		bool found = false;
		for (size_t i = 0; i < positionCounts.size(); i++) {
			if (positionCounts[i].first == num) {
				positionCounts[i].second++;
				found = true;
				break;
			}
		}
		if (!found) {
			positionCounts.push_back({ num, 1 });
		}
	}

	uint32_t lowestFuelConsumption = UINT32_MAX, bestPosition = -1;
	for (uint32_t pos = minPos; pos <= maxPos; pos++) {
		uint32_t fuelConsumption = 0;
		for (const auto& position : positionCounts) {
			uint32_t dist = abs(position.first - pos);
			fuelConsumption += ((dist * dist + dist) / 2) * position.second;
		}
		if (fuelConsumption < lowestFuelConsumption) {
			lowestFuelConsumption = fuelConsumption;
			bestPosition = pos;
		}
	}

	return lowestFuelConsumption;
}

int main() {
	util::test({ fistChallenge, secondChallenge }, { 37, 168 });
}
