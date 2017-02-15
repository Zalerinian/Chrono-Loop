#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "../Common/Breakpoint.h"

using namespace std;

SystemLogger *SystemLogger::Info = nullptr;
SystemLogger *SystemLogger::Error = nullptr;

SystemLogger::SystemLogger(const char * _path) {
	output.open(_path, ios::app);
	if(!output.is_open()) {
		// TODO: We need folder operations, as this will crash without the Logs folder premade.
		cout << "[Error] Failed to open file stream \"" << _path << "\"" << endl;
		Debug::SetBreakpoint();
	}
	// booleans print true/false instead of 1/0
	cout << boolalpha;
	output << boolalpha;
}

SystemLogger::~SystemLogger() {
	output.close();
}

void SystemLogger::PrintTime() {
	if (canPrintTime) {
		canPrintTime = false;
		time_t now_t = chrono::system_clock::to_time_t(chrono::system_clock::now());
		tm now;
		localtime_s(&now, &now_t);
		output << nouppercase << "[" << put_time(&now, "%D %r%p") << "] ";
	}
}

SystemLogger &SystemLogger::operator<<(const char i) {
	cout << i;
	PrintTime();
	output << i;
	return *this;
}
	
SystemLogger &SystemLogger::operator<<(const short i) {
	cout << i;
	PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const int i) {
	cout << i;
	PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const long i) {
	cout << i;
	PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const long long i) {
	cout << i;
	PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned char i) {
	cout << i;
	PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned short i) {
	cout << i;
	PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned int i) {
	cout << i;
	PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned long i) {
	cout << i;
	PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned long long i) {
	cout << i;
	PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const char * i) {
	cout << i;
	PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const std::string &i) {
	cout << i;
	PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(ostream &(*pf)(ostream &)) {
	cout << pf;
	output << pf;
	return *this;
}

SystemLogger & SystemLogger::operator<<(ios &(*pf)(ios &)) {
	cout << pf;
	output << pf;
	return *this;
}

SystemLogger & SystemLogger::operator<<(ios_base &(*pf)(ios_base &)) {
	cout << pf;
	output << pf;
	return *this;
}

SystemLogger & SystemLogger::operator<<(void (*pf)(SystemLogger &)) {
	pf(*this);
	return *this;
}

SystemLogger & SystemLogger::operator<<(void (SystemLogger::* pf)()) {
	(this->*(pf))();
	return *this;
}

SystemLogger & SystemLogger::flush() {
	cout.flush();
	output.flush();
	return *this;
}
