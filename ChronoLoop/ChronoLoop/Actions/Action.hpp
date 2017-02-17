#pragma once

class BaseObject;

struct Action {
protected:
	BaseObject* mObject;
	
public:
	Action(BaseObject* _parent) : mObject(_parent) {}

	virtual ~Action() {}
	
	virtual void Start() {}
	
	virtual void Awake() {}
	
	virtual void Update() {}
	
	virtual void OnDisable() {}
	
	virtual void OnEnable() {}
	
	virtual void OnDestroy() {}
};