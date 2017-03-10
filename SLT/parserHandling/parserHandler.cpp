#include "./parserHandler.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "./attributeGrammar.h"
#include "../errorHandling/applicationError.h"
#include "../fileHandling/fileHandler.h"
#include "../semanticActionHandling/semanticActionHandler.h"

extern parserHandler *parser = parserHandler::getInstance();

// ### public methods ###
parserHandler *parserHandler::getInstance() {
	if (instance == nullptr) {
		instance = new parserHandler();
	}
	return instance;
}

void parserHandler::deleteInstance() {
	if (instance != nullptr) {
		// instance gets deleted
		delete instance;
	}
}

void parserHandler::headerRequest() const {
	if (files->getConfigLine("printHeader") != "true") {
		return;
	}

	for (auto it1 : *(data.get())) {
		// if type 'nonrecurring' print label to command line for usability
		// get element from command line and parse it through defined rules
		if (it1.getType() == "nonrecurring") {

			std::string element {""};
			for (auto it2 : *(it1.getGrammar().get())) {
				std::cout << it2->getLabel() << ": ";
				std::getline(std::cin, element);
				actions->executeRules(it2, element);
			}
		}
	}
	std::cout << std::endl;
}

void parserHandler::parseInputFile() const {
	char delimiterChar = char(std::stoi(files->getConfigLine("delimiterChar")));
	std::string line = files->getLine();

	while (!line.empty()) {
		parseInputLine(line, delimiterChar);
		line = files->getLine();
	}
}

// ## public low level methods
std::string parserHandler::getAttributeFromConfigLine(std::string line) const {
	// get attribute before '=' and trim whitespaces
	line = line.substr(0, line.find("="));
	line = line.substr(0, line.find_first_of(' '));
	return line;
}

std::string parserHandler::getValueFromConfigLine(std::string line) const {
	// get value after '=' and before ';' and trim whitespaces
	line = line.substr(line.find('=') + 1, line.find(';') - line.find('=') - 1);
	line = line.substr(line.find_first_not_of(' '));
	return line;
}

std::string parserHandler::getMnemonicFromRulesStruct(const std::string &name) const {
	// cycle through rulesStruct to find corresponding mnemonic
	for (auto it : *(rules.get())) {
		if (it.getName() == name) {
			return it.getMnemonic();
		}
	}
	errors->raiseError("Warning", "Rule with name [" + name + "] in configuration file not found. Mnemonic could not be returned");
	return "";
}

// ### private methods
parserHandler *parserHandler::instance = nullptr;

parserHandler::parserHandler()
: data {new parsingStruct}
, rules {new rulesStruct} {

	std::cout << "StockListTransformation [Version 17w10a]" << std::endl;
	std::cout << "(c) 2017 Mysliwietz Florian. Alle Rechte vorbehalten." << std::endl << std::endl;

	fillRulesStruct();
	findGrammarNames();
	fillParsingStruct();
	controlSizes();
}

// ### private high level methods ###
void parserHandler::fillRulesStruct() const {
	files->resetConfigFile();
	std::string line = files->getConfigLine();

	while (!line.empty()) {
		if (line.find("rule") == 0) {

			std::string name = getAttributeFromConfigLine(line);
			name = name.substr(name.find('[') + 1, name.find(']') - name.find('[') - 1);
			std::string mnemonic = getValueFromConfigLine(line);

			rules.get()->push_back(rulesType {name, mnemonic});
		}
		line = files->getConfigLine();
	}
	files->resetConfigFile();

	if (rules.get()->size() == 0) {
		errors->raiseError("Fatal", "No semantic action rules were added in configuration file");
	}
}

void parserHandler::findGrammarNames() const {
	files->resetConfigFile();
	std::string line {files->getConfigLine()};

	while (!line.empty()) {
		if (line.find("[]") != -1) {
			std::string attr = getAttributeFromConfigLine(line);
			attr = attr.substr(0, attr.find("[]"));
			std::string value = getValueFromConfigLine(line);

			if (value == "recurring" || value == "nonrecurring") {
				data.get()->push_back(attrGrammarType {attr, value});
			} else {
				errors->raiseError("Fatal", "Only \"recurring\" and \"nonrecurring\" is possible as value [" + line + "]");
			}
		}
		line = files->getConfigLine();
	}
	files->resetConfigFile();

	if (data.get()->size() == 0) {
		errors->raiseError("Fatal", "No attribute grammar was added in configuration file");
	}
}

void parserHandler::fillParsingStruct() const {
	files->resetConfigFile();
	std::string line {files->getConfigLine() };

	while(!line.empty()) {
		std::string attr = getAttributeFromConfigLine(line);
		std::string value = getValueFromConfigLine(line);

		if (attr.find('[') != -1 && value.front() == '{') {
			std::shared_ptr<attrGrammarPtrVec> grammar = getGrammarFromTypeName(attr);
			grammar.get()->push_back(getAttrGrammarPtr(line));
		}
		line = files->getConfigLine();
	}
	files->resetConfigFile();
}

void parserHandler::controlSizes() const {
	for (auto it : *(data.get())) {
		if (it.getGrammar()->size() == 0) {
			errors->raiseError("Warning", "No attribute grammar is defined for [" + it.getName() + "]");
		}
	}
}

void parserHandler::parseInputLine(std::string line, char delimiterChar) const {
	std::cout << "Processing: " << line << std::endl;

	for (auto it1 : *(data.get())) {
		if (it1.getType() == "recurring") {

			std::string element {""};
			for (auto it2 : *(it1.getGrammar().get())) {
				// get element without semicolon from input file line
				element = line.substr(0, line.find_first_of(delimiterChar));
				// trim whitespaces from element
				element = element.erase(0, element.find_first_not_of(' '));
				element = element.erase(element.find_last_not_of(' ') + 1);

				// delete element with semicolon from input file line
				line.erase(0, line.find(delimiterChar) + 1);
				actions->executeRules(it2, element);
			}
			std::cout << std::endl;
		}
	}
}

// ### private low level methods ###
std::string parserHandler::getElementFromValue(std::string &value) const {
	std::string element = "";

	// return if value is empty
	if (value.empty() || (value.find("{}") == 0 && value.size() == 2)) {
		return element;
	}

	// raise error if no closing curly bracket is set in configuration file
	if (value.back() != '}') {
		errors->raiseError("Fatal", "Value [" + value + "] in configuration file is not closed with curly brackets '}'");
	}

	// erase empty square brackets from value
	if (value.find("[]") != -1) {
		value = value.erase(value.find("[]"), 2);
	}

	// erase leading spaces after open curly bracket
	value = value.erase(1, value.find_first_not_of(' ', 1) - 1);

	// find out whether second character is begin of rules or not
	int pos = 0;
	(value.find('[') != 1) ? (pos = 1) : (pos = 2);

	if (value.find(',') != -1) {
		// get single element from value
		element = value.substr(pos, value.find(',') - pos);
		element = element.erase(0, element.find_first_not_of(' '));
		element = element.erase(element.find_last_not_of(' ') + 1);

		// delete first element in value
		value = value.erase(pos, value.find(',') - pos + 1);
		value = value.erase(pos, value.find_first_not_of(' ', pos) - pos);
	} else {
		// get last and only element from value
		element = value.substr(pos, value.find('}') - pos);
		element = element.erase(0, element.find_first_not_of(' '));

		if (pos == 2) {
			element = element.erase(element.find(']'));
			element = element.erase(element.find_last_not_of(' ') + 1);
		}

		// delete last and only element in value
		if (pos == 1) {
			value = value.erase(pos, value.find('}') - pos);
		} else if (pos == 2) {
			value = value.erase(pos, value.find(']') - pos);
			value = value.erase(value.find(']') + 1, value.find('}') - value.find(']') - 1);
		}
	}

	// erase empty square brackets from value
	if (value.find("[]") != -1) {
		value = value.erase(value.find("[]"), 2);
	}

	return element;
}

std::shared_ptr<attrGrammarPtrVec> parserHandler::getGrammarFromTypeName(std::string attr) const {
	attr = attr.substr(0, attr.find('['));

	for (auto it : *(data.get())) {
		if (it.getName() == attr) {
			return it.getGrammar();
		}
	}
	errors->raiseError("Fatal", "Attribute grammar with name [" + attr + "] in configuration file not found. Grammar could not be returned");
	return std::make_shared<attrGrammarPtrVec>();
}

attrGrammarPtr parserHandler::getAttrGrammarPtr(const std::string &line) const {
	std::string name {""};
	std::string id {""};
	int offset {0};
	int length {0};
	char mode {' '};
	std::string label {""};

	name = getAttributeFromConfigLine(line);
	name = name.substr(0, name.find('['));

	id = getAttributeFromConfigLine(line);
	id = id.substr(id.find('[') + 1, id.find(']') - id.find('[') - 1);

	std::string value = getValueFromConfigLine(line);

	if (value.find("{}") != 0 && value.size() != 0) {
		offset = std::stoi(getElementFromValue(value));
	} else {
		errors->raiseError("Fatal", "Wrong number of arguments in line [" + line + "] from configuration file");
	}

	if (value.find("{}") != 0 && value.size() != 0) {
		length = std::stoi(getElementFromValue(value));
	} else {
		errors->raiseError("Fatal", "Wrong number of arguments in line [" + line + "] from configuration file");
	}

	if (value.find("{}") != 0 && value.size() != 0) {
		mode = getElementFromValue(value).at(0);
		if (mode != 'A' && mode != 'N') {
			errors->raiseError("Fatal", "In configuration file [" + line +"] argument mode [" + mode + "] has to be 'A' or 'N'");
		}
	} else {
		errors->raiseError("Fatal", "Wrong number of arguments in line [" + line + "] from configuration file");
	}

	if (value.find("{}") != 0 && value.size() != 0) {
		label = getElementFromValue(value);
	} else {
		errors->raiseError("Fatal", "Wrong number of arguments in line [" + line + "] from configuration file");
	}

	auto attrGr = std::make_shared<attributeGrammar>(name, id, offset, length, mode, label);
	auto attrGrRules = attrGr.get()->getRules();

	while (value.find("{}") != 0 && value.size() != 0) {
		attrGrRules.get()->push_back(getElementFromValue(value));
	}

	if (attrGrRules.get()->size() == 0) {
		errors->raiseError("Fatal", "No rules are defined in " + name + "[" + id + "] from configuration file");
	}

	return attrGr;
}
