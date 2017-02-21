#include "stdafx.h"
#include "LevelManager.h"

LevelManager* LevelManager::mLevelmanager = nullptr;

LevelManager::LevelManager()
{
	mLevelmanager = nullptr;
}

LevelManager::~LevelManager()
{
	Destroy();
}

void LevelManager::ClearLevels()
{
	for(unsigned int i = 0; i< mLevelmanager->mLevelList.size(); i++)
	{
		delete  mLevelmanager->mLevelList[i];
	}
	mLevelmanager->mLevelList.clear();
}

LevelManager * LevelManager::Instance() {
	if (mLevelmanager == nullptr)
		mLevelmanager = new LevelManager();

	return mLevelmanager;
}

void LevelManager::Destroy()
{
	ClearLevels();
	delete mLevelmanager;
}
