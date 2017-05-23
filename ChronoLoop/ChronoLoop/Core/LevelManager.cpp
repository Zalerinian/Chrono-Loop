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
		mLoadingLevel->BinaryLoadLevel(_path);


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
			VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateYawPitchRollRotation(_next->mStartRotation);
			VRInputManager::GetInstance().GetPlayerPosition() *= matrix4::CreateTranslation(_next->mStartPosition);
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
			TimeManager::Instance()->Destroy();
			VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateYawPitchRollRotation(mRequested->mStartRotation);
			VRInputManager::GetInstance().GetPlayerPosition() *= matrix4::CreateTranslation(mRequested->mStartPosition);
			mCurrentLevel = mRequested;
			Renderer::Instance()->ClearRenderSet();
			mCurrentLevel->SetupObjects();
			for (auto it = mCurrentLevel->GetLevelObjects().begin(); it != mCurrentLevel->GetLevelObjects().end(); ++it)
			{
				if(((*it)->Flags & BaseObject_Flag_Record_In_Timeline) != 0 )
				{
					TimeManager::Instance()->AddObjectToTimeline(*it);
				}
				if ((*it)->mComponents[eCOMPONENT_COLLIDER].size() > 0)
				{
					Physics::Instance()->mObjects.push_back((*it));
					if ((((Collider*)(*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mShouldMove && ((*it)->Flags & BaseObject_Flag_Record_In_Timeline) == 0))
					{
						//TODO: GIT RID OF THIS
						if(!((*it)->GetName() == "L3Button"))
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