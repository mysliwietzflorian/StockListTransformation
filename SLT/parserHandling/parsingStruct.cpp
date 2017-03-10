#include "./parsingStruct.h"

#include <iostream>
#include <memory>
#include <string>

// ### public methods ###
attrGrammarType::attrGrammarType(const std::string &name, const std::string &type)
: name {name}
, type {type}
, grammar {std::make_shared<attrGrammarPtrVec>()} {}

std::string attrGrammarType::getName() const {
	return name;
}

std::string attrGrammarType::getType() const {
	return type;
}

std::shared_ptr<attrGrammarPtrVec> attrGrammarType::getGrammar() const {
	return grammar;
}

void attrGrammarType::print() const {
	std::cout << "name:    " << name << std::endl;
	std::cout << "type:    " << type << std::endl;
	std::cout << "grammar: " << std::endl;

	for (auto it : *(grammar.get())) {
		it.get()->print();
	}
}

// ### public methods ###
rulesType::rulesType(const std::string &name, const std::string &mnemonic)
: name {name}
, mnemonic {mnemonic} {}

std::string rulesType::getName() const {
	return name;
}

std::string rulesType::getMnemonic() const {
	return mnemonic;
}

void rulesType::print() const {
	std::cout << "name:     " << name << std::endl;
	std::cout << "mnemonic: " << mnemonic << std::endl;
}
