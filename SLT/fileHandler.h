#ifndef fileHandler_h
#define fileHandler_h

#include <fstream>
#include <string>

// singleton pattern: only one instance of this class can be created
class fileHandler final {
public:
	// get instance with always same address
	static fileHandler *getInstance();

	// delete instance after usage
	void deleteInstance();

	// write to outFile
	void write(std::string str);

	// read line from inFile
	std::string getLine();

	// read line from configFile
	std::string getConfigLine();

	// read values from configFile with specific attribute
	std::string getConfigLine(std::string attribute);

	// jump back to begin of configFile
	void resetConfigFile();

private: // hidden constructor && destructor
	static fileHandler *instance;

	fileHandler();
	virtual ~fileHandler();
	// copy and move constructors are deleted
	fileHandler(const fileHandler &) = delete;
	fileHandler(fileHandler &&) = delete;

private: // components
	std::fstream configFile {"stl.conf"};
	std::fstream inFile {};
	std::fstream outFile {};
};

extern fileHandler *files;

#endif
