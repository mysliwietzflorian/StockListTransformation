#include "./semanticActionHandler.h"

#include <iostream>
#include <string>
#include <vector>
#include "../fileHandling/fileHandler.h"
#include "../errorHandling/applicationError.h"
#include "../parserHandling/parserHandler.h"
#include "../parserHandling/parsingStruct.h"

extern semanticAction *actions = semanticAction::getInstance();

// ### public methods ###
semanticAction *semanticAction::getInstance() {
	if (instance == nullptr) {
		instance = new semanticAction();
	}
	return instance;
}

void semanticAction::deleteInstance() {
	if (instance != nullptr) {
		// instance gets deleted
		delete instance;
	}
}

void semanticAction::executeRules(attrGrammarPtr attrGr, std::string element) {
	for (auto it : *(attrGr.get()->getRules().get())) {
		std::string mnemonic = parser->getMnemonicFromRulesStruct(it);

		// push raw element with semicolon if comment line and push mnemonic
		if (ignoreRules && mnemonic == "push" && element.size() != 0) {
			pushToVec(element + ";");
		// find the corresponding function to mnemonic
		} else if (!ignoreRules || (ignoreRules && mnemonic == "writeLine")) {
			findRuleFunction(mnemonic, element, attrGr);
		}
	}
}

// ### private methods ###
semanticAction *semanticAction::instance = nullptr;

semanticAction::semanticAction()
: prepareToWriteVec {}
, ignoreRules {false} {}

void semanticAction::findRuleFunction(const std::string &mnemonic, std::string &element,
	attrGrammarPtr attrGr) {

	if (mnemonic == "integrityCondition") {
		element = checkIntegrityCondition(element, attrGr->getLength(),
			attrGr->getMode(), attrGr->getLabel());
	} else if (mnemonic == "writeLine") {
		writeLine();
	} else if (mnemonic == "push") {
		pushToVec(element);
	} else if (mnemonic == "comment") {
		checkComment(element);
	} else if (mnemonic == "decuple") {
		element = decouple(element);
	} else if (mnemonic == "panel") {
		element = combinePanel(element);
	} else if (mnemonic == "edge") {
		element = checkEdge(element, attrGr->getLabel());
	} else if (mnemonic == "articleNr") {
		insertArticleNr(element);
	} else {
		errors->raiseError("Warning", "Rule with value [" + mnemonic + "] is not implemented");
	}
}

// ### application defined rules ###
std::string semanticAction::checkIntegrityCondition(std::string element,
	int length, char mode, const std::string &label) const {

	checkMode(element, mode, label);
	return checkLength(element, length, mode, label);
}

void semanticAction::checkMode(const std::string &element, char mode, const std::string &label) const {
	for (auto it : element) {
		if (mode == 'A') {
			if (!(it >= 'A' && it <= 'Z') &&
				!(it >= 'a' && it <= 'z') &&
				!(it >= '0' && it <= '9') &&

				// umlauts and the sharp s
				(it != -60) && (it != -42) && (it != -36) &&
				(it != -28) && (it != -10) && (it != -4) && (it != -33) &&

				(it != '_') && (it != '-') && (it != ' ') && (it != '+') &&
				(it != ',') && (it != '&') && (it != '.') && (it != ';') &&
				(it != ':') && (it != ' ') && (it != '{') && (it != '}') &&
				(it != '/') && (it != '\\') && (it != '|')) {

				errors->raiseError("Warning", "Element [" + label + "] is not alpha-numerical");
			}
		} else if (mode == 'N') {
			if (!(it >= '0' && it <= '9') && (it != '.')) {
				errors->raiseError("Fatal", "Element [" + label + "] is not numerical");
			}
		} else {
			errors->raiseError("Fatal", "Element [" + label + "] has mode that is not implemented");
		}
	}
}

std::string semanticAction::checkLength(std::string element, int length, char mode, const std::string &label) const {
	if (element.size() > unsigned(length) && mode == 'A') {
		element = element.substr(0, length);
		errors->raiseError("Warning", "Element [" + label + "] is longer than defined in configuration file");
	} else if (element.size() > unsigned(length) && mode == 'N') {
		element = "";
		errors->raiseError("Fatal", "Element [" + label + "] is longer than defined in configuration file");
	} else if (mode == 'A') {
		while (element.size() < unsigned(length)) {
			element = element + ' ';
		}
	} else if (mode == 'N') {
		while (element.size() < unsigned(length)) {
			element = ' ' + element;
		}
	} else {
		errors->raiseError("Fatal", "Element [" + label + "] has mode that is not implemented");
	}
	return element;
}

void semanticAction::writeLine() {
	std::string line {""};

	for (auto it : prepareToWriteVec) {
		line += it;
	}

	// trim comment line if too long
	if (ignoreRules) {
		int commentLength = std::stoi(files->getConfigLine("commentLength"));
		line = checkIntegrityCondition(line, commentLength, 'A', "Comment");
	}

	line += "\n";

	prepareToWriteVec.clear();
	files->write(line);
	ignoreRules = false;
}

void semanticAction::pushToVec(const std::string &element) {
	prepareToWriteVec.push_back(element);
}

// ### user defined rules ###
void semanticAction::checkComment(const std::string &element) {
	if (prepareToWriteVec.size() <= 0) {
		errors->raiseError("Fatal", "rule [comment] must not be used in first attribute grammar");
	}

	// make comment if first two elements are empty
	// ignore rest of the rules and write as is to output file
	if (element.find_first_not_of(' ') == -1 &&
		prepareToWriteVec[0].find_first_not_of(' ') == -1) {

		prepareToWriteVec[0] = char(std::stoi(files->getConfigLine("commentChar")));
		ignoreRules = true;
	}
}

std::string semanticAction::decouple(std::string element) const {
	if (element.size() == 0) {
		return "";
	} else {
		element = std::to_string(std::stod(element) * 10);
		return element.erase(element.find_first_of('.'));
	}
}

std::string semanticAction::combinePanel(std::string element) {
	if (prepareToWriteVec.size() <= 5) {
		errors->raiseError("Fatal", "rule [panel] must not be used in the first six attribute grammars");
	}

	// save and format panel and hoFM for later
	int element_size = element.size();
	std::string hoFM = prepareToWriteVec[5];
	int hoFM_size = hoFM.size();
	hoFM = hoFM.erase(0, hoFM.find_first_not_of(' '));

	// if panel begins with '+' or '-'
	if (element.front() == '+' || element.front() == '-') {
		element = element.front() + hoFM + " " + element.substr(1);

	// if panel begins with '&'
	} else if (element.front() != '&') {
		element = hoFM + " " + element;
	}

	element = checkIntegrityCondition(element, element_size + hoFM_size, 'A', "Platte");
	prepareToWriteVec[5] = "";
	return element;
}

std::string semanticAction::checkEdge(std::string element, const std::string &label) const {
	if (element.size() == 0) {
		return "";
	}

	// insert 1 before string if "U_" or "u_"
	// insert 0 else
	if (element.find("U_") == 0 || element.find("u_") == 0) {
		element.erase(0, 2);
		element = "1" + element;
	} else {
		element = "0" + element;
	}

	// delete commas
	while (element.find(',') != -1) {
		element.erase(element.find(','), 1);
	}

	// format numerical and alpha-numberical part
	if (element.find_last_of(' ') != -1) {
		int pos = element.find_last_of(' ');

		bool isNumeric {true};
		while (unsigned(pos) < element.size() - 1 && isNumeric) {
			char it = element[pos + 1];
			if (!(it >= '0' && it <= '9') && (it != '.')) {
				isNumeric = false;
			}
			pos++;
		}

		if (isNumeric) {
			std::string front = element.substr(0, element.find_last_of(' '));
			std::string back = element.substr(element.find_last_of(' ') + 1);
			back = decouple(back);

			front = checkIntegrityCondition(front, 5, 'A', label);
			back = checkIntegrityCondition(back, 3, 'N', label);
			element = front + back;
		}
	}

	return element;
}

void semanticAction::insertArticleNr(std::string element) {
	if (prepareToWriteVec.size() < 16) {
		return;
	}

	// write column 17 to 07
	if (element.find_first_not_of(' ') != -1) {
		element = checkIntegrityCondition(element, 12, 'A', "Lieferant");
		prepareToWriteVec[5] = "";
		prepareToWriteVec[6] = element;
	}

	// write column 03 -> 14
	element = prepareToWriteVec[2];
	if (element.find_first_not_of(' ') != -1) {
		element = checkIntegrityCondition(element, 30, 'A', "Bemerkung");
		prepareToWriteVec[13] = element;
	}

	// write column 16 -> 03
	element = prepareToWriteVec[15];
	if (element.find_first_not_of(' ') != -1) {
		element = checkIntegrityCondition(element, 18, 'A', "Teil");
		prepareToWriteVec[2] = element;
	}
	prepareToWriteVec[15] = "";
	prepareToWriteVec[2] = checkIntegrityCondition(prepareToWriteVec[2], 18, 'A', "Teil");
}
