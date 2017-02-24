#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"

struct CCElasticAABBtoAABB : public CodeComponent
{
	bool colliding = false;
	virtual void OnCollision(Collider& _col, Collider& _other, float _time)
	{
		if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane)
		{
			colliding = true;
			//SystemLogger::GetLog() << "BOX COLLISION" << std::endl;

			float avgElasticity = (_col.mElasticity + _other.mElasticity) / 2;
			vec4f otherPos = (((CubeCollider*)&_other)->mMax + ((CubeCollider*)&_other)->mMin) * 0.5f;
			vec4f collisionNormal = (_col.GetPos() - otherPos).Normalize();
			//vec4f relativeVelocity = _col.mVelocity - _other.mVelocity;
			_col.mVelocity += collisionNormal * (1 + _col.mElasticity);

			//float impulseMagnitude = -(1 + avgElasticity) * _col.mMass * _other.mMass * (relativeVelocity * collisionNormal) / (_col.mMass + _other.mMass);
			//vec4f impulse = collisionNormal * impulseMagnitude;
			//_col.mImpulsiveForce = impulse / _time;
			//_other.mImpulsiveForce = -impulse / _time;
			//_col.mVelocity = _col.mVelocity + impulse / _col.mMass;
			//_other.mVelocity = _other.mVelocity - impulse / _other.mMass;
			//_col.mTotalForce = _col.mImpulsiveForce;
			//_other.mTotalForce = _other.mImpulsiveForce;
		}
		else
			colliding = false;
	}
};