#pragma once
class BaseObject;
class Transform;
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
	bool isEnabled() { return isEnable; }
	void Disable() { isEnable = false; }
	void Enable() { isEnable = true; }
	void Update();
	void Destroy();
	Transform& GetTransform() const;
};

class Listener : public Component
{
public:

	void Update();
	void Destroy();

};

class Emitter : public Component
{
public:
	void MakeEvent(long _id);

	void Update();
	void Destroy();
};

/*
business entity- gmail, twitter, facebook, steam account
art, audio, marketing, designer students ?

first initial last name, password lower case
gdserv.fullsail.com:8080
install doc, follow it




*/










