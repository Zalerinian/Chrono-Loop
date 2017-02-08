#include "stdafx.h"
#include "TimeManager.h"


TimeManager::TimeManager()
{
}


TimeManager::~TimeManager()
{
}

void TimeManager::Update(float _delta)
{
	mLevelTime += _delta;

	if (mLevelTime - mlastRecordedTime > mRecordingTime)
	{
		mlastRecordedTime = mLevelTime + mRecordingTime;
		Snapshot s = mTimeline.GenerateSnapShot;
		mTimeline.AddSnapshot(s.mTime,s);
	}
}
