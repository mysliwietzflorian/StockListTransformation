#include "parserHandler.h"

#include <iostream>
#include "fileHandler.h"

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
: data {nullptr} {

}
