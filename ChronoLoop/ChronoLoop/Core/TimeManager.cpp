//#include "stdafx.h"
#include "TimeManager.h"
#include "Timeline.h"
#include "../Objects/BaseObject.h"
#include "../Common/Logger.h"
#include "../Rendering/Draw2D.h"
#include "../Input/CommandConsole.h"
#include "../Core/Pool.h"
#include "../Core/Level.h"
#include "../Input/VRInputManager.h"
#include "../Common/Breakpoint.h"

namespace Epoch {

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
		ClearClones();
	}

	void TimeManager::Update(float _delta) {
		mTimestamp += _delta;
		mDeltaTime = _delta;

		//If its time for a snapshot
		if (mTimestamp >= RecordingRate) {
			mTimestamp -= RecordingRate;
		#if _DEBUG
			mTimestamp = 0;
		#endif
			//Generate 
			Snapshot* s = mTimeline->GenerateSnapShot(mLevelTime, mClones);
			mTimeline->AddSnapshot(mLevelTime, s);
			mLevelTime = mTimeline->GetCurrentGameTimeIndx() + 1;
		}

		//SystemLogger::GetLog() << mTimestamp / mRecordingTime << std::endl; 
		for (auto Interp : mCloneInterpolators) {
			if (Interp.second)
				Interp.second->Update(mTimestamp / RecordingRate);

		}

		//Update inputTimeLine
		if (VRInputManager::GetInstance().IsVREnabled()) {
			InputTimeline::InputNode* temp = VRInputManager::GetInstance().GetInputTimeline()->GetCurr();
			while (temp && temp->mNext && temp->mNext->mData.mLastFrame < mLevelTime) {
				if (temp->mData.mLastFrame < temp->mNext->mData.mLastFrame || (temp->mData.mLastFrame == temp->mNext->mData.mLastFrame && (temp->mNext->mData.mTime < (mTimestamp / RecordingRate)))) {
					for (unsigned int i = 0; i < mClones.size(); i++) {
						if (mClones[i]->GetUniqueId() == temp->mNext->mData.mControllerId) {
							if (DoesCloneExist(mClones[i]->GetUniqueId(), mLevelTime)) {
								//	SystemLogger::GetLog() << "Clone:" << "id " << temp->mData.mControllerId << " " << temp->mNext->mData.mButton << ':' << temp->mNext->mData.mButtonState << std::endl;
							} else {
								//	SystemLogger::GetLog() << "Found false" << std::endl;
							}
						}
					}
					VRInputManager::GetInstance().GetInputTimeline()->SetCurr(temp->mNext);
					temp = temp->mNext;
				} else {
					break;
				}

			}
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

	void TimeManager::AddInterpolatorForClone(BaseObject * _obj) {
		Interpolator<matrix4>* temp = new Interpolator<matrix4>(InterpolatorType::I_Matrix4);
		mCloneInterpolators[_obj->GetUniqueID()] = temp;
	}

	void TimeManager::UpdatePlayerObjectInTimeline(BaseObject *  _obj) {
		if (_obj != nullptr)
			mTimeline->UpdatePlayerBaseObject(_obj, _obj->GetUniqueID());
	}

	void TimeManager::ClearClones() {
		mClones.clear();
		//Clean up the interpolators
		for (auto Interp : mCloneInterpolators) {
			if (Interp.second)
				delete Interp.second;
		}
		mCloneInterpolators.clear();
	}

	bool TimeManager::CheckRewindAvaliable(unsigned int _frame) {
		//wrapped
		if (mTimeline->GetCurrentGameTimeIndx() - _frame > mTimeline->GetCurrentGameTimeIndx())
			return false;
		else
			return true;
	}

	unsigned int TimeManager::GetCurrentSnapFrame() {
		return mTimeline->GetCurrentGameTimeIndx();
	}

	Interpolator<matrix4>* TimeManager::GetCloneInterpolator(unsigned short _id) {

		if (mCloneInterpolators.find(_id) != mCloneInterpolators.end())
			return mCloneInterpolators[_id];

		return nullptr;
	}

	unsigned int TimeManager::GetTotalSnapsmade() {
		return mTimeline->GetTotalSnaps();
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

	void TimeManager::RewindMakeClone(unsigned int _frame, BaseObject*& _ob1, BaseObject*& _ob2, BaseObject*& _ob3) {
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

	bool TimeManager::DoesCloneExist(unsigned int _id, unsigned int _frame) {
		ObjectLifeTime* lifetemp = mTimeline->GetObjectLifetime(_id);
		if (lifetemp && (unsigned int)lifetemp->mBirth < _frame && (unsigned int)lifetemp->mDeath > _frame) {
			return true;
		}
		return false;

	}

	void TimeManager::ToggleCloneCountDisplay(void * _command, std::wstring _ifOn) {
		CommandConsole* cc = (CommandConsole*)_command;
		if (_ifOn == L"ON") {
			instanceTimemanager->mCloneCountOn = true;
			CommandConsole::Instance().DisplaySet(L"");
		} else if (_ifOn == L"OFF") {
			instanceTimemanager->mCloneCountOn = false;
			CommandConsole::Instance().DisplaySet(L"");

		} else {
			CommandConsole::Instance().DisplaySet(L"INVALID INPUT: " + _ifOn + L"\nCORRECT INPUT: /CLONECOUNT (ON/OFF)");
		}
	}
	void TimeManager::ToggleSnapshotCountDisplay(void * _command, std::wstring _ifOn) {
		CommandConsole* cc = (CommandConsole*)_command;
		if (_ifOn == L"ON") {
			instanceTimemanager->mSnapshotCountOn = true;
			CommandConsole::Instance().DisplaySet(L"");
		} else if (_ifOn == L"OFF") {
			instanceTimemanager->mSnapshotCountOn = false;
			CommandConsole::Instance().DisplaySet(L"");

		} else {
			CommandConsole::Instance().DisplaySet(L"INVALID INPUT: " + _ifOn + L"\nCORRECT INPUT: /SNAPCOUNT (ON/OFF)");
		}
	}
	void TimeManager::DisplayCloneCount() {
		if (instanceTimemanager->mCloneCountOn) {
			std::wstring CloneCount = L"Clone(s): " + std::to_wstring(mClones.size());

			Font* tempFont;
			if (!CommandConsole::Instance().isVRon()) {
				tempFont = new Font(L"Times New Roman", 25, (D2D1::ColorF(D2D1::ColorF::Purple, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
			} else {
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
	void TimeManager::DisplaySnapshotCount() {
		if (instanceTimemanager->mSnapshotCountOn) {
			std::wstring CloneCount = L"Snapshots: " + std::to_wstring(mTimeline->GetCurrentGameTimeIndx());

			Font* tempFont;
			if (!CommandConsole::Instance().isVRon()) {
				tempFont = new Font(L"Times New Roman", 25, (D2D1::ColorF(D2D1::ColorF::Blue, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
			} else {
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
	void TimeManager::HotfixResetTimeline() {
		RewindTimeline(0, Level::Instance()->iGetLeftController()->GetUniqueID(), Level::Instance()->iGetRightController()->GetUniqueID(), Level::Instance()->iGetHeadset()->GetUniqueID());
		mTimeline->HotFixResetLevel();
		for (int i = 0; i < mClones.size(); ++i) {
			std::vector<Component*> components = mClones[i]->GetComponents(eCOMPONENT_COLLIDER);
			for (int j = 0; j < components.size(); ++j) {
				components[j]->Destroy();
			}

			 components = mClones[i]->GetComponents(eCOMPONENT_MESH);
			for (int j = 0; j < components.size(); ++j) {
				components[j]->Destroy();
			}

			for (int k = 0; k < Physics::Instance()->mObjects.size(); ++k) {
				if (Physics::Instance()->mObjects[k]->GetUniqueID() == mClones[i]->GetUniqueID()) {
					//I know I could have just iterated through it with an iterator but im lazy and tired
					Physics::Instance()->mObjects.erase(Physics::Instance()->mObjects.begin() + k);
				}
			}
			Pool::Instance()->iRemoveObject(mClones[i]->GetUniqueID());
		}
		ClearClones();
	
		VRInputManager::GetInstance().GetPlayerPosition()[3].Set(1.9f, -1.0f, 8, 1.0f);
	}													 
}
