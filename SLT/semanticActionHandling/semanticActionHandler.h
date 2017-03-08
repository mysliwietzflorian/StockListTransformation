#ifndef semanticActionHandler_h
#define semanticActionHandler_h

#include <string>
#include <vector>
#include "../parserHandling/parsingStruct.h"

// singleton pattern: only one instance of this class can be created
class semanticAction final {
public:
	virtual ~semanticAction() = default;

	// get instance with always same address
	static semanticAction *getInstance();

	// delete instance after usage
	void deleteInstance();

    // execute all rules on a given element from attrGr
    void executeRules(attrGrammarPtr attrGr, std::string element);

private: // hidden constructor && destructor
	static semanticAction *instance;

	semanticAction();
	// delete copy and move constructors
	semanticAction(const semanticAction &) = delete;
	semanticAction(semanticAction &&) = delete;

    // call function from mnemonic
    void findRuleFunction(std::string mnemonic, std::string &element, attrGrammarPtr attrGr);

    // ### private application defined rules ###
    // abstract mode and length checks
    std::string checkIntegrityCondition(std::string element, int length,
        char mode, std::string label);

    // check if all characters in element fit the mode
    void checkMode(std::string element, char mode, std::string label);

    // check for length of element and truncate if needed
    // make left-aligned or right-aligned depending on the mode
    std::string checkLength(std::string element, int length, char mode, std::string label);

    // pop all elements from prepareToWriteVec
    // add \n and write to output file defined in configuration file
    void writeLine();

    // push element into prepareToWriteVec
    void pushToVec(std::string element);

    // pop element from prepareToWriteVec and save in buffer
    std::string popFromVec(std::string element);

    // ### private user defined rules ###
	// add commentChar if both elements are empty
	// trim to commentLength defined in configuration file
	void checkComment(std::string element);

private: // components
    std::vector<std::string> prepareToWriteVec {};
	bool ignoreRules {false};
};

extern semanticAction *actions;

#endif
