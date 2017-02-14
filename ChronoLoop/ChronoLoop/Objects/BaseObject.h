#pragma once
#include <string>
#include <list>
#include <vector>
#include "Transform.h"
#include "Component.h"
#include "..\Physics\Physics.h"
#include "..\Core\TimeManager.h"


class Component;

class BaseObject
{
private:
	std::string name;
	BaseObject* parent;
	unsigned short  UniqueID, id;
	std::list<BaseObject*> children;
	std::vector<Component*> components;
	Transform transform;

public:
	//**CONSTRUCTORS**//
	BaseObject();
	BaseObject(std::string _name, Transform _transform);
	~BaseObject();
	BaseObject Clone();
	BaseObject Clone(BaseObject _clone);
	BaseObject const* operator=(BaseObject _equals);
	unsigned short& GetUniqueId();

	void Destroy() { delete this; };

	//**GETTERS/SETTERS**//
	std::string					GetName() { return name; };
	void						SetName(std::string _name) { name = _name; };

	BaseObject*					GetParent() { return parent; };
	void						SetParent(BaseObject* _parent) { parent = _parent; };

	Transform&					GetTransform() { return transform; };
	void						SetTransform(Transform _transform) { transform = _transform; };

	unsigned short&				GetUniqueID() { return UniqueID; };
	void						SetUniqueID(unsigned short _id) { UniqueID = _id; };

	std::list<BaseObject*>		GetChildren() { return children; };
	void						SetChildren(std::list<BaseObject*> _children) { children = _children; };

	std::vector<Component*>		GetComponents() { return components; };
	void						SetComponents(std::vector<Component*> _components) { components = _components; };
	

	//**FUNCTION**//
	void						AddComponent(Component* _comp) { components.push_back(_comp); };
	//void						RemoveComponent(Component _comp) { components.remove(_comp); };
	void						AddChild(BaseObject* _obj) { children.push_back(_obj); };
	void						RemoveChild(BaseObject* _obj) { children.remove(_obj); };
};