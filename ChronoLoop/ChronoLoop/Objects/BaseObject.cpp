//#include "stdafx.h"
#include "BaseObject.h"


BaseObject::BaseObject()
{
	
	parent = nullptr;
	//TESTING VALUES GET RID OF THIS EVENTUALLY
	id = 2;
	
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
BaseObject BaseObject::Clone()
{
	//clone copies properties execpt id
	BaseObject temp;
	temp.name = this->name;
	temp.transform = this->transform;
	temp.parent = this->parent;
	temp.mComponents = this->mComponents;
	temp.children = this->children;
	return temp;
}
BaseObject BaseObject::Clone(BaseObject _clone)
{
	//clone copies properties execpt id
	_clone.name = this->name;
	_clone.transform = this->transform;
	_clone.parent = this->parent;
	_clone.mComponents = this->mComponents;
	_clone.children = this->children;
	return _clone;
}
BaseObject const* BaseObject::operator=(BaseObject _equals)
{
	if (this->id != _equals.id) this->id = _equals.id;
	if (this->name != _equals.name) this->name = _equals.name;
	if (this->parent != _equals.parent) this->parent = _equals.parent;
	if (this->children != _equals.children) this->children = _equals.children;
	//if (this->transform != _equals.transform) this->transform = _equals.transform;
	if (this->mComponents != _equals.mComponents) this->mComponents = _equals.mComponents;
	return this;
}

unsigned short& BaseObject::GetUniqueId()
{
	return id;
}

void BaseObject::AddComponent(ComponentType _type, Component* _comp)
{
	mComponents[_type].push_back(_comp);
}
void BaseObject::CalcPosition(vec4f& _force, float _dt)
{
	CalcVelocity(_force, _dt);
	m_pos = Physics::CalcPosition(m_pos, m_vel, _dt);
}

Component* const BaseObject::GetComponet(unsigned int _indx) {
	return m_components[_indx];
}

unsigned int BaseObject::GetNumofComponets() {
	return m_components.size();
}
