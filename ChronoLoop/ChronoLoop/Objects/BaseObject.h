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

namespace Epoch {

	class BaseObject {
		// The number of objects that exist in the world.
		static unsigned int ObjectCount;

	private:
		std::string mName;
		BaseObject* mParent;
		unsigned int mUniqueID;
		std::list<BaseObject*> mChildren;
		Transform mTransform;
		bool mDestroyed = false;


		void Construct(std::string _name, Transform _transform, BaseObject* _parent);

	public:
		std::unordered_map<ComponentType, std::vector<Component*>> mComponents;
		BaseObject();
		BaseObject(std::string _name);
		BaseObject(std::string _name, Transform _transform);
		BaseObject(std::string _name, Transform _transform, BaseObject* _parent);
		~BaseObject();
		BaseObject& operator=(BaseObject& _equals);
		void Destroy();
		void Update();
		BaseObject* Reset(std::string _name);
		BaseObject* Reset(std::string _name, Transform _transform);
		BaseObject* Reset(std::string _name, Transform _transform, BaseObject* _parent);
		unsigned int GetTotalAmountofComponents();
		void RemoveAllComponents();

		inline unsigned int GetUniqueId() {
			return mUniqueID;
		}

		void SetName(std::string _name);
		inline std::string GetName() {
			return mName;
		};


		inline BaseObject* GetParent() {
			return mParent;
		};

		inline void SetParent(BaseObject* _parent) {
			mParent = _parent;
		};

		inline Transform& GetTransform() {
			return mTransform;
		};

		matrix4 GetWorld();

		inline void SetTransform(Transform _transform) {
			mTransform = _transform;
		}

		inline unsigned int GetUniqueID() {
			return mUniqueID;
		};
		
		inline void SetUniqueID(unsigned short _id) {
			mUniqueID = _id;
		};

		inline std::list<BaseObject*> GetChildren() { 
			return mChildren;
		}

		inline void SetChildren(std::list<BaseObject*> _children) {
			mChildren = _children;
		}

		inline std::vector<Component*>& GetComponents(ComponentType _type) {
			return mComponents[_type];
		}

		inline Component* GetComponentIndexed(ComponentType _type, unsigned int _index) {
			return mComponents[_type][_index];
		}

		inline unsigned int GetComponentCount(ComponentType _type) {
			return (unsigned int)mComponents[_type].size();
		}


		BaseObject* AddComponent(Component* _comp);
		bool RemoveComponent(Component* _comp);

		//**FUNCTION**//
		inline void AddChild(BaseObject* _obj) { mChildren.push_back(_obj); };
		inline void RemoveChild(BaseObject* _obj) { mChildren.remove(_obj); };
		//BaseObject& Clone();
		//BaseObject& Clone(BaseObject& _clone);
	};

}