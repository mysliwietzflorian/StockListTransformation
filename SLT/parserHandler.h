#ifndef parser_h
#define parser_h

#include <memory>
#include <string>
#include <vector>
#include "attributeGrammar.h"
#include "parsingStruct.h"

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

    // ### private high level methods ###
    // finds grammar names from configuration file
    // initializes grammarTypes in parsingStruct
    void findGrammarNames();

    // fills rules with mnemonics from configuration file
    void fillRulesStruct();

    // ### private low level methods ###
    // get the attribute from a given configuration file line
    std::string getAttributeFromConfigLine(std::string line);

    // get the value(s) from a given configuration file line
    std::string getValueFromConfigLine(std::string line);

    // get corresponding mnemonic to name from rulesStruct
    std::string getMnemonicFromRulesStruct(std::string name);

private: // components
	std::shared_ptr<parsingStruct> data {new parsingStruct};
    std::shared_ptr<rulesStruct> rules {new rulesStruct};
};

extern parserHandler *parser;

#endif
