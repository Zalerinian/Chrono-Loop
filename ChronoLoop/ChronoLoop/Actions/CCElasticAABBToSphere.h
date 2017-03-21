#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "../Common/Logger.h"

namespace Epoch
{

	struct CCElasticAABBToSphere : public CodeComponent
	{
		bool colliding = false;
		virtual void OnCollision(Collider& _col, Collider& _other, float _time)
		{
			if (!colliding && _other.mColliderType == Collider::eCOLLIDER_Sphere)
			{
				colliding = true;

				float avgElasticity = (_col.mElasticity + _other.mElasticity) / 2;
				vec3f collisionNormal = (_col.GetPos() - _other.GetPos()).Normalize();

				vec3f rest = _col.mVelocity;
				float dir = (collisionNormal * vec3f(0, 1, 0));
				if ((dir < 0) && (fabsf(rest.x) < 0.1f && fabsf(rest.y) < 0.1f && fabsf(rest.z) < 0.1f))
					return;

				_col.mVelocity += collisionNormal * (1 + avgElasticity);
			}
			else
				colliding = false;
		}
	};

}