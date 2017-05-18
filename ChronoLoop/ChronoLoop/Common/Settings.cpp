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

	bool Settings::_Settings::GetBool(StringKey _key) {
		mLock.lock();
		auto it = mBools.find(_key);
		if (it == mBools.end()) {
			mLock.unlock();
			return false;
		} else {
			mLock.unlock();
			return it->second;
		}
	}

	unsigned char Settings::_Settings::GetByte(StringKey _key) {
		mLock.lock();
		auto it = mBytes.find(_key);
		if (it != mBytes.end()) {
			mLock.unlock();
			return it->second;
		} else {
			mLock.unlock();
			return unsigned char();
		}
	}

	unsigned short Settings::_Settings::GetUShort(StringKey _key) {
		mLock.lock();
		auto it = mUShorts.find(_key);
		if (it != mUShorts.end()) {
			mLock.unlock();
			return it->second;
		} else {
			mLock.unlock();
			return unsigned short();
		}
	}

	short Settings::_Settings::GetShort(StringKey _key) {
		mLock.lock();
		auto it = mShorts.find(_key);
		if (it != mShorts.end()) {
			mLock.unlock();
			return it->second;
		} else {
			mLock.unlock();
			return short();
		}
	}

	float Settings::_Settings::GetFloat(StringKey _key) {
		mLock.lock();
		auto it = mFloats.find(_key);
		if (it != mFloats.end()) {
			mLock.unlock();
			return it->second;
		} else {
			mLock.unlock();
			return float();
		}
	}

	int Settings::_Settings::GetInt(StringKey _key) {
		mLock.lock();
		auto it = mInts.find(_key);
		if (it != mInts.end()) {
			mLock.unlock();
			return it->second;
		} else {
			mLock.unlock();
			return int();
		}
	}

	unsigned int Settings::_Settings::GetUInt(StringKey _key) {
		mLock.lock();
		auto it = mUInts.find(_key);
		if (it != mUInts.end()) {
			mLock.unlock();
			return it->second;
		} else {
			mLock.unlock();
			return unsigned int();
		}
	}

	std::string Settings::_Settings::GetString(StringKey _key) {
		mLock.lock();
		auto it = mStrings.find(_key);
		if (it != mStrings.end()) {
			mLock.unlock();
			return it->second;
		} else {
			mLock.unlock();
			return std::string();
		}
	}

	void Settings::_Settings::SetBool(StringKey _key, bool _v) {
		mLock.lock();
		auto it = mBools.find(_key);
		if (it != mBools.end()) {
			it->second = _v;
		} else {
			mBools[_key] = _v;
		}
		mLock.unlock();
	}

	void Settings::_Settings::SetByte(StringKey _key, unsigned char _v) {
		mLock.lock();
		auto it = mBytes.find(_key);
		if (it != mBytes.end()) {
			it->second = _v;
		} else {
			mBytes[_key] = _v;
		}
		mLock.unlock();
	}

	void Settings::_Settings::SetUShort(StringKey _key, unsigned short _v) {
		mLock.lock();
		auto it = mUShorts.find(_key);
		if (it != mUShorts.end()) {
			it->second = _v;
		} else {
			mUShorts[_key] = _v;
		}
		mLock.unlock();
	}

	void Settings::_Settings::SetShort(StringKey _key, short _v) {
		mLock.lock();
		auto it = mShorts.find(_key);
		if (it != mShorts.end()) {
			it->second = _v;
		} else {
			mShorts[_key] = _v;
		}
		mLock.unlock();
	}

	void Settings::_Settings::SetFloat(StringKey _key, float _v) {
		mLock.lock();
		auto it = mFloats.find(_key);
		if (it != mFloats.end()) {
			it->second = _v;
		} else {
			mFloats[_key] = _v;
		}
		mLock.unlock();
	}

	void Settings::_Settings::SetInt(StringKey _key, int _v) {
		mLock.lock();
		auto it = mInts.find(_key);
		if (it != mInts.end()) {
			it->second = _v;
		} else {
			mInts[_key] = _v;
		}
		mLock.unlock();
	}

	void Settings::_Settings::SetUInt(StringKey _key, unsigned int _v) {
		mLock.lock();
		auto it = mUInts.find(_key);
		if (it != mUInts.end()) {
			it->second = _v;
		} else {
			mUInts[_key] = _v;
		}
		mLock.unlock();
	}

	void Settings::_Settings::SetString(StringKey _key, std::string _v) {
		mLock.lock();
		auto it = mStrings.find(_key);
		if (it != mStrings.end()) {
			it->second = _v;
		} else {
			mStrings[_key] = _v;
		}
		mLock.unlock();
	}

}