#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "../Core/TimeManager.h"
#include "../Common/Logger.h"
#include <unordered_set>

namespace Epoch {

	struct BoxSnapToControllerAction : public CodeComponent {
		ControllerType mControllerRole = eControllerType_Primary;
		bool mHeld = false;
		ControllerCollider* mCollider;
		Collider* mPickUp = nullptr;
		InputTimeline::InputNode* mInput = nullptr;

		virtual void Start() {
			mCollider = (ControllerCollider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0));
		}

		virtual void Update() override {
			Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();

			bool mRight = false;
			bool mLeft = false;

			if (cLevel->GetRightTimeManipulator() != nullptr || cLevel->GetLeftTimeManipulator() != nullptr) {
				mRight = cLevel->GetRightTimeManipulator()->isTimePaused();
				mLeft = cLevel->GetLeftTimeManipulator()->isTimePaused();
			}
			if (VRInputManager::GetInstance().IsVREnabled() && mCollider) {
				InputTimeline::InputNode*temp;
				if (mRight || mLeft) {
					temp = VRInputManager::GetInstance().FindLastInput(mCollider->GetBaseObject()->GetUniqueID(), true);
				}
				else {
					temp = VRInputManager::GetInstance().FindLastInput(mCollider->GetBaseObject()->GetUniqueID(), false);
				}
				if (mInput && temp && mPickUp &&
					(temp->mData.mLastFrame < mInput->mData.mLastFrame ||
					(temp->mData.mLastFrame == mInput->mData.mLastFrame && temp->mData.mTime < mInput->mData.mTime))) {
					mHeld = false;
					mPickUp->mShouldMove = true;
					//	SystemLogger::GetLog() << "Should move on:: Old: snap = " << mInput->mData.mLastFrame << " time= " << mInput->mData.mTime << "   New: snap = " << temp->mData.mLastFrame << " time= " << temp->mData.mTime << std::endl;
				}
				mInput = temp;
			}
			//SystemLogger::GetLog() << mCollider->mHitting.size() << std::endl;
			if (VRInputManager::GetInstance().IsVREnabled() && mInput) {
				if (mHeld && mPickUp != nullptr) {
					matrix4 m = mObject->GetTransform().GetMatrix();
					mPickUp->SetPos(m.Position);
					if (mInput->mData.mButton == vr::k_EButton_SteamVR_Trigger && mInput->mData.mButtonState == 1 && mHeld || (mLeft || mRight)) {
						SystemLogger::GetLog() << "Id: " << mCollider->GetBaseObject()->GetUniqueId() << " Released object" << std::endl;
						if (mInput->mData.mControllerId == LevelManager::GetInstance().GetCurrentLevel()->GetLeftController()->GetUniqueId() || mInput->mData.mControllerId == LevelManager::GetInstance().GetCurrentLevel()->GetRightController()->GetUniqueId()) {
							VRInputManager::GetInstance().GetController((mInput->mData.mPrimary) ? eControllerType_Primary : eControllerType_Secondary).TriggerHapticPulse(500, mInput->mData.mButton);
						}
						ReleaseObject();
					}
				}
				else if (mInput->mData.mButton == vr::k_EButton_SteamVR_Trigger && mInput->mData.mButtonState == -1 && !mHeld && !mCollider->mHitting.empty() && (!mLeft && !mRight)) {
					SomethingtoController();
					if (mInput->mData.mControllerId == LevelManager::GetInstance().GetCurrentLevel()->GetLeftController()->GetUniqueId()) {
						VRInputManager::GetInstance().GetController(eControllerType_Primary).TriggerHapticPulse(500);
					}
					else if (mInput->mData.mControllerId == LevelManager::GetInstance().GetCurrentLevel()->GetRightController()->GetUniqueId()) {
						VRInputManager::GetInstance().GetController(eControllerType_Secondary).TriggerHapticPulse(500);
					}
				}
			}

			/*Controller &controller = VRInputManager::GetInstance().GetController(mControllerRole);
			if (mHeld && mPickUp != nullptr)
			{
			matrix4 m = VRInputManager::GetInstance().GetController(mControllerRole).GetPosition();
			mPickUp->SetPos(m.Position);
			if (!controller.GetPress(vr::EVRButtonId::k_EButton_SteamVR_Trigger))
			{
			ReleaseObject();
			}
			}
			else if (controller.GetPress(vr::EVRButtonId::k_EButton_SteamVR_Trigger) && !mHeld && !mCollider->mHitting.empty())
			{
			SomethingtoController();
			}
			*/

#pragma region Gestures
			//vec2f touch = leftController.GetAxis();
			//mBootleg.AddHead(touch);

			////SystemLogger::GetLog() << "(" << touch.x << "," << touch.y << ")" << std::endl;
			//if (mBootleg.mSize == mBootleg.mLimit) {
			//	// Get initial point, get vector from it's negation (v - (-v)), and then cross it (v.y, -v.x)
			//	vec2f initialPoint = mBootleg[0];
			//	vec2f line = (initialPoint - (-initialPoint));
			//	vec2f counterClockwise = line.Cross().Normalize();

			//	vec2f pointEight = mBootleg[8];
			//	vec2f leg = (pointEight - initialPoint);
			//	vec2f nLeg = leg.Normalize();
			//	if (leg.SquaredMagnitude() >= 0.01f) {
			//		if (nLeg * counterClockwise < 0) {
			//			SystemLogger::GetLog() << "Somewhat Clockwise" << std::endl;
			//		}
			//		if (nLeg * counterClockwise > 0) {
			//			SystemLogger::GetLog() << "Somewhat Counter-Clockwise" << std::endl;
			//		}
			//	}
			//}
			//SystemLogger::GetLog() << "[Debug] Touchpad Axis: (" << touch.x << ", " << touch.y << ")" << std::endl;
#pragma endregion Gestures
		}
		//mObject->GetTransform().SetMatrix(Math::MatrixRotateInPlace(mObject->GetTransform().GetMatrix(), 1, 0, 0, DirectX::XM_PI / 1024.0f));

		virtual void SomethingtoController() {
			mHeld = true;
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

			if (mPickUp) {
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
			}
		}

		virtual void ReleaseObject() {
			//vec4f force = VRInputManager::GetInstance().GetController(mControllerRole).GetVelocity();
			vec4f force = mInput->mData.mVelocity;
			force[2] *= -1; // SteamVR seems to Assume +Z goes into the screen.
			mPickUp->mVelocity = force;
			mPickUp->mShouldMove = true;
			mHeld = false;
		}
	};
} // Epoch Namespace