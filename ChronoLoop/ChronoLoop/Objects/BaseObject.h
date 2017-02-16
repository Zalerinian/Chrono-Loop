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
	inline void Destroy() { delete this; };

	//**GETTERS/SETTERS**//
	inline float GetMass() { return m_mass; };
	inline void SetMass(float _mass) { m_mass = _mass; };

	inline std::string GetName() { return m_name; };
	inline void SetName(std::string _name) { m_name = _name; };

	inline BaseObject* GetParent() { return m_parent; };
	inline void SetParent(BaseObject* _parent) { m_parent = _parent; };

	inline Transform GetTransform() { return m_transform; };
	inline void SetTransform(Transform _transform) { m_transform = _transform; };

	inline vec4f GetPosition() { return m_pos; };
	inline vec4f GetVelocity() { return m_vel; };
	inline vec4f GetAcceleration() { return m_acc; };

	inline unsigned short& GetUniqueID() { return m_UniqueId; };
	inline void SetUniqueID(unsigned short _id) { m_UniqueId = _id; };

	inline std::list<BaseObject*> GetChildren() { return m_children; };
	inline void SetChildren(std::list<BaseObject*> _children) { m_children = _children; };

	inline std::vector<Component*> GetComponents() { return m_components; };
	inline void SetComponents(std::vector<Component*> _components) { m_components = _components; };
	

	//**FUNCTION**//
	inline void AddComponent(Component* _comp) { m_components.push_back(_comp); };
	inline //void RemoveComponent(Component _comp) { components.remove(_comp); };
	inline void AddChild(BaseObject* _obj) { m_children.push_back(_obj); };
	inline void RemoveChild(BaseObject* _obj) { m_children.remove(_obj); };
	//BaseObject Clone();
	//BaseObject Clone(BaseObject _clone);
	BaseObject& operator=(BaseObject _equals);
};