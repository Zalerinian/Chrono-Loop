#pragma once
#include <unordered_map>
#include <mutex>


namespace Epoch {
	struct SettingsKey {
		const char* path;
		SettingsKey(const char* _path) : path(_path) {}
		SettingsKey(std::string _path) : path(_path.c_str()) {}
	};
}

namespace std {
	template<> struct hash<Epoch::SettingsKey> {
		size_t operator()(const Epoch::SettingsKey& _key) const {
			size_t hash = 14695981039346656037;
			for (unsigned int i = 0; i < strlen(_key.path); ++i) {
				hash *= 1099511628211;
				hash ^= _key.path[i];
			}
			return hash;
		}
	};

	template<> struct equal_to<Epoch::SettingsKey> {
		bool operator()(const Epoch::SettingsKey& _l, const Epoch::SettingsKey& _r) const {
			return _l.path == _r.path;
		}
	};
}

namespace Epoch {

	typedef unsigned char byte;
	
	class Settings {
	

		class _Settings {
			std::unordered_map<SettingsKey, bool>           mBools;
			std::unordered_map<SettingsKey, unsigned char>  mBytes;
			std::unordered_map<SettingsKey, unsigned short> mUShorts;
			std::unordered_map<SettingsKey, short>          mShorts;
			std::unordered_map<SettingsKey, float>          mFloats;
			std::unordered_map<SettingsKey, int>            mInts;
			std::unordered_map<SettingsKey, unsigned int>   mUInts;
			std::unordered_map<SettingsKey, std::string>    mStrings;
			std::mutex                                      mLock;

		public:
			bool GetBool(SettingsKey _key);
			byte GetByte(SettingsKey _key);
			unsigned short GetUShort(SettingsKey _key);
			short GetShort(SettingsKey _key);
			float GetFloat(SettingsKey _key);
			int GetInt(SettingsKey _key);
			unsigned int GetUInt(SettingsKey _key);
			std::string GetString(SettingsKey _key);

			void SetBool(SettingsKey _key, bool _v);
			void SetByte(SettingsKey _key, byte _v);
			void SetUShort(SettingsKey _key, unsigned short _v);
			void SetShort(SettingsKey _key, short _v);
			void SetFloat(SettingsKey _key, float _v);
			void SetInt(SettingsKey _key, int _v);
			void SetUInt(SettingsKey _key, unsigned int _v);
			void SetString(SettingsKey _key, std::string _v);
		};

		static _Settings* sInstance;

	public:
		static _Settings& GetInstance();
		static void DestroyInstance();
	};


}

