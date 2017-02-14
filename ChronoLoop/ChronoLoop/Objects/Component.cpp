//#include "stdafx.h"
#include "Component.h"
#include "BaseObject.h"

#pragma region Base Component


Transform& Component::GetTransform() { return object->GetTransform(); }
Transform& Component::GetTransform() const { return object->GetTransform(); }
#pragma endregion

#pragma region Listener

//void Listener::Update(){}

#pragma endregion

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

vec4f Collider::GetPos()
{
	return object->GetTransform().GetMatrix().fourth;

}
void Collider::SetPos(vec4f _newPos)
{
	object->GetTransform().GetMatrix().fourth = _newPos;
	if (mIsCube)
	{
		mCubeMax += _newPos;
		mCubeMin += _newPos;
	}
}
//void Collider::CalcObjAcceleration(vec4f& _force)
//{
//	mAcceleration = Physics::Instance()->CalcAcceleration(_force, mMass);
//}
//void Collider::CalcObjVelocity(vec4f& _force, float _dt)
//{
//	mVelocity = Physics::Instance()->CalcVelocity(mVelocity, mAcceleration, _dt);
//}
//void Collider::CalcObjPosition(vec4f& _force, float _dt)
//{
//	object->GetTransform().GetMatrix().fourth = Physics::Instance()->CalcPosition(object->GetTransform().GetMatrix().fourth, mVelocity, _dt);
//}