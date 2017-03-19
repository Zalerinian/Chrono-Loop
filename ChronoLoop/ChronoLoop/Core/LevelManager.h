#pragma once
#include "Level.h"
#include <mutex>

namespace Epoch {

	class LM {
		Level* mCurrentLevel = nullptr, *mLoadingLevel = nullptr, *mRequested = nullptr;

		enum class LoadStatus
		{
			None = 0,
			Loading,
			Complete,
			Error
		};

		LoadStatus mLevelStatus = LoadStatus::None;
		std::mutex mMutex;

		void ThreadLoadLevel(std::string _path);

	public:
		enum class LevelStatus {
			Loading = 0,
			Success,
			Error
		};


		//void SetCurrentLevel(Level* _level);
		void RequestLevelChange(Level* _next);
		Level* GetCurrentLevel();
		LevelStatus LoadLevelAsync(const char* _path, Level** _out);
		void Destroy();
		void Update();
	};



	class LevelManager {
		static LM* sInstance;
	
	public:
		static LM& GetInstance();
		static void DestroyInstance();
	};
}