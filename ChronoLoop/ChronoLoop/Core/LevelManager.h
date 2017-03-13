#pragma once
#include "Level.h"

namespace Epoch {

	class LM {
		Level* mCurrentLevel = nullptr;


	public:
		enum class LevelStatus {
			Loading = 0,
			Success,
			Error
		};


		void SetCurrentLevel(Level* _level);
		Level* GetCurrentLevel();
		LevelStatus LoadLevelAsync(const char* _path, Level* _out);
	};



	class LevelManager {
		static LM* sInstance;
	
	public:
		static LM& GetInstance();
		static void DestroyInstance();
	};
}