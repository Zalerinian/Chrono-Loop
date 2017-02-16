#pragma once
#include <sstream>
#include <fstream>
//#include <mutex>

class SystemLogger {
		
	static SystemLogger *Info, *Error;

	SystemLogger(const char *_path);
	~SystemLogger();
	std::ofstream output;
	bool canPrintTime = true;
	void PrintTime();
public:

	inline static SystemLogger& GetLog() { 
		if (nullptr == Info) { 
			Info = new SystemLogger("../Logs/Log.log");
		}
		Info->canPrintTime = true;
		return *Info;
	}
	inline static SystemLogger& GetError() {
		if (nullptr == Error) {
			Error = new SystemLogger("../Logs/Error.log");
		}
		Error->canPrintTime = true;
		return *Error;
	}
	inline static void CloseLog() { if (Info != nullptr) { delete Info; } }
	inline static void CloseError() { if (Error != nullptr) { delete Error; } }

	//static void lock(SystemLogger& _log);

	SystemLogger& operator<<(const char i);
	SystemLogger& operator<<(const short i);
	SystemLogger& operator<<(const int i);
	SystemLogger& operator<<(const float i);
	SystemLogger& operator<<(const double i);
	SystemLogger& operator<<(const long i);
	SystemLogger& operator<<(const long long i);
	SystemLogger& operator<<(const unsigned char i);
	SystemLogger& operator<<(const unsigned short i);
	SystemLogger& operator<<(const unsigned int i);
	SystemLogger& operator<<(const unsigned long i);
	SystemLogger& operator<<(const unsigned long long i);
	SystemLogger& operator<<(const char *i);
	SystemLogger& operator<<(const std::string &i);
	SystemLogger& operator<<(std::ostream& (*pf)(std::ostream&)); // Stream manipulator
	SystemLogger& operator<<(std::ios& (*pf)(std::ios&));
	SystemLogger& operator<<(std::ios_base& (*pf)(std::ios_base&));
	SystemLogger& operator<<(void (*pf)(SystemLogger&));  // For custom manipulators, with which things such as mutex locking can occur.
	SystemLogger& operator<<(void (SystemLogger::*pf)());	// For custom manipulators, with which things such as mutex locking can occur.
	SystemLogger& flush();
};