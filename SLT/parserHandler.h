#ifndef parser_h
#define parser_h

#include <memory>
#include <string>
#include <vector>
#include "attributeGrammar.h"

// typedefs for parsingStruct
typedef std::shared_ptr<attributeGrammar> attrGrammarPtr;
typedef std::vector<attrGrammarPtr> attrGrammarPtrVec;
typedef struct attrGrammarType {
    std::string type {"<no_type>"};
    std::shared_ptr<attrGrammarPtrVec> grammar {nullptr};
} attrGrammarType;
typedef std::vector<attrGrammarType> parsingStruct;

// singleton pattern: only one instance of this class can be created
class parserHandler final {
public:
	// get instance with always same address
	static parserHandler *getInstance();

	// delete instance after usage
	void deleteInstance();

    // (if configured) request header and write to output file
    void headerRequest();

    // parse input file, execute rules and write to output file
    void parseInputFile();

private: // hidden constructor && destructor
	static parserHandler *instance;

	parserHandler();
	virtual ~parserHandler() = default;
	// copy and move constructors are deleted
	parserHandler(const parserHandler &) = delete;
	parserHandler(parserHandler &&) = delete;

private: // components
	std::shared_ptr<parsingStruct> data {nullptr};
};

extern parserHandler *parser;

#endif
