#pragma once
#include "../ChronoLoop/Objects/BaseObject.h"
#include "../ChronoLoop/Rendering/renderer.h"
#include "../Rendering/Draw2D.h"
#include "../Actions/TimeManipulation.h"
#include "../Input/CommandConsole.h"


namespace Epoch
{


	class Level
	{
		friend class LM;
		BaseObject* mHeadset;
		BaseObject* mController1;
		BaseObject* mController2;
		TimeManipulation* mTMComponent = nullptr;
		//PauseMenu* mPauseMenu = nullptr;
		std::list<BaseObject*> mObjectList;
		unsigned short mId;
		vec4f mStartPosition;
		vec3f mStartRotation;
		unsigned int mMaxNumofClones = 0;

	public:
		Level();
		~Level();
		bool ChronoLoop = true;
		bool mmflip = true;
		Interpolator<matrix4>* playerInterp;

		void AssignPlayerControls(BaseObject *_headset, BaseObject *_lController, BaseObject *_rController);

		//**GETTERS**//
		BaseObject* FindObjectWithName(std::string _name);
		BaseObject* FindObjectWithID(unsigned short _id);
		std::vector<BaseObject*> FindAllObjectsByPattern(std::string _name);
		std::vector<BaseObject*> FindAllObjectsWithName(std::string _name);
		std::list<BaseObject*>& GetLevelObjects() { return mObjectList; };
		void AddObject(BaseObject* _obj);
		bool RemoveObject(BaseObject* _obj);
		unsigned short GetId() { return mId; };
		inline BaseObject* GetHeadset() { return mHeadset; }
		inline BaseObject* GetLeftController() { return mController1; }
		inline BaseObject* GetRightController() { return mController2; }
		inline unsigned int GetMaxClones() { return mMaxNumofClones; }
		inline vec4f& GetStartPos() { return mStartPosition; }
		inline vec4f GetStartRot() { return mStartRotation; }
		TimeManipulation* GetTimeManipulator() { return mTMComponent; }
		//PauseMenu* GetPauseMenu() { return mPauseMenu; }


		//**SETTERS**//
		void SetId(unsigned short _set) { mId = _set; };
		void SetHeadsetAndControllers(BaseObject*& _headset, BaseObject*& _controller1, BaseObject*& _controller2, bool _addNewHeadsetToLevel);
		void SwapPlayerComponentIds(BaseObject *& _first, BaseObject*& _other);

		//**FUNCTIONS**//
		void SetupObjects();
		void CallStart();
		void LoadLevel(std::string _file);
		void Level::BinaryLoadLevel(std::string _file);
		void Update();

		//**CONSOLE COMMAND FUNCS**//
		static void ToggleEntireLevelsWireframe(void * _command, std::wstring _ifOn);
		static void LoadLevelCmnd(void* _commandConsole, std::wstring _Level);

	};

}