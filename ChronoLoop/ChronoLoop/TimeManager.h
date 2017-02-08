#pragma once
#include "Timeline.h"
#include <chrono>



class TimeManager
{
	Timeline mTimeline;
	float mRecordingTime = 100;			// 1/10th of a second in milliseconds 
	float mLevelTime = 0;
	float mlastRecordedTime = mLevelTime;
public:
	TimeManager();
	~TimeManager();
	void Update(float _delta);
};

