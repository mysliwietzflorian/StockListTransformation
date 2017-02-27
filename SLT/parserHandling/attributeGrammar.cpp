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

stringVec attributeGrammar::getRules() {
    return *(rules.get());
}

void attributeGrammar::print() {
    std::cout << "name:   " << name << std::endl;
    std::cout << "id:     " << id << std::endl;
    std::cout << "offset: " << offset << std::endl;
    std::cout << "length: " << length << std::endl;
    std::cout << "mode:   " << mode << std::endl;
    std::cout << "label:  " << label << std::endl;
    std::cout << "rules:  " << std::endl;

    for (auto it : *rules) {
        std::cout << "	" + it << std::endl;
    }
}
