#pragma once
class BaseObject;
struct matrix4;

enum ComponentType
{
	Unknown,
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
	BaseObject* object = nullptr;

public:
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
	bool mIsCube;
	bool mIsSphere;
	bool mIsPlane;
	vec4f mCubeMax;
	vec4f mCubeMin;
	vec4f mPlaneNorm;
	float mPlaneOffset;
	float mSphereRadius;
	float mMass, mRadius;
	vec4f mVelocity, mAcceleration;

	void CalcObjAcceleration(vec4f& _force);
	void CalcObjVelocity(vec4f& _force, float _dt);
	void CalcObjPosition(vec4f& _force, float _dt);
};