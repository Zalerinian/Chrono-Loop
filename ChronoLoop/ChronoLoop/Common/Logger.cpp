#include "Logger.h"
#include <iostream>
#include <chrono>

using namespace std;

SystemLogger *SystemLogger::Info = nullptr;
SystemLogger *SystemLogger::Error = nullptr;

SystemLogger::SystemLogger(const char * _path) {
	output.open(_path, ios::app);
	if(!output.is_open()) {
		// TODO: We need folder operations, as this will crash without the Logs folder premade.
		throw "something catastropihc has happened.";
	}
}

SystemLogger::~SystemLogger() {
	output.close();
}

void SystemLogger::PrintTime() {
	long long seconds =  chrono::system_clock::now().time_since_epoch().count() / 1000 / 1000;
	long long hour = (seconds / 60 / 60) % 24;
	long long minutes = (seconds / 60) % 60;
	long long remaining = seconds % 60;
}

SystemLogger &SystemLogger::operator<<(const char i) {
	cout << i;
	output << i;
	//PrintTime();
	return *this;
}
	
SystemLogger &SystemLogger::operator<<(const short i) {
	cout << i;
	output << i;
	//PrintTime();
	return *this;
}

SystemLogger & SystemLogger::operator<<(const int i) {
	cout << i;
	output << i;
	//PrintTime();
	return *this;
}

SystemLogger & SystemLogger::operator<<(const long i) {
	cout << i;
	output << i;
	//PrintTime();
	return *this;
}

SystemLogger & SystemLogger::operator<<(const long long i) {
	cout << i;
	output << i;
	//PrintTime();
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned char i) {
	cout << i;
	output << i;
	//PrintTime();
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned short i) {
	cout << i;
	output << i;
	//PrintTime();
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned int i) {
	cout << i;
	output << i;
	//PrintTime();
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned long i) {
	cout << i;
	output << i;
	//PrintTime();
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned long long i) {
	cout << i;
	output << i;
	//PrintTime();
	return *this;
}

SystemLogger & SystemLogger::operator<<(const char * i) {
	cout << i;
	output << i;
	//PrintTime();
	return *this;
}

SystemLogger & SystemLogger::operator<<(const std::string &i) {
	cout << i;
	output << i;
	//PrintTime();
	return *this;
}

SystemLogger & SystemLogger::operator<<(ostream &(*pf)(ostream &)) {
	cout << pf;
	output << pf;
	//PrintTime();
	return *this;
}

SystemLogger & SystemLogger::operator<<(ios &(*pf)(ios &)) {
	cout << pf;
	output << pf;
	//PrintTime();
	return *this;
}

SystemLogger & SystemLogger::operator<<(ios_base &(*pf)(ios_base &)) {
	cout << pf;
	output << pf;
	//PrintTime();
	return *this;
}

SystemLogger & SystemLogger::flush() {
	cout.flush();
	output.flush();
	return *this;
}
