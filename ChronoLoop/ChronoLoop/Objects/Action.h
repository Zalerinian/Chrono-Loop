#pragma once
class BaseObject;

struct Action {
	// TODO Consider changing Action.h/cpp files to .hpp

	// TODO maybe make a better method for getting the object and components for the code components.
	BaseObject* mObject;
	Action(BaseObject* _parent) : mObject(_parent) {}
	virtual ~Action();
	virtual void Start();
	virtual void Awake();
	virtual void Update();
	virtual void OnDisable();
	virtual void OnEnable();
	virtual void OnDestroy();
};