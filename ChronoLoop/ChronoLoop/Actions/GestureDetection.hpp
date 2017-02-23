#pragma once
#include "CodeComponent.hpp"
#include "../Objects/BaseObject.h"
#include "../Input/VRInputManager.h"
#include "../Common/LimitedList.h"

struct GestureDetection : CodeComponent {
	bool mIsHeld = false;
	char mHolder = -1;
	vec2f mInitialContactPoint;
	LimitedList mList;

	virtual void Update() {
		vec2f zero2f;
		for (char i = 0; i < 2; ++i) {
			Controller &controller = VRInputManager::Instance().iGetController(i == 0);
			if (!mIsHeld) {
				// Check to see if this controller is being touched appropriately.
				if (controller.GetAxis() != zero2f) {
					// The controller touchpad is being touched
					mIsHeld = true;
					mHolder = i;
				}
			}

			if (i == mHolder) {
				// This controller started touching the touchpad first.
				if (controller.GetAxis() == zero2f) {
					// The touchpad has been released.
					mIsHeld = false;
					mHolder = -1;
					mList.Clear();
				} else {
					// The touchpad is still being touched.
				}
			}
		}
	}
};