#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "..\Physics\Physics.h"
#include "..\Common\Logger.h"

struct CCButtonPress : public CodeComponent
{
	bool colliding = false;
	virtual void OnCollision(Collider& _col, Collider& _other, float _time)
	{
		if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane)
		{
			colliding = true;
			_col.mTotalForce += _other.mTotalForce;
			_col.mVelocity += _other.mVelocity;
			_col.mAcceleration += _other.mAcceleration;
		}
		else
			colliding = false;
	}
};