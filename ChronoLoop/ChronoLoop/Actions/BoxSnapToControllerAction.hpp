#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"
#include "../Common/Logger.h"
#include <unordered_set>
#include "../Common/Settings.h"
#include "../Input/VrInputManager.h"


namespace Epoch {
	
	

	struct BoxSnapToControllerAction : public CodeComponent {
		ControllerType mControllerRole = eControllerType_Primary;
		static std::vector<BoxSnapToControllerAction*> mHands;
		bool mHeld = false;
		ControllerCollider* mCollider;
		Collider* mPickUp = nullptr;
		InputTimeline::InputNode* mInput = nullptr;

		virtual void Start();
		virtual void OnDestroy();
		virtual void Update() override;
		void CheckIfBoxAlreadyHeld();
		void CheckIfBoxShouldMove();
		virtual void SomethingtoController();
		virtual void ReleaseObject();
	};

	

} // Epoch Namespace