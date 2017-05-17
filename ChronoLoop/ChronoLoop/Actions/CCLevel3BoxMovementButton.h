#pragma once
#include "../Actions/CodeComponent.hpp"
#include "../Common/Interpolator.h"
#include "../Core/LevelManager.h"
#include "../Common/Settings.h"

namespace Epoch {
	struct CCLevel3BoxMovementButton : public CodeComponent {
		bool isColliding = false;
		unsigned int mCount = 0;
		virtual void OnCollision(Collider& _col, Collider& _other, float _time) {
			if (!Settings::GetInstance().GetBool("PauseMenuUp")) {
				if (!isColliding && _other.mColliderType != Collider::eCOLLIDER_Plane && ((Component*)&_other)->GetBaseObject()->GetName() != "Buttonstand") {
					isColliding = true;
					mCount = 0;
					ButtonCollider* butCol = (ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);

					if (_other.mVelocity * butCol->mPushNormal < .1f)
						butCol->mVelocity = vec3f(0, 0, 0);

					vec3f norm = ((ButtonCollider*)&_col)->mPushNormal;
					vec3f tForce = norm * (norm * _other.mTotalForce);
					vec3f vel = norm * (norm * _other.mVelocity);
					vec3f accel = norm * (norm * _other.mAcceleration);
					if (tForce * norm < 0 && vel * norm < 0 && accel * norm < 0) {
						_col.mTotalForce = tForce;
						_col.mVelocity = vel;
						_col.mAcceleration = vel / _time;
					}
				}
			}
		}
		virtual void Update() {
			if(isColliding && mCount < 6)
			{
				mCount++;
			}
			else
			{
			isColliding = false;	
			}

		}
		bool GetisColliding() { return isColliding; }
		void SetisColliding(bool _set) { isColliding = _set; }
	};
}