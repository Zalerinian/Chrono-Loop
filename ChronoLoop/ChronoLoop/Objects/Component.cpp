//#include "stdafx.h"
#include "Component.h"
#include "BaseObject.h"
#include "..\Messager\Messager.h"


// 0 is reserved for the player.
unsigned short Component::mComponentCount = 0;

Component::Component()
{
	mComponentId = Component::mComponentCount++;
}
Component::Component(ComponentType _cType)
{
	mType = _cType;
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
Transform& Component::GetTransform() { return mObject->GetTransform(); }
Transform& Component::GetTransform() const { return mObject->GetTransform(); }

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

void Emitter::Play(int _id)
{
	if (_id < 0 || _id > mSFX[ePlayLoop].size() - 1)
		return;
	if (mIsSounds[_id].first)
		return;
	m_Event* evnt = new m_Event(mSFX[ePlayLoop][_id], this);
	Message* msg = new Message(msgTypes::mSound, soundMsg::MAKEEVENT_Event, 0, false, (void*)evnt);
	Messager::Instance().SendInMessage(msg);
	mIsSounds[_id].first = true;
}

void Emitter::Pause(int _id)
{
	if (_id < 0 || _id > mSFX[ePauseLoop].size() - 1 || mSFX[eResumeLoop].size() - 1)
		return;

	if (mIsSounds[_id].second)
	{
		m_Event* evnt = new m_Event(mSFX[eResumeLoop][_id], this);
		Message* msg = new Message(msgTypes::mSound, soundMsg::MAKEEVENT_Event, 0, false, (void*)evnt);
		Messager::Instance().SendInMessage(msg);
		mIsSounds[_id].second = false;
	}
	else
	{
		m_Event* evnt = new m_Event(mSFX[ePauseLoop][_id], this);
		Message* msg = new Message(msgTypes::mSound, soundMsg::MAKEEVENT_Event, 0, false, (void*)evnt);
		Messager::Instance().SendInMessage(msg);
		mIsSounds[_id].second = true;
	}
}

void Emitter::Stop(int _id)
{
	if (_id < 0 || _id > mSFX[eStopLoop].size() - 1)
		return;
	if (!mIsSounds[_id].first)
		return;
	m_Event* evnt = new m_Event(mSFX[eStopLoop][_id], this);
	Message* msg = new Message(msgTypes::mSound, soundMsg::MAKEEVENT_Event, 0, false, (void*)evnt);
	Messager::Instance().SendInMessage(msg);
	mIsSounds[_id].first = false;

}

void Emitter::PlaySFX(int _id)
{
	if (_id < 0 || _id > mSFX[ePlaySFX].size() - 1)
		return;

	const vec4f * pos = GetTransform().GetPosition();
	m_LocEvent* evnt = new m_LocEvent(mSFX[ePlaySFX][_id], pos);
	Message* msg = new Message(msgTypes::mSound, soundMsg::MAKEEVENT_Loc, 0, false, (void*)evnt);
	Messager::Instance().SendInMessage(msg);
}
void Emitter::PlaySFX(int _id, const vec4f* _pos)
{
	if (_id < 0 || _id > mSFX[ePlaySFX].size() - 1)
		return;

	const vec4f * pos = _pos;
	m_LocEvent* evnt = new m_LocEvent(mSFX[ePlaySFX][_id], pos);
	Message* msg = new Message(msgTypes::mSound, soundMsg::MAKEEVENT_Loc, 0, false, (void*)evnt);
	Messager::Instance().SendInMessage(msg);
}

void Emitter::AddSoundEvent(sfxTypes _type, int64_t _event)
{
	switch (_type)
	{
	case sfxTypes::ePlayLoop:
	{
		mSFX[_type].push_back(_event);
	}
	break;
	case sfxTypes::ePauseLoop:
	{
		mSFX[_type].push_back(_event);
	}
	break;
	case sfxTypes::eResumeLoop:
	{
		mSFX[_type].push_back(_event);
	}
	break;
	case sfxTypes::eStopLoop:
	{
		mSFX[_type].push_back(_event);
	}
	break;
	case sfxTypes::ePlaySFX:
	{
		mSFX[_type].push_back(_event);
	}
	break;
	}

	if (_type != sfxTypes::ePlaySFX)
	{
		mIsSounds.push_back(std::pair<bool, bool>(false, false));
	}
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
	mMass = 2;
	mForces = vec4f(0, -1, 0, 0);
	mGravity = vec4f(0,-2.0f,0,0);
	mType = eCOMPONENT_COLLIDER;
	mColliderType = eCOLLIDER_Controller;
	mTotalForce = { 0,-2,0,0 };
	mAcceleration = { 0,-2,0,0 };
	mShouldMove = false;
	mRewind = false;
	mMinOffset = _min;
	mMin = _min + mObject->GetTransform().GetMatrix().fourth;
	mMaxOffset = _max;
	mMax = _max + mObject->GetTransform().GetMatrix().fourth;
}