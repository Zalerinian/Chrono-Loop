#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "../Core/TimeManager.h"
#include "../Common/Logger.h"


struct Bootleg {
	struct Node {
		vec2f data;
		Node *mNext, *mPrev;
		Node(vec2f _d) : data(_d) {}
	};
	Node *mHead, *mTail;
	unsigned int mSize, mLimit = 9;

	void AddHead(vec2f _node) {
		if (mHead == nullptr) {
			mHead = new Node(_node);
			mTail = mHead;
			mHead->mNext = mHead->mPrev = nullptr;
			mSize = 1;
		} else {
			if (mSize >= mLimit) {
				Node *tail = mTail->mPrev;
				delete mTail;
				mTail = tail;
				mTail->mNext = nullptr;
				--mSize;
			}
			Node* n = new Node(_node);
			n->mPrev = nullptr;
			n->mNext = mHead;
			mHead->mPrev = n;
			mHead = n;
			mSize++;
		}
	}

	vec2f& operator[](unsigned int _index) {
		Node* n = mHead;
		for (unsigned int i = 0; i < _index; ++i) {
			n = mHead->mNext;
		}
		return n->data;
	}

	~Bootleg() {
		Node* n = mHead;
		while (n) {
			Node* self = n;
			n = n->mNext;
			delete self;
		}
	}
};

struct BoxSnapToControllerAction : public CodeComponent {
	bool mHeld = false;
	bool mHeldLeft = false;
	Bootleg mBootleg;

	virtual void Update() override {
		Controller &leftController = VRInputManager::Instance().iGetController(true), &rightController = VRInputManager::Instance().iGetController(false);
		if (VRInputManager::Instance().iIsInitialized()) {
			if (leftController.GetPress(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) {
				SnapToController(true);
				mHeldLeft = true;
			} else if (rightController.GetPress(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) {
				SnapToController(false);
				mHeldLeft = false;
			} else if (mHeld) {
				ReleaseCube();
			}
			if (leftController.GetPressDown((vr::EVRButtonId::k_EButton_Grip))) {
				TimeManager::Instance()->RewindTimeline();
			} else if (rightController.GetPressDown((vr::EVRButtonId::k_EButton_Grip))) {
				TimeManager::Instance()->RewindTimeline();
			}


			vec2f touch = leftController.GetAxis();
			mBootleg.AddHead(touch);
			//SystemLogger::GetLog() << "(" << touch.x << "," << touch.y << ")" << std::endl;
			if (mBootleg.mSize == mBootleg.mLimit) {
				// Get initial point, get vector from it's negation (v - (-v)), and then cross it (v.y, -v.x)
				vec2f initialPoint = mBootleg[0];
				vec2f line = (initialPoint - (-initialPoint));
				vec2f clockwise = line.Cross().Normalize();

				vec2f pointEight = mBootleg[8];
				vec2f leg = (pointEight - initialPoint);
				vec2f nLeg = leg.Normalize();
				if (leg.SquaredMagnitude() >= 0.01f) {
					if (nLeg * clockwise < 0) {
						SystemLogger::GetLog() << "Somewhat Clockwise" << std::endl;
					}
					if (nLeg * clockwise > 0) {
						SystemLogger::GetLog() << "Somewhat Counter-Clockwise" << std::endl;
					}
				}
			}
		}
		//SystemLogger::GetLog() << "[Debug] Touchpad Axis: (" << touch.x << ", " << touch.y << ")" << std::endl;
	}

	virtual void SnapToController(bool left) {
		mHeld = true;
		mHeldLeft = left;
		matrix4 m = Math::FromMatrix(VRInputManager::Instance().iGetController(left).GetPose().mDeviceToAbsoluteTracking);
		((CubeCollider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->SetPos(Math::MatrixTranspose(m).tiers[3]);
		mObject->GetTransform().SetMatrix(m);
		((Collider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->mShouldMove = false;
	}

	virtual void ReleaseCube() {
		if (mHeldLeft) {
			vec4f force = VRInputManager::Instance().iGetController(true).GetVelocity();
			((Collider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->mVelocity = force;
		} else {
			vec4f force = VRInputManager::Instance().iGetController(false).GetVelocity();
			((Collider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->mVelocity = force;
		}
		((Collider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->mShouldMove = true;
		mHeldLeft = mHeld = false;
	}
};