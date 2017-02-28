#ifndef parsingStruct_h
#define parsingStruct_h

#include <memory>
#include <string>
#include <vector>
#include "./attributeGrammar.h"

// typedefs for parsingStruct
typedef std::shared_ptr<attributeGrammar> attrGrammarPtr;
typedef std::vector<attrGrammarPtr> attrGrammarPtrVec;

class attrGrammarType final {
public:
    attrGrammarType(std::string name, std::string type);
    virtual ~attrGrammarType() = default;

    //getter methods
    std::string getName();
    std::string getType();
    std::shared_ptr<attrGrammarPtrVec> getGrammar();

    // temporary method to print components of class
    void print();

private:
    std::string name {"<no_name>"};
    std::string type {"<no_type>"};
    std::shared_ptr<attrGrammarPtrVec> grammar {new attrGrammarPtrVec};
};

typedef std::vector<attrGrammarType> parsingStruct;

// typedefs for rulesStruct
class rulesType final {
public:
    rulesType(std::string name, std::string mnemonic);
    virtual ~rulesType() = default;

    // getter and setter methods
    std::string getName();
    std::string getMnemonic();

    // temporary method to print components of class
    void print();

private:
    std::string name {"<no_name>"};
    std::string mnemonic {"<no_mnemonic>"};
};

typedef std::vector<rulesType> rulesStruct;


#endif
