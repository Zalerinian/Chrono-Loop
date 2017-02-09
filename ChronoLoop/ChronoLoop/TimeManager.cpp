#include "stdafx.h"
#include "TimeManager.h"

TimeManager* TimeManager::instanceTimemanager = nullptr;
Timeline* TimeManager::mTimeline = nullptr;

TimeManager::TimeManager()
{
	mTimeline = new Timeline();
}


TimeManager::~TimeManager()
{
	int poop = 1;
}

void TimeManager::Update(float _delta)
{
	mLevelTime += _delta;
	mTimestamp += _delta;
	if (mTimestamp >= mRecordingTime)
	{
		mTimestamp = 0;
		Snapshot s = mTimeline->GenerateSnapShot(mLevelTime);
		mTimeline->AddSnapshot(s.mTime,s);
	}
}

TimeManager * TimeManager::Instance()
{
	if (!instanceTimemanager)
		instanceTimemanager = new TimeManager();
	
	return instanceTimemanager;
}

Timeline * TimeManager::GetTimeLine()
{
	if(!mTimeline)
	{
		mTimeline = new Timeline();
	};
	return mTimeline;
}

void TimeManager::Destroy()
{
	delete instanceTimemanager;
}
