#pragma once
#include <string>
#include <list>
#include <vector>
#include "Transform.h"
#include "Component.h"
#include "../TimeManager.h"

class Component;

class BaseObject
{
private:
	std::string name;
	unsigned short id;
	std::vector<Component*> components;
	Component* parent;
	Transform transform;
	std::list<BaseObject*> children;

public:
	BaseObject();
	BaseObject(std::string _name, Transform _transform);
	~BaseObject();
	BaseObject Clone();
	BaseObject Clone(BaseObject _clone);
	BaseObject const* operator=(BaseObject _equals);
	unsigned short& GetUniqueId();
	Transform& GetTransform();

	void Destroy() { delete this; };
	void AddComponent(Component* _comp) { components.push_back(_comp); };
	//void RemoveComponent(Component _comp) { components.remove(_comp); };
	void AddChild(BaseObject* _obj) { children.push_back(_obj); };
	void RemoveChild(BaseObject* _obj) { children.remove(_obj); };
};