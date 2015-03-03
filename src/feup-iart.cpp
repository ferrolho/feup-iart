#include <iostream>
#include <cmath>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>

constexpr float logBase2(float x) {
	return log(x) / log(2);
}

float entropy(int yes, int no) {
	int total = yes + no;

	float yesByTotal = static_cast<float>(yes) / total;
	float noByTotal = static_cast<float>(no) / total;

	return -yesByTotal * logBase2(yesByTotal) - noByTotal * logBase2(noByTotal);
}

std::vector<std::string> split(const std::string& str,
		const std::string& separators) {
	// convert the passed string to a c string
	char *cStr = new char[str.length() + 1];
	strcpy(cStr, str.c_str());

	std::vector<std::string> tokens;
	char *p = strtok(cStr, separators.c_str());

	while (p) {
		tokens.push_back(p);

		p = strtok(nullptr, separators.c_str());
	}

	// delete the no longer used c string
	delete[] cStr;

	return tokens;
}

#include <iomanip>

int main() {
	std::ifstream dataFile;
	dataFile.open("parkinson-data/train-data.txt");

	if (dataFile.good()) {
		std::vector<std::vector<float>> trainData;

		while (!dataFile.eof()) {
			std::string entryStr;
			dataFile >> entryStr;

			/*
			 * C++11 introduced 'rvalue references' that let a mutable reference be bound to an rvalue.
			 * rvalue references use the && syntax instead of just &.
			 *
			 * In this particular case, before C++11 was introduced, doing something like
			 * std::vector<std::string> entryTokens = split(line, ","); would copy the temporary vector
			 * built inside the split function to entryTokens, and then destroy the temporary vector.
			 * That is not very smart! Why copy and then destroy the vector, when it could be *moved*
			 * to entryTokens instead? That is exactly what the rvalue references are used for.
			 */
			std::vector<std::string>&& entryTokens = split(entryStr, ",");

			std::vector<float> entryData;
			for (auto str : entryTokens)
				entryData.push_back(stof(str));

			trainData.push_back(entryData);
		}

		dataFile.close();

		for (auto entry : trainData) {
			for (auto i : entry)
				std::cout << i << " ";
			std::cout << std::endl;
		}
	} else {
		std::cerr << "Error: Could not read training data." << std::endl;
		return -1;
	}

	return 0;
}
