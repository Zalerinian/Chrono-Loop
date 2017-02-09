#pragma once
////////////////////
//2/9/2017
//Written by: Ryan Bronk
///////////////////
#include "../ChronoLoop/Objects/BaseObject.h"
#include "../ChronoLoop/Rendering/renderer.h"
#include <iterator>
//#include "../ChronoLoop/TimeManager.h"
using namespace RenderEngine;
class BaseLevel
{
private:
	std::list<BaseObject*> m_ObjectList;
	//TimeManager* m_TimeManager;
	Renderer* m_Renderer;
	matrix4 m_viewMatrix;
	unsigned short m_ID;

public:
	//**CONSTRUCTORS/DESTRUCTORS**//
	BaseLevel();
	BaseLevel(Renderer* _renderer, matrix4 _view, unsigned int _id);
	~BaseLevel();
	//**GETTERS**//
	virtual	std::list<BaseObject*>		GetLevelObjects() { return m_ObjectList; };
	virtual BaseObject*					GetLevelObject(int _index);// { return m_ObjectList[_index]; };
	virtual BaseObject*					GetLevelObject(unsigned short _id);
	//virtual TimeManager*				GetTimeManager() { return m_TimeManager; };
	virtual matrix4						GetViewMatrix() { return m_viewMatrix; };
	virtual Renderer*					GetRenderer() { return m_Renderer; }
	virtual unsigned short				GetId() { return m_ID; };


	//**SETTERS**//
	virtual void						SetLevelObject(BaseObject* _obj, int _index);// { m_ObjectList[_index] = _obj; };
	virtual void						SetLevelObject(BaseObject* _obj, unsigned short _id);
	//virtual void						SetTimeManager(TimeManager* _set) { m_TimeManager = _set; };
	virtual void						SetViewMatrix(matrix4 _set) { m_viewMatrix = _set; };
	virtual void						SetRenderer(Renderer* _set) { m_Renderer = _set; };
	virtual void						SetId(unsigned short _set) { m_ID = _set; };

	//**FUNCTIONS**//
	virtual void						RemoveLevelObject(BaseObject* _obj) { m_ObjectList.remove(_obj); };
	virtual void						AddLevelObject(BaseObject* _obj) { m_ObjectList.push_back(_obj); };
	virtual void						LoadLevel();
	virtual void						Update();
	virtual void						Shutdown();


};