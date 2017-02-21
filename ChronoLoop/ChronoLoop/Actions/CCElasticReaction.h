#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"

struct CCElasticReaction : public CodeComponent
{
	virtual void OnCollision(Collider& _col, Collider& _other, float _time) 
	{
		if (_other.mColliderType == Collider::eCOLLIDER_Plane)
		{
			vec4f normalVel = (((PlaneCollider*)&_other)->mNormal * -(_col.mVelocity * ((PlaneCollider*)&_other)->mNormal));
			_col.mVelocity += normalVel * (1 + _col.mElasticity);
			return;
		}
		//Doesnt work
		//float avgElasticity = (_col.mElasticity + _other.mElasticity) / 2;
		//vec4f collisionNormal = (_col.GetPos() - _other.GetPos()).Normalize();
		//vec4f relativeVelocity = _col.mVelocity - _other.mVelocity;
		//
		//float impulseMagnitude = -(1 + avgElasticity) * _col.mMass * _other.mMass * (relativeVelocity * collisionNormal) / (_col.mMass + _other.mMass);
		//vec4f impulse = collisionNormal * impulseMagnitude;
		//_col.mImpulsiveForce = impulse / _time;
		//_other.mImpulsiveForce = -impulse / _time;
		//_col.mVelocity = _col.mVelocity + impulse / _col.mMass;
		//_other.mVelocity = _other.mVelocity - impulse / _other.mMass;
		//_col.mTotalForce = _col.mImpulsiveForce;
		//_other.mTotalForce = _other.mImpulsiveForce;
	}
};