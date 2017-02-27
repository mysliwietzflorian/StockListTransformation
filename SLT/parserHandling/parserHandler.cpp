#include "./parserHandler.h"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include "../fileHandling/fileHandler.h"
#include "../errorHandling/applicationError.h"

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
		delete instance;
	}
}

void parserHandler::headerRequest() {
    std::cout << "StockListTransformation [Version 17w08b]" << std::endl;
    std::cout << "(c) 2017 Mysliwietz Florian. Alle Rechte vorbehalten." << std::endl << std::endl;

    if (files->getConfigLine("printHeader") != "true") {
        return;
    }

    std::string auftragName {"<Auftragsname>"};
	std::string objektName {"<Objektname>"};
	std::string auftragNr {"<Auftrags-Nr.>"};
	std::cout << " === Daten fuer Auftragskopf === " << std::endl;
	std::cout << " Auftragsname: ";
	std::cin >> auftragName;
	std::cout << " Objektname: ";
	std::cin >> objektName;
	std::cout << " Auftrags-Nr.: ";
	std::cin >> auftragNr;
}

void parserHandler::parseInputFile() {
    std::cout << "log: parseInputFile()" << std::endl;
}

// ### private methods
parserHandler *parserHandler::instance = nullptr;

parserHandler::parserHandler()
: data {new parsingStruct}
, rules {new rulesStruct} {

    fillRulesStruct();
    findGrammarNames();
}

// ### private high level methods ###
void parserHandler::fillRulesStruct() {
    files->resetConfigFile();
    std::string line {files->getConfigLine()};

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

void parserHandler::findGrammarNames() {
    files->resetConfigFile();
    std::string line {files->getConfigLine()};

    while (!line.empty()) {
        if (line.find("[]") != -1) {

            std::string attr = getAttributeFromConfigLine(line);
            attr = attr.substr(0, attr.find("[]"));
            std::string value = getValueFromConfigLine(line);

            data.get()->push_back(attrGrammarType {attr, value});
        }
        line = files->getConfigLine();
    }
    files->resetConfigFile();

    if (data.get()->size() == 0) {
        errors->raiseError("Fatal", "No attribute grammar was added in configuration file");
    }
}

// ### private low level methods ###
std::string parserHandler::getAttributeFromConfigLine(std::string line) {
    line = line.substr(0, line.find("="));
    line = line.substr(0, line.find_first_of(' '));
    return line;
}

std::string parserHandler::getValueFromConfigLine(std::string line) {
    line = line.substr(line.find('=') + 1, line.find(';') - line.find('=') - 1);
    line = line.substr(line.find_first_not_of(' '));
    return line;
}

std::string parserHandler::getMnemonicFromRulesStruct(std::string name) {
    for (auto it : *(rules.get())) {
        if (it.getName() == name) {
            return it.getMnemonic();
        }
    }
    errors->raiseError("Warning", "Rule with name [" + name + "] in configuration file not found. Mnemonic could not be returned");
    return "";
}
