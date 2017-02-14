#pragma once
#include "..\Common\Math.h"
class BaseObject;
class Mesh;

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
	bool isEnabled() { return mIsEnabled; };
	void Disable() { mIsEnabled = false; };
	void Enable() { mIsEnabled = true; };
	virtual void Update() {};
	virtual void Destroy() {};
	//matrix4 GetMatrix() { return object->GetTransform().GetMatrix(); };
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

class Collider : public Component
{
public:
	bool mIsCube, mIsSphere, mIsPlane, mIsMesh, mShouldMove;
	vec4f mCubeMax, mCubeMin, mPlaneNorm, mVelocity, mAcceleration, mTotalForce, mImpulsiveForce, mGravity;
	float mPlaneOffset, mSphereRadius, mMass, mRadius, mElasticity;
	Mesh* mMesh;

	vec4f GetPos();
	void SetPos(vec4f _newPos);
	//void CalcObjAcceleration(vec4f& _force);
	//void CalcObjVelocity(vec4f& _force, float _dt);
	//void CalcObjPosition(vec4f& _force, float _dt);
};