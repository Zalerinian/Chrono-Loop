#pragma once
#include <string>
#include <list>
#include <vector>
#include "Transform.h"
#include "Component.h"

class BaseObject
{
private:
	std::string name;
	//uint32 id;
	std::vector<Component*> components;
	Component* parent;
	Transform transform;
	std::list<BaseObject*> children;
public:
	BaseObject();
	BaseObject(std::string _name, Transform _transform);
	~BaseObject();
	void Destroy() { delete this; };
	void AddComponent(Component* _comp) { components.push_back(_comp); };
	//void RemoveComponent(Component _comp) { components.remove(_comp); };
	void AddChild(BaseObject* _obj) { children.push_back(_obj); };
	void RemoveChild(BaseObject* _obj) { children.remove(_obj); };
	BaseObject Clone();
	BaseObject Clone(BaseObject _clone);
	BaseObject const* operator=(BaseObject _equals);

};