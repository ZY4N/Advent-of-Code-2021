#include <util.hpp>
#include <Logger.hpp>
#include <string>
#include <unordered_map>


class StateMachine {
private:
	std::vector<bool> states;
	std::vector<std::pair<uint32_t, uint32_t>> transitions;
	uint32_t startState, endState;
	std::unordered_map<std::string, uint32_t> nameToState;

public:
	uint32_t addState(const std::string& name) {
		const auto it = nameToState.find(name);
		if (it == nameToState.end()) {
			uint32_t index = states.size();
			if (name == "start") startState = index;
			if (name == "end") endState = index;
			states.push_back(isupper(name[0]));
			nameToState.insert({ name, index });
			return index;
		}
		return it->second;
	}

	void addTransition(uint32_t from, uint32_t to) {
		transitions.emplace_back(from, to);
	}
	
	static StateMachine parse(const std::vector<std::string>& lines) {
		StateMachine s;
		for (const auto& line : lines) {
			const auto names = util::split(line, "-");
			uint32_t state1 = s.addState(names[0]);
			uint32_t state2 = s.addState(names[1]);
			s.addTransition(state1, state2);
			s.addTransition(state2, state1);
		}
		return s;
	}

	std::vector<std::vector<uint32_t>> findAllPaths(bool visitTwice) {
		return findAllPaths({}, startState, visitTwice);
	}

	std::vector<std::vector<uint32_t>> findAllPaths(const std::vector<uint32_t>& pathSoFar, uint32_t currentState, bool visitTwice) {
		if (!states[currentState]) {
			uint32_t count = std::count(pathSoFar.begin(), pathSoFar.end(), currentState);
			if (currentState == startState && count > 0 || count > visitTwice) {
				return {};
			} else if (visitTwice && count == 1) {
				visitTwice = false;
			}
		}

		auto totalPath = pathSoFar;
		totalPath.push_back(currentState);
		
		if (currentState == endState) {
			return { totalPath };
		} else {
			std::vector<std::vector<uint32_t>> allPaths;
			for (const auto [from, to] : transitions) {
				if (from == currentState) {
					const auto newPaths = findAllPaths(totalPath, to, visitTwice);
					allPaths.insert(allPaths.end(), newPaths.begin(), newPaths.end());
				}
			}
			return allPaths;
		}
	}
};

int64_t fistChallenge(const std::vector<std::string>& lines) {
	return StateMachine::parse(lines).findAllPaths(false).size();
}

int64_t secondChallenge(const std::vector<std::string>& lines) {
	return StateMachine::parse(lines).findAllPaths(true).size();
}

int main() {
	util::test({ fistChallenge, secondChallenge }, { 226, 3509 });
}
