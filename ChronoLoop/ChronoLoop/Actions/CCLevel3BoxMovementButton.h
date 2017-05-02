#pragma once
#include "../Actions/CodeComponent.hpp"
#include "../Common/Interpolator.h"
#include "../Core/LevelManager.h"
#include "../Common/Settings.h"

namespace Epoch {
	struct CCLevel3BoxMovementButton : public CodeComponent {
		bool isColliding = false;
		virtual void OnCollision(Collider& _col, Collider& _other, float _time) {
			if (!Settings::GetInstance().GetBool("PauseMenuUp")) {
				if (!isColliding && _other.mColliderType != Collider::eCOLLIDER_Plane && ((Component*)&_other)->GetBaseObject()->GetName() != "Buttonstand") {
					isColliding = true;
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
			//isColliding = false;
		}
		bool GetisColliding() { return isColliding; }
		void SetisColliding(bool _set) { isColliding = _set; }
	};
}