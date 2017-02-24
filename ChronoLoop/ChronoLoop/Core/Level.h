#pragma once
////////////////////
//2/9/2017
//Written by: Ryan Bronk & Drew Ritter
///////////////////
#include "../ChronoLoop/Objects/BaseObject.h"
#include "../ChronoLoop/Rendering/renderer.h"


using namespace RenderEngine;


class Level {
	static Level* sInstance;
	BaseObject* mHeadset;
	BaseObject* mController1;
	BaseObject* mController2;
	std::unordered_map<std::string, std::vector<BaseObject*>> mObjectMap;
	unsigned short mId;


	Level();
	~Level();
public:
	static Level* Instance();
	static void DestroyInstance();
	static void Initialize(BaseObject *_headset, BaseObject *_lController, BaseObject *_rController);

	//**GETTERS**//
	BaseObject* iFindObjectWithName(std::string _name);
	std::vector<BaseObject*> iFindAllObjectsWithName(std::string _name);
	std::unordered_map<std::string, std::vector<BaseObject*>> iGetLevelObjects() { return mObjectMap; };
	inline void iAddObject(BaseObject* _obj) { mObjectMap[_obj->GetName()].push_back(_obj); }
	bool iRemoveObject(BaseObject* _obj);
	unsigned short iGetId() { return mId; };


	//**SETTERS**//
	void iSetId(unsigned short _set) { mId = _set; };
	void iSetHeadsetAndControllers(BaseObject* _headset, BaseObject* _controller1, BaseObject* _controller2);

	//**FUNCTIONS**//
	void iCallStart();
	void iLoadLevel();
	void iUpdate();
	bool iOnObjectNamechange(BaseObject* _obj, std::string _name);

};