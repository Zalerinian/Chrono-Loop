#include "stdafx.h"
#include "BaseLevel.h"

BaseLevel::BaseLevel() {
	mId = 0;
}

BaseLevel::BaseLevel(unsigned int _id) {
	SetId(_id);
}

BaseLevel::~BaseLevel() {
	for (std::list<BaseObject*>::const_iterator iter = mObjectList.begin(); iter != mObjectList.end(); ++iter) { 
			BaseObject* rtn = (*iter);
			delete rtn;
	}
	mObjectList.clear();
}

BaseObject * BaseLevel::GetLevelObject(int _index) {
	int cnt = 0;
	if (_index > mObjectList.size() - 1)
		return nullptr;
	for (std::list<BaseObject*>::const_iterator iter = mObjectList.begin(); iter != mObjectList.end(); ++iter) {
		if (cnt == _index) {
			BaseObject* rtn = (*iter);
			return rtn;
		}
		cnt++;
	}
	return nullptr;
}
BaseObject * BaseLevel::GetLevelObject(unsigned short _id) {
	for (std::list<BaseObject*>::const_iterator iter = mObjectList.begin(); iter != mObjectList.end(); ++iter) {
		if ((*iter)->GetUniqueID() == _id) {
			BaseObject* rtn = (*iter);
			return rtn;
		}
	}
	return nullptr;
}
void BaseLevel::SetLevelObject(BaseObject* _obj, int _index) {
	int cnt = 0;
	if (_index > mObjectList.size() - 1)
		return;
	for (std::list<BaseObject*>::const_iterator iter = mObjectList.begin(); iter != mObjectList.end(); ++iter) {
		if (cnt == _index) {
			BaseObject* rtn = (*iter);
			(*iter)->SetName(_obj->GetName());
			(*iter)->SetParent(_obj->GetParent());
			(*iter)->SetTransform(_obj->GetTransform());
			(*iter)->SetChildren(_obj->GetChildren());
		}
		cnt++;
	}
}
void BaseLevel::SetLevelObject(BaseObject * _obj, unsigned short _id) {
	for (std::list<BaseObject*>::const_iterator iter = mObjectList.begin(); iter != mObjectList.end(); ++iter) {
		if ((*iter)->GetUniqueID() == _id) {
			BaseObject* rtn = (*iter);
			(*iter)->SetName(_obj->GetName());
			(*iter)->SetParent(_obj->GetParent());
			(*iter)->SetTransform(_obj->GetTransform());
			(*iter)->SetChildren(_obj->GetChildren());


		}
	}
}

void BaseLevel::LoadLevel() {
	//*Insert Code Here When we Get to It**//
}

void BaseLevel::Update() {
	//*Insert Code Here When we Get to It**//
}

void BaseLevel::Shutdown() {
	delete this;
}
