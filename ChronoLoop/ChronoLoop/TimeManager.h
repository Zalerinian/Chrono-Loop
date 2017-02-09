#pragma once
#include "Timeline.h"
#include <chrono>



class TimeManager
{
	static TimeManager* instanceTimemanager;

	static Timeline* mTimeline;
	float mRecordingTime = 100;			// 1/10th of a second in milliseconds 
	float mLevelTime = 0;
	float mlastRecordedTime = mLevelTime;

	TimeManager();
	~TimeManager();
public:
	
	void Update(float _delta);
	static TimeManager* Instance();
	static Timeline* GetTimeLine();
	static void Destroy();
};

