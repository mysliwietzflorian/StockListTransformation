#include "./applicationError.h"

#include <iostream>
#include <stdlib.h>
#include "../fileHandling/fileHandler.h"
#include "../parserHandling/parserHandler.h"
#include "../semanticActionHandling/semanticActionHandler.h"

extern applicationError *errors = applicationError::getInstance();

// ### public methods ###
applicationError *applicationError::getInstance() {
	if (instance == nullptr) {
		instance = new applicationError();
	}
	return instance;
}

void applicationError::deleteInstance() {
	if (instance != nullptr) {
		// instance gets deleted
		delete instance;
	}
}

void applicationError::atExitFunction(void) {
	files->deleteInstance();
	errors->deleteInstance();
	parser->deleteInstance();
	actions->deleteInstance();
	system("pause");
}

void applicationError::raiseError() {
	std::cout << std::endl << "Fatal. Compilation aborted." << std::endl;
	files->resetOutputFile();
	atExitFunction();
	std::exit(EXIT_FAILURE);
}

void applicationError::raiseError(std::string severity, std::string message, bool abort) {
	// print error or warning if configured
	if (severity == "Fatal" && files->getConfigLine("printErrors") == "true" ||
		severity == "Warning" && files->getConfigLine("printWarnings") == "true") {

		std::cout << severity + ". " + message + "." << std::endl;
	}

	// raise error (or warning if configured)
	if (abort && (severity == "Fatal" ||
		(severity == "Warning" && files->getConfigLine("treatWarnings") == "true"))) {

		raiseError();
	}
}

// ### private methods ###
applicationError *applicationError::instance = nullptr;
