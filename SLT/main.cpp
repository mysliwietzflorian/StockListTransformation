/* SLT [Version 17w10a]
 * (c) 2017 Mysliwietz Florian. All rights reserved.
 *
 * StockListTransformation Application
 */

#include <iostream>
#include "./errorHandling/applicationError.h"
#include "./parserHandling/parserHandler.h"

int main() {

	parser->headerRequest();
	parser->parseInputFile();

	std::cout << std::endl << "Tool completed successfully." << std::endl;
	applicationError::atExitFunction();
	return 0;
}
