#ifndef semanticActionHandler_h
#define semanticActionHandler_h

#include <vector>
#include <string>
#include "../parserHandling/parsingStruct.h"

namespace semanticAction {

void executeRules(attrGrammarPtr attrGr, std::string element);

}

#endif
