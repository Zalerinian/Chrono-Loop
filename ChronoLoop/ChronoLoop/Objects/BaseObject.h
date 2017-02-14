#pragma once
#include <string>
#include <list>
#include <vector>
#include "Transform.h"
#include "Component.h"
#include "../Physics/Physics.h"
using namespace Physics;
class BaseObject
{
private:
	float m_mass;
	std::string m_name;
	BaseObject* m_parent;
	Transform m_transform;
	vec4f m_pos, m_vel, m_acc;
	unsigned short m_UniqueId;
	std::list<BaseObject*> m_children;
	std::vector<Component*> m_components;
	
public:
	//**CONSTRUCTORS**//
	BaseObject();
	BaseObject(std::string _name, Transform _transform);
	BaseObject(std::string name, Transform _transform, float mass);
	~BaseObject();
Component* const GetComponet(unsigned int _indx);
	unsigned int GetNumofComponets();
	void Destroy() { delete this; };

	//**GETTERS/SETTERS**//
	float						GetMass() { return m_mass; };
	void						SetMass(float _mass) { m_mass = _mass; };

	std::string					GetName() { return m_name; };
	void						SetName(std::string _name) { m_name = _name; };

	BaseObject*					GetParent() { return m_parent; };
	void						SetParent(BaseObject* _parent) { m_parent = _parent; };

	Transform					GetTransform() { return m_transform; };
	void						SetTransform(Transform _transform) { m_transform = _transform; };

	vec4f						GetPosition() { return m_pos; };
	vec4f						GetVelocity() { return m_vel; };
	vec4f						GetAcceleration() { return m_acc; };

	unsigned short&				GetUniqueID() { return m_UniqueId; };
	void						SetUniqueID(unsigned short _id) { m_UniqueId = _id; };

	std::list<BaseObject*>		GetChildren() { return m_children; };
	void						SetChildren(std::list<BaseObject*> _children) { m_children = _children; };

	std::vector<Component*>		GetComponents() { return m_components; };
	void						SetComponents(std::vector<Component*> _components) { m_components = _components; };
	

	//**FUNCTION**//
	void						AddComponent(Component* _comp) { m_components.push_back(_comp); };
	//void						RemoveComponent(Component _comp) { components.remove(_comp); };
	void						AddChild(BaseObject* _obj) { m_children.push_back(_obj); };
	void						RemoveChild(BaseObject* _obj) { m_children.remove(_obj); };
	void						CalcAcceleration(vec4f& _force);
	void						CalcVelocity(vec4f& _force, float _dt);
	void						CalcPosition(vec4f& _force, float _dt);
	BaseObject					Clone();
	BaseObject					Clone(BaseObject _clone);
	BaseObject const*			operator=(BaseObject _equals);
};