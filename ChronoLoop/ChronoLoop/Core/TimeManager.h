#pragma once
#include <chrono>
#include <vector>

class BaseObject;

class Timeline;
class TimeManager
{
	static TimeManager* instanceTimemanager;
	static Timeline* mTimeline;

	float mRecordingTime =.1f;		// 1/10th of a second in milliseconds 
	float mlastRecordedTime =0, mTimestamp = 0;
	unsigned int mLevelTime = 0;
	bool mRewindTime = false, mRewindMakeClone = false;
	std::vector<BaseObject*>mClones;
	Timeline* GetTimeLine();
	

	TimeManager();
	~TimeManager();
public:
	
	void Update(float _delta);
	static TimeManager* Instance();
	void AddObjectToTimeline(BaseObject* _obj);
	//Clears the list of BaseObject* the Timemanager has refrence to.
	void ClearClones();
	//Go back into time. Send in dest frame and send in player headset and conrollers id
	void RewindTimeline(unsigned int _frame, unsigned short _id1, unsigned short _id2, unsigned short _id3);
	//Go back into time and make clone. Send in dest frame and send in player headset and conrollers baseObjects
	void RewindMakeClone(unsigned int _frame, BaseObject* _ob1, BaseObject* _ob2, BaseObject* _ob3);
	
	static void Destroy();
};

