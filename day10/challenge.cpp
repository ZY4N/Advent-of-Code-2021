#include <util.hpp>
#include <Logger.hpp>
#include <string>
#include <stack>
#include <ranges>

int64_t fistChallenge(const std::vector<std::string>& lines) {
	char openingSymboles[] { '(', '[', '{', '<' };
	char closingSymboles[] { ')', ']', '}', '>' };
	uint32_t errorScore[] { 3, 57, 1197, 25137 };

	uint32_t totalScore = 0;

	for (const auto& line : lines) {
		std::stack<char> openChunks;
		for (char c : line) {
			if (auto it = std::find(openingSymboles, std::end(openingSymboles), c); it != std::end(openingSymboles)) {
				openChunks.push(closingSymboles[it - openingSymboles]);
			} else if (c == openChunks.top()) {
				openChunks.pop();
			} else {
				size_t index = std::find(closingSymboles, std::end(closingSymboles), c) - closingSymboles;
				totalScore += errorScore[index];
				break;
			}
		}
	}

	return totalScore;
}

int64_t secondChallenge(const std::vector<std::string>& lines) {
	char openingSymboles[] { '(', '[', '{', '<' };
	char closingSymboles[] { ')', ']', '}', '>' };
	std::vector<uint64_t> scores;

	for (const auto& line : lines) {
		std::stack<char> openChunks;
		bool corrupted = false;
		for (char c : line) {
			if (auto it = std::find(openingSymboles, std::end(openingSymboles), c); it != std::end(openingSymboles)) {
				openChunks.push(closingSymboles[it - openingSymboles]);
			} else if (c == openChunks.top()) {
				openChunks.pop();
			} else {
				corrupted = true;
				break;
			}
		}
		if (!corrupted) {
			uint64_t score = 0;
			while (openChunks.size() > 0) {
				score *= 5;
				score += 1 + std::find(closingSymboles, std::end(closingSymboles), openChunks.top()) - closingSymboles;
				openChunks.pop();
			}
			scores.push_back(score);
		}
	}

	auto it = scores.begin() + scores.size() / 2;
	std::nth_element(scores.begin(), it, scores.end());
	return scores[scores.size() / 2];
}

int main() {
	util::test({ fistChallenge, secondChallenge }, { 26397, 288957 });
}
