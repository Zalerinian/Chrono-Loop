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
			if (VRInputManager::GetInstance().IsVREnabled() && mCollider)
			{
				InputTimeline::InputNode*temp = VRInputManager::GetInstance().FindLastInput(mCollider->GetBaseObject()->GetUniqueID());
				//This is gross but i dont know how to get around this without storing mheld and should move in timeline
				if (mInput && temp && mPickUp && temp->mData.mLastFrame < mInput->mData.mLastFrame) {
					mHeld = false;
					mPickUp->mShouldMove = true;
				}

				mInput = temp;
			}
			//SystemLogger::GetLog() << mCollider->mHitting.size() << std::endl;
			if (VRInputManager::GetInstance().IsVREnabled() && mInput) {

				if (mHeld && mPickUp != nullptr) {
					matrix4 m = mObject->GetTransform().GetMatrix();
					mPickUp->SetPos(m.Position);
					if (mInput->mData.mButton == vr::k_EButton_SteamVR_Trigger && mInput->mData.mButtonState == 1) {
						ReleaseObject();
					}
				} else if (mInput->mData.mButton == vr::k_EButton_SteamVR_Trigger && mInput->mData.mButtonState == -1 && !mHeld && !mCollider->mHitting.empty()) {
					SomethingtoController();
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
		}

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
				mPickUp->mForces = { 0,0,0,0 };
				mPickUp->mVelocity = { 0,0,0,0 };
				mPickUp->mAcceleration = { 0,0,0,0 };
			}
		}

		virtual void ReleaseObject() {
			//vec4f force = VRInputManager::GetInstance().GetController(mControllerRole).GetVelocity();
			vec4f force = mCollider->mVelocity;
			force[2] *= -1; // SteamVR seems to Assume +Z goes into the screen.
			mPickUp->mVelocity = force;
			mPickUp->mShouldMove = true;
			mHeld = false;
		}
	};

} // Epoch Namespace