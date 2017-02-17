//#include "stdafx.h"
#include "Component.h"
#include "BaseObject.h"

// 0 is reserved for the player.
unsigned short Component::mComponentCount = 0;

Component::Component()
{
	mComponentId = Component::mComponentCount++;
}
Component::~Component()
{
	
}
//Base Component---------------------------------------------
#pragma region Base Component
void Component::GetMatrix(matrix4& _m)
{
	//_m = object->transform.m_matrix4;
}

<<<<<<< .mine

Transform& Component::GetTransform() { return object->GetTransform(); }
Transform& Component::GetTransform() const { return object->GetTransform(); }
=======



>>>>>>> .theirs
#pragma endregion
//------------------------------------------------------------
//Listener----------------------------------------------------
#pragma region Listener

//void Listener::Update(){}











#pragma endregion
//------------------------------------------------------------
//Emitter-----------------------------------------------------
#pragma region Emitter

void Emitter::Update()
{

}

void Emitter::Play()
{

}

void Emitter::Pause()
{

}

void Emitter::Stop()
{

}

void Emitter::Destroy()
{

}
#pragma endregion
//------------------------------------------------------------

void Collider::Update() {}

void Collider::Destroy() {}

vec4f Collider::GetPos() {
	return mObject->GetTransform().GetMatrix().fourth;
}

void Collider::SetPos(vec4f _newPos) {
	mObject->GetTransform().GetMatrix().fourth = _newPos;
}

MeshCollider::MeshCollider(bool _move, vec4f _gravity, float _mass, float _elasticity, float _friction, char * _path) {
	mType = eCOMPONENT_COLLIDER;
	mColliderType = eCOLLIDER_Mesh;
	mGravity = _gravity;
	mVelocity = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mAcceleration = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mTotalForce = mGravity;
	mImpulsiveForce = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mShouldMove = _move;
	mMass = _mass;
	mRewind = false;
	mElasticity = _elasticity;
	mFriction = _friction;
	mMesh = &Mesh(_path);
}

SphereCollider::SphereCollider(bool _move, vec4f _gravity, float _mass, float _elasticity, float _friction, float _radius) {
	mType = eCOMPONENT_COLLIDER;
	mColliderType = eCOLLIDER_Sphere;
	mGravity = _gravity;
	mVelocity = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mAcceleration = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mTotalForce = mGravity;
	mImpulsiveForce = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mShouldMove = _move;
	mMass = _mass;
	mRewind = false;
	mElasticity = _elasticity;
	mFriction = _friction;
	mRadius = _radius;
}

CubeCollider::CubeCollider(bool _move, vec4f _gravity, float _mass, float _elasticity, float _friction, vec4f _min, vec4f _max) {
	mType = eCOMPONENT_COLLIDER;
	mColliderType = eCOLLIDER_Cube;
	mGravity = _gravity;
	mVelocity = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mAcceleration = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mTotalForce = mGravity;
	mImpulsiveForce = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mShouldMove = _move;
	mColliding = false;
	mRewind = false;
	mMass = _mass;
	mElasticity = _elasticity;
	mFriction = _friction;
	mMin = _min;
	mMinOffset = mMin;
	mMax = _max;
	mMaxOffset = mMax;
}

void CubeCollider::SetPos(vec4f _newPos) {
	mObject->GetTransform().GetMatrix().fourth = _newPos;
	mMin = _newPos + mMinOffset;
	mMax = _newPos + mMaxOffset;
}

PlaneCollider::PlaneCollider(bool _move, vec4f _gravity, float _mass, float _elasticity, float _friction, float _offset, vec4f _norm) {
	mType = eCOMPONENT_COLLIDER;
	mColliderType = eCOLLIDER_Plane;
	mGravity = _gravity;
	mVelocity = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mAcceleration = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mTotalForce = mGravity;
	mImpulsiveForce = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mShouldMove = _move;
	mRewind = false;
	mMass = _mass;
	mElasticity = _elasticity;
	mFriction = _friction;
	mOffset = _offset;
	mNormal = _norm;
}

