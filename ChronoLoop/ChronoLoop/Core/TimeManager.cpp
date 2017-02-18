//#include "stdafx.h"
#include "TimeManager.h"
#include "Timeline.h"
#include "../Input/KeyboardInput.h"

TimeManager* TimeManager::instanceTimemanager = nullptr;
Timeline* TimeManager::mTimeline = nullptr;

TimeManager::TimeManager()
{
	mTimeline = new Timeline();
}


TimeManager::~TimeManager()
{
	delete mTimeline;
}

void TimeManager::Update(float _delta)
{
	//Snap Update 
	mTimestamp += _delta;
	if (mTimestamp >= mRecordingTime)
	{
		mTimestamp = 0;
		//Generate 
		Snapshot* s = mTimeline->GenerateSnapShot(mLevelTime);
		mTimeline->AddSnapshot(mLevelTime,s);
		mLevelTime = mTimeline->GetCurrentGameTimeIndx() + 1;
	}

	if (mRewindTime == true)
	{
		mTimeline->RewindNoClone(mTimeline->GetCurrentGameTimeIndx() - 10);
		//Tell the time manager what frame the timeline its on
		mLevelTime = mTimeline->GetCurrentGameTimeIndx() + 1;
		mRewindTime = false;
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

void TimeManager::RewindTimeline()
{
	mRewindTime = true;
}

void TimeManager::Destroy()
{
	delete instanceTimemanager;
}
