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
		return mBools[_key];
	}

	byte Settings::_Settings::GetByte(std::string _key) {
		return mBytes[_key];
	}

	unsigned short Settings::_Settings::GetUShort(std::string _key) {
		return mUShorts[_key];
	}

	short Settings::_Settings::GetShort(std::string _key) {
		return mShorts[_key];
	}

	float Settings::_Settings::GetFloat(std::string _key) {
		return mFloats[_key];
	}

	int Settings::_Settings::GetInt(std::string _key) {
		return mInts[_key];
	}

	unsigned int Settings::_Settings::GetUInt(std::string _key) {
		return mUInts[_key];
	}

	std::string Settings::_Settings::GetString(std::string _key) {
		return mStrings[_key];
	}

	void Settings::_Settings::SetBool(std::string _key, bool _v) {
		mBools[_key] = _v;
	}

	void Settings::_Settings::SetByte(std::string _key, byte _v) {
		mBytes[_key] = _v;
	}

	void Settings::_Settings::SetUShort(std::string _key, unsigned short _v) {
		mUShorts[_key] = _v;
	}

	void Settings::_Settings::SetShort(std::string _key, short _v) {
		mShorts[_key] = _v;
	}

	void Settings::_Settings::SetFloat(std::string _key, float _v) {
		mFloats[_key] = _v;
	}

	void Settings::_Settings::SetInt(std::string _key, int _v) {
		mInts[_key] = _v;
	}

	void Settings::_Settings::SetUInt(std::string _key, unsigned int _v) {
		mUInts[_key] = _v;
	}

	void Settings::_Settings::SetString(std::string _key, std::string _v) {
		mStrings[_key] = _v;
	}

}