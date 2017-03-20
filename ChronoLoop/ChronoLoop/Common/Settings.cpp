#include "Settings.h"

namespace Epoch {

	Settings::_Settings* Settings::sInstance = nullptr;


	Settings::_Settings& Settings::GetInstance() {
		if (nullptr == sInstance) {
			sInstance = new _Settings;
		}
		return *sInstance;
	}

	void Settings::DestroyInstance() {
		if (nullptr != sInstance) {
			delete sInstance;
			sInstance = nullptr;
		}
	}

	bool Settings::_Settings::GetBool(std::string _key) {
		mLock.lock();
		auto val = mBools[_key];
		mLock.unlock();
		return val;
	}

	byte Settings::_Settings::GetByte(std::string _key) {
		mLock.lock();
		auto val = mBytes[_key];
		mLock.unlock();
		return val;
	}

	unsigned short Settings::_Settings::GetUShort(std::string _key) {
		mLock.lock();
		auto val = mUShorts[_key];
		mLock.unlock();
		return val;
	}

	short Settings::_Settings::GetShort(std::string _key) {
		mLock.lock();
		auto val = mShorts[_key];
		mLock.unlock();
		return val;
	}

	float Settings::_Settings::GetFloat(std::string _key) {
		mLock.lock();
		auto val = mFloats[_key];
		mLock.unlock();
		return val;
	}

	int Settings::_Settings::GetInt(std::string _key) {
		mLock.lock();
		auto val = mInts[_key];
		mLock.unlock();
		return val;
	}

	unsigned int Settings::_Settings::GetUInt(std::string _key) {
		mLock.lock();
		auto val = mUInts[_key];
		mLock.unlock();
		return val;
	}

	std::string Settings::_Settings::GetString(std::string _key) {
		mLock.lock();
		auto val = mStrings[_key];
		mLock.unlock();
		return val;
	}

	void Settings::_Settings::SetBool(std::string _key, bool _v) {
		mLock.lock();
		mBools[_key] = _v;
		mLock.unlock();
	}

	void Settings::_Settings::SetByte(std::string _key, byte _v) {
		mLock.lock();
		mBytes[_key] = _v;
		mLock.unlock();
	}

	void Settings::_Settings::SetUShort(std::string _key, unsigned short _v) {
		mLock.lock();
		mUShorts[_key] = _v;
		mLock.unlock();
	}

	void Settings::_Settings::SetShort(std::string _key, short _v) {
		mLock.lock();
		mShorts[_key] = _v;
		mLock.unlock();
	}

	void Settings::_Settings::SetFloat(std::string _key, float _v) {
		mLock.lock();
		mFloats[_key] = _v;
		mLock.unlock();
	}

	void Settings::_Settings::SetInt(std::string _key, int _v) {
		mLock.lock();
		mInts[_key] = _v;
		mLock.unlock();
	}

	void Settings::_Settings::SetUInt(std::string _key, unsigned int _v) {
		mLock.lock();
		mUInts[_key] = _v;
		mLock.unlock();
	}

	void Settings::_Settings::SetString(std::string _key, std::string _v) {
		mLock.lock();
		mStrings[_key] = _v;
		mLock.unlock();
	}

}