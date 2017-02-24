/* SLT [Version 17w07a]
 * (c) 2017 Mysliwietz Florian. All rights reserved.
 *
 * StockListTransformation Application
 */

#include <iostream>
#include <string>
#include "fileHandler.h"
#include "applicationError.h"

int main() {
	std::cout << std::endl << "Tool completed successfully." << std::endl;
	atExitFunction();
	return 0;
}

void atExitFunction(void) {
	files->deleteInstance();
	errors->deleteInstance();
	system("pause");
}

/*void headerRequest() {
	std::cout << "StockListTransformation [Version 0.1]" << std::endl;
	std::cout << "(c) 2017 Mysliwietz Florian. Alle Rechte vorbehalten." << std::endl << std::endl;

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

	std::cout << std::endl << "Tool completed successfully." << std::endl;
}*/
