#include "stdafx.h"
#include "Level.h"
#include "../Actions/CodeComponent.hpp"
#include "../Objects/MeshComponent.h"
#include "../tinyxml/tinyxml.h"
#include "../tinyxml/tinystr.h"

namespace Epoch
{

	Level* Level::sInstance = nullptr;

	Level::Level()
	{}

	Level::~Level()
	{
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it)
		{
			delete *it;
		}
		mObjectList.clear();

		//for (auto iter = mObjectMap.begin(); iter != mObjectMap.end(); ++iter) { 
		//	for (int i = 0; i < iter->second.size(); ++i) {
		//		// TODO: Put objects back in the object pool.
		//		// Since this is a singleton and this is only destroyed when closing 
		//		// the program, would it be more efficient to just delete them directly?
		//		delete iter->second[i];
		//	}
		//}
		//mObjectMap.clear();
	}

	Level *Level::Instance()
	{
		return sInstance;
	}

	void Level::DestroyInstance()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
			sInstance = nullptr;
		}
	}

	void Level::Initialize(BaseObject * _headset, BaseObject * _lController, BaseObject * _rController)
	{
		if (nullptr == sInstance)
		{
			sInstance = new Level;
			sInstance->mHeadset = _headset;
			sInstance->mController1 = _lController;
			sInstance->mController2 = _rController;
			CommandConsole::Instance().AddCommand(L"/WIREFRAME", ToggleEntireLevelsWireframe);
		}
	}

	BaseObject * Level::iFindObjectWithName(std::string _name)
	{
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it)
		{
			if ((*it)->GetName() == _name)
			{
				return *it;
			}
		}
		return nullptr;
	}

	std::vector<BaseObject*> Level::iFindAllObjectsWithName(std::string _name)
	{
		std::vector<BaseObject*> objects;
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it)
		{
			if ((*it)->GetName() == _name)
			{
				objects.push_back(*it);
			}
		}
		return objects;
	}

	void Level::iAddObject(BaseObject * _obj)
	{
		mObjectList.push_back(_obj);
	}

	bool Level::iRemoveObject(BaseObject * _obj)
	{
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it)
		{
			if ((*it) == _obj)
			{
				mObjectList.erase(it);
				return true;
			}
		}
		return false;
	}

void Level::iSetHeadsetAndControllers(BaseObject *& _headset, BaseObject *& _controller1, BaseObject *& _controller2, ControllerCollider* _c1Collider, ControllerCollider* _c2Collider) {
	//Remove the action componets and 
	//Set the new BaseObjects to the current controller so new objects can follow old controller movement as clones.
	unsigned short headid = _headset->GetUniqueID();
	unsigned short cl1id = _controller1->GetUniqueID();
	unsigned short cl2id = _controller2->GetUniqueID();
	unsigned short c1paramCodeCollid = _c1Collider->GetColliderId();
	unsigned short c2paramCodeCollid = _c2Collider->GetColliderId();

		Component* controller1Collider = mController1->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);
		Component* controller2Collider = mController2->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);

		std::string headname = _headset->GetName();
		std::string Controller1name = _controller1->GetName();
		std::string Controller2name = _controller2->GetName();

		//mObjectMap[_headset->GetName()].push_back(_headset);
		//mObjectMap[_controller1->GetName()].push_back(_controller1);
		//mObjectMap[_controller2->GetName()].push_back(_controller2);

	_headset->SetUniqueID(mHeadset->GetUniqueID());
	_controller1->SetUniqueID(mController1->GetUniqueID());
	_controller2->SetUniqueID(mController2->GetUniqueID());
	_headset->SetName(mHeadset->GetName());
	_controller1->SetName(mController1->GetName());
	_controller2->SetName(mController2->GetName());
	_c1Collider->SetComponentId(controller1Collider->GetColliderId());
	_c2Collider->SetComponentId(controller2Collider->GetColliderId());
	
	mHeadset->SetUniqueID(headid);
	mController1->SetUniqueID(cl1id);
	mController2->SetUniqueID(cl2id);
	mHeadset->SetName(headname);
	mController1->SetName(Controller1name);
	mController2->SetName(Controller2name);
	controller1Collider->SetComponentId(c1paramCodeCollid);
	controller2Collider->SetComponentId(c2paramCodeCollid);

		mHeadset->SetUniqueID(headid);
		mController1->SetUniqueID(cl1id);
		mController2->SetUniqueID(cl2id);
		mHeadset->SetName(headname);
		mController1->SetName(Controller1name);
		mController2->SetName(Controller2name);
		controller1Collider->SetComponentId(c1paramCodeCollid);
		controller2Collider->SetComponentId(c2paramCodeCollid);

		mObjectList.push_back(_headset);
		mObjectList.push_back(_controller1);
		mObjectList.push_back(_controller2);

		////Add the headset and controllers to the time manager with their new ids
		TimeManager::Instance()->AddObjectToTimeline(mHeadset);
		TimeManager::Instance()->AddObjectToTimeline(mController1);
		TimeManager::Instance()->AddObjectToTimeline(mController2);

	}

	void Level::iCallStart()
	{
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it)
		{
			auto codes = (*it)->GetComponents(eCOMPONENT_CODE);
			for (auto oit = codes.begin(); oit != codes.end(); ++oit)
			{
				((CodeComponent*)*oit)->Start();
			}
		}
	}

	void Level::iLoadLevel()
	{
		//*TODO: Insert Code Here When we Get to It**//
	}

	void Level::iLoadLevel(std::string _file)
	{
		TiXmlDocument doc(_file.c_str());
		bool loaded = doc.LoadFile();
		if (loaded)
		{
			doc.
		}
		else
			return;
	}

	void Level::iUpdate()
	{
		//*Insert Code Here When we Get to It**//
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it)
		{
			(*it)->Update();
		}
	}

	bool Level::iOnObjectNamechange(BaseObject * _obj, std::string _name)
	{
		return true;

		// This is only need for a map.
		//std::vector<BaseObject*>& objectsON = mObjectMap[_obj->GetName()];
		//for (auto it = objectsON.begin(); it != objectsON.end(); ++it) {
		//	if ((*it) == _obj) {
		//		objectsON.erase(it);
		//		mObjectMap[_name].push_back(_obj);
		//		return true;
		//	}
		//}
		//return false;
	}

	void Level::ToggleEntireLevelsWireframe(void* _command, std::wstring _ifOn)
	{
		CommandConsole* cc = (CommandConsole*)_command;
		if (_ifOn == L"ON")
		{
			for (auto it = sInstance->mObjectList.begin(); it != sInstance->mObjectList.end(); ++it)
			{
				for (size_t x = 0; x < (*it)->GetComponents(ComponentType::eCOMPONENT_MESH).size(); ++x)
				{
					MeshComponent* tempMComp = (MeshComponent*)((*it)->GetComponents(ComponentType::eCOMPONENT_MESH)[x]);

					tempMComp->SetRasterState(eRS_WIREFRAME);//< - This line
				}
			}
			CommandConsole::Instance().DisplaySet(L"");
		}
		else if (_ifOn == L"OFF")
		{
			for (auto it = sInstance->mObjectList.begin(); it != sInstance->mObjectList.end(); ++it)
			{
				for (size_t x = 0; x < (*it)->GetComponents(ComponentType::eCOMPONENT_MESH).size(); ++x)
				{
					MeshComponent* tempMComp = (MeshComponent*)((*it)->GetComponents(ComponentType::eCOMPONENT_MESH)[x]);
					tempMComp->SetRasterState(eRS_FILLED);
				}
			}
			CommandConsole::Instance().DisplaySet(L"");
		}
		else
		{
			CommandConsole::Instance().DisplaySet(L"INVALID INPUT: " + _ifOn + L"\nCORRECT INPUT: /WIREFRAME (ON/OFF)");
		}
	}

}