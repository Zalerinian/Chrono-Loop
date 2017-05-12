#pragma once
#include "CodeComponent.hpp"
#include "../Objects/MeshComponent.h"
#include "../Objects/BaseObject.h"
#include "../Input/VRInputManager.h"

namespace Epoch {

	class CCCloneHeadCollider : public CodeComponent {
		Collider* Col = nullptr;
	private:

	public:
		virtual void Start() {
			Col = (Collider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);
		}
		virtual void Update() {
			if (!VRInputManager::GetInstance().IsVREnabled()) {
				return;
			}

			matrix4 mat = mObject->GetTransform().GetMatrix();
			if (Col)
				Col->SetPos(mat.Position);
		}

	};
	}
