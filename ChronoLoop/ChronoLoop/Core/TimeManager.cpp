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
	//Level manager will clear delete clones
	delete mTimeline;
	instanceTimemanager->mClones.clear();
}

void TimeManager::Update(float _delta) {
	mTimestamp += _delta;
	mDeltaTime = _delta;
	if (mTimestamp >= mRecordingTime) {
		mTimestamp = 0;
		//Generate 
		Snapshot* s = mTimeline->GenerateSnapShot(mLevelTime, mClones);
		mTimeline->AddSnapshot(mLevelTime, s);
		mLevelTime = mTimeline->GetCurrentGameTimeIndx() + 1;
	}

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
	//wrapped
	if (mTimeline->mCurrentGameTimeIndx - _frame > mTimeline->mCurrentGameTimeIndx)
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
	mTimeline->RewindNoClone(_frame, _id1, _id2, _id3);
	//Tell the time manager what frame the timeline its on
	mLevelTime = mTimeline->GetCurrentGameTimeIndx() + 1;
}

void TimeManager::RewindMakeClone(unsigned int _frame, BaseObject* _ob1, BaseObject* _ob2, BaseObject* _ob3) {
	if (_ob1 == nullptr || _ob2 == nullptr || _ob3 == nullptr)
		SystemLogger::GetLog() << "When you tried to rewind time, you gave the timemanager bad BaseObject pointer(s)";
	mTimeline->RewindMakeClone(_frame);
	mClones.push_back(_ob1);
	mClones.push_back(_ob2);
	mClones.push_back(_ob3);
	//Tell the time manager what frame the timeline its on
	mLevelTime = mTimeline->GetCurrentGameTimeIndx() + 1;
}

void TimeManager::Destroy() {
	
	delete instanceTimemanager;
	
}
