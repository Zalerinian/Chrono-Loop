#pragma once
#include <chrono>

class Timeline;
class TimeManager
{
	static TimeManager* instanceTimemanager;

	static Timeline* mTimeline;
	float mRecordingTime =.1f;		// 1/10th of a second in milliseconds 
	unsigned int mLevelTime = 0;
	float mlastRecordedTime =0, mTimestamp = 0;
	
	//GIT RID OF THIS 
	bool mRewindTime = false;

	TimeManager();
	~TimeManager();
public:
	
	void Update(float _delta);
	static TimeManager* Instance();
	static Timeline* GetTimeLine();
	void RewindTimeline();
	static void Destroy();
};

