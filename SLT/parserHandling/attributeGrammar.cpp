#include "./attributeGrammar.h"

#include <memory>
#include <string>
#include <vector>
#include <iostream>

// ### public methods ###
attributeGrammar::attributeGrammar(std::string name, std::string id, int offset,
	int length, char mode, std::string label)
: name {name}
, id {id}
, offset {offset}
, length {length}
, mode {mode}
, label {label}
, rules {new stringVec} {}

std::string attributeGrammar::getName() {
	return name;
}

std::string attributeGrammar::getId() {
	return id;
}

int attributeGrammar::getOffset() {
	return offset;
}

int attributeGrammar::getLength() {
	return length;
}

char attributeGrammar::getMode() {
	return mode;
}

std::string attributeGrammar::getLabel() {
	return label;
}

std::shared_ptr<stringVec> attributeGrammar::getRules() {
	return rules;
}

void attributeGrammar::print() {
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
