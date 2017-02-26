#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "../Common/Logger.h"

struct CCElasticAABBtoAABB : public CodeComponent
{
	bool colliding = false;
	virtual void OnCollision(Collider& _col, Collider& _other, float _time)
	{
		if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane)
		{
			colliding = true;
			SystemLogger::GetLog() << "BOX COLLISION" << std::endl;

			vec4f max = ((CubeCollider*)&_other)->mMax;
			vec4f min = ((CubeCollider*)&_other)->mMin;
			vec4f maxt = ((CubeCollider*)&_col)->mMax;
			vec4f mint = ((CubeCollider*)&_col)->mMin;
			vec4f otherCenter = (max + min) * 0.5f;
			vec4f center = _col.GetPos();
			
			vec4f norm;
			if (center.x <= max.x && center.x >= min.x)
				norm = { 1,0,0,0 };
			else if (center.y <= max.y && center.y >= min.y)
				norm = { 0,1,0,0 };
			else if (center.z <= max.z && center.z >= min.z)
				norm = { 0,0,1,0 };
			////////////////////////////////////////////////////Add corner cases

			vec4f V = center - otherCenter;
			float dist = norm * V;
			vec4f N = norm * dist;
			vec4f point = N + otherCenter;
			vec4f collisionNormal = (center - point).Normalize();
			float avgElasticity = (_col.mElasticity + _other.mElasticity) / 2;
			_col.mVelocity += collisionNormal * (1 + _col.mElasticity);


			////SystemLogger::GetLog() << "BOX COLLISION" << std::endl;
			//
			//float avgElasticity = (_col.mElasticity + _other.mElasticity) / 2;
			//vec4f otherPos = (((CubeCollider*)&_other)->mMax + ((CubeCollider*)&_other)->mMin) * 0.5f;
			//vec4f collisionNormal = (_col.GetPos() - otherPos).Normalize();
			//
			////vec4f relativeVelocity = _col.mVelocity - _other.mVelocity;
			//
			//_col.mVelocity += collisionNormal * (1 + _col.mElasticity);
			//
			////float impulseMagnitude = -(1 + avgElasticity) * _col.mMass * _other.mMass * (relativeVelocity * collisionNormal) / (_col.mMass + _other.mMass);
			////vec4f impulse = collisionNormal * impulseMagnitude;
			////_col.mImpulsiveForce = impulse / _time;
			////_other.mImpulsiveForce = -impulse / _time;
			////_col.mVelocity = _col.mVelocity + impulse / _col.mMass;
			////_other.mVelocity = _other.mVelocity - impulse / _other.mMass;
			////_col.mTotalForce = _col.mImpulsiveForce;
			////_other.mTotalForce = _other.mImpulsiveForce;
		}
		else
			colliding = false;
	}
};