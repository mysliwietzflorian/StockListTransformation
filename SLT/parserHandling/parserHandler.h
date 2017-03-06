#ifndef parser_h
#define parser_h

#include <memory>
#include <string>
#include <vector>
#include "./attributeGrammar.h"
#include "./parsingStruct.h"

// singleton pattern: only one instance of this class can be created
class parserHandler final {
public:
	virtual ~parserHandler() = default;

	// get instance with always same address
	static parserHandler *getInstance();

	// delete instance after usage
	void deleteInstance();

	// (if configured) request header and write to output file
	void headerRequest();

	// parse input file, execute rules and write to output file
	void parseInputFile();

	// ## public low level methods
	// get the attribute from a given configuration file line
	std::string getAttributeFromConfigLine(std::string line);

	// get the value(s) from a given configuration file line
	std::string getValueFromConfigLine(std::string line);

	// get corresponding mnemonic to name from rulesStruct
	std::string getMnemonicFromRulesStruct(std::string name);

	// convert string to integer and check for errors
	int stringToInt(std::string str);

private: // hidden constructor && destructor
	static parserHandler *instance;

	parserHandler();
	// delete copy and move constructors
	parserHandler(const parserHandler &) = delete;
	parserHandler(parserHandler &&) = delete;

	// ### private high level methods ###
	// fill rules with mnemonics from configuration file
	void fillRulesStruct();

	// find grammar names from configuration file
	// initialize grammarTypes in parsingStruct
	void findGrammarNames();

	// tranverse configuration file and fill grammarTypes with attributeGrammar
	void fillParsingStruct();

	// control sizes of attrGrammarPtrVec
	void controlSizes();

	// parse line of input file line
	void parseInputLine(std::string line, char delimiterChar);

	// ### private low level methods ###
	// return first element of a configuration file value
	// erase that element from value
	std::string getElementFromValue(std::string &value);

	// get attrGrammarPtrVec in attrGrammarType from attribute
	std::shared_ptr<attrGrammarPtrVec> getGrammarFromTypeName(std::string attr);

	// parse line and construct an object of attributeGrammar
	// make that object to shared_ptr and return result
	attrGrammarPtr getAttrGrammarPtr(std::string line);

private: // components
	std::shared_ptr<parsingStruct> data {std::make_shared<parsingStruct>()};
	std::shared_ptr<rulesStruct> rules {std::make_shared<rulesStruct>()};
};

extern parserHandler *parser;

#endif
