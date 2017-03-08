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
		if (ignoreRules && mnemonic == "push") {
			pushToVec(element + ";");
			if (prepareToWriteVec.size() > 0) {
				prepareToWriteVec[1] = "";
			}

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

void semanticAction::findRuleFunction(std::string mnemonic, std::string &element,
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
	} /*else if (mnemonic == "decuple") {

	} else if (mnemonic == "panel") {

	} else if (mnemonic == "surface") {

	} else if (mnemonic == "edge") {

	} else if (mnemonic == "articleNr") {

	} */else {
		// errors->raiseError("Warning", "Rule with value [" + mnemonic + "] is not implemented");
	}
}

// ### application defined rules ###
std::string semanticAction::checkIntegrityCondition(std::string element,
	int length, char mode, std::string label) {

	checkMode(element, mode, label);
	return checkLength(element, length, mode, label);
}

void semanticAction::checkMode(std::string element, char mode, std::string label) {
	for (auto it : element) {
		if (mode == 'A') {
			if (!(it >= 'A' && it <= 'Z') &&
				!(it >= 'a' && it <= 'z') &&
				!(it >= '0' && it <= '9') &&

				// TODO: umlauts and sharp s
				/*(it != 196) && (it != 214) && (it != 220) &&
				(it != 228) && (it != 246) && (it != 252) && (it != 223) &&*/

				(it != '_') && (it != '-') && (it != ' ') &&
				(it != ',') && (it != '/') && (it != '.') &&
				(it != ':') && (it != ' ') && (it != '{') && (it != '}')) {

				// errors->raiseError("Fatal", "Element [" + label + "] is not alpha-numerical");
			}
		} else if (mode == 'N') {
			if (!(it >= '0' && it <= '9')) {
				errors->raiseError("Fatal", "Element [" + label + "] is not numerical");
			}
		} else {
			errors->raiseError("Fatal", "Element [" + label + "] has mode that is not implemented");
		}
	}
}

std::string semanticAction::checkLength(std::string element, int length, char mode, std::string label) {
	if (element.size() > unsigned(length)) {
		element = element.substr(0, length);
		errors->raiseError("Warning", "Element [" + label + "] is longer than defined in configuration file");
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
		int commentLength = parser->stringToInt(files->getConfigLine("commentLength"));
		line = checkIntegrityCondition(line, commentLength, 'A', "Comment");
	}

	line += "\n";

	prepareToWriteVec.clear();
	files->write(line);
	std::cout << "#" << line << std::endl;
	ignoreRules = false;
}

void semanticAction::pushToVec(std::string element) {
	prepareToWriteVec.push_back(element);
}

// ### user defined rules ###
void semanticAction::checkComment(std::string element) {
	if (prepareToWriteVec.size() <= 0) {
		errors->raiseError("Fatal", "rule [comment] must not be used in first attribute grammar");
	}

	// make comment if first two elements are empty
	// ignore rest of the rules and write as is to output file
	if (element.find_first_not_of(' ') == -1 &&
		prepareToWriteVec[0].find_first_not_of(' ') == -1) {

		prepareToWriteVec[0] = char(parser->stringToInt(files->getConfigLine("commentChar")));
		ignoreRules = true;
	}
}
