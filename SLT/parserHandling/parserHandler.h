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
	void headerRequest() const;

	// parse input file, execute rules and write to output file
	void parseInputFile() const;

	// ## public low level methods
	// get the attribute from a given configuration file line
	std::string getAttributeFromConfigLine(std::string line) const;

	// get the value(s) from a given configuration file line
	std::string getValueFromConfigLine(std::string line) const;

	// get corresponding mnemonic to name from rulesStruct
	std::string getMnemonicFromRulesStruct(const std::string &sname) const;

private: // hidden constructor && destructor
	static parserHandler *instance;

	parserHandler();
	// delete copy and move constructors
	parserHandler(const parserHandler &) = delete;
	parserHandler(parserHandler &&) = delete;

	// ### private high level methods ###
	// fill rules with mnemonics from configuration file
	void fillRulesStruct() const;

	// find grammar names from configuration file
	// initialize grammarTypes in parsingStruct
	void findGrammarNames() const;

	// tranverse configuration file and fill grammarTypes with attributeGrammar
	void fillParsingStruct() const;

	// control sizes of attrGrammarPtrVec
	void controlSizes() const;

	// parse line of input file line
	void parseInputLine(std::string line, char delimiterChar) const;

	// ### private low level methods ###
	// return first element of a configuration file value
	// erase that element from value
	std::string getElementFromValue(std::string &value) const;

	// get attrGrammarPtrVec in attrGrammarType from attribute
	std::shared_ptr<attrGrammarPtrVec> getGrammarFromTypeName(std::string attr) const;

	// parse line and construct an object of attributeGrammar
	// make that object to shared_ptr and return result
	attrGrammarPtr getAttrGrammarPtr(const std::string &line) const;

private: // components
	std::shared_ptr<parsingStruct> data {std::make_shared<parsingStruct>()};
	std::shared_ptr<rulesStruct> rules {std::make_shared<rulesStruct>()};
};

extern parserHandler *parser;

#endif
