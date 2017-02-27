#include "parsingStruct.h"

#include <memory>
#include <string>
#include <iostream>

// ### public methods ###
attrGrammarType::attrGrammarType(std::string name, std::string type)
: name {name}
, type {type}
, grammar {new attrGrammarPtrVec} {}

std::string attrGrammarType::getName() {
    return name;
}

std::string attrGrammarType::getType() {
    return type;
}

attrGrammarPtrVec attrGrammarType::getGrammar() {
    return *(grammar.get());
}

void attrGrammarType::print() {
    std::cout << "name:    " << name << std::endl;
    std::cout << "type:    " << type << std::endl;
    std::cout << "grammar: " << grammar << std::endl;
}

// ### public methods ###
rulesType::rulesType(std::string name, std::string mnemonic)
: name {name}
, mnemonic {mnemonic} {}

std::string rulesType::getName() {
    return name;
}

std::string rulesType::getMnemonic() {
    return mnemonic;
}

void rulesType::print() {
    std::cout << "name:     " << name << std::endl;
    std::cout << "mnemonic: " << mnemonic << std::endl;
}
