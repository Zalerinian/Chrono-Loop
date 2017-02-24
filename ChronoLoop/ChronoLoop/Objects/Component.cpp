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
	if (mMass == 0)
		mInvMass = 0;
	else
		mInvMass = 1 / mMass;
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
	if (mMass == 0)
		mInvMass = 0;
	else
		mInvMass = 1 / mMass;
	mRewind = false;
	mElasticity = _elasticity;
	mFriction = _friction;
	mRadius = _radius;
}

CubeCollider::CubeCollider(BaseObject* _obj, bool _move, vec4f _gravity, float _mass, float _elasticity, float _friction, vec4f _min, vec4f _max) {
	mObject = _obj;
	mType = eCOMPONENT_COLLIDER;
	mColliderType = eCOLLIDER_Cube;
	mGravity = _gravity;
	mVelocity = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mAcceleration = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mTotalForce = mGravity;
	mImpulsiveForce = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mShouldMove = _move;
	mRewind = false;
	mMass = _mass;
	if (mMass == 0)
		mInvMass = 0;
	else
		mInvMass = 1 / mMass;
	mElasticity = _elasticity;
	mFriction = _friction;
	mMinOffset = _min;
	mMin = _min + mObject->GetTransform().GetMatrix().fourth;
	mMaxOffset = _max;
	mMax = _max + mObject->GetTransform().GetMatrix().fourth;
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
	if (mMass == 0)
		mInvMass = 0;
	else
		mInvMass = 1 / mMass;
	mElasticity = _elasticity;
	mFriction = _friction;
	mOffset = _offset;
	mNormal = _norm;
}

ButtonCollider::ButtonCollider(BaseObject* _obj, vec4f _min, vec4f _max, float _mass, float _normForce, vec4f _pushNormal)
{
	mObject = _obj;
	mType = eCOMPONENT_COLLIDER;
	mColliderType = eCOLLIDER_Button;
	mGravity =  _pushNormal * _normForce;
	mVelocity = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mAcceleration = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mTotalForce = mGravity * _mass;
	mImpulsiveForce = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mShouldMove = true;
	mRewind = false;
	mMass = _mass;
	if (mMass == 0)
		mInvMass = 0;
	else
		mInvMass = 1 / mMass;
	mElasticity = 0;
	mFriction = 0;
	mMinOffset = _min;
	mMin = _min + mObject->GetTransform().GetMatrix().fourth;
	mMaxOffset = _max;
	mMax = _max + mObject->GetTransform().GetMatrix().fourth;
	mPushNormal = _pushNormal;
	mUpperBound.mNormal = mPushNormal;
	mUpperBound.mOffset = mMax * mPushNormal;
	mLowerBound.mNormal = mPushNormal;
	mLowerBound.mOffset = (mMin - mMax) * mPushNormal * 2;
}

ControllerCollider::ControllerCollider(BaseObject* _obj, vec4f _min, vec4f _max, bool _left)
{
	mObject = _obj;
	mLeft = _left;
	mType = eCOMPONENT_COLLIDER;
	mColliderType = eCOLLIDER_Controller;
	mShouldMove = false;
	mRewind = false;
	mMinOffset = _min;
	mMin = _min + mObject->GetTransform().GetMatrix().fourth;
	mMaxOffset = _max;
	mMax = _max + mObject->GetTransform().GetMatrix().fourth;
}