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

	bool Settings::_Settings::GetBool(SettingsKey _key) {
		mLock.lock();
		auto val = mBools[_key];
		mLock.unlock();
		return val;
	}

	byte Settings::_Settings::GetByte(SettingsKey _key) {
		mLock.lock();
		auto val = mBytes[_key];
		mLock.unlock();
		return val;
	}

	unsigned short Settings::_Settings::GetUShort(SettingsKey _key) {
		mLock.lock();
		auto val = mUShorts[_key];
		mLock.unlock();
		return val;
	}

	short Settings::_Settings::GetShort(SettingsKey _key) {
		mLock.lock();
		auto val = mShorts[_key];
		mLock.unlock();
		return val;
	}

	float Settings::_Settings::GetFloat(SettingsKey _key) {
		mLock.lock();
		auto val = mFloats[_key];
		mLock.unlock();
		return val;
	}

	int Settings::_Settings::GetInt(SettingsKey _key) {
		mLock.lock();
		auto val = mInts[_key];
		mLock.unlock();
		return val;
	}

	unsigned int Settings::_Settings::GetUInt(SettingsKey _key) {
		mLock.lock();
		auto val = mUInts[_key];
		mLock.unlock();
		return val;
	}

	std::string Settings::_Settings::GetString(SettingsKey _key) {
		mLock.lock();
		auto val = mStrings[_key];
		mLock.unlock();
		return val;
	}

	void Settings::_Settings::SetBool(SettingsKey _key, bool _v) {
		mLock.lock();
		mBools[_key] = _v;
		mLock.unlock();
	}

	void Settings::_Settings::SetByte(SettingsKey _key, byte _v) {
		mLock.lock();
		mBytes[_key] = _v;
		mLock.unlock();
	}

	void Settings::_Settings::SetUShort(SettingsKey _key, unsigned short _v) {
		mLock.lock();
		mUShorts[_key] = _v;
		mLock.unlock();
	}

	void Settings::_Settings::SetShort(SettingsKey _key, short _v) {
		mLock.lock();
		mShorts[_key] = _v;
		mLock.unlock();
	}

	void Settings::_Settings::SetFloat(SettingsKey _key, float _v) {
		mLock.lock();
		mFloats[_key] = _v;
		mLock.unlock();
	}

	void Settings::_Settings::SetInt(SettingsKey _key, int _v) {
		mLock.lock();
		mInts[_key] = _v;
		mLock.unlock();
	}

	void Settings::_Settings::SetUInt(SettingsKey _key, unsigned int _v) {
		mLock.lock();
		mUInts[_key] = _v;
		mLock.unlock();
	}

	void Settings::_Settings::SetString(SettingsKey _key, std::string _v) {
		mLock.lock();
		mStrings[_key] = _v;
		mLock.unlock();
	}

}