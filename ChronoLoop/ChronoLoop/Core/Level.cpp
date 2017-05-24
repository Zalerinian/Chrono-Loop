#include "stdafx.h"
#include "Level.h"
#include "../Actions/TeleportAction.hpp"
#include "../Actions/CCElasticReactionWithPlane.h"
#include "../Actions/BoxSnapToControllerAction.hpp"
#include "../Actions/CCElasticSphereToSphere.h"
#include "../Actions/CCElasticAABBtoAABB.h"
#include "../Actions/CCElasticAABBToSphere.h"
#include "../Actions/TimeManipulation.h"
#include "../Actions/HeadsetFollow.hpp"
#include "../Actions/CodeComponent.hpp"
#include "../Actions/CCButtonPress.h"
#include "../Actions/CCEnterLevel1.h"
#include "../Actions/CCEnterLevel2.h"
#include "../Actions/CCEnterLevel3.h"
#include "../Actions/CCEnterLevel4.h"
#include "../Actions/CCEnterLevel5.h"
#include "../Actions/CCLevel5Button.h"
#include "../Actions/CCLevel5Button2.h"
#include "../Actions/CCLevel5Fields.h"

#include "../Actions/CCEnterLevel5.h"

#include "../Actions/MainMenuBT.h"
#include "../Actions/CCLoadHub.h"
#include "../Actions/CCBoxSpin.h"
#include "../Actions/CCExit.h"
#include "../Actions/CCLevel1TutorialButton.h"
#include "../Actions/CCStartButton.h"
#include "../Objects/MeshComponent.h"
#include "../Objects/LightComponent.h"
#include "../tinyxml/tinyxml.h"
#include "../tinyxml/tinystr.h"
#include "../Common/Settings.h"
#include "../Particles/ParticleSystem.h"
#include "../Input/CommandConsole.h"
#include "../Actions/CCButtonHold.h"
#include "../Core/Pool.h"
#include "../Actions/CCLevel3ElevatorButton.h"
#include "../Actions/CCBoxSpinRandom.h"
#include "../Actions/CCBoxSpinRandomSmall.h"
#include "../Actions/CCLevel3BoxSmash.h"
#include "../Actions/CCPlatformButton.h"
#include "../Actions/CCPlatform.h"
#include "../Actions/CCLevel5DoorButton.h"
#include "../Actions/CCPrevButton.h"
#include "../Actions/CCLevel5BoxFieldCheck.h"

namespace Epoch {

	Level::Level() 
	{
		CommandConsole::Instance().AddCommand(L"/LOAD", LoadLevelCmnd);
	}

	Level::~Level() {
		if (playerInterp)
			delete playerInterp;

		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			delete *it;
		}
		mObjectList.clear();
	}

	void Level::AssignPlayerControls(BaseObject * _headset, BaseObject * _lController, BaseObject * _rController) {
		mHeadset = _headset;
		mController1 = _lController;
		mController2 = _rController;
		playerInterp = new Interpolator<matrix4>();
		CommandConsole::Instance().AddCommand(L"/WIREFRAME", ToggleEntireLevelsWireframe);
		std::vector<Component*> codes1 = mController1->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
		for (size_t x = 0; x < codes1.size(); ++x) {
			if (dynamic_cast<TimeManipulation*>(codes1[x])) {
				mTMComponent = ((TimeManipulation*)codes1[x]);
				break;
			}
		}

		std::vector<Component*> codes2 = mController2->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
		for (size_t x = 0; x < codes2.size(); ++x) {
			if (dynamic_cast<TimeManipulation*>(codes2[x])) {
				if (!mTMComponent) 
					mTMComponent = ((TimeManipulation*)codes2[x]);
					break;
			}
		}
		//std::vector<Component*> codes3 = mHeadset->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
		//for (size_t x = 0; x < codes2.size(); ++x) {
		//	if (dynamic_cast<PauseMenu*>(codes2[x])) {
		//		mPauseMenu = ((PauseMenu*)codes2[x]);
		//		break;
		//	}
		//}
	}

	BaseObject * Level::FindObjectWithName(std::string _name) {
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			if ((*it)->GetName() == _name) {
				return *it;
			}
		}
		return nullptr;
	}

	BaseObject * Level::FindObjectWithID(unsigned short _id) {
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			if ((*it)->GetUniqueID() == _id) {
				return *it;
			}
		}
		return nullptr;
	}

	std::vector<BaseObject*> Level::FindAllObjectsWithName(std::string _name) {
		std::vector<BaseObject*> objects;
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			if ((*it)->GetName() == _name) {
				objects.push_back(*it);
			}
		}
		return objects;
	}

	std::vector<BaseObject*> Level::FindAllObjectsByPattern(std::string _name) {
		std::vector<BaseObject*> objects;
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			if ((*it)->GetName().find(_name) != std::string::npos) {
				objects.push_back(*it);
			}
		}
		return objects;
	}

	void Level::AddObject(BaseObject * _obj) {
		mObjectList.push_back(_obj);
	}

	bool Level::RemoveObject(BaseObject * _obj) {
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			if ((*it) == _obj) {
				mObjectList.erase(it);
				return true;
			}
		}
		return false;
	}
	
	//This func assumes all components are attached in the EXACT order,
	void Level::SwapPlayerComponentIds(BaseObject *& _first, BaseObject*& _other)
	{
		std::vector<Component*> components = _first->GetComponents(eCOMPONENT_COLLIDER);
		std::vector<Component*> othersComponents = _other->GetComponents(eCOMPONENT_COLLIDER);
		//This only works for the controllers because the head doesnt have a collider
		for (unsigned int i = 0; i < othersComponents.size(); i++)
		{
			for (int j = 0; j < components.size(); ++j) {
				if (dynamic_cast<ControllerCollider*>(othersComponents[i]) && dynamic_cast<ControllerCollider*>(components[j]))
				{
					unsigned int FirstCompId = components[j]->GetColliderId();
					components[j]->SetComponentId(othersComponents[i]->GetColliderId());
					othersComponents[i]->SetComponentId(FirstCompId);
				}
			}
		}


		components = _first->GetComponents(eCOMPONENT_CODE);
		othersComponents = _other->GetComponents(eCOMPONENT_CODE);

		for (unsigned int i = 0; i < othersComponents.size(); i++)
		{
			for (int j = 0; j < components.size(); ++j) {
				if (othersComponents[i] && components[j] && dynamic_cast<BoxSnapToControllerAction*>(othersComponents[i]) && dynamic_cast<BoxSnapToControllerAction*>(components[j]))
				{
					unsigned int FirstCompId = components[j]->GetColliderId();
					components[j]->SetComponentId(othersComponents[i]->GetColliderId());
					othersComponents[i]->SetComponentId(FirstCompId);
				}

			}
		}

	
		components = _first->GetComponents(eCOMPONENT_MESH);
		othersComponents = _other->GetComponents(eCOMPONENT_MESH);
		if (components.size() > 0)
		{
			for (int i = 0; i < othersComponents.size(); ++i) {
				unsigned int FirstCompId = components[i]->GetColliderId();
				components[i]->SetComponentId(othersComponents[i]->GetColliderId());
				othersComponents[i]->SetComponentId(FirstCompId);
			}
		}
	}

	void Level::SetHeadsetAndControllers(BaseObject *& _headset, BaseObject *& _controller1, BaseObject *& _controller2, bool _addNewHeadsetToLevel) {
		//Swap component ids
		SwapPlayerComponentIds(mHeadset, _headset);
		SwapPlayerComponentIds(mController1, _controller1);
		SwapPlayerComponentIds(mController2, _controller2);
		//Set the new BaseObjects to the current controller so new objects can follow old controller movement as clones.
		unsigned short headid = _headset->GetUniqueID();
		unsigned short cl1id = _controller1->GetUniqueID();
		unsigned short cl2id = _controller2->GetUniqueID();


		std::string headname = _headset->GetName();
		std::string Controller1name = _controller1->GetName();
		std::string Controller2name = _controller2->GetName();


		_headset->SetUniqueID(mHeadset->GetUniqueID());
		_controller1->SetUniqueID(mController1->GetUniqueID());
		_controller2->SetUniqueID(mController2->GetUniqueID());
		_headset->SetName(mHeadset->GetName());
		_controller1->SetName(mController1->GetName());
		_controller2->SetName(mController2->GetName());
		

		mHeadset->SetUniqueID(headid);
		mController1->SetUniqueID(cl1id);
		mController2->SetUniqueID(cl2id);
		mHeadset->SetName(headname);
		mController1->SetName(Controller1name);
		mController2->SetName(Controller2name);

		//Update the clone pair of the new baseObjects if it already exist
		Clonepair* temp = TimeManager::Instance()->GetClonePair(mHeadset->GetUniqueID());
		if (temp)
		{
			temp->mCur = _headset->GetUniqueID();
			temp->mOther1 = _controller1->GetUniqueID();
			temp->mOther2 = _controller2->GetUniqueID();
			//remove the one on the old key and create a new one with the new key
			TimeManager::Instance()->EraseClonePair(mHeadset->GetUniqueID());
			TimeManager::Instance()->SetClonePair(_headset->GetUniqueID(), temp);
		}

		temp = TimeManager::Instance()->GetClonePair(mController1->GetUniqueID());
		if (temp)
		{
			temp->mCur = _controller1->GetUniqueID();
			temp->mOther1 = _headset->GetUniqueID();
			temp->mOther2 = _controller2->GetUniqueID();
			//remove the one on the old key and create a new one with the new key
			TimeManager::Instance()->EraseClonePair(mController1->GetUniqueID());
			TimeManager::Instance()->SetClonePair(_controller1->GetUniqueID(), temp);
		}

		temp = TimeManager::Instance()->GetClonePair(mController2->GetUniqueID());
		if (temp)
		{
			temp->mCur = _controller2->GetUniqueID();
			temp->mOther1 = _controller1->GetUniqueID();
			temp->mOther2 = _headset->GetUniqueID();
			//remove the one on the old key and create a new one with the new key
			TimeManager::Instance()->EraseClonePair(mController2->GetUniqueID());
			TimeManager::Instance()->SetClonePair(_controller2->GetUniqueID(), temp);
		}
	
	
		if (_addNewHeadsetToLevel)
		{
			mObjectList.push_back(_headset);
			mObjectList.push_back(_controller1);
			mObjectList.push_back(_controller2);
		}
		////Add the headset and controllers to the time manager with their new ids
		TimeManager::Instance()->AddObjectToTimeline(mHeadset);
		TimeManager::Instance()->AddObjectToTimeline(mController1);
		TimeManager::Instance()->AddObjectToTimeline(mController2);

	}

	void Level::SetupObjects()
	{
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			auto& meshes = (*it)->GetComponents(eCOMPONENT_MESH);
			auto& lights = (*it)->GetComponents(eCOMPONENT_LIGHT);
			auto b = meshes.begin();
			auto e = meshes.end();
			for (auto cit = meshes.begin(); cit != meshes.end(); ++cit) {
				MeshComponent* mesh = (MeshComponent*)*cit;
				if(mesh->IsVisible())
				{
					mesh->SetVisible(false)->SetVisible(true);
				}
			}
			for (auto cit = lights.begin(); cit != lights.end(); ++cit) {
				LightComponent *light = (LightComponent*)*cit;
				if (light->IsVisible()) {
					light->SetVisible(false)->SetVisible(true);
				}
			}
		}
	}

	void Level::CallStart() {
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			auto codes = (*it)->GetComponents(eCOMPONENT_CODE);
			for (auto oit = codes.begin(); oit != codes.end(); ++oit) {
				((CodeComponent*)*oit)->Start();
			}
		}
	}

	void Level::BinaryLoadLevel(std::string _file)
	{
		Settings::GetInstance().SetBool("CantTeleport", false);
		std::ifstream file(_file, std::ios::in | std::ios::binary);
		if (file.is_open())
		{
			UINT32 version = 0;
			INT32 settingOffest = 0, objectOffest = 0;
			file.read((char *)&version, sizeof(UINT32));
			file.read((char *)&settingOffest, sizeof(INT32));
			file.read((char *)&objectOffest, sizeof(INT32));
			//settings
			file.seekg(settingOffest, std::ios_base::beg);
			vec3f startPosition, startRotation, objectScale;
			unsigned short MaxClones = 0;
			file.read((char *)&startPosition.x, sizeof(float));
			file.read((char *)&startPosition.y, sizeof(float));
			file.read((char *)&startPosition.z, sizeof(float));

			file.read((char *)&startRotation.x, sizeof(float));
			file.read((char *)&startRotation.y, sizeof(float));
			file.read((char *)&startRotation.z, sizeof(float));
			if (version >= 4) {
				file.read((char *)&MaxClones, sizeof(unsigned short));;
			}
			mMaxNumofClones = MaxClones;
			mStartPosition = startPosition;
			mStartRotation = startRotation;
			//Objects
			file.seekg(objectOffest, std::ios_base::beg);
			INT32 objectCount = 0;
			file.read((char *)&objectCount, sizeof(INT32));

			BaseObject* parent = nullptr;
			INT8 objOperation = 0;

			for (INT32 i = 0; i < objectCount; i++)
			{
				BaseObject* obj = nullptr;
				//Components
				INT32 componentCount = 0;
				file.read((char *)&componentCount, sizeof(INT32));
				for (INT32 j = 0; j < componentCount; j++)
				{
					INT16 type = 0;
					file.read((char *)&type, sizeof(INT16));
					switch (type)
					{
					case 0: //None
						break;
					case 1: //BoxCollider
					{
						float mass = 0, staticFriction = 0, kineticFriction = 0, elasticity = 0, drag = 0;
						byte movable = 0, trigger = 0, pickupable = false;
						file.read((char *)&mass, sizeof(float));
						file.read((char *)&staticFriction, sizeof(float));
						file.read((char *)&kineticFriction, sizeof(float));
						file.read((char *)&elasticity, sizeof(float));
						file.read((char *)&drag, sizeof(float));
						file.read((char *)&movable, sizeof(byte));
						file.read((char *)&trigger, sizeof(byte));
						if (version >= 4) {
							file.read((char *)&pickupable, sizeof(byte));
						}

						vec3f position, rotation, scale, gravity(0, -9.81f, 0);
						file.read((char *)&position.x, sizeof(float));
						file.read((char *)&position.y, sizeof(float));
						file.read((char *)&position.z, sizeof(float));

						file.read((char *)&rotation.x, sizeof(float));
						file.read((char *)&rotation.y, sizeof(float));
						file.read((char *)&rotation.z, sizeof(float));

						file.read((char *)&scale.x, sizeof(float));
						file.read((char *)&scale.y, sizeof(float));
						file.read((char *)&scale.z, sizeof(float));

						if(version >= 5)
						{
							file.read((char *)&gravity.x, sizeof(float));
							file.read((char *)&gravity.y, sizeof(float));
							file.read((char *)&gravity.z, sizeof(float));
						}
						if (obj)
						{
							std::string temp = obj->GetName();
							vec3f offset = vec3f(objectScale.x * scale.x, objectScale.y * scale.y, objectScale.z * scale.z) / 2;
							vec3f min = position - offset;
							vec3f max = position + offset;
							CubeCollider* col = new CubeCollider(obj, movable == 1, trigger == 1, gravity, mass, elasticity, staticFriction, kineticFriction, drag, min, max);
							col->mPickUpAble = pickupable != 0;
							obj->AddComponent(col);
						}
					}
						break;
					case 2: //ButtonCollider
					{
						float mass = 0, force = 0;
						file.read((char *)&mass, sizeof(float));
						file.read((char *)&force, sizeof(float));

						vec3f position, scale, normal;
						file.read((char *)&position.x, sizeof(float));
						file.read((char *)&position.y, sizeof(float));
						file.read((char *)&position.z, sizeof(float));

						file.read((char *)&scale.x, sizeof(float));
						file.read((char *)&scale.y, sizeof(float));
						file.read((char *)&scale.z, sizeof(float));

						file.read((char *)&normal.x, sizeof(float));
						file.read((char *)&normal.y, sizeof(float));
						file.read((char *)&normal.z, sizeof(float));
						if (obj)
						{
							vec3f offset = vec3f(objectScale.x * scale.x, objectScale.y * scale.y, objectScale.z * scale.z) / 2;
							vec3f min = position - offset;
							vec3f max = position + offset;
							ButtonCollider* col = new ButtonCollider(obj, min, max, mass, force, normal);
							obj->AddComponent(col);
						}
					}
						break;
					case 3: //PlaneCollider
					{
						float mass = 0, staticFriction = 0, kineticFriction = 0, elasticity = 0, drag = 0, offset = 0;
						byte movable = 0, trigger = 0, pickupable = 0;
						file.read((char *)&mass, sizeof(float));
						file.read((char *)&staticFriction, sizeof(float));
						file.read((char *)&kineticFriction, sizeof(float));
						file.read((char *)&elasticity, sizeof(float));
						file.read((char *)&drag, sizeof(float));
						file.read((char *)&movable, sizeof(byte)); // This is ignored, but it's written for Plane Colliders anyway.
						file.read((char *)&trigger, sizeof(byte));
						if (version >= 4) {
							file.read((char *)&pickupable, sizeof(byte));
						}
						file.read((char *)&offset, sizeof(float));

						vec3f normal;
						file.read((char *)&normal.x, sizeof(float));
						file.read((char *)&normal.y, sizeof(float));
						file.read((char *)&normal.z, sizeof(float));
						if (obj)
						{
							PlaneCollider* col = new PlaneCollider(obj, trigger == 1, staticFriction, kineticFriction, offset, normal);//TODO: Fix offset
							obj->AddComponent(col);
						}
					}
						break;
					case 4: //SphereCollider
					{
						float mass = 0, staticFriction = 0, kineticFriction = 0, elasticity = 0, drag = 0, radius = 0;
						byte movable = 0, trigger = 0, pickupable = 0;
						file.read((char *)&mass, sizeof(float));
						file.read((char *)&staticFriction, sizeof(float));
						file.read((char *)&kineticFriction, sizeof(float));
						file.read((char *)&elasticity, sizeof(float));
						file.read((char *)&drag, sizeof(float));
						file.read((char *)&movable, sizeof(byte));
						file.read((char *)&trigger, sizeof(byte));
						if (version >= 4) {
							file.read((char *)&pickupable, sizeof(byte));
						}
						file.read((char *)&radius, sizeof(float));

						vec3f position;
						file.read((char *)&position.x, sizeof(float));
						file.read((char *)&position.y, sizeof(float));
						file.read((char *)&position.z, sizeof(float));

						vec3f gravity;
						if (version >= 5) {
							file.read((char *)&gravity.x, sizeof(float));
							file.read((char *)&gravity.y, sizeof(float));
							file.read((char *)&gravity.z, sizeof(float));
						}

						if (obj)
						{
							SphereCollider* col = new SphereCollider(obj, movable == 1, trigger == 1, gravity, mass, elasticity, staticFriction, kineticFriction, drag, radius);
							obj->AddComponent(col);
						}
					}
						break;
					case 5: //ColoredMesh
					{
						INT32 pathLength = 0, argbColor = 0;
						std::string mesh;

						float transparency = 1.0;
						if (version >= 2) {
							file.read((char*)&transparency, sizeof(float));
						}

						PixelShaderFormat psf = ePS_TEXTURED;
						VertexShaderFormat vsf = eVS_TEXTURED;
						GeometryShaderFormat gsf = eGS_PosNormTex;

						// Shaders - All stored as 1 byte.	
						if (version >= 3) {
							file.read((char*)&psf, 1);
							file.read((char*)&vsf, 1);
							file.read((char*)&gsf, 1);
						}

						file.read((char *)&pathLength, sizeof(INT32));
						char* temp = new char[pathLength];
						file.read(temp, pathLength);
						mesh = temp;
						delete[] temp;
						file.read((char *)&argbColor, sizeof(INT32));
						//Was informed to ignore it
						//if (obj)
						//{
						//	std::string path = "../Resources/";
						//	path.append(mesh);
						//	MeshComponent* mesh = new MeshComponent(path.c_str());
						//	obj->AddComponent(mesh);
						//}
					}
						break;
					case 6: //TexturedMesh
					{
						INT32 pathLength = 0;
						std::string mesh, diffuse, emissive, specular, normal;
						float transparency = 1.0f;
						if (version >= 2) {
							file.read((char*)&transparency, sizeof(float));
						}

						PixelShaderFormat psf = ePS_TEXTURED;
						VertexShaderFormat vsf = eVS_TEXTURED;
						GeometryShaderFormat gsf = eGS_PosNormTex;

						// Shaders - All stored as 1 byte.	
						if (version >= 3) {
							file.read((char*)&psf, 1);
							file.read((char*)&vsf, 1);
							file.read((char*)&gsf, 1);
						}



						// Mesh file
						file.read((char *)&pathLength, sizeof(INT32));
						char* temp = new char[pathLength];
						file.read(temp, pathLength);
						mesh = temp;
						delete[] temp;

						// Diffuse
						file.read((char *)&pathLength, sizeof(INT32));
						temp = new char[pathLength];
						file.read(temp, pathLength);
						diffuse = temp;
						delete[] temp;

						// Emissive
						// Todo: Add a check for if there is no emissive texture.
						if (version >= 2) {
							file.read((char *)&pathLength, sizeof(INT32));
							temp = new char[pathLength];
							file.read(temp, pathLength);
							emissive = temp;
							delete[] temp;
						}

						if (version >= 6) {
							file.read((char*)&pathLength, sizeof(INT32));
							temp = new char[pathLength];
							file.read(temp, pathLength);
							specular = temp;
							delete[] temp;
						}

						if (version >= 7) {
							file.read((char*)&pathLength, sizeof(INT32));
							temp = new char[pathLength];
							file.read(temp, pathLength);
							normal = temp;
							delete[] temp;
						}

						if (obj)
						{
							MeshComponent* mc = new MeshComponent(mesh.c_str(), transparency, psf, vsf, gsf);
							mc->AddTexture(diffuse.c_str(), eTEX_DIFFUSE);
							if (version >= 2 && emissive != "..\\Resources\\") {
								mc->AddTexture(emissive.c_str(), eTEX_EMISSIVE);
							}
							if (version >= 6 && specular != "..\\Resources\\") {
								mc->AddTexture(specular.c_str(), eTEX_SPECULAR);
							}
							if (version >= 7 && normal != "..\\Resources\\") {
								mc->AddTexture(normal.c_str(), eTEX_NORMAL);
							}
							obj->AddComponent(mc);
						}
					}
						break;
					case 7: //Transform
					{
						INT32 len = 0;
						std::string name;
						vec3f position, rotation;

						file.read((char *)&len, sizeof(INT32));
						char* temp = new char[len];
						file.read(temp, len);
						name = temp;
						delete[] temp;

						file.read((char *)&position.x, sizeof(float));
						file.read((char *)&position.y, sizeof(float));
						file.read((char *)&position.z, sizeof(float));

						file.read((char *)&rotation.x, sizeof(float));
						file.read((char *)&rotation.y, sizeof(float));
						file.read((char *)&rotation.z, sizeof(float));

						file.read((char *)&objectScale.x, sizeof(float));
						file.read((char *)&objectScale.y, sizeof(float));
						file.read((char *)&objectScale.z, sizeof(float));

						INT8 recorded = 0;
						unsigned int flags = 0;
						if (version >= 2) {
							file.read((char*)&recorded, sizeof(recorded));
							if (recorded != 0) {
								flags = BaseObject_Flag_Record_In_Timeline;
							}
						}

						matrix4 mat = matrix4::CreateNewScale(objectScale.x, objectScale.y, objectScale.z) *
							matrix4::CreateNewYawPitchRollRotation(rotation) *
							matrix4::CreateNewTranslation(position.x, position.y, position.z);
						Transform trans;
						trans.SetMatrix(mat);
						obj = Pool::Instance()->iGetObject()->Reset(name, trans, nullptr, flags);
						//obj = new BaseObject(name, trans);
						int i = 0;
					}
						break;
					case 8: //Code
					{
						INT32 len = 0;
						std::string path;

						file.read((char *)&len, sizeof(INT32));
						char* temp = new char[len];
						file.read(temp, len);
						path = temp;
						delete[] temp;
						if (obj)
						{
							CodeComponent* codeCom = nullptr;
							if (path == "TimeManipulation.h")
								codeCom = new TimeManipulation();
							if (path == "BoxSnapControllerAction.hpp")
								codeCom = new BoxSnapToControllerAction();
							if (path == "CCBoxSpin.h")
								codeCom = new CCBoxSpin();
							if (path == "CCBoxSpinRandom.h")
								codeCom = new CCBoxSpinRandom();
							if (path == "CCBoxSpinRandomSmall.h")
								codeCom = new CCBoxSpinRandomSmall();
							if (path == "CCButtonHold.h")
								codeCom = new CCButtonHold();
							if (path == "CCButtonPress.h")
								codeCom = new CCButtonPress();
							if (path == "CCDisplayOnPause.h")
								codeCom = new CCDisplayOnPause();
							if (path == "CCElasticAABBtoAABB.h")
								codeCom = new CCElasticAABBtoAABB();
							if (path == "CCElasticAABBtoSphere.h")
								codeCom = new CCElasticAABBToSphere();
							if (path == "CCElasticReactionWithPlane.h")
								codeCom = new CCElasticReactionWithPlane();
							if (path == "CCElasticSphereToSphere.h")
								codeCom = new CCElasticSphereToSphere();
							if (path == "CCEnterLevel1.h")
								codeCom = new CCEnterLevel1();
							if (path == "CCEnterLevel2.h")
								codeCom = new CCEnterLevel2();
							if (path == "CCEnterLevel3.h")
								codeCom = new CCEnterLevel3();
							if (path == "CCEnterLevel4.h")
								codeCom = new CCEnterLevel4();
							if (path == "CCEnterLevel5.h")
								codeCom = new CCEnterLevel5();
							if (path == "CCExit.h")
								codeCom = new CCExit();
							if (path == "CCLevel3BoxSmash.h")
								codeCom = new CCLevel3BoxSmash();
							if (path == "CCLevel3ElevatorButton.h")
								codeCom = new CCLevel3ElevatorButton();
							if (path == "CCLoadHub.h")
								codeCom = new CCLoadHub();
							if (path == "CCMazeHelper.h")
								codeCom = new CCMazeHelper();
							if (path == "CCPauseToCancel.h")
								codeCom = new CCPauseToCancel();
							if (path == "CCStartButton.h")
								codeCom = new CCStartButton();
							if (path == "CCPrevButton.h")
								codeCom = new CCPrevButton();
							if (path == "CCTeleToPlay.h")
								codeCom = new CCTeleToPlay();
							if (path == "CodeComponent.hpp")
								codeCom = new CodeComponent();
							if (path == "HeadsetFollow.hpp")
								codeCom = new HeadsetFollow();
							if (path == "UIClonePlusToMinus.h")
								codeCom = new UIClonePlusToMinus();
							if (path == "UICloneText.h")
								codeCom = new UICloneText();
							if (path == "UICreateToDeleteClone.h")
								codeCom = new UICreateToDeleteClone();
							if (path == "UIRewind.h")
								codeCom = new UIRewind();
							if (path == "CCLevel1TutorialButton.h")
								codeCom = new CCLevel1TutorialButton();
							if (path == "CCPlatformButton.h")
								codeCom = new CCPlatformButton();
							if (path == "CCPlatform.h")
								codeCom = new CCPlatform();
							if (path == "CCLevel5Button.h")
								codeCom = new CCLevel5Button();
							if (path == "CCLevel5Button2.h")
								codeCom = new CCLevel5Button2();
							if (path == "CCLevel5Fields.h")
								codeCom = new CCLevel5Fields();
							if (path == "CCLevel5DoorButton.h")
								codeCom = new CCLevel5DoorButton();
							if (path == "CCLevel5BoxFieldCheck.h")
								codeCom = new CCLevel5BoxFieldCheck();

							if (codeCom)
							{
								obj->AddComponent(codeCom);
							}
						}
					}
						break;
					case 9: //Audio
					{
						INT32 numSounds = 0;
						file.read((char *)&numSounds, sizeof(INT32));
						AudioEmitter* sound = nullptr;
						if (obj)
							sound = new AudioEmitter();
						for (INT32 k = 0; k < numSounds; k++)
						{
							char playType = 0;
							UINT64 id = 0;
							file.read(&playType, sizeof(char));
							file.read((char *)&id, sizeof(UINT64));
							if (sound)
								sound->AddEvent((AudioEmitter::EventType)playType, id);
						}
						if (sound)
						{
							AudioWrapper::GetInstance().AddEmitter(sound, (obj->GetName() + "_SOUND").c_str());
							obj->AddComponent(sound);
						}
					}
						break;
					case 10:
					{
						// Light component, a glorified colored mesh component.
						INT32 pathLength = 0;
						std::string mesh;

						struct {
							union {
								struct {
									unsigned char b, g, r, a;
								};
								int color;
							};
						} argbColor;

						float transparency = 1.0;
						if (version >= 2) {
							file.read((char*)&transparency, sizeof(float));
						}

						unsigned char waste = 0;
						// Shaders - All stored as 1 byte.	
						if (version >= 3) {
							file.read((char*)&waste, 1);
							file.read((char*)&waste, 1);
							file.read((char*)&waste, 1);
						}

						file.read((char *)&pathLength, sizeof(INT32));
						char* temp = new char[pathLength];
						file.read(temp, pathLength);
						mesh = temp;
						delete[] temp;
						file.read((char *)&argbColor.color, sizeof(INT32));
						if (obj) {
							std::string path = "../Resources/" + mesh;
							vec4f lightColor(argbColor.r / 255.f * transparency, argbColor.g / 255.f * transparency, argbColor.b / 255.f * transparency, 1.0f);
							LightComponent *light = new LightComponent(path.c_str(), lightColor);
							obj->AddComponent(light);
						}
					}
						break;
					default:
						break;
					}
				}
				if (parent == nullptr) {
					AddObject(obj);
				}
				file.read((char *)&objOperation, sizeof(INT8));
				if (objOperation == 0 && parent)
				{
					parent->AddChild(obj);
				}
				if (objOperation == 1)
					parent = obj;
				if (objOperation == 2 && parent)
					parent = parent->GetParent();
			}
			file.close();
		}
	}

	void Level::Update() {
		//*Insert Code Here When we Get to It**//
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			(*it)->Update();
		}
	}
	void Level::ToggleEntireLevelsWireframe(void* _command, std::wstring _ifOn) {
		CommandConsole* cc = (CommandConsole*)_command;
		if (_ifOn == L"ON") {
			Settings::GetInstance().SetInt("RasterizerStateOverride", eRS_WIREFRAME);
			CommandConsole::Instance().DisplaySet(L"Wireframe ON");
		}
		else if (_ifOn == L"OFF") {
			Settings::GetInstance().SetInt("RasterizerStateOverride", eRS_MAX);
			CommandConsole::Instance().DisplaySet(L"Wireframe OFF");
		}
		else {
			CommandConsole::Instance().DisplaySet(L"INVALID INPUT: " + _ifOn + L"\nCORRECT INPUT: /WIREFRAME (ON/OFF)");
		}
	}

	void Level::LoadLevelCmnd(void* _commandConsole, std::wstring _Level)
	{
		CommandConsole* self = (CommandConsole*)_commandConsole;

		CCEnterLevel1* accessLevelOne = nullptr;
		CCEnterLevel2* accessLevelTwo = nullptr;
		CCEnterLevel3* accessLevelThree = nullptr;
		CCEnterLevel4* accessLevelFour = nullptr;
		CCEnterLevel5* accessLevelFive = nullptr;
		CCLoadHub* accessHub = nullptr;
		std::list<BaseObject*> copyList = LevelManager::GetInstance().GetCurrentLevel()->GetLevelObjects();


		for (auto it = copyList.begin(); it != copyList.end(); ++it) {
			std::vector<Component*> CodeComps = (*it)->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
			if (CodeComps.size() > 0) 
			{
				for (size_t x = 0; x < CodeComps.size(); ++x) 
				{
					if (dynamic_cast<CCEnterLevel1*>(CodeComps[x])) 
					{
						accessLevelOne = ((CCEnterLevel1*)CodeComps[x]);
						break;
					}
					else if (dynamic_cast<CCEnterLevel2*>(CodeComps[x]))
					{
						accessLevelTwo = ((CCEnterLevel2*)CodeComps[x]);
						break;
					}
					else if (dynamic_cast<CCEnterLevel3*>(CodeComps[x]))
					{
						accessLevelThree = ((CCEnterLevel3*)CodeComps[x]);
						break;
					}
					else if (dynamic_cast<CCEnterLevel4*>(CodeComps[x]))
					{
						accessLevelFour = ((CCEnterLevel4*)CodeComps[x]);
						break;
					}
					else if (dynamic_cast<CCEnterLevel5*>(CodeComps[x]))
					{
						accessLevelFive = ((CCEnterLevel5*)CodeComps[x]);
						break;
					}
				}
				if (accessLevelOne &&
					accessLevelTwo &&
					accessLevelThree && 
					accessLevelFour &&
					accessLevelFive)
					break;
			}
		}

		//for (auto it = copyList.begin(); it != copyList.end(); ++it)
		//{
		//	std::vector<Component*> CodeComps = (*it)->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
		//	if (CodeComps.size() > 0)
		//	{
		//		for (size_t x = 0; x < CodeComps.size(); ++x)
		//		{
		//			if (dynamic_cast<CCEnterLevel*>(CodeComps[x]))
		//			{
		//				accessLevelTwo = ((CCEnterLevel*)CodeComps[x]);
		//				break;
		//			}
		//		}
		//		if (accessLevelTwo != nullptr)
		//			break;
		//	}
		//}

		//for (auto it = copyList.begin(); it != copyList.end(); ++it)
		//{
		//	std::vector<Component*> CodeComps = (*it)->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
		//	if (CodeComps.size() > 0)
		//	{
		//		for (size_t x = 0; x < CodeComps.size(); ++x)
		//		{
		//			if (dynamic_cast<CCEnterLevel3*>(CodeComps[x]))
		//			{
		//				accessLevelThree = ((CCEnterLevel3*)CodeComps[x]);
		//				break;
		//			}
		//		}
		//		if (accessLevelThree != nullptr)
		//			break;
		//	}
		//}

		for (auto it = copyList.begin(); it != copyList.end(); ++it) {
			std::vector<Component*> CodeComps = (*it)->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
			if (CodeComps.size() > 0) {
				for (size_t x = 0; x < CodeComps.size(); ++x) {
					if (dynamic_cast<CCLoadHub*>(CodeComps[x])) 
					{
						accessHub = ((CCLoadHub*)CodeComps[x]);
						break;
					}
				}
				if (accessHub != nullptr)
					break;
			}
		}

		//for (auto it = copyList.begin(); it != copyList.end(); ++it)
		//{
		//	std::vector<Component*> CodeComps = (*it)->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
		//	if (CodeComps.size() > 0)
		//	{
		//		for (size_t x = 0; x < CodeComps.size(); ++x)
		//		{
		//			if (dynamic_cast<CCLoadTutorial*>(CodeComps[x]))
		//			{
		//				accessLevelFour = ((CCEnterLevel3*)CodeComps[x]);
		//				break;
		//			}
		//		}
		//		if (accessLevelFour != nullptr)
		//			break;
		//	}
		//}

		if ((_Level == L"LEVELONE" || _Level == L"LVLONE"))
		{
			if (accessLevelOne)
			{

				accessLevelOne->SetOnce(false);
				CommandConsole::Instance().Toggle();
			}
			else
			{
				CommandConsole::Instance().DisplaySet(L"Failed to load level 1.");
			}
			accessLevelOne = nullptr;
			accessLevelTwo = nullptr;
			accessLevelThree = nullptr;
			accessLevelFour = nullptr;
			accessLevelFive = nullptr;
			accessHub = nullptr;
		}
		else if ((_Level == L"LEVELTWO" || _Level == L"LVLTWO"))
		{
			if (accessLevelTwo) {
				accessLevelTwo->SetOnce(false);
				CommandConsole::Instance().Toggle();
			} else {
				CommandConsole::Instance().DisplaySet(L"Failed to load level 2.");
			}
			accessLevelOne = nullptr;
			accessLevelTwo = nullptr;
			accessLevelThree = nullptr;
			accessLevelFour = nullptr;
			accessLevelFive = nullptr;
			accessHub = nullptr;
		}
		else if ((_Level == L"LEVELTHREE" || _Level == L"LVLTHREE"))
		{
			if (accessLevelThree)
			{
				accessLevelThree->SetOnce(false);
				CommandConsole::Instance().Toggle();
			}
			else
			{
				CommandConsole::Instance().DisplaySet(L"Failed to load level 3.");
			}
			accessLevelOne = nullptr;
			accessLevelTwo = nullptr;
			accessLevelThree = nullptr;
			accessLevelFour = nullptr;
			accessLevelFive = nullptr;
			accessHub = nullptr;
		}
		else if ((_Level == L"LEVELFOUR" || _Level == L"LVLFOUR"))
		{
			if (accessLevelFour)
			{
				accessLevelFour->SetOnce(false);
				CommandConsole::Instance().Toggle();
			}
			else
			{
				CommandConsole::Instance().DisplaySet(L"Failed to load level 4.");
			}
			accessLevelOne = nullptr;
			accessLevelTwo = nullptr;
			accessLevelThree = nullptr;
			accessLevelFour = nullptr;
			accessLevelFive = nullptr;
			accessHub = nullptr;
		}
		else if ((_Level == L"LEVELFIVE" || _Level == L"LVLFIVE"))
		{
			if (accessLevelFive)
			{
				accessLevelFive->SetOnce(false);
				CommandConsole::Instance().Toggle();
			}
			else
			{
				CommandConsole::Instance().DisplaySet(L"Failed to load level 5.");
			}
			accessLevelOne = nullptr;
			accessLevelTwo = nullptr;
			accessLevelThree = nullptr;
			accessLevelFour = nullptr;
			accessLevelFive = nullptr;
			accessHub = nullptr;
		}
		else if ((_Level == L"HUBWORLD" || _Level == L"HUB"))
		{
			if (accessHub) {
				accessHub->SetOnce(false);
				CommandConsole::Instance().Toggle();
			} else {
				CommandConsole::Instance().DisplaySet(L"Failed to load hub.");
			}
			accessLevelOne = nullptr;
			accessLevelTwo = nullptr;
			accessLevelThree = nullptr;
			accessLevelFour = nullptr;
			accessLevelFive = nullptr;
			accessHub = nullptr;
		}
	}


} // Epoch Namespace