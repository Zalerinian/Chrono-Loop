#include "stdafx.h"
#include "BaseObject.h"


BaseObject::BaseObject()
{
	//name = nullptr;
	m_parent = nullptr;
}
BaseObject::BaseObject(std::string _name, Transform _transform)
{
	m_name = _name;
	m_parent = nullptr;
	m_transform = _transform;
	m_mass = 0.0f;
}
BaseObject::BaseObject(std::string _name, Transform _transform, float _mass)
{
	m_name = _name;
	m_parent = nullptr;
	m_transform = _transform;
	m_mass = _mass;
}
BaseObject::~BaseObject()
{
	delete m_parent;
	m_components.clear();
	m_children.clear();
}
BaseObject BaseObject::Clone()
{
	BaseObject temp;
	temp.m_name = this->m_name;
	temp.m_transform = this->m_transform;
	temp.m_parent = this->m_parent;
	temp.m_components = this->m_components;
	temp.m_children = this->m_children;
	return temp;
}
BaseObject BaseObject::Clone(BaseObject _clone)
{
	_clone.m_name = this->m_name;
	_clone.m_transform = this->m_transform;
	_clone.m_parent = this->m_parent;
	_clone.m_components = this->m_components;
	_clone.m_children = this->m_children;
	return _clone;
}
BaseObject const* BaseObject::operator=(BaseObject _equals)
{
	if (this->m_name != _equals.m_name) this->m_name = _equals.m_name;
	if (this->m_parent != _equals.m_parent) this->m_parent = _equals.m_parent;
	if (this->m_children != _equals.m_children) this->m_children = _equals.m_children;
	//if (this->transform != _equals.transform) this->transform = _equals.transform;
	if (this->m_components != _equals.m_components) this->m_components = _equals.m_components;
	return this;
}
void BaseObject::CalcAcceleration(vec4f& _force)
{
	m_acc = Physics::CalcAcceleration(_force, m_mass);
}
void BaseObject::CalcVelocity(vec4f& _force, float _dt)
{
	CalcAcceleration(_force);
	m_vel = Physics::CalcVelocity(m_vel, m_acc, _dt);
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
