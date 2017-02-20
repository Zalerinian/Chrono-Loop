#pragma once
#include "../Objects/Component.h"

struct CodeComponent : public Component {
	CodeComponent() {
		mType = eCOMPONENT_CODE;
	}

	// This function can't be overridden, as it is required for a component to be instantiable.
	void Destroy() {
		OnDestroy();
	}
	 
	virtual ~CodeComponent() {}

	virtual void Start() {}

	virtual void Awake() {}

	virtual void Update() {}

	virtual void OnDisable() {}

	virtual void OnEnable() {}

	virtual void OnDestroy() {}
};