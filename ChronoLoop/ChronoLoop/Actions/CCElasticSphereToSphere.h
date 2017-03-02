#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "../Common/Logger.h"

struct CCElasticSphereToSphere : public CodeComponent
{
	bool colliding = false;
	virtual void OnCollision(Collider& _col, Collider& _other, float _time)
	{
		if (!colliding && _other.mColliderType == Collider::eCOLLIDER_Sphere)
		{
			colliding = true;
			
			float avgElasticity = (_col.mElasticity + _other.mElasticity) / 2;
			vec4f collisionNormal = (_col.GetPos() - _other.GetPos()).Normalize();
			_col.mVelocity += collisionNormal * (1 + avgElasticity);
		}
		else
			colliding = false;
	}
};