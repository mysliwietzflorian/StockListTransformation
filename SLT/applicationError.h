#ifndef applicationError_h
#define applicationError_h

#include <string>

extern void atExitFunction(void);

// singleton pattern: only one instance of this class can be created
class applicationError final {
public:
	// get instance with always same address
	static applicationError *getInstance();

	// delete instance after usage
	void deleteInstance();

	// raise unspecific error that leads to abortion of program
	// error message is provided by user; application gets stopped safely
	// should only be used in constructor of fileHandler, as no config statements are accessible
	void raiseError();

	// raise locationally unspecific error with severity (fatal or warning) and message
	void raiseError(std::string severity, std::string message, bool abort = true);

private: // hidden constructor && destructor
	static applicationError *instance;

	// default constructor and destructor
	applicationError() = default;
	virtual ~applicationError() = default;
	// copy and move constructors are deleted
	applicationError(const applicationError &) = delete;
	applicationError(applicationError &&) = delete;
};

extern applicationError *errors;

#endif
