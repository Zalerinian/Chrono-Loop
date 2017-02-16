//#include "stdafx.h"
#include "Component.h"
#include "BaseObject.h"
#include "..\Rendering\Mesh.h"

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

vec4f Collider::GetPos()
{
	return object->GetTransform().GetMatrix().fourth;
}

void Collider::SetPos(vec4f _newPos)
{
	object->GetTransform().GetMatrix().fourth = _newPos;
}

MeshCollider::MeshCollider(bool _move, vec4f _gravity, float _mass, float _elasticity, char * _path)
{
	mType = eCOLLIDER_Mesh;
	mGravity = _gravity;
	mVelocity = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mAcceleration = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mTotalForce = mGravity;
	mImpulsiveForce = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mShouldMove = _move;
	mMass = _mass;
	mElasticity = _elasticity;
	mMesh = &Mesh(_path);
}

SphereCollider::SphereCollider(bool _move, vec4f _gravity, float _mass, float _elasticity, float _radius)
{
	mType = eCOLLIDER_Sphere;
	mGravity = _gravity;
	mVelocity = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mAcceleration = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mTotalForce = mGravity;
	mImpulsiveForce = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mShouldMove = _move;
	mMass = _mass;
	mElasticity = _elasticity;
	mRadius = _radius;
}

CubeCollider::CubeCollider(bool _move, vec4f _gravity, float _mass, float _elasticity, vec4f _min, vec4f _max)
{
	mType = eCOLLIDER_Cube;
	mGravity = _gravity;
	mVelocity = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mAcceleration = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mTotalForce = mGravity;
	mImpulsiveForce = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mShouldMove = _move;
	mColliding = false;
	mMass = _mass;
	mElasticity = _elasticity;
	mMin = _min;
	mMinOffset = mMin;
	mMax = _max;
	mMaxOffset = mMax;
}

void CubeCollider::SetPos(vec4f _newPos)
{
	object->GetTransform().GetMatrix().fourth = _newPos; 
	mMin = _newPos + mMinOffset; 
	mMax = _newPos + mMaxOffset; 
}

PlaneCollider::PlaneCollider(bool _move, vec4f _gravity, float _mass, float _elasticity, float _offset, vec4f _norm)
{
	mType = eCOLLIDER_Plane;
	mGravity = _gravity;
	mVelocity = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mAcceleration = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mTotalForce = mGravity;
	mImpulsiveForce = vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	mShouldMove = _move;
	mMass = _mass;
	mElasticity = _elasticity;
	mOffset = _offset;
	mNormal = _norm;
}


