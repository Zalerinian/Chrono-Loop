#pragma once
#include "..\Common\Math.h"
class BaseObject;
class Transform;

enum ComponentType
{
	Code,
	PhysicsCollider,
	AudioEmitter,
	AudioListener,
	UI
};

class Component
{
	friend class Physics;
protected:
	bool mIsEnabled;

public:
	BaseObject* object = nullptr;
	ComponentType type;
	bool isEnabled() { return mIsEnabled; }
	void Disable() { mIsEnabled = false; }
	void Enable() { mIsEnabled = true; }
	virtual void Update() {}
	virtual void Destroy() {}
	Transform& GetTransform();
	Transform& GetTransform() const;
};

class Listener : public Component
{
public:
	//void Update();
};

class Emitter : public Component
{
public:
	void Play();
	void Pause();
	void Stop();

	void Update();
	void Destroy();
};

class Collider : public Component
{
public:
	bool mIsCube, mIsSphere, mIsPlane, mShouldMove;
	vec4f mCubeMax, mCubeMin, mPlaneNorm, mVelocity, mAcceleration;
	float mPlaneOffset, mSphereRadius, mMass, mRadius;

	vec4f GetPos();
	void SetPos(vec4f _newPos);
	//void CalcObjAcceleration(vec4f& _force);
	//void CalcObjVelocity(vec4f& _force, float _dt);
	//void CalcObjPosition(vec4f& _force, float _dt);
};
