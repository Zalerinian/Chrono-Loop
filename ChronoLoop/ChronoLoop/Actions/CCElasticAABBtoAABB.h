#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "../Common/Logger.h"

namespace Epoch
{

	struct CCElasticAABBtoAABB : public CodeComponent
	{
		vec4f norm = { 0,0,0,0 };
		bool colliding = false;
		virtual void OnCollision(Collider& _col, Collider& _other, float _time)
		{
			if (!colliding && _other.mColliderType == Collider::eCOLLIDER_Cube)
			{
				colliding = true;

				vec4f max = ((CubeCollider*)&_other)->mMax;
				vec4f min = ((CubeCollider*)&_other)->mMin;
				vec4f otherCenter = (max + min) * 0.5f;
				vec4f center = _col.GetPos();
				vec4f V = center - otherCenter;

				//vec4f norm(0,0,0,0);
				if (center.y >= max.y)
				{
					norm = { 0,1,0,0 };

					vec4f rest = _other.mVelocity;
					if (fabsf(rest.y) < 1)
					{
						if (fabsf(_col.mVelocity.y) < 1)
						{
							_col.mVelocity.y = 0;
							_col.mAcceleration.y = 0;
							_col.mTotalForce.y = 0;
							_col.mDragForce.y = 0;
						}
						else
						{
							vec4f normalVel = norm * -(_col.mVelocity * norm);
							_col.mVelocity += normalVel * (1 + _col.mElasticity);
						}

						Physics::Instance()->CalcFriction(_col, ((CubeCollider*)&_other)->mMax, _other.mStaticFriction, _other.mKineticFriction);

						if (_col.mColliderType == Collider::eCOLLIDER_Cube)
						{
							if (((CubeCollider*)&_col)->mMin.y < ((CubeCollider*)&_other)->mMax.y)
								_col.SetPos(vec4f(center.x, ((CubeCollider*)&_other)->mMax.y + fabsf(((CubeCollider*)&_col)->mMinOffset.y), center.z, 1));
						}
						else if (_col.mColliderType == Collider::eCOLLIDER_Sphere)
						{
							if (center.y - ((SphereCollider*)&_col)->mRadius < ((CubeCollider*)&_other)->mMax.y)
								_col.SetPos(vec4f(center.x, ((CubeCollider*)&_other)->mMax.y + fabsf(((SphereCollider*)&_col)->mRadius), center.z, 1));
						}
						
						return;
					}
				}
				else if (center.y <= min.y)
				{
					vec4f rest = _col.mVelocity;
					if (fabsf(rest.y) < 1)
						return;

					norm = { 0,-1,0,0 };
				}
				else if ((center.x <= max.x && center.x >= min.x) && V.z > 0)
					norm = { 0,0,1,0 };
				else if ((center.x <= max.x && center.x >= min.x) && V.z < 0)
					norm = { 0,0,-1,0 };
				else if ((center.z <= max.z && center.z >= min.z) && V.x > 0)
					norm = { 1,0,0,0 };
				else if ((center.z <= max.z && center.z >= min.z) && V.x < 0)
					norm = { -1,0,0,0 };


				//float avgElasticity = (_col.mElasticity + _other.mElasticity) / 2;
				//_col.mVelocity = norm * (1 + avgElasticity);

				//TODO: Fix velocivty in the collision normal direction
				float flip = -_col.mVelocity * norm;
				vec4f prev = _col.mVelocity;
				prev += norm * flip;
				_col.mVelocity = { 0,0,0 };
				float avgElasticity = (_col.mElasticity + _other.mElasticity) / 2;
				_col.mVelocity = norm * (1 + avgElasticity) + prev;

				//float flip = -_col.mVelocity * norm;
				//vec4f prev = _col.mVelocity;
				//prev += norm * flip;
				//_col.mVelocity = { 0,0,0,1 };
				//float avgElasticity = (_col.mElasticity + _other.mElasticity) / 2;
				//_col.mVelocity = norm * (flip * (1 - avgElasticity)) + prev;
			}
			else
				colliding = false;
		}
	};

} // Epoch Namespace