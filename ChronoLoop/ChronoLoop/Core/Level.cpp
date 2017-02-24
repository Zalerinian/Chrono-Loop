#include "stdafx.h"
#include "Level.h"
#include "../Actions/CodeComponent.hpp"

Level* Level::sInstance = nullptr;

Level::Level() {}

Level::~Level() {
	for (auto iter = mObjectMap.begin(); iter != mObjectMap.end(); ++iter) { 
		for (int i = 0; i < iter->second.size(); ++i) {
			// TODO: Put objects back in the object pool.
			// Since this is a singleton and this is only destroyed when closing 
			// the program, would it be more efficient to just delete them directly?
			delete iter->second[i];
		}
	}
	mObjectMap.clear();
}

Level *Level::Instance() {
	return sInstance;
}

void Level::DestroyInstance() {
	if (sInstance != nullptr) {
		delete sInstance;
		sInstance = nullptr;
	}
}

void Level::Initialize(BaseObject * _headset, BaseObject * _lController, BaseObject * _rController) {
	if (nullptr == sInstance) {
		sInstance = new Level;
		sInstance->mHeadset = _headset;
		sInstance->mController1 = _lController;
		sInstance->mController2 = _rController;

	}
}

BaseObject * Level::iFindObjectWithName(std::string _name) {
	std::vector<BaseObject*> vec = mObjectMap[_name];
	if (vec.size() > 0) {
		return vec[0];
	}
	return nullptr;
}

std::vector<BaseObject*> Level::iFindAllObjectsWithName(std::string _name) {
	return mObjectMap[_name];
}

bool Level::iRemoveObject(BaseObject * _obj) {
	for (auto mit = mObjectMap.begin(); mit != mObjectMap.end(); ++mit) {
		for (auto vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
			if ((*vit) == _obj) {
				//TODO: add objects back to the pool.
				mit->second.erase(vit);
				return true;
			}
		}
	}

	return false;
}

void Level::iSetHeadsetAndControllers(BaseObject * _headset, BaseObject * _controller1, BaseObject * _controller2) {
	//Remove the action componets and 
	//Set the new BaseObjects to the current controller so new objects can follow old controller movement as clones.
	unsigned short headid = _headset->GetUniqueId();
	unsigned short cl1id = _controller1->GetUniqueId();
	unsigned short cl2id = _controller2->GetUniqueId();
	std::string headname = _headset->GetName();
	std::string Controller1name = _controller1->GetName();
	std::string Controller2name = _controller2->GetName();
	mObjectMap[_headset->GetName()].push_back(_headset);
	mObjectMap[_controller1->GetName()].push_back(_controller1);
	mObjectMap[_controller2->GetName()].push_back(_controller2);


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

	//Add the headset and controllers to the time manager with their new ids
	TimeManager::Instance()->AddObjectToTimeline(mHeadset);
	TimeManager::Instance()->AddObjectToTimeline(mController1);
	TimeManager::Instance()->AddObjectToTimeline(mController2);

}

void Level::iCallStart() {
	for (auto mit = mObjectMap.begin(); mit != mObjectMap.end(); ++mit) {
		for (auto vit = mit->second.begin(); vit != mit->second.end(); ++vit) {
			auto codes = (*vit)->GetComponents(eCOMPONENT_CODE);
			for (auto oit = codes.begin(); oit != codes.end(); ++oit) {
				((CodeComponent*)(*oit))->Start();
			}
		 }
	}
}

void Level::iLoadLevel() {
	//*TODO: Insert Code Here When we Get to It**//
}

void Level::iUpdate() {
	//*Insert Code Here When we Get to It**//
	for (auto mit = mObjectMap.begin(); mit != mObjectMap.end(); ++mit) {
		for (unsigned int i = 0; i < mit->second.size(); ++i) {
			mit->second[i]->Update();
		}
	}
}

bool Level::iOnObjectNamechange(BaseObject * _obj, std::string _name) {
	std::vector<BaseObject*>& objectsON = mObjectMap[_obj->GetName()];
	for (auto it = objectsON.begin(); it != objectsON.end(); ++it) {
		if ((*it) == _obj) {
			objectsON.erase(it);
			mObjectMap[_name].push_back(_obj);
			return true;
		}
	}
	return false;
}
