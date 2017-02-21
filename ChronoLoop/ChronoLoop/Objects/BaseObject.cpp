//#include "stdafx.h"
#include "BaseObject.h"
#include "../Common/Logger.h"

// 0 is reserved for the player.
unsigned int BaseObject::ObjectCount = 1;

BaseObject::BaseObject()
{
	mParent = nullptr;
	mUniqueID = BaseObject::ObjectCount++;
}
BaseObject::BaseObject(std::string _name, Transform _transform)
{
	mName = _name;
	mParent = nullptr;
	mTransform = _transform;
}

BaseObject::~BaseObject()
{
	delete mParent;
	for(auto iter = mComponents.begin(); iter != mComponents.end(); ++iter)
	{
		for (int i = 0; i < iter->second.size(); ++i)
			delete iter->second[i];
	}
	mComponents.clear();
	mChildren.clear();
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
	if (this->mUniqueID != _equals.mUniqueID) this->mUniqueID = _equals.mUniqueID;
	if (this->mName != _equals.mName) this->mName = _equals.mName;
	if (this->mParent != _equals.mParent) this->mParent = _equals.mParent;
	if (this->mChildren != _equals.mChildren) this->mChildren = _equals.mChildren;
	if (this->mTransform != _equals.mTransform) this->mTransform = _equals.mTransform;
	if (this->mComponents != _equals.mComponents) this->mComponents = _equals.mComponents;
	return *this;
}

void BaseObject::Destroy()
{
	for (auto iter = mComponents.begin(); iter != mComponents.end(); ++iter) {
		for (int i = 0; i < iter->second.size(); ++i) {
			delete iter->second[i];
		}
	}
}

void BaseObject::Update() {
	for (auto it = mComponents.begin(); it != mComponents.end(); ++it) {
		if (it->first == eCOMPONENT_COLLIDER) {
			continue;
		}
		for (auto cIt = it->second.begin(); cIt != it->second.end(); ++cIt) {
			(*cIt)->Update();
		}
	}
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