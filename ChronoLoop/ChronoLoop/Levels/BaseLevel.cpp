#include "stdafx.h"
#include "BaseLevel.h"

BaseLevel::BaseLevel() {
	mId = 0;
}

BaseLevel::BaseLevel(unsigned int _id) {
	SetId(_id);
}

BaseLevel::BaseLevel(BaseObject* _headset, BaseObject* _controller1, BaseObject* _controller2)
{
	mHeadset = _headset;
	mController1 = _controller1;
	mController2 = _controller2;
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

void BaseLevel::SetHeadsetAndControllers(BaseObject * _headset, BaseObject * _controller1, BaseObject * _controller2)
{
	//Remove the action componets and 
	//Set the new BaseObjects to the current controller so new objects can follow old controller movement as clones.
	unsigned short headid = _headset->GetUniqueId();
	unsigned short cl1id = _controller1->GetUniqueId();
	unsigned short cl2id = _controller2->GetUniqueId();
	std::string headname = _headset->GetName();
	std::string Controller1name = _controller1->GetName();
	std::string Controller2name = _controller2->GetName();

	_headset->SetUniqueID(mHeadset->GetUniqueId());
	_controller1->SetUniqueID(mController1->GetUniqueId());
	_controller2->SetUniqueID(mController2->GetUniqueId());
	_headset->SetName(mHeadset->GetName());
	_controller1->SetName(mController1->GetName());
	_controller2->SetName(mController2->GetName());
	
	mHeadset->SetUniqueID(headid);
	mController1->SetUniqueID(cl1id);
	mController2->SetUniqueID(cl2id);
	mHeadset->SetName(headname);
	mController1->SetName(Controller1name);
	mController2->SetName(Controller2name);

	mObjectList.push_back(_headset);
	mObjectList.push_back(_controller1);
	mObjectList.push_back(_controller2);

}

void BaseLevel::LoadLevel() {
	//*Insert Code Here When we Get to It**//
}

void BaseLevel::Update() {
	//*Insert Code Here When we Get to It**//
	auto& objects = mObjectList;
	for (auto it = objects.begin(); it != objects.end(); ++it) {
		(*it)->Update();
	}
}

void BaseLevel::Shutdown() {
	delete this;
}
