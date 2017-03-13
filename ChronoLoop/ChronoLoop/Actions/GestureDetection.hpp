#pragma once
#include "CodeComponent.hpp"
#include "../Objects/BaseObject.h"
#include "../Input/VRInputManager.h"
#include "../Common/LimitedList.h"

namespace Epoch {

	struct GestureDetection : CodeComponent {
		ControllerType mControllerRole = eControllerType_Primary;
		bool mIsHeld = false;
		char mHolder = -1;
		const vec2f mInvalid;
		vec2f mInitialContactPoint;
		LimitedList<vec2f> mList;

		GestureDetection(ControllerType _t) : mControllerRole(_t), mInvalid(2, 2), mInitialContactPoint(2, 2) {}

		virtual void Update() {
		//	if (VRInputManager::GetInstance().IsVREnabled()) {

		//		Controller &controller = VRInputManager::GetInstance().GetController(mControllerRole);
		//		
		//		if (mInitialContactPoint != mInvalid && !controller.GetTouch(vr::k_EButton_SteamVR_Touchpad)) {
		//			mInitialContactPoint = mInvalid;
		//			mList.Clear();
		//		}
		//		if (controller.GetTouch(vr::k_EButton_SteamVR_Touchpad)) {
		//			vec2f contactPoint = controller.GetAxis(vr::k_EButton_SteamVR_Touchpad);
		//			mList.AddHead(contactPoint);
		//			if (mInitialContactPoint == mInvalid) {
		//				mInitialContactPoint = contactPoint;
		//			}

		//		}
		//	}

		}
	};

}