#pragma once
#include "BaseLevel.h"
#include <vector>
class LevelManager
{
private:
	std::vector<BaseLevel*> mLevelList;
public:

	//The List of Levels (m_LevelList)
	std::vector<BaseLevel*> GetLevelList() { return mLevelList; };

	//Gets Level from List of Levels (m_LevelList)
	BaseLevel* GetLevel(int _index) { return mLevelList[_index]; };

	//Push_back a New Level into m_LevelList
	void AddLevel(BaseLevel* _level) { mLevelList.push_back(_level); };

	//Clears everything from m_LevelList
	void ClearLevels() { mLevelList.clear(); }
};