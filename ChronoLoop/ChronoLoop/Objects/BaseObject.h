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
	std::string mName;
	BaseObject* mParent;
	unsigned int mUniqueID;
	std::list<BaseObject*> mChildren;
	Transform mTransform;
	bool mDestroyed = false;

public:
	std::unordered_map<ComponentType, std::vector<Component*>> mComponents;
	BaseObject();
	BaseObject(std::string _name, Transform _transform);
	~BaseObject();
	BaseObject& operator=(BaseObject& _equals);
	void Destroy();
	void Update();

	inline unsigned int GetUniqueId() { return mUniqueID; }

	inline std::string GetName() { return mName; };
	inline void SetName(std::string _name) { mName = _name; };

	inline BaseObject* GetParent() { return mParent; };
	inline void SetParent(BaseObject* _parent) { mParent = _parent; };

	inline Transform& GetTransform() { return mTransform; };
	inline void SetTransform(Transform _transform) { mTransform = _transform; };

	inline unsigned int GetUniqueID() { return mUniqueID; };
	inline void SetUniqueID(unsigned short _id) { mUniqueID = _id; };

	inline std::list<BaseObject*> GetChildren() { return mChildren; };
	inline void SetChildren(std::list<BaseObject*> _children) { mChildren = _children; };

	inline std::vector<Component*> GetComponents(ComponentType _type) { return mComponents[_type]; }
	inline Component* GetComponentIndexed(ComponentType _type, unsigned int _index) { return mComponents[_type][_index]; }
	inline unsigned int GetComponentCount(ComponentType _type) { return (unsigned int)mComponents[_type].size(); }
	unsigned int AddComponent(Component* _comp);
	bool RemoveComponent(Component* _comp);

	//**FUNCTION**//
	inline void AddChild(BaseObject* _obj) { mChildren.push_back(_obj); };
	inline void RemoveChild(BaseObject* _obj) { mChildren.remove(_obj); };
	//BaseObject& Clone();
	//BaseObject& Clone(BaseObject& _clone);
};