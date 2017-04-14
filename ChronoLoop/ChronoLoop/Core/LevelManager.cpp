#include "LevelManager.h"
#include <thread>
#include "../Common/Settings.h"

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
			Renderer::Instance()->ClearRenderSet();
			mCurrentLevel->SetupObjects();
			for (auto it = mCurrentLevel->GetLevelObjects().begin(); it != mCurrentLevel->GetLevelObjects().end(); ++it)
			{
				//TODO PAT: GET RID OF THIS WHEN WE START LOADING ALL BINARY
				//TODO PAT FIX THIS THIS ISNT CORRECT
				if(((*it)->Flags & BaseObject_Flag_Record_In_Timeline) != 0 || ((*it)->GetName().find("Wire") != std::string::npos))
				{
					TimeManager::Instance()->AddObjectToTimeline(*it);
				}
				if ((*it)->mComponents[eCOMPONENT_COLLIDER].size() > 0)
				{
					Physics::Instance()->mObjects.push_back((*it));
					if ((((Collider*)(*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mShouldMove && ((*it)->Flags & BaseObject_Flag_Record_In_Timeline) == 0) ||
						// Bootleg loading for the XML files
						// TODO: remove this
						(((*it)->GetName() == "TransparentDoor1" || (*it)->GetName() == "TransparentDoor2") && ((*it)->Flags & BaseObject_Flag_Record_In_Timeline) == 0))
					{
						TimeManager::Instance()->AddObjectToTimeline(*it);
					}
				}
				
			}
			mCurrentLevel->CallStart();
			mRequested = nullptr;
		}
		mCurrentLevel->Update();
	}

} // Epoch Namespace