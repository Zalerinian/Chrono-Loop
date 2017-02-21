#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"

struct CCElasticReactionWithPlane : public CodeComponent
{
	virtual void OnCollision(Collider& _col1, Collider& _col2, float _time)
	{
		vec4f normalVel = (((PlaneCollider*)&_col2)->mNormal * (_col1.mVelocity * ((PlaneCollider*)&_col2)->mNormal));
		_col1.mVelocity = (normalVel * -_col1.mElasticity) + (_col1.mVelocity - normalVel);
	}
};