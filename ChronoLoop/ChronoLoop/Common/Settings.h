#pragma once
#include <unordered_map>
#include <mutex>


namespace Epoch {
	struct StringKey {
		const char* path;
		StringKey(const char* _path) : path(_path) {}
		StringKey(std::string _path) : path(_path.c_str()) {}
	};
}

namespace std {
	template<> struct hash<Epoch::StringKey> {
		size_t operator()(const Epoch::StringKey& _key) const {
			size_t hash = 14695981039346656037;
			for (unsigned int i = 0; i < strlen(_key.path); ++i) {
				hash *= 1099511628211;
				hash ^= _key.path[i];
			}
			return hash;
		}
	};

	template<> struct equal_to<Epoch::StringKey> {
		bool operator()(const Epoch::StringKey& _l, const Epoch::StringKey& _r) const {
			return _l.path == _r.path;
		}
	};
}

namespace Epoch {
	class Settings {
	

		class _Settings {
			std::unordered_map<StringKey, bool>           mBools;
			std::unordered_map<StringKey, unsigned char>  mBytes;
			std::unordered_map<StringKey, unsigned short> mUShorts;
			std::unordered_map<StringKey, short>          mShorts;
			std::unordered_map<StringKey, float>          mFloats;
			std::unordered_map<StringKey, int>            mInts;
			std::unordered_map<StringKey, unsigned int>   mUInts;
			std::unordered_map<StringKey, std::string>    mStrings;
			std::mutex                                      mLock;

		public:
			bool GetBool(StringKey _key);
			unsigned char GetByte(StringKey _key);
			unsigned short GetUShort(StringKey _key);
			short GetShort(StringKey _key);
			float GetFloat(StringKey _key);
			int GetInt(StringKey _key);
			unsigned int GetUInt(StringKey _key);
			std::string GetString(StringKey _key);

			void SetBool(StringKey _key, bool _v);
			void SetByte(StringKey _key, unsigned char _v);
			void SetUShort(StringKey _key, unsigned short _v);
			void SetShort(StringKey _key, short _v);
			void SetFloat(StringKey _key, float _v);
			void SetInt(StringKey _key, int _v);
			void SetUInt(StringKey _key, unsigned int _v);
			void SetString(StringKey _key, std::string _v);
		};

		static _Settings* sInstance;

	public:
		static _Settings& GetInstance();
		static void DestroyInstance();
	};


}

