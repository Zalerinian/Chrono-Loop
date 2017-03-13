#include "LevelManager.h"

namespace Epoch {

	LM* LevelManager::sInstance = nullptr;

	LM& LevelManager::GetInstance() {
		if (nullptr == sInstance) {
			sInstance = new LM;
		}
		return *sInstance;
	}

	void LevelManager::DestroyInstance() {
		if (sInstance) {
			delete sInstance;
			sInstance = nullptr;
		}
	}

	void LM::SetCurrentLevel(Level* _level) {
		mCurrentLevel = _level;
	}

	Level* LM::GetCurrentLevel() {
		return mCurrentLevel;
	}

	LM::LevelStatus LM::LoadLevelAsync(const char * _path, Level* _out) {
		return LevelStatus::Error;
	}

} // Epoch Namespace