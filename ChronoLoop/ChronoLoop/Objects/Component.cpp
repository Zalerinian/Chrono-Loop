#include "stdafx.h"
#include "Component.h"
#include "BaseObject.h"
#include "..\Common\Math.h"

#pragma region Base Component



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

void Collider::CalcObjAcceleration(vec4f& _force)
{
	mAcceleration = Physics::Instance()->CalcAcceleration(_force, mMass);
}
void Collider::CalcObjVelocity(vec4f& _force, float _dt)
{
	mVelocity = Physics::Instance()->CalcVelocity(mVelocity, mAcceleration, _dt);
}
void Collider::CalcObjPosition(vec4f& _force, float _dt)
{
	object->GetTransform().GetMatrix().fourth = Physics::Instance()->CalcPosition(object->GetTransform().GetMatrix().fourth, mVelocity, _dt);
}