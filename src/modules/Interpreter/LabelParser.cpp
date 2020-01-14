#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "../../SztosException.hpp"

using Line = std::vector<std::string>;
using InsnList = std::vector<Line>;

struct Label {
	std::string name;
	int lineID = 0;
	int targetNum = 0;
};

struct Target {
	Label &label;
	std::string &value;
	bool isAddr = false;

	Target(Label &label, std::string &value, bool isAddr) : label(label), value(value), isAddr(isAddr) {}
};

int getDoubleDot(const std::string &s) {
	for (size_t i = 0; i < s.length(); ++i) {
		if (s[i] == ':') return i;
	}
	return -1;
}

int getLength(int value) {
	if (value == 0) return 1;

	int i = 0;
	while (value > 0) {
		value /= 10;
		++i;
	}

	return i;
}

bool contains(const std::vector<Label> &labels, const std::string &s) {
	return std::any_of(labels.begin(), labels.end(),
		[&](const Label &label) { return label.name == s; });
}

std::vector<Label> extractLabels(InsnList &insnList) {
	std::vector<Label> labels;

	Label label;
	int charNum = 0, mode = 0, dd;
	for (size_t i = 0; i < insnList.size(); ++i) {
		if (insnList[i].size() == 0) continue;

		std::string &word = insnList[i][0];
		if (word == ".text") {
			mode = 0;
			continue;
		}
		else if (word == ".data") {
			//--charNum;
			mode = 1;
			continue;
		}

		dd = getDoubleDot(word);

		while (dd != -1) {
			label.name = word.substr(0, dd);
			label.targetNum = charNum;
			label.lineID = i;

			word = word.substr(dd + 1);

			if (!label.name.empty() && !word.empty()
				&& !contains(labels, word)) {
				labels.push_back(label);
			}

			dd = getDoubleDot(word);
		}
		if (mode == 0) {
			for (const std::string& param : insnList[i]) {
				charNum += param.length() + 1;
			}
		}
		else {
			charNum += 1;
		}
	}

	return labels;
}

std::vector<Target> getTargets(InsnList &insnList, std::vector<Label> &labels) {
	std::vector<Target> targets;

	for (auto &line : insnList) {
		for (auto &word : line) {
			if (word.front() == '<' && word.back() == '>') {
				for (Label &label : labels) {
					if (label.name == word.substr(1, word.length() - 2)) {
						targets.push_back({label, word, false});
						break;
					}
				}
			}
			else if (word.length() > 4
				&& word.front() == '[' && word[1] == '<'
				&& word[word.length() - 2] == '>' && word.back() == ']') {

				for (Label& label : labels) {
					if (label.name == word.substr(2, word.length() - 4)) {
						targets.push_back({ label, word, true });
						break;
					}
				}
			}
		}
	}

	return targets;
}

bool validateAndUpdate(InsnList &insnList, std::vector<Label> &labels) {
	if (labels.size() == 0) return true;

	bool valid = true;
	int currLabelNum = 0, mode = 0, charNum = 0;
	for (size_t i = 0; i < insnList.size(); ++i) {
		Line &line = insnList[i];
		if (!line.empty()) {
			if (line[0] == ".text") {
				mode = 0;
				continue;
			}
			else if (line[0] == ".data") {
				//--charNum;
				mode = 1;
				continue;
			}
		}

		Label &label = labels[currLabelNum];

		if (i == label.lineID) {
			if (label.targetNum != charNum) {
				label.targetNum = charNum;
				valid = false;
			}

			++currLabelNum;
		}

		if (mode == 0) {
			for (const std::string& param : line) {
				charNum += param.length() + 1;
			}
		}
		else {
			charNum += 1;
		}
	}
	return valid;
}

void applyTargets(std::vector<Target> &targets) {
	for (Target &target : targets) {
		std::string &value = target.value;
		int targetNum = target.label.targetNum;

		if (target.isAddr) {
			if ((getLength(targetNum) + 2) != value.length()) {
				value = '[' + std::to_string(targetNum) + ']';
			}
			else {
				for (int i = value.length() - 2; i >= 1; --i) {
					value[i] = (char)('0' + targetNum % 10);
					targetNum /= 10;
				}
			}
		}
		else {
			if (getLength(targetNum) != value.length()) {
				value = std::to_string(targetNum);
			}
			else {
				for (int i = value.length() - 1; i >= 0; --i) {
					value[i] = (char)('0' + targetNum % 10);
					targetNum /= 10;
				}
			}
		}
	}
}

void parseCode(const std::string &inName, const std::string &outName) {
	std::string param;

	std::cout << "Wprowadz nazwe pliku do przetworzenia: ";
	std::getline(std::cin, param);
	std::cout << std::endl;
	
	std::ifstream in(inName);
	std::ofstream out(outName);

	if (!in.is_open()) {
		throw SztosException("Nie udalo sie otworzyc pliku wejœciowego (" + inName + ").");
	}

	if (!out.is_open()) {
		throw SztosException("Nie udalo sie otworzyc pliku wyjœciowego (" + outName + ").");
	}

	InsnList insnList;

	int lineNum = 0;
	std::string line;
	std::stringstream buffer;

	while (std::getline(in, line)) {
		insnList.push_back({});

		buffer = std::stringstream(line);
		while (buffer >> param) {
			insnList[lineNum].push_back(param);
		}
		++lineNum;
	}

	std::vector<Label> labels = extractLabels(insnList);
	std::vector<Target> targets = getTargets(insnList, labels);

	applyTargets(targets);

	while (!validateAndUpdate(insnList, labels)) {
		applyTargets(targets);
	}

	for (Line &line : insnList) {
		for (size_t i = 0; i < line.size() - 1; ++i) {
			out << line[i] << " ";
		}
		out << line.back() << std::endl;
	}
}