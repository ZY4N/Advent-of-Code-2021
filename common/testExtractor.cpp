#include <util.hpp>
#include <fstream>
#include <string>
#include <iostream>

int main(int numArgs, char* argc[]) {
	if (numArgs < 3) {
		std::cout << "need to specify input and output file" << std::endl;
		return -1;
	}

	std::cout << "from:\t\"" << argc[1] << '"' << std::endl;
	std::cout << "to:\t\"" << argc[2] << '"' << std::endl;

	std::vector<std::string> rawInput = util::loadStrings(argc[1]);
	std::vector<std::string> parsedData;

	std::ofstream outputFile;
	outputFile.open(argc[2]);

	uint32_t numParsedLines = 0;
	for (const std::string& line : rawInput) {
		if (line.starts_with("<pre><code>")) {
			outputFile << line.substr(11) << '\n';
			numParsedLines++;
		} else if (numParsedLines > 0) {
			if (line.starts_with("</code></pre>")) {
				break;
			} else {
				outputFile << line << '\n';
				numParsedLines++;
			}
		}
	}
	outputFile.close();

	std::cout << "extracted " << std::to_string(numParsedLines) << " lines\n";
	std::cout << "finished!\n";
}