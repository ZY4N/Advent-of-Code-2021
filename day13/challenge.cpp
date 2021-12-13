#include <util.hpp>
#include <Logger.hpp>
#include <string>
#include <vector>
#include <cmath>

struct vec2D {
	uint32_t x, y;
	bool operator==(const vec2D& v) {
		return x == v.x && y == v.y;
	}
	bool operator<(const vec2D& v) {
		return (*(uint64_t*)this) < (*(uint64_t*)&v);
	}
};

std::vector<vec2D> parseDots(const std::vector<std::string>& lines) {
	std::vector<vec2D> dots;
	for (size_t i = 0; lines[i].length() > 0; i++) {
		size_t offset;
		uint32_t x = std::stoul(lines[i], &offset);
		uint32_t y = std::stoul(lines[i].substr(offset + 1));
		dots.push_back({ x, y });
	}
	return dots;
}

void fold(std::vector<vec2D>& dots, const std::string& str) {
	bool isY = str[0] == 'y';
	uint32_t foldLine = std::stoul(str.substr(2));

	dots.erase(std::remove_if(dots.begin(), dots.end(), [&](auto dot) {
		return (isY ? dot.y : dot.x) == foldLine;
	}), dots.end());

	uint32_t* coords = (uint32_t*)dots.data();
	for (size_t i = 0; i < dots.size(); i++) {
		coords[i * 2 + isY] -= 2 * std::max(
			(int64_t)coords[i * 2 + isY] - (int64_t)foldLine, 0L
		);
	}
	
	std::sort(dots.begin(), dots.end());
	dots.erase(std::unique(dots.begin(), dots.end()), dots.end());
}

void renderSortedDots(std::vector<vec2D>& dots) {
	uint32_t py = 0, px = 0;
	for (const auto dot : dots) {
		if (dot.y > py) {
			while (dot.y > py) {
				fputc('\n', stdout);
				py++;
			}
			px = 0;
		}
		while (dot.x > px++) fputc(' ', stdout);
		fputc('#', stdout);
	}
	fputc('\n', stdout);
}

int64_t fistChallenge(const std::vector<std::string>& lines) {
	auto dots = parseDots(lines);
	fold(dots, lines[dots.size() + 1].substr(sizeof("fold along")));
	return dots.size();
}

int64_t secondChallenge(const std::vector<std::string>& lines) {
	auto dots = parseDots(lines);
	for (size_t i = dots.size() + 1; i < lines.size(); i++) {
		fold(dots, lines[i].substr(sizeof("fold along")));
	}
	renderSortedDots(dots);
	return 0;
}

/**
 * ####  ##  #### #  # #    #  # #### ####
 * #    #  # #    #  # #    #  #    # #
 * ###  #    ###  #### #    ####   #  ###
 * #    #    #    #  # #    #  #  #   #
 * #    #  # #    #  # #    #  # #    #
 * ####  ##  #    #  # #### #  # #### #
 * */

int main() {
	util::test({ fistChallenge, secondChallenge }, { 17, 0 });
}
