#pragma once
#include "Timeline.h"
#include <chrono>



class TimeManager
{
	static TimeManager* instanceTimemanager;

	static Timeline* mTimeline;
	float mRecordingTime =.1f;		// 1/10th of a second in milliseconds 
	float mLevelTime=0, mlastRecordedTime =0, mTimestamp = 0;
	

	TimeManager();
	~TimeManager();
public:
	
	void Update(float _delta);
	static TimeManager* Instance();
	static Timeline* GetTimeLine();
	static void Destroy();
};

