#include <util.hpp>
#include <Logger.hpp>
#include <string>

int64_t fistChallenge(const std::vector<std::string>& lines) {
	int horizontal = 0, depth = 0;

	for (const std::string& line : lines) {
		int delta = line[line.length() - 1] - '0';
		if (line.starts_with("forward")) {
			horizontal += delta;
		} else if(line.starts_with("down")) {
			depth += delta;
		} else if(line.starts_with("up")) {
			depth -= delta;
		}
	}

	return horizontal * depth;
}

int64_t secondChallenge(const std::vector<std::string>& lines) {	
	int horizontal = 0, depth = 0, aim = 0;

	for (const std::string& line : lines) {
		int delta = line[line.length() - 1] - '0';
		if (line.starts_with("forward")) {
			horizontal += delta;
			depth += aim * delta;
		} else if(line.starts_with("down")) {
			aim += delta;
		} else if(line.starts_with("up")) {
			aim -= delta;
		}
	}

	return horizontal * depth;
}

int main() {
	util::test({ fistChallenge, secondChallenge }, { 150, 900 });
}
