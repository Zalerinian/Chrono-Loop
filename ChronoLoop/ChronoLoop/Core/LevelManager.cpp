#include "LevelManager.h"
#include <thread>

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

	void LM::ThreadLoadLevel(std::string _path)
	{
		mLoadingLevel = new Level;

		// Actually load the level here
		mLoadingLevel->LoadLevel(_path);


		// Assuming everything went dandy
		mMutex.lock();
		mLevelStatus = LoadStatus::Complete;
		mMutex.unlock();
	}

	//void LM::SetCurrentLevel(Level* _level) {
	//	mCurrentLevel = _level;
	//}

	void LM::RequestLevelChange(Level * _next)
	{
		if (!mCurrentLevel) {
			mCurrentLevel = _next;
			VRInputManager::GetInstance().GetPlayerPosition().Position = _next->mStartPosition;
			return;
		}
		mRequested = _next;
	}

	Level* LM::GetCurrentLevel() {
		return mCurrentLevel;
	}

	LM::LevelStatus LM::LoadLevelAsync(const char * _path, Level** _out) {
		mMutex.lock();
		if (mLevelStatus == LoadStatus::Loading)
		{
			mMutex.unlock();
			return LevelStatus::Loading;
		}
		else if (mLevelStatus == LoadStatus::None)
		{
			mLevelStatus = LoadStatus::Loading;
			std::thread(&LM::ThreadLoadLevel, this, std::string(_path)).detach();
			mMutex.unlock();
			return LevelStatus::Loading;
		}
		else if (mLevelStatus == LoadStatus::Complete)
		{
			mLevelStatus = LoadStatus::None;
			mMutex.unlock();
			(*_out) = mLoadingLevel;
			mLoadingLevel = nullptr;
			return LevelStatus::Success;
		}
		else
		{
			// Error loading
			SystemLogger::Error() << "Ya dun goofed." << std::endl;
			mMutex.unlock();
			return LevelStatus::Error;
		}

	}

	void LM::Destroy()
	{
		if (mCurrentLevel)
			delete mCurrentLevel;
	}

	void LM::Update()
	{
		if (mRequested) {
			if (mCurrentLevel) {
				delete mCurrentLevel;
			}
			VRInputManager::GetInstance().GetPlayerPosition().Position = mRequested->mStartPosition;
			mCurrentLevel = mRequested;
			mCurrentLevel->CallStart();
			mRequested = nullptr;
		}
		mCurrentLevel->Update();
	}

} // Epoch Namespace