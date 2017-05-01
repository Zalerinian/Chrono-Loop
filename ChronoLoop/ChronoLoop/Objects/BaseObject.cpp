//#include "stdafx.h"
#include "BaseObject.h"
#include "../Actions/CodeComponent.hpp"
#include "../Common/Logger.h"
#include "../Common/Breakpoint.h"
#include "../Core/Level.h"
#include "../Core/LevelManager.h"
#include "../Common/Settings.h"

namespace Epoch {
	unsigned int BaseObject::ObjectCount = 0;

	void BaseObject::Construct(std::string _name, Transform _transform, BaseObject* _parent) {
		mName = _name;
		mTransform = _transform;
		mParent = _parent;
		mUniqueID = ++BaseObject::ObjectCount;

		if (mComponents.size() > 0)
			RemoveAllComponents();
	}

	BaseObject::BaseObject() : Flags(mFlags) {
		Construct("", Transform(), nullptr);
	}

	BaseObject::BaseObject(std::string _name) : Flags(mFlags) {
		Construct(_name, Transform(), nullptr);
	}
	BaseObject::BaseObject(std::string _name, Transform _transform) : Flags(mFlags) {
		Construct(_name, _transform, nullptr);
	}

	BaseObject::BaseObject(std::string _name, Transform _transform, BaseObject * _parent) : Flags(mFlags) {
		Construct(_name, _transform, _parent);
	}

	BaseObject::~BaseObject() {
		Destroy();
	}

	//BaseObject BaseObject::Clone()
	//{
	//	BaseObject temp;
	//	temp.m_name = this->m_name;
	//	temp.m_transform = this->m_transform;
	//	temp.m_parent = this->m_parent;
	//	temp.m_components = this->m_components;
	//	temp.m_children = this->m_children;
	//	return temp;
	//}
	//
	//BaseObject BaseObject::Clone(BaseObject _clone)
	//{
	//	_clone.m_name = this->m_name;
	//	_clone.m_transform = this->m_transform;
	//	_clone.m_parent = this->m_parent;
	//	_clone.m_components = this->m_components;
	//	_clone.m_children = this->m_children;
	//	return _clone;
	//}

	BaseObject& BaseObject::operator=(BaseObject& _equals) {
		if (&_equals != this) {
			mUniqueID = _equals.mUniqueID;
			mName = _equals.mName;
			mParent = _equals.mParent;
			mChildren = _equals.mChildren;
			mTransform = _equals.mTransform;
			mComponents = _equals.mComponents;
		}
		return *this;
	}

	void BaseObject::Destroy() {
		mParent = nullptr;
		for (auto it = mChildren.begin(); it != mChildren.end(); ++it) {
			LevelManager::GetInstance().GetCurrentLevel()->RemoveObject(*it);
			delete (*it);
		}
		mChildren.clear();
		for (auto iter = mComponents.begin(); iter != mComponents.end(); ++iter) {
			for (int i = 0; i < iter->second.size(); ++i) {
				iter->second[i]->Destroy();
				delete iter->second[i];
			}
		}
		mComponents.clear();
	}

	void BaseObject::Update() {
		for (auto it = mComponents.begin(); it != mComponents.end(); ++it) {
			if (it->first == eCOMPONENT_COLLIDER) {
				//continue;
			}
			for (auto cIt = it->second.begin(); cIt != it->second.end(); ++cIt) {
				if ((*cIt)->IsEnabled()) {
					(*cIt)->Update();
				}
			}
		}
	}

	BaseObject * BaseObject::Reset(std::string _name) {
		return Reset(_name, Transform(), nullptr);
	}

	BaseObject * BaseObject::Reset(std::string _name, Transform _transform) {
		return Reset(_name, _transform, nullptr);
	}

	BaseObject * BaseObject::Reset(std::string _name, Transform _transform, BaseObject * _parent) {
		// This will increment the object's ID, which is needed for the timeline to function properly with objects obtained from the pool.
		Construct(_name, _transform, _parent);
		//mName = _name;
		//mTransform = _transform;
		//mParent = _parent;
		return this;
	}

	BaseObject * BaseObject::Reset(std::string _name, Transform _transform, BaseObject * _parent, unsigned int _flags) {
		Reset(_name, _transform, _parent);
		mFlags = _flags;
		return this;
	}

	unsigned int BaseObject::GetTotalAmountofComponents()
	{
		int count = 0;
		for (auto vec : mComponents)
		{
			count += (int)vec.second.size();
		}

		return count;
	}

	void BaseObject::SetName(std::string _name) {
		mName = _name;
	}

	matrix4 BaseObject::GetWorld() {
		if (mParent) {
			return mTransform.GetMatrix() * mParent->GetWorld();
		} else {
			return mTransform.GetMatrix();
		}
	}

	BaseObject* BaseObject::AddComponent(Component * _comp) {
		if (_comp->GetType() == eCOMPONENT_MAX) {
			SystemLogger::Error() << "Trying to add a component with an invalid type. This is not allowed, returning -1U." << std::endl;
			return nullptr;
		}
		if (mComponents.size() + 1 > 30) {
			SystemLogger::Error() << "Attempted to add a component after reaching the max amount you can add. Deleting this component" << std::endl;
			delete _comp;
			return nullptr;
		}
		_comp->mObject = this;
		mComponents[_comp->GetType()].push_back(_comp);
		_comp->mComponentNum = (unsigned short)(GetTotalAmountofComponents());	//sets the component number for knowing the position in the bitset

		//If level has already started call start on new component
		if (!Settings::GetInstance().GetBool("LevelIsLoading")) {
			if (_comp->GetType() == eCOMPONENT_CODE && TimeManager::Instance()->GetTotalSnapsmade() > 0) {
				((CodeComponent*)_comp)->Start();
			}
		}
		return this;
	}

	bool BaseObject::RemoveComponent(Component * _comp) {
		if (_comp->GetType() >= eCOMPONENT_MAX) {
			SystemLogger::Error() << "Trying to remove a component with an invalid type. This is not allowed." << std::endl;
			return false;
		}
		ComponentType type = _comp->GetType();
		unsigned int size = (unsigned int)mComponents[type].size();
		for (auto it = mComponents[type].begin(); it != mComponents[type].end(); ++it) {
			if ((*it) == _comp) {
				(*it)->Destroy();
				delete (*it);
				mComponents[type].erase(it);
				return true;
			}
		}
		return false;
	}
	void BaseObject::RemoveAllComponents() {

		for (auto it = mComponents.begin(); it != mComponents.end(); ++it) {
			for (int i = (int)it->second.size() - 1; i >= 0; --i) {
				it->second[i]->Destroy();
				delete it->second[i];
			}
			it->second.clear();
		}

		return;
		
		

		
	
		
		
		return;
	}

	


	

	//void BaseObject::AddComponent(ComponentType _type, Component* _comp)
	//{
	//	mComponents[_type].push_back(_comp);
	//}

}