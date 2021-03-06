#ifndef applicationError_h
#define applicationError_h

#include <string>

// singleton pattern: only one instance of this class can be created
class applicationError final {
public:
	virtual ~applicationError() = default;

	// get instance with always same address
	static applicationError *getInstance();

	// delete instance after usage
	void deleteInstance();

	// call function before error is raised or application is closed
	static void atExitFunction(void);

	// raise unspecific error that leads to abortion of program
	// error message is provided by user; application gets stopped safely
	// should only be used in constructor of fileHandler, as no config statements are accessible
	void raiseError() const;

	// raise locationally unspecific error with severity (fatal or warning) and message
	void raiseError(const std::string &severity, const std::string &message, bool abort = true) const;

private: // hidden constructor && destructor
	static applicationError *instance;

	// default constructor
	applicationError() = default;
	// delete copy and move constructors
	applicationError(const applicationError &) = delete;
	applicationError(applicationError &&) = delete;
};

extern applicationError *errors;

#endif
