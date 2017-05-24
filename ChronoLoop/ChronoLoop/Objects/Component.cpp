//#include "stdafx.h"
#include "Component.h"
#include "BaseObject.h"
#include "../Rendering/renderer.h"
#include "../Sound/SoundEngine.h"
#include "../Common/Settings.h"

namespace Epoch
{

	unsigned short Component::mComponentCount = 0;

	Component::Component()
	{
		//SystemLogger::Debug() << mComponentCount << std::endl;
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

	void Listener::Destroy()
	{
		AudioWrapper::GetInstance().RemoveListener(this);
	}











#pragma endregion
	//------------------------------------------------------------
	//Emitter-----------------------------------------------------
#pragma region Audio

	void Emitter::Update()
	{

	}

	void Emitter::Destroy()
	{
		AudioWrapper::GetInstance().StopEmitter(this);
		AudioWrapper::GetInstance().RemoveEmitter(this);
	}

	void Emitter::SendEvent(uint64_t _event)
	{
		if (AudioWrapper::GetInstance().IsInitialized())
			AudioWrapper::GetInstance().MakeEvent(_event, this);
	}

	void AudioEmitter::AddEvent(EventType _et, uint64_t _event)
	{
		if (eNone == _et)
			return;

		mEvents[_et] = _event;
	}

	void AudioEmitter::StopAllSound()
	{
		AudioWrapper::GetInstance().StopEmitter(this);
	}

	void AudioEmitter::CallEvent(EventType _et)
	{
		if (_et != eNone && mEvents[_et])
			SendEvent(mEvents[_et]);
	}

	void SFXEmitter::SetEvent(uint64_t _event)
	{
		mEvent = _event;
	}
	void SFXEmitter::CallEvent(EventType _et)
	{
		SendEvent(mEvent);
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
		vec4f v(_newPos);
		mObject->GetTransform().GetMatrix().fourth = v;
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

	SphereCollider::SphereCollider(BaseObject* _obj, bool _move, bool _trigger, vec3f _gravity, float _mass, float _elasticity, float _staticFriction, float _kineticFriction, float _drag, float _radius, std::string _ColliderShape)
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
		mShape = new RenderShape(_ColliderShape.c_str(), true, ePS_TEXTURED, eVS_TEXTURED, eGS_PosNormTex);
		mShape->GetContext().mRasterState = eRS_WIREFRAME;
	}

	CubeCollider::CubeCollider(BaseObject* _obj, bool _move, bool _trigger, vec3f _gravity, float _mass, float _elasticity, float _staticFriction, float _kineticFriction, float _drag, vec3f _min, vec3f _max, std::string _ColliderShape)
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
		mShape = new RenderShape(_ColliderShape.c_str(), true, ePS_TEXTURED, eVS_TEXTURED, eGS_PosNormTex);
		mShape->GetContext().mRasterState = eRS_WIREFRAME;
	}

	void CubeCollider::Update()
	{
		if (Settings::GetInstance().GetBool("ShowColliders") || mShowCol)
		{
			visible = true;
			if (mNode == nullptr)
			{
				mShape->AddTexture("../Resources/red.png", eTEX_DIFFUSE);
				if(!mShowCol)
				mNode = Renderer::Instance()->AddOpaqueNode(*mShape);
				else
				mNode = Renderer::Instance()->AddTopmostNode(*mShape);
			}
			vec3f size = mMax - mMin;
			matrix4 pos = matrix4::CreateNewScale(size.x, size.y, size.z);
			pos.Position = (mMax - mMin) / 2.0f + mMin;
			mNode->data = pos;
		}
		else if (visible)
		{
			visible = false;
			memset(&mNode->data, 0, sizeof(matrix4));
		}
			
	}
	void CubeCollider::Destroy()
	{
		delete mNode;
		delete mShape;
	}
	void SphereCollider::Update() {
		if (Settings::GetInstance().GetBool("ShowColliders") || mShowCol) {
			visible = true;
			if (mNode == nullptr) {
				mShape->AddTexture("../Resources/red.png", eTEX_DIFFUSE);
				if (!mShowCol)
					mNode = Renderer::Instance()->AddOpaqueNode(*mShape);
				else
					mNode = Renderer::Instance()->AddTopmostNode(*mShape);
			}
		
			matrix4 pos =  GetTransform().GetMatrix() * matrix4::CreateNewScale(mRadius, mRadius, mRadius);
			pos.Position = GetPos();
			mNode->data = pos;
		} else if (visible) {
			visible = false;
			memset(&mNode->data, 0, sizeof(matrix4));
		}
	}
	void SphereCollider::Destroy()
	{
		delete mNode;
		delete mShape;
	}


	void CubeCollider::SetPos(const vec3f& _newPos)
	{
		mObject->GetTransform().GetMatrix().fourth = vec4f(_newPos);
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
		mLowerBound.mOffset = (mMin * mPushNormal) - .07f;
		mShouldMove = true;
		mIsTrigger = false;
		mPress = false;

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
		mForces = vec3f(0, 0, 0);
		mGravity = vec3f(0, -9.8f, 0);
		mType = eCOMPONENT_COLLIDER;
		mColliderType = eCOLLIDER_Controller;
		mTotalForce = { 0, 0, 0 };
		mAcceleration = { 0, 0, 0 };
		mShape = new RenderShape("../Resources/Controller.obj", true, ePS_TEXTURED, eVS_TEXTURED, eGS_PosNormTex);
		mShape->GetContext().mRasterState = eRS_WIREFRAME;
	}
	void ControllerCollider::Update()
	{
			if (Settings::GetInstance().GetBool("ShowColliders") || mShowCol) {
				visible = true;
				if (mNode == nullptr) {
					mShape->AddTexture("../Resources/red.png", eTEX_DIFFUSE);
					if (!mShowCol)
						mNode = Renderer::Instance()->AddOpaqueNode(*mShape);
					else
						mNode = Renderer::Instance()->AddTopmostNode(*mShape);
				}
				matrix4 pos = GetTransform().GetMatrix();
				mNode->data = pos;
			} else if (visible) {
				visible = false;
				memset(&mNode->data, 0, sizeof(matrix4));
			}

	}

}