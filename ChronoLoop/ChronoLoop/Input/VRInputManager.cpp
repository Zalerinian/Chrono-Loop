#include "stdafx.h"
#include "VrInputManager.h"
#include "../Common/Logger.h"
#include "../Core/TimeManager.h"
#include "../Core/LevelManager.h"

namespace Epoch {

	VIM* VRInputManager::sInstance = nullptr;

	VIM& VRInputManager::GetInstance() {
		return *sInstance;
	}

	void VRInputManager::Initialize(vr::IVRSystem * _vr) {
		if (nullptr == sInstance) {
			if (nullptr == _vr) {
				SystemLogger::Debug() << "No valid VR system was found, VR will be disabled." << std::endl;
			}
			sInstance = new VIM(_vr);
		}
	}

	void VRInputManager::DestroyInstance() {
		if (nullptr != sInstance) {
			delete sInstance;
			sInstance = nullptr;
		}
	}

	VRInputManager::VRInputManager() {}

	VRInputManager::~VRInputManager() {}




	VIM::VIM(vr::IVRSystem *_vr) {
		if (nullptr == _vr) {
			SystemLogger::Warn() << "VR Input is disabled." << std::endl;
			return;
		}
		mVRSystem = _vr;
		int rightID = mVRSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
		int leftID = mVRSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
		SystemLogger::Info() << "Right controller ID: " << rightID << std::endl;
		SystemLogger::Info() << "Left controller ID:  " << leftID << std::endl;
		mRightController.Setup(rightID);
		mLeftController.Setup(leftID);
		mPlayerPosition = matrix4::CreateNewTranslation(8, 0, -4);
		mInputTimeline = new InputTimeline();
	}

	VIM::~VIM() {
		delete mInputTimeline;
	}

	void VIM::Update() {
		int GestureCheck = 0;
		if (mRightController.GetIndex() < 0) {
			mRightController.mIndex = mVRSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);
			if (mRightController.GetIndex() > 0) {
				mRightController.Update();
				SystemLogger::Info() << "Right Controller connected at index " << mRightController.GetIndex() << std::endl;
			}
		} else {
			mRightController.Update();
		}
		if (mLeftController.GetIndex() < 0) {
			mLeftController.mIndex = mVRSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
			if (mLeftController.GetIndex() > 0) {
				mLeftController.Update();
				SystemLogger::Info() << "Left Controller connected at index " << mLeftController.GetIndex() << std::endl;
			}
		} else {
			mLeftController.Update();
		}

		vr::VRCompositor()->WaitGetPoses(mPoses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);

		if (!TimeManager::Instance()->GetInterpolatorsUpdating())
			GestureCheck = mRightController.CheckGesture();
		else
			GestureCheck = 0;
		TimeManager::Instance()->BrowseTimeline(GestureCheck, 1);


		Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();
		//Pull vr events to find button press or up
		vr::VREvent_t tempEvent;
		bool paused = false;
		if (cLevel->GetTimeManipulator() != nullptr) {
			paused= cLevel->GetTimeManipulator()->isTimePaused();
		}
		
		//if there is a event avaliable and the game is focused
		while (mVRSystem->PollNextEvent(&tempEvent, sizeof(tempEvent)) && !mVRSystem->IsInputFocusCapturedByAnotherProcess() && (!paused)) {
			if ((tempEvent.eventType == vr::EVREventType::VREvent_ButtonPress || tempEvent.eventType == vr::EVREventType::VREvent_ButtonUnpress) && tempEvent.data.controller.button != vr::k_EButton_Grip  && tempEvent.data.controller.button != vr::k_EButton_ApplicationMenu) {
				AddInputNode(&tempEvent);
			}
		}

	}

	Controller& VIM::GetController(ControllerType _t) {
		if (mIsLeftPrimary) {
			return _t == eControllerType_Primary ? mLeftController : mRightController;
		} else {
			return _t == eControllerType_Primary ? mRightController : mLeftController;
		}
	}

	void VIM::AddInputNode(vr::VREvent_t* _event) {
		Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();
		InputTimeline::InputNode* node = new InputTimeline::InputNode();

		
		node->mData.mButton = (vr::EVRButtonId)_event->data.controller.button;
		node->mData.mLastFrame = TimeManager::Instance()->GetCurrentSnapFrame();
		float time = TimeManager::Instance()->GetSnapTweenTime() - (_event->eventAgeSeconds / RecordingRate);
		if(time < 0.0f)
		{
			time += 1;
			node->mData.mLastFrame -= 1;
		}
		node->mData.mTime = time;

		if (_event->eventType == vr::EVREventType::VREvent_ButtonPress) {
			node->mData.mButtonState = -1;
		} else if (_event->eventType == vr::EVREventType::VREvent_ButtonUnpress) {
			node->mData.mButtonState = 1;
		}

		if (_event->trackedDeviceIndex == mVRSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand)) {
			node->mData.mControllerId = cLevel->GetLeftController()->GetUniqueID();
			node->mData.mVelocity = mLeftController.GetVelocity();
			node->mData.mPrimary = mIsLeftPrimary;
		} else {
			node->mData.mControllerId = cLevel->GetRightController()->GetUniqueID();
			node->mData.mVelocity = mRightController.GetVelocity();
			node->mData.mPrimary = !mIsLeftPrimary;
		}
		mInputTimeline->Insert(node);
		//mInputTimeline->DisplayTimeline();
	}


	void VIM::RewindInputTimeline(unsigned int _frame, unsigned short _id1, unsigned short _id2) {

		InputTimeline::InputNode* temp = mInputTimeline->GetCurr();
		if(!temp)
		{
			temp = mInputTimeline->GetHead();
		}
		while (temp) {
			//Have reached the point we want to stop
			if (temp->mData.mLastFrame < _frame) {
				if ((temp->mData.mControllerId == _id1 || temp->mData.mControllerId == _id2) && temp->mData.mButtonState == -1) {
					temp->mData.mButtonState = 1;
					if (temp->mPrev && (temp->mPrev->mData.mControllerId == _id1 || temp->mPrev->mData.mControllerId == _id2) && temp->mPrev->mData.mButtonState == -1) {
						temp->mPrev->mData.mButtonState = 1;
					}
				}
				break;
			}

				//Delete old controller input
				if (temp->mData.mControllerId == _id1 || temp->mData.mControllerId == _id2) {
					InputTimeline::InputNode* del = temp;
					if (temp->mPrev) {
						temp = temp->mPrev;
						temp->mNext = del->mNext;
						if (del->mNext)
							del->mNext->mPrev = temp;
					} else {
						temp = nullptr;
					}
					if(del == mInputTimeline->GetHead())
					{
						if (del == mInputTimeline->GetCurr())
							mInputTimeline->SetCurr(nullptr);
						mInputTimeline->SetHead(nullptr);
					}
					delete del;
				} else if (temp->mPrev) {
					temp = temp->mPrev;
				} else
					break;
			
			mInputTimeline->SetCurr(temp);
		}
		//mInputTimeline->DisplayTimeline();
	}

	void VIM::MoveInputTimeline(unsigned int _frame) {

		InputTimeline::InputNode* temp = mInputTimeline->GetCurr();
		while (temp) {
			//Have reached the point we want to stop
			if (temp->mData.mLastFrame < _frame) {
				break;
			}
			if (temp->mPrev) {
				temp = temp->mPrev;
			} else if (!temp->mPrev) {
				break;
			}
		mInputTimeline->SetCurr(temp);
		}
	}
	//_fromTempCurrent is if you want to go back to current of rewind action
	InputTimeline::InputNode * VIM::FindLastInput(unsigned short _id, bool _fromTempCurrent) {
		InputTimeline::InputNode* temp = mInputTimeline->GetCurr();

		if(_fromTempCurrent)
		{
			//find the spot where its before mTempCurrentGameIndx
			unsigned int frame = TimeManager::Instance()->GetTempCurSnap();
			temp = mInputTimeline->GetCurr();
			while(temp)
			{
				//Have reached the point we want to stop
				if (temp->mData.mLastFrame < frame) {
					break;
				}
				if (temp->mPrev) {
					temp = temp->mPrev;
				} else if (!temp->mPrev) {
					break;
				}
			}
		}

		while (temp) {
			if (temp->mData.mControllerId == _id) {
				return temp;
			}
			if (temp->mPrev) {
				temp = temp->mPrev;
			}
			else if (!temp->mPrev) {
				return nullptr;
			}
		}
		return nullptr;
	}
}
