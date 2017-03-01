/* SLT [Version 17w09a]
 * (c) 2017 Mysliwietz Florian. All rights reserved.
 *
 * StockListTransformation Application
 */

#include <iostream>

#include "./parserHandling/parserHandler.h"
#include "./errorHandling/applicationError.h"

int main() {

	parser->headerRequest();
	parser->parseInputFile();

	std::cout << std::endl << "Tool completed successfully." << std::endl;
	applicationError::atExitFunction();
	return 0;
}
