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
	mass = 0.0f;
}
BaseObject::BaseObject(std::string _name, Transform _transform, float _mass)
{
	name = _name;
	parent = nullptr;
	transform = _transform;
	mass = _mass;
}
BaseObject::~BaseObject()
{
	delete parent;
	for(auto c : components)
	{
		delete c;
	}
	components.clear();
	children.clear();
}
BaseObject BaseObject::Clone()
{
	//clone copies properties execpt id
	BaseObject temp;
	temp.name = this->name;
	temp.transform = this->transform;
	temp.parent = this->parent;
	temp.components = this->components;
	temp.children = this->children;
	return temp;
}
BaseObject BaseObject::Clone(BaseObject _clone)
{
	//clone copies properties execpt id
	_clone.name = this->name;
	_clone.transform = this->transform;
	_clone.parent = this->parent;
	_clone.components = this->components;
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
	if (this->components != _equals.components) this->components = _equals.components;
	return this;
}
void BaseObject::CalcAcceleration(vec4f& _force)
{
	acc = Physics::CalcAcceleration(_force, mass);
}
void BaseObject::CalcVelocity(vec4f& _force, float _dt)
{
	CalcAcceleration(_force);
	vel = Physics::CalcVelocity(vel, acc, _dt);
}
void BaseObject::CalcPosition(vec4f& _force, float _dt)
{
	CalcVelocity(_force, _dt);
	pos = Physics::CalcPosition(pos, vel, _dt);
}

unsigned short& BaseObject::GetUniqueId()
{
	return id;
}