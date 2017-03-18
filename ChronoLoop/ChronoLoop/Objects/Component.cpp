//#include "stdafx.h"
#include "Component.h"
#include "BaseObject.h"
#include "..\Messager\Messager.h"
#include "../Rendering/renderer.h"

namespace Epoch
{
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
		if (_id < 0 || _id > mSFX[ePlayLoop].size() - 1 || mSFX[ePlayLoop].size() == 0)
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
		if (_id < 0 || _id > mSFX[ePauseLoop].size() - 1 || mSFX[eResumeLoop].size() - 1 || mSFX[eResumeLoop].size() == 0 || mSFX[ePauseLoop].size() == 0)
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
		if (_id < 0 || _id > mSFX[eStopLoop].size() - 1 || mSFX[eStopLoop].size() == 0)
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
		//TODO: DOUBLE CHECK THIS
		if (_id < 0 || _id > mSFX[ePlaySFX].size() - 1 || mSFX[ePlaySFX].size() == 0)
			return;

		const vec4f * pos = GetTransform().GetPosition();
		m_LocEvent* evnt = new m_LocEvent(mSFX[ePlaySFX][_id], pos);
		Message* msg = new Message(msgTypes::mSound, soundMsg::MAKEEVENT_Loc, 0, false, (void*)evnt);
		Messager::Instance().SendInMessage(msg);
	}
	void Emitter::PlaySFX(int _id, const vec4f* _pos)
	{
		if (_id < 0 || _id > mSFX[ePlaySFX].size() - 1 || mSFX[ePlaySFX].size() == 0)
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

	vec3f Collider::GetPos()
	{
		return mObject->GetTransform().GetMatrix().fourth;
	}

	void Collider::SetPos(const vec3f& _newPos)
	{
		mObject->GetTransform().GetMatrix().fourth = vec4f(_newPos);
	}

	//MeshCollider::MeshCollider(BaseObject* _obj, bool _move, vec3f _gravity, float _mass, float _elasticity, float _staticFriction, float _kineticFriction, float _drag, char * _path)
	//{
	//	mObject = _obj;
	//	mType = eCOMPONENT_COLLIDER;
	//	mColliderType = eCOLLIDER_Mesh;
	//	mGravity = _gravity;
	//	mVelocity = vec3f(0.0f, 0.0f, 0.0f, 1.0f);
	//	mAcceleration = vec3f(0.0f, 0.0f, 0.0f, 1.0f);
	//	mTotalForce = mGravity;
	//	mImpulsiveForce = vec3f(0.0f, 0.0f, 0.0f, 1.0f);
	//	mShouldMove = _move;
	//	mMass = _mass;
	//	mDrag = _drag;
	//	if (mMass == 0)
	//		mInvMass = 0;
	//	else
	//		mInvMass = 1 / mMass;
	//	mWeight = mGravity * mMass;
	//	mRewind = false;
	//	mElasticity = _elasticity;
	//	mStaticFriction = _staticFriction;
	//	mKineticFriction = _kineticFriction;
	//	mMesh = &Mesh(_path);
	//}

	SphereCollider::SphereCollider(BaseObject* _obj, bool _move, bool _trigger, vec3f _gravity, float _mass, float _elasticity, float _staticFriction, float _kineticFriction, float _drag, float _radius)
	{
		mObject = _obj;
		mType = eCOMPONENT_COLLIDER;
		mColliderType = eCOLLIDER_Sphere;
		mRadius = _radius;
		mShouldMove = _move;
		mIsTrigger = _trigger;

		mGravity = _gravity;
		mVelocity;// = vec3f(0.0f, 0.0f, 0.0f);
		mAcceleration = vec3f(0.0f, 0.0f, 0.0f);
		mTotalForce = mGravity;
		mImpulsiveForce = vec3f(0.0f, 0.0f, 0.0f);
		mMass = _mass;
		if (mMass == 0)
			mInvMass = 0;
		else
			mInvMass = 1 / mMass;
		mWeight = mGravity * mMass;
		mElasticity = _elasticity;
		mStaticFriction = _staticFriction;
		mKineticFriction = _kineticFriction;
		mRHO = 1;
		mDrag = _drag;
		mArea = 4 * DirectX::XM_PI * powf(mRadius, 2.0f);
		mDragForce = mVelocity * (-0.5f * mRHO * mVelocity.Magnitude() * mDrag * mArea);
	}

	CubeCollider::CubeCollider(BaseObject* _obj, bool _move, bool _trigger, vec3f _gravity, float _mass, float _elasticity, float _staticFriction, float _kineticFriction, float _drag, vec3f _min, vec3f _max)
	{
		mObject = _obj;
		mType = eCOMPONENT_COLLIDER;
		mColliderType = eCOLLIDER_Cube;
		mMinOffset = _min;
		mMin = _min + mObject->GetTransform().GetMatrix().fourth;
		mMaxOffset = _max;
		mMax = _max + mObject->GetTransform().GetMatrix().fourth;
		mIsTrigger = _trigger;
		mShouldMove = _move;

		mGravity = _gravity;
		mVelocity = vec3f(0.0f, 0.0f, 0.0f);
		mAcceleration = vec3f(0.0f, 0.0f, 0.0f);
		mTotalForce = mGravity;
		mImpulsiveForce = vec3f(0.0f, 0.0f, 0.0f);
		mMass = _mass;
		if (mMass == 0)
			mInvMass = 0;
		else
			mInvMass = 1 / mMass;
		mWeight = mGravity * mMass;
		mElasticity = _elasticity;
		mStaticFriction = _staticFriction;
		mKineticFriction = _kineticFriction;
		mRHO = 1;
		mDrag = _drag;
		float a = (mMaxOffset - mMinOffset) * vec3f(0, 0, 1);
		float b = (mMaxOffset - mMinOffset) * vec3f(0, 1, 0);
		float c = (mMaxOffset - mMinOffset) * vec3f(1, 0, 0);
		mArea = (2 * (a * b)) + (2 * (b * c)) + (2 * (a * c));
		mDragForce = mVelocity * (-0.5f * mRHO * mVelocity.Magnitude() * mDrag * mArea);
		mShape = new RenderShape("../Resources/UnitCube.obj", true, ePS_TEXTURED, eVS_TEXTURED, eGS_PosNormTex);
		mShape->GetContext().mRasterState = eRS_WIREFRAME;
	}

	void CubeCollider::Update() {
		//if (mNode == nullptr) {
		//	mNode = Renderer::Instance()->AddNode(mShape);
		//}
		//vec3f size = mMax - mMin;
		//matrix4 pos = matrix4::CreateScale(size.x, size.y, size.z);
		//pos.Position = (mMax - mMin) / 2 + mMin;
		//mNode->data = pos;
	}

	void CubeCollider::Destroy()
	{
		delete mNode;
		delete mShape;
	}

	void CubeCollider::SetPos(const vec3f& _newPos)
	{
		mObject->GetTransform().GetMatrix().fourth = _newPos;
		mMin = mMinOffset + _newPos;
		mMax = mMaxOffset + _newPos;
	}

	OrientedCubeCollider::OrientedCubeCollider(BaseObject * _obj, bool _move, bool _trigger, vec3f _gravity, float _mass, float _elasticity, float _staticFriction, float _kineticFriction, float _drag, float _xRadius, float _yRadius, float _zRadius)
	{
		mObject = _obj;
		mType = eCOMPONENT_COLLIDER;
		mColliderType = eCOLLIDER_OrientedCube;
		mAxis[0] = mObject->GetTransform().GetMatrix().first;
		mAxis[1] = mObject->GetTransform().GetMatrix().second;
		mAxis[2] = -mObject->GetTransform().GetMatrix().third;
		mShouldMove = _move;
		mIsTrigger = _trigger;

		mGravity = _gravity;
		mVelocity = vec3f(0.0f, 0.0f, 0.0f);
		mAcceleration = vec3f(0.0f, 0.0f, 0.0f);
		mTotalForce = mGravity;
		mImpulsiveForce = vec3f(0.0f, 0.0f, 0.0f);
		mMass = _mass;
		if (mMass == 0)
			mInvMass = 0;
		else
			mInvMass = 1 / mMass;
		mWeight = mGravity * mMass;
		mElasticity = _elasticity;
		mStaticFriction = _staticFriction;
		mKineticFriction = _kineticFriction;
		mRHO = 1;
		mDrag = _drag;
		mWidth = _xRadius;
		mHeight = _yRadius;
		mDepth = _zRadius;

		vec3f maxOffset, minOffset, Center = GetPos();
		maxOffset.x = Center.x + mWidth;
		maxOffset.y = Center.y + mHeight;
		maxOffset.z = Center.z + mDepth;
		minOffset.x = Center.x - mWidth;
		minOffset.y = Center.y - mHeight;
		minOffset.z = Center.z - mDepth;
		float a = (maxOffset - minOffset) * vec3f(0, 0, 1);
		float b = (maxOffset - minOffset) * vec3f(0, 1, 0);
		float c = (maxOffset - minOffset) * vec3f(1, 0, 0);
		mArea = (2 * (a * b)) + (2 * (b * c)) + (2 * (a * c));
		mDragForce = mVelocity * (-0.5f * mRHO * mVelocity.Magnitude() * mDrag * mArea);
	}

	void Epoch::OrientedCubeCollider::SetPos(const vec3f & _newPos)
	{
		mObject->GetTransform().GetMatrix().fourth = _newPos;
		mAxis[0] = mObject->GetTransform().GetMatrix().first;
		mAxis[1] = mObject->GetTransform().GetMatrix().second;
		mAxis[2] = -mObject->GetTransform().GetMatrix().third;
	}

	PlaneCollider::PlaneCollider(BaseObject* _obj, bool _trigger, float _staticFriction, float _kineticFriction, float _offset, vec3f _norm)
	{
		mObject = _obj;
		mType = eCOMPONENT_COLLIDER;
		mColliderType = eCOLLIDER_Plane;
		mOffset = _offset;
		mNormal = _norm;
		mShouldMove = false;
		mIsTrigger = _trigger;

		mStaticFriction = _staticFriction;
		mKineticFriction = _kineticFriction;
	}

	ButtonCollider::ButtonCollider(BaseObject* _obj, vec3f _min, vec3f _max, float _mass, float _normForce, vec3f _pushNormal)
	{
		mObject = _obj;
		mType = eCOMPONENT_COLLIDER;
		mColliderType = eCOLLIDER_Button;
		mMinOffset = _min;
		mMin = _min + mObject->GetTransform().GetMatrix().fourth;
		mMaxOffset = _max;
		mMax = _max + mObject->GetTransform().GetMatrix().fourth;
		mPushNormal = _pushNormal;
		mUpperBound.mNormal = mPushNormal;
		mUpperBound.mOffset = mMax * mPushNormal;
		mLowerBound.mNormal = mPushNormal;
		mLowerBound.mOffset = (mMin * mPushNormal) - .1f;
		mShouldMove = true;
		mIsTrigger = false;

		mGravity = _pushNormal * _normForce;
		mVelocity = vec3f(0.0f, 0.0f, 0.0f);
		mAcceleration = vec3f(0.0f, 0.0f, 0.0f);
		mTotalForce = mGravity * _mass;
		mImpulsiveForce = vec3f(0.0f, 0.0f, 0.0f);
		
		mMass = _mass;
		if (mMass == 0)
			mInvMass = 0;
		else
			mInvMass = 1 / mMass;
		mWeight = mGravity * mMass;
		mElasticity = 0;
		mShape = new RenderShape("../Resources/UnitCube.obj", true, ePS_TEXTURED, eVS_TEXTURED, eGS_PosNormTex);
		mShape->GetContext().mRasterState = eRS_WIREFRAME;
	}

	ControllerCollider::ControllerCollider(BaseObject* _obj, vec3f _min, vec3f _max, bool _left)
	{
		mObject = _obj;
		mLeft = _left;
		mMinOffset = _min;
		mMin = _min + mObject->GetTransform().GetMatrix().fourth;
		mMaxOffset = _max;
		mMax = _max + mObject->GetTransform().GetMatrix().fourth;
		mShouldMove = false;
		mIsTrigger = false;

		mMass = 2;
		mForces = vec3f(0, -1, 0);
		mGravity = vec3f(0, -2.0f, 0);
		mType = eCOMPONENT_COLLIDER;
		mColliderType = eCOLLIDER_Controller;
		mTotalForce = { 0, -2, 0 };
		mAcceleration = { 0, -2, 0 };
		mShape = new RenderShape("../Resources/UnitCube.obj", true, ePS_TEXTURED, eVS_TEXTURED, eGS_PosNormTex);
		mShape->GetContext().mRasterState = eRS_WIREFRAME;
	}

}