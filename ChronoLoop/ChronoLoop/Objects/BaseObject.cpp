#include "stdafx.h"
#include "BaseObject.h"
#include "../TimeManager.h"

BaseObject::BaseObject()
{
	//Testing
	id = 2;
	TimeManager::Instance()->GetTimeLine()->AddBaseObject(*this,id);
}

BaseObject::~BaseObject()
{
}

BaseObject::BaseObject(BaseObject const & _copy)
{
	*this = _copy;
}

BaseObject& BaseObject::operator=(const BaseObject _b)
{
	//TODO PAT RYAN: add all BaseObject information
	//this->transform.m_matrix = _b.transform.GetMatrix();
	this->id = _b.id;
	return *this;
}


short& BaseObject::GetUniqueId()
{
	return id;
}

Transform & BaseObject::GetTransform()
{
	return transform;
}


