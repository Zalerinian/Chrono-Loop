#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "Math.h"

using namespace std;
namespace Epoch {

	SL* SystemLogger::Log = nullptr;

	void SystemLogger::PrintTime() {
		time_t now_t = chrono::system_clock::to_time_t(chrono::system_clock::now());
		tm now;
		localtime_s(&now, &now_t);
		GetLog() << nouppercase << "[" << put_time(&now, "%D %r%p")._Fmtfirst << "] ";
	}

	SL::SL(const char * _path) {
		mOutput.open(_path, ios::app);
		if (!mOutput.is_open()) {
			cout << "[Error] Failed to open file stream \"" << _path << "\"" << endl;
		}
		// booleans print true/false instead of 1/0
		cout << boolalpha;
		mOutput << boolalpha;
	}

	SL::~SL() {
		mOutput.close();
	}

	SL &SL::operator<<(const char i) {
		cout << i;
		mOutput << i;
		return *this;
	}

	SL &SL::operator<<(const short i) {
		cout << i;
		mOutput << i;
		return *this;
	}

	SL & SL::operator<<(const int i) {
		cout << i;
		mOutput << i;
		return *this;
	}

	SL & SL::operator<<(const float i) {
		cout << i;
		mOutput << i;
		return *this;
	}

	SL & SL::operator<<(const double i) {
		cout << i;
		mOutput << i;
		return *this;
	}

	SL & SL::operator<<(const long i) {
		cout << i;
		mOutput << i;
		return *this;
	}

	SL & SL::operator<<(const long long i) {
		cout << i;
		mOutput << i;
		return *this;
	}

	SL & SL::operator<<(const unsigned char i) {
		cout << i;
		mOutput << i;
		return *this;
	}

	SL & SL::operator<<(const unsigned short i) {
		cout << i;
		mOutput << i;
		return *this;
	}

	SL & SL::operator<<(const unsigned int i) {
		cout << i;
		mOutput << i;
		return *this;
	}

	SL & SL::operator<<(const unsigned long i) {
		cout << i;
		mOutput << i;
		return *this;
	}

	SL & SL::operator<<(const unsigned long long i) {
		cout << i;
		mOutput << i;
		return *this;
	}

	SL & SL::operator<<(const char * i) {
		cout << i;
		mOutput << i;
		return *this;
	}

	SL & SL::operator<<(const std::string &i) {
		cout << i;
		mOutput << i;
		return *this;
	}

	SL& SL::operator<<(const vec4i & i) {
		cout << "(" << i.x << ", " << i.y << ", " << i.z << ", " << i.w << ")i";
		mOutput << "(" << i.x << ", " << i.y << ", " << i.z << ", " << i.w << ")i";
		return *this;
	}

	SL& SL::operator<<(const vec4f & i) {
		cout << "(" << i.x << ", " << i.y << ", " << i.z << ", " << i.w << ")f";
		mOutput << "(" << i.x << ", " << i.y << ", " << i.z << ", " << i.w << ")f";
		return *this;
	}

	SL& SL::operator<<(const vec3f & i) {
		cout << "(" << i.x << ", " << i.y << ", " << i.z << ")f";
		mOutput << "(" << i.x << ", " << i.y << ", " << i.z << ")f";
		return *this;
	}

	SL& SL::operator<<(const vec2f & i) {
		cout << "(" << i.x << ", " << i.y << ")f";
		mOutput << "(" << i.x << ", " << i.y << ")f";
		return *this;
	}

	SL& SL::operator<<(const matrix4 & m) {
		cout << "[";
		mOutput << "[";
		for (unsigned int i = 0; i < 16; ++i) {
			cout << m[i / 4][i % 4];
			mOutput << m[i / 4][i % 4];
			if (i != 15) {
				cout << ", ";
				mOutput << ", ";
			}
		}
		cout << "]";
		mOutput << "]";
		return *this;
	}

	SL & SL::operator<<(ostream &(*pf)(ostream &)) {
		cout << pf;
		mOutput << pf;
		return *this;
	}

	SL & SL::operator<<(ios &(*pf)(ios &)) {
		cout << pf;
		mOutput << pf;
		return *this;
	}

	SL & SL::operator<<(ios_base &(*pf)(ios_base &)) {
		cout << pf;
		mOutput << pf;
		return *this;
	}

	//SL & SL::operator<<(void (*pf)(SL &)) {
	//	pf(*this);
	//	return *this;
	//}
	//
	//SL & SL::operator<<(void (SL::* pf)()) {
	//	(this->*(pf))();
	//	return *this;
	//}

	SL & SL::flush() {
		cout.flush();
		mOutput.flush();
		return *this;
	}

}