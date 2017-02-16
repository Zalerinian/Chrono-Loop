#pragma once
#include "..\Common\Math.h"
#include "..\Rendering\Mesh.h"
class BaseObject;
//class Mesh;

enum ComponentType
{
	eCOMPONENT_Code,
	eCOMPONENT_PhysicsCollider,
	eCOMPONENT_AudioEmitter,
	eCOMPONENT_AudioListener,
	eCOMPONENT_UI
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
	enum ColliderType
	{
		eCOLLIDER_Mesh,
		eCOLLIDER_Sphere,
		eCOLLIDER_Cube,
		eCOLLIDER_Plane
	};

	bool mShouldMove;
	vec4f mVelocity, mAcceleration, mTotalForce, mImpulsiveForce, mGravity;
	float mMass, mElasticity;
	ColliderType mType;

	vec4f AddForce(vec4f _force) { mTotalForce += _force; return mTotalForce; };
	virtual vec4f GetPos();
	virtual void SetPos(vec4f _newPos);
};

class MeshCollider : public Collider
{
public:
	MeshCollider(bool _move, vec4f _gravity, float _mass, float _elasticity, char* _path);
	Mesh* mMesh;
};

class SphereCollider : public Collider
{
public:
	SphereCollider(bool _move, vec4f _gravity, float _mass, float _elasticity, float _radius);
	float mRadius;
};

class CubeCollider : public Collider
{
public:
	CubeCollider(bool _move, vec4f _gravity, float _mass, float _elasticity, vec4f _min, vec4f _max);
	vec4f mMin, mMax, mMinOffset, mMaxOffset;

	void SetPos(vec4f _newPos);
};

class PlaneCollider : public Collider
{
public:
	PlaneCollider(bool _move, vec4f _gravity, float _mass, float _elasticity, float _offset, vec4f _norm);
	vec4f mNormal;
	float mOffset;
};