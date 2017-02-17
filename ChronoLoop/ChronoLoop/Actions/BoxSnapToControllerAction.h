#pragma once
#include "../Objects/Action.h"

struct BoxSnapToControllerAction : public Action {
	bool mHeld = false;
	bool mHeldLeft = false;

	BoxSnapToControllerAction(BaseObject* _parent);
	virtual void Update() override;
	virtual void SnapToController(bool left);
	virtual void ReleaseCube();
};