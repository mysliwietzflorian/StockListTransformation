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
	void findRuleFunction(const std::string &mnemonic, std::string &element, attrGrammarPtr attrGr);

	// ### private application defined rules ###
	// abstract mode and length checks
	std::string checkIntegrityCondition(std::string element, int length,
		char mode, const std::string &label) const;

	// check if all characters in element fit the mode
	void checkMode(const std::string &element, char mode, const std::string &label) const;

	// check for length of element and truncate if needed
	// make left-aligned or right-aligned depending on the mode
	std::string checkLength(std::string element, int length, char mode, const std::string &label) const;

	// pop all elements from prepareToWriteVec
	// add \n and write to output file defined in configuration file
	void writeLine();

	// push element into prepareToWriteVec
	void pushToVec(const std::string &element);

	// ### private user defined rules ###
	// add commentChar if both elements are empty
	// trim to commentLength defined in configuration file
	void checkComment(const std::string &element);

	// multiply number by 10 and return result
	std::string decouple(std::string element) const;

	// combine 6th and 7th attribute with special rules
	// repeat checkIntegrityCondition
	std::string combinePanel(std::string element);

	// insert 1 before string if "U_" or "u_"
	// insert 0 else
	// delete commas
	// format in a way defined in documentation
	std::string checkEdge(std::string element, const std::string &label) const;

	// insert articleNr into 3rd attribute grammar if panel starts with '&'
	// repeat checkIntegrityCondition
	void insertArticleNr(std::string element);

private: // components
	std::vector<std::string> prepareToWriteVec {};
	bool ignoreRules {false};
};

extern semanticAction *actions;

#endif
