#include "stdafx.h"
#include "Controller.h"

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

Controller::Controller()
{
	mHairTriggerDelta = 0.1f; //trigger deadzone
}

void Controller::Update()
{
	//update the contoller pose/state when called. 
	mPrevState = mState;
	if (mHmd != NULL)
	{
		mValid = mHmd->GetControllerStateWithPose(mTrackingSpace, mIndex, &mState, sizeof(mState), &mPose);
		if (mPrevState.ulButtonPressed != mState.ulButtonPressed)
			int breakpoint = 0;
		UpdateHairTrigger();
	}
}

void Controller::UpdateHairTrigger()
{
	mHairTriggerPrevState = mHairTriggerState;
	float value = mState.rAxis[1].x;
	if (mHairTriggerState)
		if (value < mHairTriggerLimit - mHairTriggerDelta || value <= 0.0f)
			mHairTriggerState = false;
	else
		if (value > mHairTriggerLimit + mHairTriggerDelta || value >= 1.0f)
			mHairTriggerState = true;

	mHairTriggerLimit = mHairTriggerState ? MAX(mHairTriggerLimit, value) : MIN(mHairTriggerLimit, value);
}


vec2f Controller::GetAxis(vr::EVRButtonId buttonId)
{
	Update();
	vec2f axis_value;
	u_int axisId = (u_int)buttonId - (u_int)vr::k_EButton_Axis0;
	switch (axisId)
	{
	case 0: axis_value = vec2f(mState.rAxis[0].x, mState.rAxis[0].y); break;
	case 1: axis_value = vec2f(mState.rAxis[1].x, mState.rAxis[1].y); break;
	case 2: axis_value = vec2f(mState.rAxis[2].x, mState.rAxis[2].y); break;
	case 3: axis_value = vec2f(mState.rAxis[3].x, mState.rAxis[3].y); break;
	case 4: axis_value = vec2f(mState.rAxis[4].x, mState.rAxis[4].y); break;
	}
	return axis_value;
}

void Controller::TriggerHapticPulse(u_int duration_micro_sec, vr::EVRButtonId buttonId)
{
	if (mHmd != NULL)
	{
		u_int axisId = (u_int)buttonId - (u_int)vr::k_EButton_Axis0;
		mHmd->TriggerHapticPulse(mIndex, axisId, (char)duration_micro_sec);
	}
}
