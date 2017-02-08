#include "stdafx.h"
#include "BaseObject.h"


BaseObject::BaseObject()
{
	name = nullptr;
	parent = nullptr;
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
	components.clear();
	children.clear();
}
BaseObject BaseObject::Clone()
{
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
	_clone.name = this->name;
	_clone.transform = this->transform;
	_clone.parent = this->parent;
	_clone.components = this->components;
	_clone.children = this->children;
	return _clone;
}
BaseObject const* BaseObject::operator=(BaseObject _equals)
{
	if (this->name != _equals.name) this->name = _equals.name;
	if (this->parent != _equals.parent) this->parent = _equals.parent;
	if (this->children != _equals.children) this->children = _equals.children;
	if (this->transform != _equals.transform) this->transform = _equals.transform;
	if (this->components != _equals.components) this->components = _equals.components;
	return this;
}