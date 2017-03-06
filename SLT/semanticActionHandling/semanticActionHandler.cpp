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

		findRuleFunction(mnemonic, element, attrGr);
	}
}

// ### private methods ###
semanticAction *semanticAction::instance = nullptr;

semanticAction::semanticAction()
: prepareToWriteVec {}
, buffer {} {}

void semanticAction::findRuleFunction(std::string mnemonic, std::string &element,
	attrGrammarPtr attrGr) {

	if (mnemonic == "integrityCondition") {
		element = checkIntegrityCondition(element, attrGr->getLength(),
			attrGr->getMode(), attrGr->getLabel());
	} else if (mnemonic == "writeLine") {
		writeLine();
	} else if (mnemonic == "push") {
		pushToVec(element);
	} else if (mnemonic == "pop") {
		buffer = popFromVec(element);
	} else if (mnemonic == "comment") {
		std::cout << mnemonic << ": Not implemented yet." << std::endl;
	} else if (mnemonic == "decuple") {
		std::cout << mnemonic << ": Not implemented yet." << std::endl;
	} else if (mnemonic == "panel") {
		std::cout << mnemonic << ": Not implemented yet." << std::endl;
	} else if (mnemonic == "surface") {
		std::cout << mnemonic << ": Not implemented yet." << std::endl;
	} else if (mnemonic == "edge") {
		std::cout << mnemonic << ": Not implemented yet." << std::endl;
	} else if (mnemonic == "articleNr") {
		std::cout << mnemonic << ": Not implemented yet." << std::endl;
	} else {
		errors->raiseError("Warning", "Rule with value [" + mnemonic + "] is not implemented");
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
				(it != '_') && (it != '-') && (it != ' ') &&
				(it != ',') && (it != '/') && (it != '.') &&
				(it != ':') && (it != ' ') && (it != '{') && (it != '}')) {

				errors->raiseError("Fatal", "Element [" + label + "] is not alpha-numerical");
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
	line += "\n";

	prepareToWriteVec.clear();
	files->write(line);
}

void semanticAction::pushToVec(std::string element) {
	prepareToWriteVec.push_back(element);
}

std::string semanticAction::popFromVec(std::string element) {
	std::string result = prepareToWriteVec.back();
	prepareToWriteVec.pop_back();
	return result;
}

// ### user defined rules ###
