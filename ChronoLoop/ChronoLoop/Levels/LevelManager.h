#pragma once
#include "BaseLevel.h"
#include <vector>
class LevelManager
{
private:
	std::vector<BaseLevel*> m_LevelList;
public:

	//The List of Levels (m_LevelList)
	std::vector<BaseLevel*> GetLevelList() { return m_LevelList; };

	//Gets Level from List of Levels (m_LevelList)
	BaseLevel* GetLevel(int _index) { return m_LevelList[_index]; };

	//Push_back a New Level into m_LevelList
	void AddLevel(BaseLevel* _level) { m_LevelList.push_back(_level); };

	//Clears everything from m_LevelList
	void ClearLevels() { m_LevelList.clear(); }
};