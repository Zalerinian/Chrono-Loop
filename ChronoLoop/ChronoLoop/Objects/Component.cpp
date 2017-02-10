#include "stdafx.h"
#include "Component.h"
#include "BaseObject.h"
#include "../Common/Math.h"

using namespace Math;

//Base Component---------------------------------------------
#pragma region Base Component

Transform& Component::GetTransform() const
{
	  return object->transform;
}

#pragma endregion
//------------------------------------------------------------
//Listener----------------------------------------------------
#pragma region Listener

void Listener::Update()
{

}

void Listener::Destroy() 
{

}

#pragma endregion
//------------------------------------------------------------
//Emitter-----------------------------------------------------
#pragma region Emitter

void Emitter::Update()
{

}

void Emitter::MakeEvent(long _id)
{

}

void Emitter::Destroy()
{

}
#pragma endregion
//------------------------------------------------------------
