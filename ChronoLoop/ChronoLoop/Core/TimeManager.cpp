//#include "stdafx.h"
#include "TimeManager.h"
#include "Timeline.h"
#include "../Objects/BaseObject.h"
#include "../Common/Logger.h"
#include "../Rendering/Draw2D.h"
#include "../Input/CommandConsole.h"

TimeManager* TimeManager::instanceTimemanager = nullptr;
Timeline* TimeManager::mTimeline = nullptr;

TimeManager::TimeManager() {
	mTimeline = new Timeline();
	mCloneCountOn = false;
	mSnapshotCountOn = false;
	CommandConsole::Instance().AddCommand(L"/CLONECOUNT", ToggleCloneCountDisplay);
	CommandConsole::Instance().AddCommand(L"/SNAPCOUNT", ToggleSnapshotCountDisplay);

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
		mTimeline->AddBaseObject(_obj, _obj->GetUniqueID());
}

void TimeManager::AddPlayerObjectToTimeline(BaseObject * _obj) {
	if (_obj != nullptr)
		mTimeline->AddPlayerBaseObject(_obj, _obj->GetUniqueID());
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
	mTimeline->SetCloneCreationTime(_ob1->GetUniqueID(), _ob2->GetUniqueID(), _ob3->GetUniqueID());
	//Tell the time manager what frame the timeline its on
	mLevelTime = mTimeline->GetCurrentGameTimeIndx() + 1;
}

void TimeManager::Destroy() {
	delete instanceTimemanager;
}

void TimeManager::ToggleCloneCountDisplay(void * _command, std::wstring _ifOn)
{
	CommandConsole* cc = (CommandConsole*)_command;
	if (_ifOn == L"ON") {
		instanceTimemanager->mCloneCountOn = true;
		CommandConsole::Instance().DisplaySet(L"");
	}
	else if (_ifOn == L"OFF") {
		instanceTimemanager->mCloneCountOn = false;
		CommandConsole::Instance().DisplaySet(L"");

	}
	else {
		CommandConsole::Instance().DisplaySet(L"INVALID INPUT: " + _ifOn + L"\nCORRECT INPUT: /CLONECOUNT (ON/OFF)");
	}
}
void TimeManager::ToggleSnapshotCountDisplay(void * _command, std::wstring _ifOn)
{
	CommandConsole* cc = (CommandConsole*)_command;
	if (_ifOn == L"ON") {
		instanceTimemanager->mSnapshotCountOn = true;
		CommandConsole::Instance().DisplaySet(L"");
	}
	else if (_ifOn == L"OFF") {
		instanceTimemanager->mSnapshotCountOn = false;
		CommandConsole::Instance().DisplaySet(L"");

	}
	else {
		CommandConsole::Instance().DisplaySet(L"INVALID INPUT: " + _ifOn + L"\nCORRECT INPUT: /SNAPCOUNT (ON/OFF)");
	}
}
void TimeManager::DisplayCloneCount()
{
	if (instanceTimemanager->mCloneCountOn) {
		std::wstring CloneCount = L"Clone(s): " + std::to_wstring(mClones.size());

		Font* tempFont;
		if (!CommandConsole::Instance().isVRon()) {
			tempFont = new Font(L"Times New Roman" ,25, (D2D1::ColorF(D2D1::ColorF::Purple, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		}
		else {
			tempFont = new Font(L"Calibri", 40, (D2D1::ColorF(D2D1::ColorF::Purple, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		}
		Draw::Instance().DrawTextToBitmap(
			(*Draw::Instance().GetContext2D())->GetSize().width*(25.0f / 32.0f),
			(*Draw::Instance().GetContext2D())->GetSize().height*(29.0f / 32.0f),
			(*Draw::Instance().GetContext2D())->GetSize().width,
			(*Draw::Instance().GetContext2D())->GetSize().height*(30.5f / 32.0f), *tempFont,
			CloneCount, *(Draw::Instance().GetScreenBitmap()).get());
	}
}
void TimeManager::DisplaySnapshotCount()
{
	if (instanceTimemanager->mSnapshotCountOn) {
		std::wstring CloneCount = L"Snapshots: " + std::to_wstring(mTimeline->mCurrentGameTimeIndx);

		Font* tempFont;
		if (!CommandConsole::Instance().isVRon()) {
			tempFont = new Font(L"Times New Roman", 25, (D2D1::ColorF(D2D1::ColorF::Blue, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		}
		else {
			tempFont = new Font(L"Calibri", 40, (D2D1::ColorF(D2D1::ColorF::Blue, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		}
		Draw::Instance().DrawTextToBitmap(
			(*Draw::Instance().GetContext2D())->GetSize().width*(25.0f / 32.0f),
			(*Draw::Instance().GetContext2D())->GetSize().height*(26.0f / 32.0f),
			(*Draw::Instance().GetContext2D())->GetSize().width,
			(*Draw::Instance().GetContext2D())->GetSize().height*(29.0f / 32.0f), *tempFont,
			CloneCount, *(Draw::Instance().GetScreenBitmap()).get());


	}
}
