#pragma once
#include "../Objects/Component.h"
#include "..\Common\Logger.h"

namespace Epoch
{

	struct CodeComponent : public Component
	{
		CodeComponent()
		{
			mType = eCOMPONENT_CODE;
		}

		// This function can't be overridden, as it is required for a component to be instantiable.
		void Destroy()
		{
			OnDestroy();
		}

		virtual ~CodeComponent() {}

		virtual void Start() {}

		virtual void Awake() {}

		virtual void Update() {}

		virtual void OnTriggerEnter(Collider& _col1, Collider& _col2) {}

		virtual void OnCollision(Collider& _col1, Collider& _col2, float _time) {}

		virtual void OnDisable() {}

		virtual void OnEnable() {}

		virtual void OnDestroy() {}

		virtual void Disable() {
			mIsEnabled = false;
			this->OnDisable();
		}

		virtual void Enable() {
			mIsEnabled = true;
			this->OnEnable();
		}
	};

}