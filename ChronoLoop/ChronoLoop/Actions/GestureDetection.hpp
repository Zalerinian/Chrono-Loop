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
		vec2f mInitialContactPoint, mZero, mInvalid;
		LimitedList<vec2f> mList;

		GestureDetection(ControllerType _t) : mControllerRole(_t), mInvalid(-2, -2), mInitialContactPoint(-2, -2) {}

		virtual void Update() {
			if (VRInputManager::GetInstance().IsVREnabled()) {

				Controller &controller = VRInputManager::GetInstance().GetController(mControllerRole);
				
				if (controller.GetTouch(vr::k_EButton_SteamVR_Touchpad)) {
					SystemLogger::Debug() << "Touchpad touched." << std::endl;
				}
				if (mInitialContactPoint == mInvalid) {
					// There was no initial point
				}
				if (!mIsHeld) {
					// Check to see if this controller is being touched appropriately.
					if (controller.GetAxis() != mZero) {
						// The controller touchpad is being touched
						mIsHeld = true;
					}
				}
			}

		}
	};

}