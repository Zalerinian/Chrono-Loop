#pragma once
class BaseObject;
struct matrix4;


enum ComponentType
{
	Unknown,
	Code,
	Collider,
	AudioEmitter,
	AudioListener,
	UI
};
class Component
{
protected:
	bool isEnable;
	ComponentType type;

	BaseObject* object = nullptr;
public:
	bool isEnabled() { return isEnable; };
	void Disable() { isEnable = false; };
	void Enable() { isEnable = true; };
	//virtual void Update() = 0;
	//virtual void Destroy() = 0;
	void GetMatrix(matrix4& _m);
};

class Listener : Component
{
public:

	//void Update();
};

class Emitter : Component
{
public:
	void Play();
	void Pause();
	void Stop();

	void Update();
	void Destroy();
};

/*
business entity- gmail, twitter, facebook, steam account
art, audio, marketing, designer students ?

first initial last name, password lower case
gdserve.fullsail.com:8080
install doc, follow it




*/










