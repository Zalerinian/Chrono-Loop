//#include "stdafx.h"
#include "TimeManager.h"
#include "Timeline.h"
#include "../Input/KeyboardInput.h"
#include "../Objects/BaseObject.h"

TimeManager* TimeManager::instanceTimemanager = nullptr;
Timeline* TimeManager::mTimeline = nullptr;
short TimeManager::CloneCreationCount = -1;

TimeManager::TimeManager()
{
	mTimeline = new Timeline();
	mPlayer = new BaseObject();

	//This id is for the player and for the player only
	mPlayer->SetUniqueID(0);

	mTimeline->AddBaseObject(mPlayer,mPlayer->GetUniqueID());
}


TimeManager::~TimeManager()
{
	Destroy();
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

	//if (mRewindTime)
	//{
	//	mTimeline->RewindNoClone(mTimeline->GetCurrentGameTimeIndx() - 10);
	//	//Tell the time manager what frame the timeline its on
	//	mLevelTime = mTimeline->GetCurrentGameTimeIndx() + 1;
	//	mRewindTime = false;
	//}
	//else if(mRewindMakeClone)
	//{
	//	mRewindMakeClone = false;
	//}
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

void TimeManager::RewindTimeline(unsigned int _frame, unsigned short _id1, unsigned short _id2, unsigned short _id3)
{
	//mRewindTime = true;
	//mTimeline->RewindNoClone(mTimeline->GetCurrentGameTimeIndx() - 10);
	mTimeline->RewindNoClone(_frame);
	//Tell the time manager what frame the timeline its on
	mLevelTime = mTimeline->GetCurrentGameTimeIndx() + 1;
}

void TimeManager::RewindMakeClone(unsigned int _frame, BaseObject* _ob1, BaseObject* _ob2, BaseObject* _ob3)
{
	//mRewindMakeClone = true;
}

void TimeManager::Destroy()
{
	//Level manager will clear delete clones
	instanceTimemanager->mClones.clear();
	delete mTimeline;
	delete instanceTimemanager;
}
