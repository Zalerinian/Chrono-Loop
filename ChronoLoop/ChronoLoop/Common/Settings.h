#pragma once
#include <unordered_map>

namespace Epoch {

	typedef unsigned char byte;
	 
	class Settings {
		class _Settings {
			std::unordered_map<std::string, bool>           mBools;
			std::unordered_map<std::string, unsigned char>  mBytes;
			std::unordered_map<std::string, unsigned short> mUShorts;
			std::unordered_map<std::string, short>          mShorts;
			std::unordered_map<std::string, float>          mFloats;
			std::unordered_map<std::string, int>            mInts;
			std::unordered_map<std::string, unsigned int>   mUInts;
			std::unordered_map<std::string, std::string>    mStrings;

		public:
			bool GetBool(std::string _key);
			byte GetByte(std::string _key);
			unsigned short GetUShort(std::string _key);
			short GetShort(std::string _key);
			float GetFloat(std::string _key);
			int GetInt(std::string _key);
			unsigned int GetUInt(std::string _key);
			std::string GetString(std::string _key);

			void SetBool(std::string _key, bool _v);
			void SetByte(std::string _key, byte _v);
			void SetUShort(std::string _key, unsigned short _v);
			void SetShort(std::string _key, short _v);
			void SetFloat(std::string _key, float _v);
			void SetInt(std::string _key, int _v);
			void SetUInt(std::string _key, unsigned int _v);
			void SetString(std::string _key, std::string _v);
		};

		static _Settings* sInstance;

	public:
		static _Settings& GetInstance();
		static void DestroyInstance();
	};


}