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
#include "../Actions/CCEnterLevel.h"
#include "../Actions/CCEnterLevel1.h"
#include "../Actions/MainMenuBT.h"
#include "../Actions/CCLoadHub.h"
#include "../Actions/CCBoxSpin.h"
#include "../Actions/CCExit.h"
#include "../Actions/CCStartButton.h"
#include "../Objects/MeshComponent.h"
#include "../tinyxml/tinyxml.h"
#include "../tinyxml/tinystr.h"
#include "../Common/Settings.h"
#include "../Particles/ParticleSystem.h"
#include "../Input/CommandConsole.h"
#include "../Actions/CCButtonHold.h"

namespace Epoch {

	CCEnterLevel* accessLevelTwo = nullptr;
	CCEnterLevel1* accessLevelOne = nullptr;
	CCLoadHub* accessHub = nullptr;

	Level::Level() 
	{
		CommandConsole::Instance().AddCommand(L"/LOAD", LoadLevelCmnd);
	}

	Level::~Level() {
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			delete *it;
		}
		mObjectList.clear();
	}

	void Level::AssignPlayerControls(BaseObject * _headset, BaseObject * _lController, BaseObject * _rController) {
		mHeadset = _headset;
		mController1 = _lController;
		mController2 = _rController;
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

	std::vector<BaseObject*> Level::FindAllObjectsWithName(std::string _name) {
		std::vector<BaseObject*> objects;
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			if ((*it)->GetName() == _name) {
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
				if (dynamic_cast<BoxSnapToControllerAction*>(othersComponents[i]) && dynamic_cast<BoxSnapToControllerAction*>(components[j]))
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
			for (auto cit = meshes.begin(); cit != meshes.end(); ++cit) {
				((MeshComponent*)(*cit))->SetVisible(false);
				((MeshComponent*)(*cit))->SetVisible(true);
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

	void Level::LoadLevel(std::string _file)
	{
		// Load the xml file, I put your XML in a file named test.xml
		TiXmlDocument XMLdoc(_file.c_str());
		bool loadOkay = XMLdoc.LoadFile();
		if (loadOkay)
		{
			SystemLogger::GetLog() << _file.c_str() << " loaded" << std::endl;
			mStartPosition.w = 1; // In the event a level doesn't define the start position, this is needed to mess up the matrix.
			mStartRotation.w = 1;
			TiXmlElement *pRoot, *pObject, *pData;
			pRoot = XMLdoc.FirstChildElement("Level");
			if (pRoot)
			{
				// Get Level Settings
				pObject = pRoot->FirstChildElement("Settings");
				if (pObject != nullptr) {
					pData = pObject->FirstChildElement();
					std::string nodeType;
					while (pData) {
						switch (pData->Type()) {
						case TiXmlNode::NodeType::TINYXML_ELEMENT:
							nodeType = pData->Value();
							pData = (TiXmlElement*)pData->FirstChild();
							if (nodeType == "StartPos") {
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									mStartPosition.xyzw[i] = std::strtof(token.c_str(), nullptr);
									i++;
									s.erase(0, pos + 1);
								}
								SystemLogger::Debug() << "Start position: " << mStartPosition << std::endl;
							} else if (nodeType == "StartRot") {
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									mStartRotation.xyzw[i] = std::strtof(token.c_str(), nullptr);
									i++;
									s.erase(0, pos + 1);
								}
								SystemLogger::Debug() << "Start rotation: " << mStartRotation << std::endl;
							}
							else if (nodeType == "MaxClones") {
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									mMaxNumofClones = std::strtol(token.c_str(), nullptr, 10);
									i++;
									s.erase(0, pos + 1);
								}
								SystemLogger::Debug() << "Max Clones: " << mMaxNumofClones << std::endl;
							}
							break;
						default:
							SystemLogger::Error() << "Wat" << std::endl;
							break;
						}

						pData = pData->Parent()->NextSiblingElement();
					}
				}

				// Parse objects
				pObject = pRoot->FirstChildElement("Object");
				while (pObject)
				{
					std::vector<std::string> codeComs;
					std::string elementType, name, meshFile, textureFile, emissiveTexture, colliderType, particleTexture, soundName;
					vec3f position, rotation, scale, colliderPosition, colliderScale, normal, pushNorm, gravity, particleRadius, startColor, endColor;
					float mass, elasticity, staticF, kineticF, normF, drag, radius, startSize, endSize, startAlpha, endAlpha;
					int totalParticles, maxParticles, PPS, lifeTime;
					bool collider = false, trigger = false, canMove = false, physical = false, particle = false, sound = false, SFX = false, Loop = false;
					unsigned long sfxFile, playFile, pauseFile, stopFile, resumeFile;
					pData = pObject->FirstChildElement();
					while (pData)
					{
						switch (pData->Type())
						{
						case TiXmlNode::NodeType::TINYXML_ELEMENT:
							elementType = std::string(pData->Value());
							if (elementType == "Collider")
								collider = true;
							else if (elementType == "ParticleEmitter")
								particle = true;
							else if (elementType == "SoundEmitter")
								sound = true;
							pData = (TiXmlElement*)pData->FirstChild();
							break;
						case TiXmlNode::NodeType::TINYXML_TEXT:
							if (elementType == "Name")
								name = pData->Value();
							else if (elementType == "Mesh")
								meshFile = pData->Value();
							else if (elementType == "Texture")
								textureFile = pData->Value();
							else if (elementType == "Emissive") {
								emissiveTexture = pData->Value();
							}
							else if (elementType == "Position")
							{
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									if (collider)
									{
										colliderPosition.xyz[i] = std::strtof(token.c_str(), nullptr);
									}
									else
									{
										position.xyz[i] = std::strtof(token.c_str(), nullptr);
									}
									i++;
									s.erase(0, pos + 1);
								}
							}
							else if (elementType == "Rotation")
							{
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									if (!collider)
									{
										rotation.xyz[i] = std::strtof(token.c_str(), nullptr);
									}
									i++;
									s.erase(0, pos + 1);
								}
							}
							else if (elementType == "Scale")
							{
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									if (collider)
									{
										colliderScale.xyz[i] = std::strtof(token.c_str(), nullptr);
									}
									else
									{
										scale.xyz[i] = std::strtof(token.c_str(), nullptr);
									}
									i++;
									s.erase(0, pos + 1);
								}
							}
							else if (elementType == "Move")
							{
								std::string temp(pData->Value());
								canMove = temp.find("True") != std::string::npos;
							}
							else if (elementType == "Type")
								colliderType = pData->Value();
							else if (elementType == "Trigger")
							{
								std::string temp(pData->Value());
								trigger = temp.find("True") != std::string::npos;
								int thing = 0;
							}
							else if (elementType == "Radius")
							{
								radius = std::strtof(pData->Value(), nullptr);
								colliderScale = vec4f(radius, radius, radius, 1);
							}
							else if (elementType == "Mass")
								mass = std::strtof(pData->Value(), nullptr);
							else if (elementType == "Elasticity")
								elasticity = std::strtof(pData->Value(), nullptr);
							else if (elementType == "StaticFriction")
								staticF = std::strtof(pData->Value(), nullptr);
							else if (elementType == "KeneticFriction")
								kineticF = std::strtof(pData->Value(), nullptr);
							else if (elementType == "Drag")
							{
								drag = std::strtof(pData->Value(), nullptr);
								if (pData->Parent()->Parent()->NextSiblingElement())
									pData = pData->Parent()->Parent()->NextSiblingElement();
								else
									pData = nullptr;
							}
							else if (elementType == "Normal")
							{
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									normal.xyz[i] = std::strtof(token.c_str(), nullptr);
									i++;
									s.erase(0, pos + 1);
								}
							}
							else if (elementType == "PushNormal")
							{
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									pushNorm.xyz[i] = std::strtof(token.c_str(), nullptr);
									i++;
									s.erase(0, pos + 1);
								}
							}
							else if (elementType == "Gravity")
							{
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									gravity.xyz[i] = std::strtof(token.c_str(), nullptr);
									i++;
									s.erase(0, pos + 1);
								}
							}
							else if (elementType == "NormalForce")
								normF = std::strtof(pData->Value(), nullptr);
							else if(elementType == "MaxParticles")
								maxParticles = (int)std::strtof(pData->Value(), nullptr);
							else if (elementType == "TotalParticles")
								totalParticles = (int)std::strtof(pData->Value(), nullptr);
							else if(elementType == "Texture")
								particleTexture = pData->Value();
							else if(elementType == "PPS")
								PPS = (int)std::strtof(pData->Value(), nullptr);
							else if(elementType == "LifeTime")
								lifeTime = (int)std::strtof(pData->Value(), nullptr);
							else if(elementType == "StartSize")
								startSize = std::strtof(pData->Value(), nullptr);
							else if(elementType == "EndSize")
								endSize = std::strtof(pData->Value(), nullptr);
							else if (elementType == "StartAlpha")
								startAlpha = std::strtof(pData->Value(), nullptr);
							else if(elementType == "EndAlpha")
								endAlpha = std::strtof(pData->Value(), nullptr);
							else if (elementType == "Radial")
							{
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									particleRadius.xyz[i] = std::strtof(token.c_str(), nullptr);
									i++;
									s.erase(0, pos + 1);
								}
							}
							else if(elementType == "StartColor")
							{
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									startColor.xyz[i] = std::strtof(token.c_str(), nullptr);
									i++;
									s.erase(0, pos + 1);
								}
							}
							else if (elementType == "StartColor")
							{
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									endColor.xyz[i] = std::strtof(token.c_str(), nullptr);
									i++;
									s.erase(0, pos + 1);
								}
							}
							else if(elementType == "SoundName")
								soundName = pData->Value();
							else if (elementType == "SFX")
							{
								SFX = true;
								std::string t = pData->Value();
								char* c;
								sfxFile = std::strtol(t.c_str(), &c, 16);
							}
							else if (elementType == "Play")
							{
								Loop = true;
								std::string t = pData->Value();
								char* c;
								playFile = std::strtol(t.c_str(), &c, 16);
							}
							else if (elementType == "Pause")
							{
								Loop = true;
								std::string t = pData->Value();
								char* c;
								pauseFile = std::strtol(t.c_str(), &c, 16);
							}
							else if (elementType == "Stop")
							{
								Loop = true;
								std::string t = pData->Value();
								char* c;
								stopFile = std::strtol(t.c_str(), &c, 16);
							}
							else if (elementType == "Resume")
							{
								Loop = true;
								std::string t = pData->Value();
								char* c;
								resumeFile = std::strtol(t.c_str(), &c, 16);
							}
							else
								codeComs.push_back(elementType);

							if (pData != nullptr)
								pData = pData->Parent()->NextSiblingElement();
							break;
						default:
							break;
						}
					}


					Transform transform;
					matrix4 mat = matrix4::CreateScale(scale.x, scale.y, scale.z) *
						matrix4::CreateXRotation(rotation.x) *
						matrix4::CreateYRotation(rotation.y) *
						matrix4::CreateZRotation(rotation.z) *
						matrix4::CreateTranslation(position.x, position.y, position.z);
					transform.SetMatrix(mat);
					BaseObject* obj = new BaseObject(name, transform);

					if (name == "mmStartButton")
					{
						CCStartButton* start = new CCStartButton();
						obj->AddComponent(start);
					}
					else if (name == "mmExitButton")
					{
						CCExit* exit = new CCExit();
						obj->AddComponent(exit);
					}
					else if (name == "mmCube")
					{
						CCBoxSpin* spin = new CCBoxSpin();
						obj->AddComponent(spin);
					}
					else if (name == "cube.001" || name == "cube.002" || name == "cube.003" || name == "cube.004")
					{
						Emitter* e = new SFXEmitter();
						((SFXEmitter*)e)->SetEvent(AK::EVENTS::SFX_BOUNCEEFFECTS);
						obj->AddComponent(e);
						AudioWrapper::GetInstance().AddEmitter(e, name.c_str());

					}
					else if (name == "TransparentDoor1" || name == "TransparentDoor2")
					{
						Emitter* e = new SFXEmitter();
						((SFXEmitter*)e)->SetEvent(AK::EVENTS::SFX_DOORSOUND);
						obj->AddComponent(e);
						AudioWrapper::GetInstance().AddEmitter(e, name.c_str());
					}
					else if (name == "Button")
					{
						Emitter* e = new SFXEmitter();
						((SFXEmitter*)e)->SetEvent(AK::EVENTS::SFX_TOGGLE);
						obj->AddComponent(e);
						AudioWrapper::GetInstance().AddEmitter(e, name.c_str());
					}
					else if (name == "mmChamber")
					{
						Emitter* e = new SFXEmitter();
						((SFXEmitter*)e)->SetEvent(AK::EVENTS::SFX_METALLICSOUND);
						obj->AddComponent(e);
						AudioWrapper::GetInstance().AddEmitter(e, name.c_str() + 0);

						e = new AudioEmitter();
						((AudioEmitter*)e)->AddEvent(Emitter::EventType::ePlay, AK::EVENTS::PLAY_FUTURETECHSOUND);
						((AudioEmitter*)e)->AddEvent(Emitter::EventType::ePause, AK::EVENTS::PAUSE_FUTURETECHSOUND);
						((AudioEmitter*)e)->AddEvent(Emitter::EventType::eResume, AK::EVENTS::RESUME_FUTURETECHSOUND);
						((AudioEmitter*)e)->AddEvent(Emitter::EventType::eStop, AK::EVENTS::STOP_FUTURETECHSOUND);
						obj->AddComponent(e);
						AudioWrapper::GetInstance().AddEmitter(e, name.c_str() + 1);


						e = new AudioEmitter();
						((AudioEmitter*)e)->AddEvent(Emitter::EventType::ePlay, AK::EVENTS::PLAY_CASUAL_LEVEL_LOOP);
						((AudioEmitter*)e)->AddEvent(Emitter::EventType::ePause, AK::EVENTS::PAUSE_CASUAL_LEVEL_LOOP);
						((AudioEmitter*)e)->AddEvent(Emitter::EventType::eResume, AK::EVENTS::RESUME_CASUAL_LEVEL_LOOP);
						((AudioEmitter*)e)->AddEvent(Emitter::EventType::eStop, AK::EVENTS::STOP_CASUAL_LEVEL_LOOP);
						obj->AddComponent(e);
						AudioWrapper::GetInstance().AddEmitter(e, name.c_str() + 2);
					}

					if (!meshFile.empty())
					{
						std::string path = "../Resources/";
						path.append(meshFile);
						MeshComponent* mesh;
						if(obj->GetName().find("Transparent") != std::string::npos)
						{
							float alpha = 1;
							//TODO PAT: FIX ALL THIS 
							if(obj->GetName().find("Door") != std::string::npos)
							{
								alpha = .6f;
							}
							else
							{
								alpha = .3f;
							}
							//mesh = new MeshComponent(path.c_str(), alpha);
							mesh = new MeshComponent(path.c_str(), alpha);
							mesh->SetPixelShader(ePS_TRANSPARENT);
						}
						else
						{
							mesh = new MeshComponent(path.c_str());
						}

						if (name == "Skybox")
							mesh->SetPixelShader(ePS_PURETEXTURE);

							path = "../Resources/";
							path.append(textureFile);
							mesh->AddTexture(path.c_str(), eTEX_DIFFUSE);
							if (!emissiveTexture.empty()) {
								path = "../Resources/";
								path.append(emissiveTexture);
								mesh->AddTexture(path.c_str(), eTEX_EMISSIVE);
							}
							obj->AddComponent(mesh);
					
					}

					if (particle)
					{
						ParticleEmitter* emitter = new ParticleEmitter(totalParticles, maxParticles, PPS, position);
						emitter->SetTexture(particleTexture.c_str());
						Particle* p = &Particle::Init();
						p->SetColors(startColor, endColor);
						p->SetLife(lifeTime);
						p->SetSize(startSize, endSize);
						emitter->SetParticle(p);
						ParticleSystem::Instance()->AddEmitter(emitter);
						Effect* e = new Effect();
						e->mEmitter = emitter;
						obj->AddComponent(e);
					}

					if (sound)
					{
						if (SFX)
						{
							Emitter* sound = new SFXEmitter();
							((SFXEmitter*)sound)->SetEvent(sfxFile);
							AudioWrapper::GetInstance().AddEmitter(sound, soundName.c_str());
							obj->AddComponent(sound);
						}
						if (Loop)
						{
							Emitter* sound = new AudioEmitter();
							((AudioEmitter*)sound)->AddEvent(Emitter::EventType::ePlay, playFile);
							((AudioEmitter*)sound)->AddEvent(Emitter::EventType::ePlay, pauseFile);
							((AudioEmitter*)sound)->AddEvent(Emitter::EventType::ePlay, resumeFile);
							((AudioEmitter*)sound)->AddEvent(Emitter::EventType::ePlay, stopFile);
							AudioWrapper::GetInstance().AddEmitter(sound, soundName.c_str());
							obj->AddComponent(sound);
						}
					}

					if (colliderType == "OBB")
					{
						physical = true;
						vec3f offset = vec3f(colliderScale.x * scale.x, colliderScale.y * scale.y, colliderScale.z * scale.z) / 2;
						vec3f min = colliderPosition - offset;
						vec3f max = colliderPosition + offset;
						CubeCollider* col = new CubeCollider(obj, canMove, trigger, gravity, mass, elasticity, staticF, kineticF, drag, min, max);
						obj->AddComponent(col);
					}
					else if (colliderType == "Sphere")
					{
						physical = true;

						SphereCollider* col = new SphereCollider(obj, canMove, trigger, gravity, mass, elasticity, staticF, kineticF, drag, radius);
						obj->AddComponent(col);
					}
					else if (colliderType == "Button")
					{
						physical = true;

						vec3f offset = vec3f(colliderScale.x * scale.x, colliderScale.y * scale.y, colliderScale.z * scale.z) / 2;
						vec3f min = colliderPosition - offset;
						vec3f max = colliderPosition + offset;
						ButtonCollider* col = new ButtonCollider(obj, min, max, mass, normF, pushNorm);
						obj->AddComponent(col);
					}
					else if (colliderType == "Plane")
					{
						physical = true;
						
						PlaneCollider* col = new PlaneCollider(obj, trigger, staticF, kineticF, fabsf((colliderPosition + position) * normal), normal);//TODO: Fix offset
						obj->AddComponent(col);
					}

					for (int i = 0; i < codeComs.size(); ++i)
					{
						if (codeComs[i] == "BoxSnapToController")
						{
							BoxSnapToControllerAction* code = new BoxSnapToControllerAction();
							obj->AddComponent(code);
						}
						else if (codeComs[i] == "ButtonHold")
						{
							CCButtonHold* code = new CCButtonHold();
							obj->AddComponent(code);
						}
						else if (codeComs[i] == "ButtonPress")
						{
							CCButtonPress* code = new CCButtonPress();
							obj->AddComponent(code);
						}
						else if (codeComs[i] == "AABBtoAABB")
						{
							CCElasticAABBtoAABB* code = new CCElasticAABBtoAABB();
							obj->AddComponent(code);
						}
						else if (codeComs[i] == "AABBtoSphere")
						{
							CCElasticAABBToSphere* code = new CCElasticAABBToSphere();
							obj->AddComponent(code);
						}
						else if (codeComs[i] == "ElasticPlane")
						{
							CCElasticReactionWithPlane* code = new CCElasticReactionWithPlane();
							obj->AddComponent(code);
						}
						else if (codeComs[i] == "SpheretoSphere")
						{
							CCElasticSphereToSphere* code = new CCElasticSphereToSphere();
							obj->AddComponent(code);
						}
						else if (codeComs[i] == "EnterLevel")
						{
							if (name == "DoorEmitter2")
							{
								CCLoadHub* code = new CCLoadHub();
								obj->AddComponent(code);
							}
							else if (name == "mmDoor")
							{
								CCEnterLevel1* code = new CCEnterLevel1();
								obj->AddComponent(code);
							}
							else
							{
								CCEnterLevel* code = new CCEnterLevel();
								obj->AddComponent(code);
							}
						}
						else if (codeComs[i] == "HeadsetFollow")
						{
							HeadsetFollow* code = new HeadsetFollow();
							obj->AddComponent(code);
						}
					}
					AddObject(obj);
					pObject = pObject->NextSiblingElement("Object");
				}

				SystemLogger::GetLog() << std::endl;
			}
			else
			{
				SystemLogger::GetLog() << "Cannot find 'Root' node" << std::endl;
			}
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
			CommandConsole::Instance().DisplaySet(L"");
		}
		else if (_ifOn == L"OFF") {
			Settings::GetInstance().SetInt("RasterizerStateOverride", eRS_MAX);
			CommandConsole::Instance().DisplaySet(L"");
		}
		else {
			CommandConsole::Instance().DisplaySet(L"INVALID INPUT: " + _ifOn + L"\nCORRECT INPUT: /WIREFRAME (ON/OFF)");
		}
	}

	void Level::LoadLevelCmnd(void* _commandConsole, std::wstring _Level)
	{
		CommandConsole* self = (CommandConsole*)_commandConsole;
		std::list<BaseObject*> copyList = LevelManager::GetInstance().GetCurrentLevel()->GetLevelObjects();
		if (accessLevelTwo == nullptr)
		{
			for (auto it = copyList.begin(); it != copyList.end(); ++it)
			{
				std::vector<Component*> CodeComps = (*it)->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
				if (CodeComps.size() > 0)
				{
					for (size_t x = 0; x < CodeComps.size(); ++x)
					{
						if (dynamic_cast<CCEnterLevel*>(CodeComps[x]))
						{
							accessLevelTwo = ((CCEnterLevel*)CodeComps[x]);
							break;
						}
					}
					if (accessLevelTwo != nullptr)
						break;
				}
			}
		}
		if (accessLevelOne == nullptr)
		{
			for (auto it = copyList.begin(); it != copyList.end(); ++it)
			{
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
					}
					if (accessLevelOne != nullptr)
						break;
				}
			}
		}

		if (accessHub == nullptr)
		{
			std::list<BaseObject*> copyList = LevelManager::GetInstance().GetCurrentLevel()->GetLevelObjects();
			for (auto it = copyList.begin(); it != copyList.end(); ++it) {
				std::vector<Component*> CodeComps = (*it)->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
				if (CodeComps.size() > 0)
				{
					for (size_t x = 0; x < CodeComps.size(); ++x)
					{
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
		}

		//std::list<BaseObject*> objects = mObjectList;

		if ((_Level == L"HUBWORLD" || _Level == L"HUB") && accessHub->GetOnce() == true) {
			accessHub->SetOnce(false);
			CommandConsole::Instance().Toggle();
			accessLevelOne = nullptr;
			accessLevelTwo = nullptr;
			accessHub = nullptr;
		}
		else if ((_Level == L"LEVELONE" || _Level == L"LVLONE") && accessLevelOne->GetOnce() == true) {
			accessLevelOne->SetOnce(false);
			CommandConsole::Instance().Toggle();
			accessLevelOne = nullptr;
			accessLevelTwo = nullptr;
			accessHub = nullptr;
		}
		else if ((_Level == L"LEVELTWO" || _Level == L"LVLTWO") && accessLevelTwo->GetOnce() == true)
		{
			accessLevelTwo->SetOnce(false);
			CommandConsole::Instance().Toggle();
			accessLevelOne = nullptr;
			accessLevelTwo = nullptr;
			accessHub = nullptr;
		}
		else if (accessLevelTwo == nullptr || accessLevelOne == nullptr || (accessHub == nullptr && (accessLevelTwo == nullptr && accessLevelOne == nullptr)))
			CommandConsole::Instance().DisplaySet(L"FAILED TO LOAD LEVEL :(");
		else if (accessLevelTwo->GetOnce() == false || accessLevelOne->GetOnce() == false)// || accessHub->GetOnce() == false)
			CommandConsole::Instance().DisplaySet(L"LEVEL IS ALREADY LOADED");
	}


} // Epoch Namespace