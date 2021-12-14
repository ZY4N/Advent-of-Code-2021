#include <util.hpp>
#include <Logger.hpp>
#include <string>
#include <cstring>
#include <unordered_map>

struct QuantativeStateMachine {
	std::vector<char> states;
	std::vector<std::pair<uint32_t, uint32_t>> transitions;
	std::unordered_map<std::string, uint32_t> nameToState;
	uint64_t letterCounts[26];
	uint64_t* stateCounts = nullptr;
	
	QuantativeStateMachine() {
		std::memset(letterCounts, 0, sizeof(letterCounts));
	}

	~QuantativeStateMachine() {
		delete[] stateCounts;
	}

	uint32_t addState(const std::string& name, char c) {
		const auto it = nameToState.find(name);
		if (it == nameToState.end()) {
			uint32_t index = states.size();
			states.push_back(c);
			nameToState.insert({ name, index });
			return index;
		}
		return it->second;
	}

	void addTransition(uint32_t from, const char* to) {
		if (const auto it = nameToState.find(to); it != nameToState.end()) {
			transitions.emplace_back(from, it->second);
		}
	}

	void performSteps(uint32_t numSteps) {
		size_t numStates = states.size();
		uint64_t* tmp = new uint64_t[numStates];
	
		for (size_t i = 0; i < numSteps; i++) {
			std::memset(tmp, 0, numStates * sizeof(uint64_t));
			uint32_t prevState= -1;
			for (const auto [from, to] : transitions) {
				tmp[to] += stateCounts[from];
				if (prevState != from) {
					letterCounts[states[from] - 'A'] += stateCounts[from];
					prevState = from;
				}
			}
			std::memcpy(stateCounts, tmp, numStates * sizeof(uint64_t));
		}

		delete[] tmp;
	}

	static QuantativeStateMachine fromText(const std::vector<std::string>& lines) {
		std::string templateLine = lines[0];

		QuantativeStateMachine s;
		for (size_t i = 2; i< lines.size(); i++) {
			s.addState(lines[i].substr(0, 2), lines[i][6]);
		}

		for (const auto [name, index] : s.nameToState) {
			const char a[]{ name[0], s.states[index], 0 };
			const char b[]{ s.states[index], name[1], 0 };
			s.addTransition(index, a);
			s.addTransition(index, b);
		}

		for (char c : templateLine)
			s.letterCounts[c - 'A']++;

		size_t numStates = s.states.size();
		s.stateCounts = new uint64_t[numStates];
		std::memset(s.stateCounts, 0, numStates * sizeof(uint64_t));

		for (size_t i = 1; i < templateLine.length(); i++) {
			const char name[]{ templateLine[i - 1], templateLine[i], 0 };
			if (auto it = s.nameToState.find(name); it != s.nameToState.end()) {
				s.stateCounts[it->second]++;
			}
		}

		std::sort(s.transitions.begin(), s.transitions.end(), [](const auto& a, const auto& b){
			return a.first < b.first;
		});
		return s;
	}
};

std::pair<uint64_t, uint64_t> findMinMax(uint64_t* it, const uint64_t* end) {
	uint64_t min = UINT64_MAX, max = 0; 
	while (it != end) {
		if (*it != 0)
			min = std::min(min, *it );
		max = std::max(max, *it );
		it++;
	}
	return { min, max };
}

int64_t fistChallenge(const std::vector<std::string>& lines) {
	QuantativeStateMachine s = QuantativeStateMachine::fromText(lines);
	s.performSteps(10);
	const auto [min, max] = findMinMax(s.letterCounts, std::end(s.letterCounts));
	return max - min;
}

int64_t secondChallenge(const std::vector<std::string>& lines) {
	QuantativeStateMachine s = QuantativeStateMachine::fromText(lines);
	s.performSteps(40);
	const auto [min, max] = findMinMax(s.letterCounts, std::end(s.letterCounts));
	return max - min;
}

int main() {
	util::test({ fistChallenge, secondChallenge }, { 1588, 2188189693529 });
}


/* naive approach
struct replacement {
	size_t pos;
	char c;
};

void applyGrammar(
	std::string& templateLine,
	const std::vector<std::pair<std::string, char>>& grammar,
	size_t numIterations
) {
	std::vector<replacement> replacements;
	for (size_t i = 0; i < numIterations; i++) {		
		for (const auto& [s, c] : grammar) {
			size_t pos = 0;
			while ((pos = templateLine.find(s, pos)) != std::string::npos) {
				replacements.emplace_back(pos, c);
				pos++;
			}
		}
		std::sort(replacements.begin(), replacements.end(), [](const auto& a, const auto b){
			return a.pos > b.pos;
		});
		replacements.reserve(replacements.size());
		for (const auto [pos, c] : replacements) {
			templateLine.insert(templateLine.begin() + pos + 1, c);
		}
		replacements.clear();
	}	
}

int64_t fistChallenge(const std::vector<std::string>& lines) {
	std::string templateLine = lines[0];
	std::vector<std::pair<std::string, char>> grammar;

	for (size_t i = 2; i < lines.size(); i++) {
		grammar.emplace_back(
			lines[i].substr(0, 2),
			lines[i][6]
		);
	}

	applyGrammar(templateLine, grammar, 10);

	uint32_t buckets[26];
	std::memset(buckets, 0, sizeof(buckets));
	for (char c : templateLine) {
		buckets[c - 'A']++;
	}

	auto start = buckets;
	auto end = std::end(buckets);
	std::sort(start, end);

	while (*start == 0) start++;
	return *(end - 1) - (*start);
}
*/