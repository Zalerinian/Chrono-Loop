#pragma once
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include "Transform.h"
#include "Component.h"
#include "..\Physics\Physics.h"
#include "..\Core\TimeManager.h"
#include "..\Objects\Component.h"

class BaseObject {
	// The number of objects that exist in the world. ID 0 is reserved the player.
	static unsigned int ObjectCount;
private:
	std::string name;
	BaseObject* parent;
	unsigned short UniqueID;
	std::list<BaseObject*> children;
	Transform transform;

public:
	std::unordered_map<ComponentType, std::vector<Component*>> mComponents;

	BaseObject();
	BaseObject(std::string _name, Transform _transform);
	~BaseObject();
	BaseObject& operator=(BaseObject& _equals);
	unsigned short& GetUniqueId();
	void Destroy();

	inline std::string GetName() { return name; };
	inline void SetName(std::string _name) { name = _name; };

	inline BaseObject* GetParent() { return parent; };
	inline void SetParent(BaseObject* _parent) { parent = _parent; };

	inline Transform& GetTransform() { return transform; };
	inline void SetTransform(Transform _transform) { transform = _transform; };

	inline unsigned short& GetUniqueID() { return UniqueID; };
	inline void SetUniqueID(unsigned short _id) { UniqueID = _id; };

	inline std::list<BaseObject*> GetChildren() { return children; };
	inline void SetChildren(std::list<BaseObject*> _children) { children = _children; };

	inline std::vector<Component*> GetComponents(ComponentType _type) { return mComponents[_type]; }
	inline Component* GetComponentIndexed(ComponentType _type, unsigned int _index) { return mComponents[_type][_index]; }
	inline unsigned int GetComponentCount(ComponentType _type) { return (unsigned int)mComponents[_type].size(); }
	unsigned int AddComponent(Component* _comp);
	bool RemoveComponent(Component* _comp);

	//**FUNCTION**//
	inline void AddChild(BaseObject* _obj) { children.push_back(_obj); };
	inline void RemoveChild(BaseObject* _obj) { children.remove(_obj); };
	//BaseObject& Clone();
	//BaseObject& Clone(BaseObject& _clone);
};