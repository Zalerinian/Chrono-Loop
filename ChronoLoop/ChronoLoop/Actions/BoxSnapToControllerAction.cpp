
#include "../Core/LevelManager.h"
#include "../Core/TimeManager.h"
#include "BoxSnapToControllerAction.hpp"

namespace Epoch {
	std::vector<BoxSnapToControllerAction*> BoxSnapToControllerAction::mHands = std::vector<Epoch::BoxSnapToControllerAction*>();


	void Epoch::BoxSnapToControllerAction::Start() {
		mCollider = (ControllerCollider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0));
		mHands.push_back(this);
	}

	void Epoch::BoxSnapToControllerAction::OnDestroy() {
		//take yourself out of the list of hands
		for (unsigned int i = 0; i < mHands.size(); i++) {
			if (mHands[i] == this) {
				mHands.erase(mHands.begin() + i);
			}
		}
	}

	void Epoch::BoxSnapToControllerAction::Update()  {
		Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();

		bool paused = false;

		if (cLevel->GetTimeManipulator() != nullptr) {
			paused = cLevel->GetTimeManipulator()->isTimePaused();
		}
		if (VRInputManager::GetInstance().IsVREnabled() && mCollider) {
			InputTimeline::InputNode*temp;
			if (paused) {
				temp = VRInputManager::GetInstance().FindLastInput(mCollider->GetBaseObject()->GetUniqueID(), true);
			} else {
				temp = VRInputManager::GetInstance().FindLastInput(mCollider->GetBaseObject()->GetUniqueID(), false);
			}
			if (mInput && temp && mPickUp &&
				(temp->mData.mLastFrame < mInput->mData.mLastFrame ||
				(temp->mData.mLastFrame == mInput->mData.mLastFrame && temp->mData.mTime < mInput->mData.mTime))) {
				mHeld = false;
				CheckIfBoxShouldMove();
			}
			mInput = temp;
		}

		if (VRInputManager::GetInstance().IsVREnabled() && mInput) {
			if (mHeld && mPickUp != nullptr) {
				matrix4 m = mObject->GetTransform().GetMatrix();
				mPickUp->SetPos(m.Position);
				if (mInput->mData.mButton == vr::k_EButton_SteamVR_Trigger && mInput->mData.mButtonState == 1 && mHeld || (paused)) {
					SystemLogger::GetLog() << "Id: " << mCollider->GetBaseObject()->GetUniqueID() << " Released object" << std::endl;
					ReleaseObject();
				}
			} else if (mInput->mData.mButton == vr::k_EButton_SteamVR_Trigger && mInput->mData.mButtonState == -1 && !mCollider->mHitting.empty() && (!paused)) {
				if (Settings::GetInstance().GetBool("PauseMenuUp"))
					Settings::GetInstance().SetBool("DidRealStuffInPauseMenu", true);
				if (mIsGrabOnCooldown) {
					(mGrabTimeout + mGrabTimestamp < TimeManager::Instance()->GetTotalGameTime()) ? mIsGrabOnCooldown = false : mIsGrabOnCooldown = true;
				}
				else {
					SomethingtoController();
				}
			}
		}
		Collider* col = (Collider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);
		if(col)
		{
			col->SetPos(mObject->GetTransform().GetMatrix().Position);
		}
		CheckIfBoxAlreadyHeld();
	}
	bool BoxSnapToControllerAction::CheckIfBoxAlreadyHeld()
	{
		for (unsigned int i = 0; i < mHands.size(); i++) {
			if (mHands[i]->mObject->GetUniqueID() == mObject->GetUniqueID())
				continue;
			//you would usally loop to find correct code component but i know that this is the only code components clones will have in our game
			BoxSnapToControllerAction* temp = mHands[i];
			if (temp) {
				//If my mPickup is the same as the other code component, make it release theirs so new code component can pick it up
				if (temp->mHeld == true && temp->mPickUp && mPickUp && temp->mPickUp == mPickUp) {
					//TODO PAT: Have a time out system for non player hands to where they cant pick up an object after a certain amount of time
					if (temp->mObject->GetUniqueID() != LevelManager::GetInstance().GetCurrentLevel()->GetLeftController()->GetUniqueID() &&
						temp->mObject->GetUniqueID() != LevelManager::GetInstance().GetCurrentLevel()->GetRightController()->GetUniqueID())
					{
					temp->mIsGrabOnCooldown = true;
					temp->mGrabTimestamp = TimeManager::Instance()->GetTotalGameTime();
					temp->ReleaseObject();
					SystemLogger::GetLog() << "Released box" << std::endl;
					return true;
					}
					return false;
				}
			}
		}
		return true;
	}
	void BoxSnapToControllerAction::CheckIfBoxShouldMove()
	{
		for (unsigned int i = 0; i < mHands.size(); i++) {
			if (mHands[i]->mPickUp == mPickUp && mHands[i] != this) {
				//dont set mShouldmove because another box is holding it
				break;
			}
			if ((mHands[i]->mPickUp != mPickUp || mHands[i] == this) && i == mHands.size() - 1) {
				mPickUp->mShouldMove = true;
			}
		}
	}
	void BoxSnapToControllerAction::SomethingtoController()
	{
		mHeld = true;
		if (Settings::GetInstance().GetInt("tutStep") == 6)//Picked up object (tut 1)
			Settings::GetInstance().SetInt("tutStep", 8);//End level 1 tutorial

		//matrix4 m = VRInputManager::GetInstance().GetController(mControllerRole).GetPosition();
		matrix4 m = mObject->GetTransform().GetMatrix();

		vec4f pos, setPos;
		vec4f controllerPos = mCollider->GetPos();
		bool unset = true;
		for (auto iter = mCollider->mHitting.begin(); iter != mCollider->mHitting.end(); ++iter) {
			pos = (*iter)->GetPos();
			if (unset) {
				setPos = pos;
				mPickUp = (*iter);
				unset = false;
			} else {
				float tx = pos.x - mCollider->GetPos().x;
				float ty = pos.y - mCollider->GetPos().y;
				float tz = pos.z - mCollider->GetPos().z;
				float sx = setPos.x - mCollider->GetPos().x;
				float sy = setPos.y - mCollider->GetPos().y;
				float sz = setPos.z - mCollider->GetPos().z;

				if (tx < sx || ty < sy || tz < sz) {
					setPos = pos;
					mPickUp = (*iter);
				}
			}
		}

		

		if (mPickUp && CheckIfBoxAlreadyHeld()) {
			if (!mPickUp->mShouldMove) {
				mCollider->mHitting.erase(mPickUp);
				return;
			}
			mHeld = true;
			mPickUp->SetPos((m).tiers[3]);
			//mObject->GetTransform().SetMatrix(m);
			mPickUp->mShouldMove = false;
			mPickUp->mForces = { 0,0,0 };
			mPickUp->mVelocity = { 0,0,0 };
			mPickUp->mAcceleration = { 0,0,0 };

			if (mInput->mData.mControllerId == LevelManager::GetInstance().GetCurrentLevel()->GetLeftController()->GetUniqueID()
				|| mInput->mData.mControllerId == LevelManager::GetInstance().GetCurrentLevel()->GetRightController()->GetUniqueID()) {
				VRInputManager::GetInstance().GetController((mInput->mData.mPrimary) ? eControllerType_Primary : eControllerType_Secondary).TriggerHapticPulse(1000);
			}
		}
		else
		{
			mPickUp = nullptr;
		}
	}
	void BoxSnapToControllerAction::ReleaseObject()
	{
		if (!mInput)
			return;

		vec4f force = mInput->mData.mVelocity;
		force[2] *= -1; // SteamVR seems to Assume +Z goes into the screen.
		//mPickUp->mTotalForce = mPickUp->mForces + (mPickUp->mGravity * mPickUp->mMass);
		//mPickUp->mAcceleration = mPickUp->mTotalForce / mPickUp->mMass;
		if (mPickUp)
		{
			mPickUp->mVelocity = force;
			CheckIfBoxShouldMove();
			mPickUp = nullptr;
			mHeld = false;
		}
	}
}
