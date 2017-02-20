#pragma once
////////////////////
//2/9/2017
//Written by: Ryan Bronk & Drew Ritter
///////////////////
#include "../ChronoLoop/Objects/BaseObject.h"
#include "../ChronoLoop/Rendering/renderer.h"

using namespace RenderEngine;


class BaseLevel {
private:
	std::list<BaseObject*> mObjectList;
	unsigned short mId;

public:
	//**CONSTRUCTORS/DESTRUCTORS**//
	BaseLevel();
	BaseLevel(unsigned int _id);
	~BaseLevel();
	//**GETTERS**//
	virtual	std::list<BaseObject*> GetLevelObjects() { return mObjectList; };
	virtual BaseObject* GetLevelObject(int _index);// { return mObjectList[_index]; };
	virtual BaseObject* GetLevelObject(unsigned short _id);
	virtual unsigned short GetId() { return mId; };


	//**SETTERS**//
	virtual void SetLevelObject(BaseObject* _obj, int _index);
	virtual void SetLevelObject(BaseObject* _obj, unsigned short _id);
	virtual void SetId(unsigned short _set) { mId = _set; };

	//**FUNCTIONS**//
	virtual void RemoveLevelObject(BaseObject* _obj) { mObjectList.remove(_obj); };
	virtual void AddLevelObject(BaseObject* _obj) { mObjectList.push_back(_obj); };
	virtual void LoadLevel();
	virtual void Update();
	virtual void Shutdown();


};