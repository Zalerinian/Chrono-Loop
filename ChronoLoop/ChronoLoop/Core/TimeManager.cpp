//#include "stdafx.h"
#include "TimeManager.h"
#include "Timeline.h"
#include "../Input/KeyboardInput.h"
#include "../Objects/BaseObject.h"
#include "../Common/Logger.h"

TimeManager* TimeManager::instanceTimemanager = nullptr;
Timeline* TimeManager::mTimeline = nullptr;

TimeManager::TimeManager() {
	mTimeline = new Timeline();
}


TimeManager::~TimeManager() {
	Destroy();
}

void TimeManager::Update(float _delta) {
	//Snap Update 
	mTimestamp += _delta;
	if (mTimestamp >= mRecordingTime) {
		mTimestamp = 0;
		//Generate 
		Snapshot* s = mTimeline->GenerateSnapShot(mLevelTime, mClones);
		mTimeline->AddSnapshot(mLevelTime, s);
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

TimeManager * TimeManager::Instance() {
	if (!instanceTimemanager)
		instanceTimemanager = new TimeManager();

	return instanceTimemanager;
}

void TimeManager::AddObjectToTimeline(BaseObject * _obj) {
	if (_obj != nullptr)
		mTimeline->AddBaseObject(_obj, _obj->GetUniqueId());
}

void TimeManager::ClearClones() {
	mClones.clear();
}

bool TimeManager::CheckRewindAvaliable(unsigned int _frame)
{
	if (mTimeline->mCurrentGameTimeIndx - _frame < 0)
		return false;
	else
		return true;
}

unsigned int TimeManager::GetCurrentSnapFrame() {
	return mTimeline->mCurrentGameTimeIndx;
}

Timeline * TimeManager::GetTimeLine() {
	if (!mTimeline) {
		mTimeline = new Timeline();
	};
	return mTimeline;
}

void TimeManager::RewindTimeline(unsigned int _frame, unsigned short _id1, unsigned short _id2, unsigned short _id3) {
	//mRewindTime = true;
	//mTimeline->RewindNoClone(mTimeline->GetCurrentGameTimeIndx() - 10);
	mTimeline->RewindNoClone(_frame, _id1, _id2, _id3);
	//Tell the time manager what frame the timeline its on
	mLevelTime = mTimeline->GetCurrentGameTimeIndx() + 1;
}

void TimeManager::RewindMakeClone(unsigned int _frame, BaseObject* _ob1, BaseObject* _ob2, BaseObject* _ob3) {
	//mRewindMakeClone = true;
	if (_ob1 == nullptr || _ob2 == nullptr || _ob3 == nullptr)
		SystemLogger::GetLog() << "When you tried to rewind time, you gave the timemanager bad BaseObject pointer(s)";
	mTimeline->RewindMakeClone(_frame);
	/*mTimeline->AddBaseObject(_ob1,_ob1->GetUniqueID());
	mTimeline->AddBaseObject(_ob2, _ob3->GetUniqueID());
	mTimeline->AddBaseObject(_ob3, _ob2->GetUniqueID());*/
	mClones.push_back(_ob1);
	mClones.push_back(_ob2);
	mClones.push_back(_ob3);
	//Tell the time manager what frame the timeline its on
	mLevelTime = mTimeline->GetCurrentGameTimeIndx() + 1;
}

void TimeManager::Destroy() {
	//Level manager will clear delete clones
	instanceTimemanager->mClones.clear();
	delete mTimeline;
	
}
