#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>

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
	// trying to use chrono
	long long seconds =  chrono::system_clock::now().time_since_epoch().count() / 1000 / 1000 / 1000;
	long long hour = (seconds / 60 / 60) % 24;
	long long minutes = (seconds / 60) % 60;
	long long remaining = seconds % 60;

	// Using ctime
	time_t now_t = time(0);
	tm now;
	localtime_s(&now, &now_t);
	bool pm = now.tm_hour >= 12;
	output << "[" << (now.tm_mon + 1) << "/" << now.tm_mday << "/" << (now.tm_year + 1900) << " " <<
		(now.tm_hour % 12) << ":" << now.tm_min << ":" << now.tm_sec << (pm ? "pm" : "am") << "] ";
}

SystemLogger &SystemLogger::operator<<(const char i) {
	cout << i;
	//PrintTime();
	output << i;
	return *this;
}
	
SystemLogger &SystemLogger::operator<<(const short i) {
	cout << i;
	//PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const int i) {
	cout << i;
	//PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const long i) {
	cout << i;
	//PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const long long i) {
	cout << i;
	//PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned char i) {
	cout << i;
	//PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned short i) {
	cout << i;
	//PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned int i) {
	cout << i;
	//PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned long i) {
	cout << i;
	//PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(const unsigned long long i) {
	cout << i;
	//PrintTime();
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
	//PrintTime();
	output << i;
	return *this;
}

SystemLogger & SystemLogger::operator<<(ostream &(*pf)(ostream &)) {
	cout << pf;
	//PrintTime();
	output << pf;
	return *this;
}

SystemLogger & SystemLogger::operator<<(ios &(*pf)(ios &)) {
	cout << pf;
	//PrintTime();
	output << pf;
	return *this;
}

SystemLogger & SystemLogger::operator<<(ios_base &(*pf)(ios_base &)) {
	cout << pf;
	//PrintTime();
	output << pf;
	return *this;
}

SystemLogger & SystemLogger::flush() {
	cout.flush();
	output.flush();
	return *this;
}
