//#include "stdafx.h"
#include "BaseObject.h"
#include "../Actions/CodeComponent.hpp"
#include "../Common/Logger.h"
#include "../Common/Breakpoint.h"
#include "../Core/Level.h"

namespace Epoch
{

	// 0 is reserved for the player.
	unsigned int BaseObject::ObjectCount = 1;

	void BaseObject::Construct(std::string _name, Transform _transform, BaseObject* _parent)
	{
		mName = _name;
		mTransform = _transform;
		mParent = _parent;
		mUniqueID = BaseObject::ObjectCount++;
	}

	BaseObject::BaseObject()
	{
		Construct("", Transform(), nullptr);
	}

	BaseObject::BaseObject(std::string _name)
	{
		Construct(_name, Transform(), nullptr);
	}
	BaseObject::BaseObject(std::string _name, Transform _transform)
	{
		Construct(_name, _transform, nullptr);
	}

	BaseObject::BaseObject(std::string _name, Transform _transform, BaseObject * _parent)
	{
		Construct(_name, _transform, _parent);
	}

	BaseObject::~BaseObject()
	{
		if (mDestroyed)
		{
			SystemLogger::GetError() << "[FATAL] Deleting an object that is marked as destroyed!" << std::endl;
			Debug::SetBreakpoint();
		}
		else
		{
			Destroy();
		}
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

	BaseObject& BaseObject::operator=(BaseObject& _equals)
	{
		mUniqueID = _equals.mUniqueID;
		mName = _equals.mName;
		mParent = _equals.mParent;
		mChildren = _equals.mChildren;
		mTransform = _equals.mTransform;
		mComponents = _equals.mComponents;
		return *this;
	}

	void BaseObject::Destroy()
	{
		if (mDestroyed)
		{
			SystemLogger::GetError() << "[Error] Attempting to destroy an object that is already marked as destroyed." << std::endl;
			return;
		}
		mParent = nullptr;
		for (auto iter = mComponents.begin(); iter != mComponents.end(); ++iter)
		{
			for (int i = 0; i < iter->second.size(); ++i)
			{
				iter->second[i]->Destroy();
				delete iter->second[i];
			}
		}
		mComponents.clear();
		for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
		{
			delete (*it);
		}
		mChildren.clear();
		mDestroyed = true;
	}

	void BaseObject::Update()
	{
		if (mDestroyed)
		{
			SystemLogger::GetError() << "[Error] Attempting to update an object that is already marked as destroyed." << std::endl;
			return;
		}
		for (auto it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			if (it->first == eCOMPONENT_COLLIDER)
			{
				continue;
			}
			for (auto cIt = it->second.begin(); cIt != it->second.end(); ++cIt)
			{
				(*cIt)->Update();
			}
		}
	}

	BaseObject * BaseObject::Reset(std::string _name)
	{
		Reset(_name, Transform(), nullptr);
		return this;
	}

	BaseObject * BaseObject::Reset(std::string _name, Transform _transform)
	{
		Reset(_name, _transform, nullptr);
		return this;
	}

	BaseObject * BaseObject::Reset(std::string _name, Transform _transform, BaseObject * _parent)
	{
		mName = _name;
		mTransform = _transform;
		mParent = _parent;
		return this;
	}

	void BaseObject::SetName(std::string _name)
	{
		if (Level::Instance()->iOnObjectNamechange(this, _name))
		{
			mName = _name;
		}
		else
		{
			SystemLogger::GetError() << "[Error] A name change was requested for an object that did not exist in the level." << std::endl;
		}
	}

	unsigned int BaseObject::AddComponent(Component * _comp)
	{
		if (mDestroyed)
		{
			SystemLogger::GetError() << "[Error] Attempting to add a compontnet to an object that is already marked as destroyed." << std::endl;
			return -1;
		}
		if (_comp->GetType() == eCOMPONENT_MAX)
		{
			SystemLogger::GetError() << "[Error] Trying to add a component with an invalid type. This is not allowed, returning -1U." << std::endl;
			return -1;
		}
		if (!_comp->IsValid())
		{
			SystemLogger::GetError() << "[Error] Attempted to add a component that is marked invalid. It will be deleted." << std::endl;
			delete _comp;
			return -1;
		}
		if (mComponents.size() + 1 > 30)
		{
			SystemLogger::GetError() << "[Error] Attempted to add a component after reaching the max amount you can add. Deleting this component" << std::endl;
			delete _comp;
			return -1;
		}
		_comp->mObject = this;
		mComponents[_comp->GetType()].push_back(_comp);
		_comp->mComponentNum = (unsigned short)mComponents.size();	//sets the component number for knowing the position in the bitset
		_comp->OnAddedToObject();
		return (unsigned int)mComponents[_comp->GetType()].size();
	}

	bool BaseObject::RemoveComponent(Component * _comp)
	{
		if (mDestroyed)
		{
			SystemLogger::GetError() << "[Error] Attempting to remove a component from an object that is already marked as destroyed." << std::endl;
			return false;
		}
		if (_comp->GetType() == eCOMPONENT_MAX)
		{
			SystemLogger::GetError() << "[Error] Trying to remove a component with an invalid type. This is not allowed, returning -1U." << std::endl;
			return false;
		}
		ComponentType type = _comp->GetType();
		unsigned int size = (unsigned int)mComponents[type].size();
		for (auto it = mComponents[type].begin(); it != mComponents[type].end(); ++it)
		{
			if ((*it) == _comp)
			{
				(*it)->Destroy();
				delete (*it);
				mComponents[type].erase(it);
				return true;
			}
		}
		return false;
	}

	//void BaseObject::AddComponent(ComponentType _type, Component* _comp)
	//{
	//	mComponents[_type].push_back(_comp);
	//}

}