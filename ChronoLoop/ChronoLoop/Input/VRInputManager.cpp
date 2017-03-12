#include "stdafx.h"
#include "VrInputManager.h"
#include "../Common/Logger.h"
#include "../Core/TimeManager.h"
#include "../Core/Level.h"

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
		mPlayerPosition = matrix4::CreateTranslation(2, -1, 8);
		mInputTimeline = new InputTimeline();
	}

	VIM::~VIM() {}

	void VIM::Update() {
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

		//Update InputSnap TweenTime 
		mTweenTimestamp += TimeManager::Instance()->GetDeltaTime();
		if(mTweenTimestamp >= RecordingRate)
		{
			mTweenTimestamp -= RecordingRate;
		}
		mSnapTweenTime = mTweenTimestamp / RecordingRate;

		//Pull vr events to find button press or up
		vr::VREvent_t tempEvent;
		//if there is a event avaliable and the game is focused
		while(mVRSystem->PollNextEvent(&tempEvent, sizeof(tempEvent)) && !mVRSystem->IsInputFocusCapturedByAnotherProcess())
		{
			if((tempEvent.eventType == vr::EVREventType::VREvent_ButtonPress || tempEvent.eventType == vr::EVREventType::VREvent_ButtonUnpress) && tempEvent.data.controller.button != vr::k_EButton_Grip  && tempEvent.data.controller.button != vr::k_EButton_ApplicationMenu)
			{
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

	void VIM::AddInputNode(vr::VREvent_t* _event )
	{
		InputTimeline::InputNode* node = new InputTimeline::InputNode();
		node->mData.mLastFrame = TimeManager::Instance()->GetCurrentSnapFrame();
		node->mData.mButton = (vr::EVRButtonId)_event->data.controller.button;
		node->mData.mTime = mSnapTweenTime;

		if (_event->eventType == vr::EVREventType::VREvent_ButtonPress) {
			/*if (node->mPrev && (node->mPrev->mData.mButtonState == -1 || node->mPrev->mData.mButtonState == 0)) {
				node->mData.mButtonState = 0;
				SystemLogger::GetLog() << std::to_string(_event->data.controller.button) << " Button Press" << std::endl;
			} else {
				node->mData.mButtonState = -1;
				SystemLogger::GetLog() << std::to_string(_event->data.controller.button) << " Button Down" << std::endl;
			}*/
			node->mData.mButtonState = -1;
			//SystemLogger::GetLog() << std::to_string(_event->data.controller.button) << " Button Down:";
		} else if (_event->eventType == vr::EVREventType::VREvent_ButtonUnpress) {
			node->mData.mButtonState = 1;
			//SystemLogger::GetLog() <<  std::to_string(_event->data.controller.button) << " Up:";
		}
		//TODO PAT: FIX THIS
		if (_event->trackedDeviceIndex == mVRSystem->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand)) {
			node->mData.mControllerId = Level::Instance()->iGetLeftController()->GetUniqueId();
			//SystemLogger::GetLog() << "Lefthand" << std::endl;
		}
		else {
			node->mData.mControllerId = Level::Instance()->iGetRightController()->GetUniqueId();
			//SystemLogger::GetLog() <<  "Righthand" << std::endl;
		}
		//SystemLogger::GetLog() << node->mData.mControllerId << std::endl;
		mInputTimeline->Insert(node);
		//mInputTimeline->DisplayTimeline();
	}

	//Todo PAT: UPDATE CURRENT AFTER REWIND
	//TODO PAT: THIS NEEDS TO BE CALLED AFTER THE SWAPPING OF IDS
	void VIM::RewindInputTimeline(unsigned int _frame, unsigned short _id1, unsigned short _id2)
	{
	
		InputTimeline::InputNode* temp = mInputTimeline->GetCurr();
		//SystemLogger::GetLog() << "Rewind to " << _frame << std::endl;
		while(temp && temp->mPrev)
		{
			//Have reached the point we want to stop
			if(temp->mData.mLastFrame < _frame)
			{
				break;
			}
			//Delete old controller input
			if(temp->mData.mControllerId == _id1 || temp->mData.mControllerId == _id2)
			{
				InputTimeline::InputNode* del = temp;
				temp = temp->mPrev;
				temp->mNext = del->mNext;
				if(del->mNext)
				del->mNext->mPrev = temp;
				delete del;
			}
			else
			temp = temp->mPrev;

			mInputTimeline->SetCurr(temp);
		}

		//mInputTimeline->DisplayTimeline();
		//SystemLogger::GetLog() << "Rewinded to before " << _frame << std::endl;
	}

	InputTimeline::InputNode * VIM::FindLastInput(unsigned short _id) {

		InputTimeline::InputNode* temp = mInputTimeline->GetCurr();
		while (temp) {
			if (temp->mData.mControllerId == _id) {
				return temp;
			}
			if (temp->mPrev) {
				temp = temp->mPrev;
			}
			if (!temp->mPrev) {
				return nullptr;
			}
		}
		return nullptr;
	}


}
