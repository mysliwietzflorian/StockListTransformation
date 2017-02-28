#include "./fileHandler.h"

#include <iostream>
#include <string>
#include "../errorHandling/applicationError.h"

extern fileHandler *files = fileHandler::getInstance();

// ### public methods ###
fileHandler *fileHandler::getInstance() {
	if (instance == nullptr) {
		instance = new fileHandler();
	}
	return instance;
}

void fileHandler::deleteInstance() {
	if (instance != nullptr) {
		// delete instance and close files
		delete instance;
	}
}

void fileHandler::write(std::string str) {
	outFile << str;
}

std::string fileHandler::getLine() {
	std::string line {""};
	std::getline(inFile, line);
	return line;
}

std::string fileHandler::getConfigLine() {
	std::string line {""};
	std::getline(configFile, line);

	while (!configFile.eof() && (line.empty() || line.front() == '#')) {
		std::getline(configFile, line);
	}
	return line;
}

std::string fileHandler::getConfigLine(std::string attribute) {
	resetConfigFile();
	std::string value {getConfigLine()};

	while (!value.empty() && value.find(attribute) == -1) {
		value = getConfigLine();
	}
	if (value.empty()) {
		errors->raiseError("Warning", "Attribute [" + attribute + "] in configuration file not set");
	} else if (value.find(attribute) != -1) {
		value = value.substr(value.find('=') + 1, value.find(';') - value.find('=') - 1);
		value = value.substr(value.find_first_not_of(' '));
	}

	resetConfigFile();
	return value;
}

void fileHandler::resetConfigFile() {
	configFile.clear();
	configFile.seekg(0, std::ios::beg);
}

// ### private methods ###
fileHandler *fileHandler::instance = nullptr;

fileHandler::fileHandler()
: configFile {"SLT.conf"} {

	// check if configuration file was found in folder
	if (!configFile.is_open()) {
		std::cout << "Fatal. Configuration file [SLT.conf] not found." << std::endl;
		errors->raiseError();
		return;
	}

	// get inFileName from configuration file and open inFile
	inFile.open(getConfigLine("inFileName"));
	// check if inFile was found in folder
	if (!inFile.is_open()) {
		std::cout << "Fatal. Source file [" + getConfigLine("inFileName") + "] not found." << std::endl;
		errors->raiseError();
		return;
	}

	// get outFileName from configuration file and open outFile
	outFile.open(getConfigLine("outFileName"), std::ios::out);
}

fileHandler::~fileHandler() {
	configFile.close();
	inFile.close();
	outFile.close();
}
