#ifndef attributeGrammar_h
#define attributeGrammar_h

#include <memory>
#include <string>
#include <vector>

typedef std::vector<std::string> stringVec;

class attributeGrammar final {
public:
    attributeGrammar() = delete;
    attributeGrammar(std::string name, std::string id, int offset,
        int length, char mode, std::string label);
    virtual ~attributeGrammar() = default;
    // copy and move constructors are deleted
    attributeGrammar(const attributeGrammar &) = delete;
    attributeGrammar(attributeGrammar &&) = delete;

    // getter and setter methods
    std::string getName();
    std::string getId();
    int getOffset();
    int getLength();
    char getMode();
    std::string getLabel();
    std::shared_ptr<stringVec> getRules();

    void setRules(std::shared_ptr<stringVec> rules);

    // temporary method to print components of class
    void print();

private:
    std::string name {"<no_name>"};
    std::string id {"<no_id>"};
    int offset {0};
    int length {0};
    char mode {'A'};
    std::string label {"<no_label>"};
    std::shared_ptr<stringVec> rules {nullptr};
};

#endif
