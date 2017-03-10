#ifndef attributeGrammar_h
#define attributeGrammar_h

#include <memory>
#include <string>
#include <vector>

typedef std::vector<std::string> stringVec;

class attributeGrammar final {
public:
	attributeGrammar() = delete;
	attributeGrammar(const std::string &name, const std::string &id, int offset,
		int length, char mode, const std::string &label);
	virtual ~attributeGrammar() = default;
	// delete copy and move constructors
	attributeGrammar(const attributeGrammar &) = delete;
	attributeGrammar(attributeGrammar &&) = delete;

	// getter and setter methods
	std::string getName() const;
	std::string getId() const;
	int getOffset() const;
	int getLength() const;
	char getMode() const;
	std::string getLabel() const;
	std::shared_ptr<stringVec> getRules() const;

	// method to print components of class
	void print() const;

private:
	std::string name {"<no_name>"};
	std::string id {"<no_id>"};
	int offset {0};
	int length {0};
	char mode {'A'};
	std::string label {"<no_label>"};
	std::shared_ptr<stringVec> rules {std::make_shared<stringVec>()};
};

#endif
