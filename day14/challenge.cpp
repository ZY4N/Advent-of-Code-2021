#include <util.hpp>
#include <Logger.hpp>
#include <string>
#include <cstring>
#include <unordered_map>

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


//this i a mess i will clean it up later
struct StateMachine {
	std::vector<char> states;
	std::vector<std::pair<uint32_t, uint32_t>> transitions;
	uint32_t startState, endState;
	std::unordered_map<std::string, uint32_t> nameToState;

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
};

int64_t secondChallenge(const std::vector<std::string>& lines) {
	std::string templateLine = lines[0];

	StateMachine s;
	for (size_t i = 2; i< lines.size(); i++) {
		s.addState(lines[i].substr(0, 2), lines[i][6]);
	}

	for (const auto [name, index] : s.nameToState) {
		const char a[]{ name[0], s.states[index], 0 };
		const char b[]{ s.states[index], name[1], 0 };
		const auto it1 = s.nameToState.find(a);
		const auto it2 = s.nameToState.find(b);
		if (it1 != s.nameToState.end())
			s.transitions.emplace_back(index, it1->second);
		if (it2 != s.nameToState.end())
			s.transitions.emplace_back(index, it2->second);
	}

	size_t numStates = s.states.size();
	uint64_t* stateCounts = new uint64_t[numStates];
	std::memset(stateCounts, 0, numStates * sizeof(uint64_t));

	uint64_t charCounts[26];
	std::memset(charCounts, 0, sizeof(charCounts));

	for (char c : templateLine) {
		charCounts[c - 'A']++;
	}

	for (size_t i = 1; i < templateLine.length(); i++) {
		const char name[]{ templateLine[i - 1], templateLine[i], 0 };
		if (auto it = s.nameToState.find(name); it != s.nameToState.end()) {
			stateCounts[it->second]++;
		}
	}

	std::sort(s.transitions.begin(), s.transitions.end(), [](const auto& a, const auto& b){
		return a.first < b.first;
	});

	uint64_t* tmp = new uint64_t[numStates];
	
	for (size_t i = 0; i < 40; i++) {
		std::memset(tmp, 0, numStates * sizeof(uint64_t));
		uint32_t prevState= -1;
		for (const auto [from, to] : s.transitions) {
			tmp[to] += stateCounts[from];
			if (prevState != from) {
				charCounts[s.states[from] - 'A'] += stateCounts[from];
				prevState = from;
			}
		}
		std::memcpy(stateCounts, tmp, numStates * sizeof(uint64_t));
	}

	uint64_t min = UINT64_MAX, max = 0; 

	for (size_t i = 0; i < 26; i++) {
		if (charCounts[i] != 0)
			min = std::min(min, charCounts[i]);
		max = std::max(max, charCounts[i]);
	}

	delete[] stateCounts;
	delete[] tmp;

	return max - min;
}

int main() {
	util::test({ fistChallenge, secondChallenge }, { 1588, 2188189693529 });
}


//2297181831
//2188189693529