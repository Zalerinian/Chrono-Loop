#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "..\Physics\Physics.h"
#include "..\Common\Logger.h"
#include "../Core/Level.h"

struct CCButtonPress : public CodeComponent
{
	bool colliding = false;
	bool mBooped = false;

	BaseObject *Block = nullptr, *Exit = nullptr;
	virtual void Start() {
		Block = Level::Instance()->iFindObjectWithName("BlockDoor");
		Exit = Level::Instance()->iFindObjectWithName("ExitWall");
	}

	virtual void OnCollision(Collider& _col, Collider& _other, float _time)
	{
		if (!colliding && _other.mColliderType != Collider::eCOLLIDER_Plane)
		{
			colliding = true;
			//SystemLogger::GetLog() << "BUTTON PRESS";
			vec4f norm = ((ButtonCollider*)&_col)->mPushNormal;
			AABB aabb1(((ButtonCollider*)&_col)->mMin, ((ButtonCollider*)&_col)->mMax);
			vec4f tForce = norm * (norm * _other.mTotalForce);
			vec4f vel = norm * (norm * _other.mVelocity);
			vec4f accel = norm * (norm * _other.mAcceleration);
			if(tForce * norm < 0 && vel * norm < 0 && accel * norm < 0)
			{
				_col.mTotalForce += tForce;
				_col.mVelocity += vel;
				_col.mAcceleration += vel / _time;
				if (!mBooped) {
					Block->GetTransform().SetMatrix(Math::MatrixTranslation(0, -1, 0));
					Exit->GetTransform().SetMatrix(Math::MatrixTranslation(0, 2, 0));
					mBooped = true;
				}
			}
		}
		else
			colliding = false;
	}
};