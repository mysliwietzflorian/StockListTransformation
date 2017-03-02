#include "./semanticActionHandler.h"

#include <vector>
#include <string>
#include <iostream>
#include "../fileHandling/fileHandler.h"
#include "../errorHandling/applicationError.h"
#include "../parserHandling/parserHandler.h"
#include "../parserHandling/parsingStruct.h"

static std::vector<std::string> prepareToWriteVec {};

// forward declarations
static void findRuleFunction(std::string mnemonic, std::string &element, attrGrammarPtr attrGr);
static std::string checkIntegrityCondition(std::string element, int length,
	char mode, std::string label);
static void checkMode(std::string element, char mode, std::string label);
static std::string checkLength(std::string element, int length, char mode, std::string label);
static void writeLine();
static void pushToVec(std::string element);
static std::string popFromVec(std::string element);

void semanticAction::executeRules(attrGrammarPtr attrGr, std::string element) {
	for (auto it : *(attrGr.get()->getRules().get())) {
		std::string mnemonic = parser->getMnemonicFromRulesStruct(it);

		findRuleFunction(mnemonic, element, attrGr);
	}
}

static void findRuleFunction(std::string mnemonic, std::string &element, attrGrammarPtr attrGr) {

	static std::string buffer {""};

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

// application defined rules
static std::string checkIntegrityCondition(std::string element, int length, char mode, std::string label) {
	checkMode(element, mode, label);
	return checkLength(element, length, mode, label);
}

static void checkMode(std::string element, char mode, std::string label) {
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

static std::string checkLength(std::string element, int length, char mode, std::string label) {
	if (element.size() > length) {
		element = element.substr(0, length);
		errors->raiseError("Warning", "Element [" + label + "] is longer than defined in configuration file");
	} else if (mode == 'A') {
		while (element.size() < length) {
			element = element + ' ';
		}
	} else if (mode == 'N') {
		while (element.size() < length) {
			element = ' ' + element;
		}
	} else {
		errors->raiseError("Fatal", "Element [" + label + "] has mode that is not implemented");
	}
	return element;
}

static void writeLine() {
	std::string line {""};

	for (auto it : prepareToWriteVec) {
		line += it;
	}
	line += "\n";

	prepareToWriteVec.clear();
	files->write(line);
}

static void pushToVec(std::string element) {
	prepareToWriteVec.push_back(element);
}

static std::string popFromVec(std::string element) {
	std::string result = prepareToWriteVec.back();
	prepareToWriteVec.pop_back();
	return result;
}

// user defined rules
