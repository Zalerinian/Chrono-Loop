#pragma once
#include <sstream>
#include <fstream>

namespace Epoch
{

	struct vec4i;
	struct vec4f;
	struct vec3f;
	struct vec2f;
	struct matrix4;

	class SL
	{
		std::ofstream mOutput;
		friend class SystemLogger;
		SL(const char* _path);
		~SL();

	public:
		SL& operator<<(const char i);
		SL& operator<<(const short i);
		SL& operator<<(const int i);
		SL& operator<<(const float i);
		SL& operator<<(const double i);
		SL& operator<<(const long i);
		SL& operator<<(const long long i);
		SL& operator<<(const unsigned char i);
		SL& operator<<(const unsigned short i);
		SL& operator<<(const unsigned int i);
		SL& operator<<(const unsigned long i);
		SL& operator<<(const unsigned long long i);
		SL& operator<<(const char *i);
		SL& operator<<(const std::string &i);
		SL& operator<<(const vec4i& i);
		SL& operator<<(const vec4f& i);
		SL& operator<<(const vec3f& i);
		SL& operator<<(const vec2f& i);
		SL& operator<<(const matrix4& i);
		SL& operator<<(std::ostream& (*pf)(std::ostream&)); // Stream manipulator
		SL& operator<<(std::ios& (*pf)(std::ios&));
		SL& operator<<(std::ios_base& (*pf)(std::ios_base&));
		SL& flush();
	};

	class SystemLogger
	{
		static SL *Log;
		SystemLogger();
		std::ofstream output;
		void PrintTime();

	public:
		inline static SL& GetLog()
		{
			if (nullptr == Log)
			{
				Log = new SL("Log.log");
			}
			return *Log;
		}

		inline static SL& GetError()
		{
			return GetLog();
		}

		inline static SL& Fatal()
		{
			SL& log = GetLog();
			log << "[FATAL] ";
			return log;
		}

		inline static SL& Error()
		{
			SL& log = GetLog();
			log << "[ERROR] ";
			return log;
		}

		inline static SL& Warn()
		{
			SL& log = GetLog();
			log << "[WARNING] ";
			return log;
		}

		inline static SL& Info()
		{
			SL& log = GetLog();
			log << "[INFO] ";
			return log;
		}

		inline static SL& Debug()
		{
			SL& log = GetLog();
			log << "[DEBUG] ";
			return log;
		}

		inline static void DestroyInstance()
		{
			if (nullptr != Log)
			{
				delete Log;
				Log = nullptr;
			}
		}
	};


}


