#pragma once
////////////////////
//2/9/2017
//Written by: Ryan Bronk & Drew Ritter
///////////////////
#include "../ChronoLoop/Objects/BaseObject.h"
#include "../ChronoLoop/Rendering/renderer.h"
#include "../Rendering/Draw2D.h"
#include "../Actions/TimeManipulation.h"
#include "../Input/CommandConsole.h"


namespace Epoch
{


	class Level
	{
		BaseObject* mHeadset;
		BaseObject* mController1;
		BaseObject* mController2;
		TimeManipulation* mTMComponent1 = nullptr, *mTMComponent2 = nullptr;
		std::list<BaseObject*> mObjectList;
		unsigned short mId;


	public:
		Level();
		~Level();
		bool ChronoLoop = true;
		bool mmflip = true;

		void Initialize(BaseObject *_headset, BaseObject *_lController, BaseObject *_rController);

		//**GETTERS**//
		BaseObject* FindObjectWithName(std::string _name);
		std::vector<BaseObject*> FindAllObjectsWithName(std::string _name);
		std::list<BaseObject*> GetLevelObjects() { return mObjectList; };
		void AddObject(BaseObject* _obj);
		bool RemoveObject(BaseObject* _obj);
		unsigned short GetId() { return mId; };
		inline BaseObject* GetHeadset() { return mHeadset; }
		inline BaseObject* GetLeftController() { return mController1; }
		inline BaseObject* GetRightController() { return mController2; }
		TimeManipulation* GetLeftTimeManinpulator() { return mTMComponent1; }
		TimeManipulation* GetRightTimeManinpulator() { return mTMComponent2; }

		//**SETTERS**//
		void SetId(unsigned short _set) { mId = _set; };
		void SetHeadsetAndControllers(BaseObject*& _headset, BaseObject*& _controller1, BaseObject*& _controller2, ControllerCollider* _c1Collider, ControllerCollider* _c2Collider);

		//**FUNCTIONS**//
		void CallStart();
		void LoadLevel(std::string _file);
		void Update();

		//**CONSOLE COMMAND FUNCS**//
		static void ToggleEntireLevelsWireframe(void * _command, std::wstring _ifOn);

	};

}