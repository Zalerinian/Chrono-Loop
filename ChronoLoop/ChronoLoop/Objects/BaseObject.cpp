//#include "stdafx.h"
#include "BaseObject.h"
#include "../Common/Logger.h"

// 0 is reserved for the player.
unsigned int BaseObject::ObjectCount = 1;

BaseObject::BaseObject()
{
	parent = nullptr;
	UniqueID = BaseObject::ObjectCount++;
}
BaseObject::BaseObject(std::string _name, Transform _transform)
{
	name = _name;
	parent = nullptr;
	transform = _transform;
}

BaseObject::~BaseObject()
{
	delete parent;
	for(auto iter = mComponents.begin(); iter != mComponents.end(); ++iter)
	{
		for (int i = 0; i < iter->second.size(); ++i)
			delete iter->second[i];
	}
	mComponents.clear();
	children.clear();
}

//BaseObject BaseObject::Clone()
//{
//	BaseObject temp;
//	temp.m_name = this->m_name;
//	temp.m_transform = this->m_transform;
//	temp.m_parent = this->m_parent;
//	temp.m_components = this->m_components;
//	temp.m_children = this->m_children;
//	return temp;
//}
//
//BaseObject BaseObject::Clone(BaseObject _clone)
//{
//	_clone.m_name = this->m_name;
//	_clone.m_transform = this->m_transform;
//	_clone.m_parent = this->m_parent;
//	_clone.m_components = this->m_components;
//	_clone.m_children = this->m_children;
//	return _clone;
//}

BaseObject& BaseObject::operator=(BaseObject& _equals)
{
	if (this->UniqueID != _equals.UniqueID) this->UniqueID = _equals.UniqueID;
	if (this->name != _equals.name) this->name = _equals.name;
	if (this->parent != _equals.parent) this->parent = _equals.parent;
	if (this->children != _equals.children) this->children = _equals.children;
	if (this->transform != _equals.transform) this->transform = _equals.transform;
	if (this->mComponents != _equals.mComponents) this->mComponents = _equals.mComponents;
	return *this;
}

unsigned short& BaseObject::GetUniqueId()
{
	return UniqueID;
}

void BaseObject::Destroy()
{
	for (auto iter = mComponents.begin(); iter != mComponents.end(); ++iter)
		for (int i = 0; i < iter->second.size(); ++i)
			delete iter->second[i];
}

unsigned int BaseObject::AddComponent(Component * _comp) {
	if (_comp->GetType() == eCOMPONENT_MAX) {
		SystemLogger::GetError() << "[Error] Trying to add a component with an invalid type. This is not allowed, returning -1U." << std::endl;
		return -1;
	}
	_comp->mObject = this;
	mComponents[_comp->GetType()].push_back(_comp);
	return (unsigned int)mComponents[_comp->GetType()].size();
}

bool BaseObject::RemoveComponent(Component * _comp) {
	if (_comp->GetType() == eCOMPONENT_MAX) {
		SystemLogger::GetError() << "[Error] Trying to remove a component with an invalid type. This is not allowed, returning -1U." << std::endl;
		return false;
	}
	ComponentType type = _comp->GetType();
	unsigned int size = (unsigned int)mComponents[type].size();
	for (auto it = mComponents[type].begin(); it != mComponents[type].end(); ++it) {
		if((*it) == _comp) {
			mComponents[type].erase(it);
			return true;
		}
	}
	return false;
}

//void BaseObject::AddComponent(ComponentType _type, Component* _comp)
//{
//	mComponents[_type].push_back(_comp);
//}