#include "./attributeGrammar.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ### public methods ###
attributeGrammar::attributeGrammar(const std::string &name, const std::string &id, int offset,
	int length, char mode, const std::string &label)
: name {name}
, id {id}
, offset {offset}
, length {length}
, mode {mode}
, label {label}
, rules {new stringVec} {}

std::string attributeGrammar::getName() const {
	return name;
}

std::string attributeGrammar::getId() const {
	return id;
}

int attributeGrammar::getOffset() const {
	return offset;
}

int attributeGrammar::getLength() const {
	return length;
}

char attributeGrammar::getMode() const {
	return mode;
}

std::string attributeGrammar::getLabel() const {
	return label;
}

std::shared_ptr<stringVec> attributeGrammar::getRules() const {
	return rules;
}

void attributeGrammar::print() const {
	std::cout << name << "[" << id << "] = {";
	std::cout << offset << ", ";
	std::cout << length << ", ";
	std::cout << mode << ", ";
	std::cout << label << ", [";

	for (auto it : *(rules.get())) {
		if (it != rules.get()->back()) {
			std::cout << it << ", ";
		}
		else {
			std::cout << it;
		}
	}
	std::cout << "]};" << std::endl;
}
