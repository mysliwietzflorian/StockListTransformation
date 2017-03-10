#ifndef fileHandler_h
#define fileHandler_h

#include <fstream>
#include <string>

// singleton pattern: only one instance of this class can be created
class fileHandler final {
public:
	virtual ~fileHandler();

	// get instance with always same address
	static fileHandler *getInstance();

	// delete instance after usage
	void deleteInstance();

	// write to outFile
	void write(const std::string &str);

	// read line from inFile
	std::string getLine();

	// read line from configFile
	std::string getConfigLine();

	// read values from configFile with specific attribute
	std::string getConfigLine(const std::string &attribute);

	// jump back to begin of configFile
	void resetConfigFile();

	// delete content of output file
	void resetOutputFile();

private: // hidden constructor && destructor
	static fileHandler *instance;

	fileHandler();
	// delete copy and move constructors
	fileHandler(const fileHandler &) = delete;
	fileHandler(fileHandler &&) = delete;

private: // components
	std::fstream configFile {"SLT.conf"};
	std::fstream inFile {};
	std::fstream outFile {};
};

extern fileHandler *files;

#endif
