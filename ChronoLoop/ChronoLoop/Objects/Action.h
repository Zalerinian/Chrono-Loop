#pragma once

struct Action {
	virtual ~Action();
	virtual void Start();
	virtual void Awake();
	virtual void Update();
	virtual void OnDisable();
	virtual void OnEnable();
	virtual void OnDestroy();
};