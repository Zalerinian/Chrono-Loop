#pragma once
#include "CodeComponent.hpp"
#include "..\Common\Logger.h"
#include "..\Core\LevelManager.h"
#include "..\Core\Pool.h"
#include "..\Common\Settings.h"
#include "..\Actions\MainMenuBT.h"
#include "..\Actions\CCStartButton.h"
#include "..\Objects\MeshComponent.h"
#include "..\Sound\SoundEngine.h"

namespace Epoch
{

	struct CCLoadHub : public CodeComponent
	{
		const wchar_t* _basePath = L"../Resources/Soundbanks/";
		const wchar_t* _initSB = L"Init.bnk";
		const wchar_t* _aSB = L"Test_Soundbank.bnk";


		bool once = true;
		void SetOnce(bool _set) { once = _set; };
		bool GetOnce() { return once; };
		virtual void OnTriggerEnter(Collider& _col1, Collider& _col2)
		{
			if (_col2.mColliderType == Collider::eCOLLIDER_Controller)
			{
				if (Settings::GetInstance().GetInt("CurrentLevel") == 1)
					Settings::GetInstance().SetBool("CompleteLevel1", true);
				else if (Settings::GetInstance().GetInt("CurrentLevel") == 2)
					Settings::GetInstance().SetBool("CompleteLevel2", true);
				else if (Settings::GetInstance().GetInt("CurrentLevel") == 3)
					Settings::GetInstance().SetBool("CompleteLevel3", true);
				else if (Settings::GetInstance().GetInt("CurrentLevel") == 4)
					Settings::GetInstance().SetBool("CompleteLevel4", true);
				else if (Settings::GetInstance().GetInt("CurrentLevel") == 5)
					Settings::GetInstance().SetBool("CompleteLevel5", true);

				once = false;
			}
		}
		virtual void Start()
		{
			once = true;
		}
		virtual void Update()
		{
			if (!once)
			{
				Settings::GetInstance().SetBool("LevelIsLoading", true);
				Level* next = new Level;
				next->BinaryLoadLevel("../Resources/MainMenu.elf");
				// Todo: Un-hardcode this
				// use a setting string for next level path?
				//LM::LevelStatus status = LevelManager::GetInstance().LoadLevelAsync("../Resources/Level1_2_6.xml", &next);
				if (/*status == LM::LevelStatus::Success*/ true)
				{
					// Clean up the current level and request the new one be used next time.
					Listener* l = ((Listener*)LevelManager::GetInstance().GetCurrentLevel()->GetHeadset()->GetComponentIndexed(eCOMPONENT_AUDIOLISTENER, 0));
					Emitter* e = ((Emitter*)LevelManager::GetInstance().GetCurrentLevel()->GetHeadset()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0));

					std::vector<Component*>& lcoms = LevelManager::GetInstance().GetCurrentLevel()->GetHeadset()->GetComponents(eCOMPONENT_AUDIOLISTENER);
					lcoms.erase(lcoms.begin());
					std::vector<Component*>& ecoms = LevelManager::GetInstance().GetCurrentLevel()->GetHeadset()->GetComponents(eCOMPONENT_AUDIOEMITTER);
					ecoms.erase(ecoms.begin());


					AudioWrapper::GetInstance().RemoveListener(l);
					AudioWrapper::GetInstance().RemoveEmitter(e);

					Physics::Instance()->PhysicsLock.lock();
					Physics::Instance()->mObjects.clear();
					LevelManager::GetInstance().RequestLevelChange(next);



					//Sound Initializing---------------------------------------------------
					//Listener* ears = new Listener();
					//Emitter* ambient = new Emitter();
					//ambient->AddSoundEvent(Emitter::sfxTypes::ePlayLoop, AK::EVENTS::PLAY_TEST2);
					//ambient->AddSoundEvent(Emitter::sfxTypes::ePauseLoop, AK::EVENTS::PAUSE_TEST2);
					//ambient->AddSoundEvent(Emitter::sfxTypes::eResumeLoop, AK::EVENTS::RESUME_TEST2);
					//ambient->AddSoundEvent(Emitter::sfxTypes::eStopLoop, AK::EVENTS::STOP_TEST2);
					//Messager::Instance().SendInMessage(new Message(msgTypes::mSound, soundMsg::ADD_Listener, 0, false, (void*)new m_Listener(ears, "Listener")));
					//Messager::Instance().SendInMessage(new Message(msgTypes::mSound, soundMsg::ADD_Emitter, 0, false, (void*)new m_Emitter(ambient, "ambiance")));
					AudioWrapper::GetInstance().STOP();

					//new stuff
					Transform identity, transform;
					BaseObject* RightController = Pool::Instance()->iGetObject()->Reset("Controller1 - 0", identity, nullptr, BaseObject_Flag_Record_In_Timeline);
					MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");
					mc->AddTexture("../Resources/Controller_Diffuse.png", eTEX_DIFFUSE);
					mc->AddTexture("../Resources/Controller_Normal", eTEX_NORMAL);
					mc->AddTexture("../Resources/Controller_Specular", eTEX_SPECULAR);
					MeshComponent *rightRaycaster = new MeshComponent("../Resources/RaycastCylinder.obj");
					rightRaycaster->AddTexture("../Resources/Teal.png", eTEX_DIFFUSE);
					rightRaycaster->SetVisible(false);
					TeleportAction* rightTele = new TeleportAction(eControllerType_Primary);
					ControllerCollider* rightConCol = new ControllerCollider(RightController, vec3f(-0.10f, -0.10f, -0.10f), vec3f(0.10f, 0.10f, 0.10f), false);
					RightController->AddComponent(mc);
					RightController->AddComponent(rightRaycaster);
					RightController->AddComponent(rightTele);
					RightController->AddComponent(rightConCol);

					BaseObject* LeftController = Pool::Instance()->iGetObject()->Reset("Controller2 - 0", identity, nullptr, BaseObject_Flag_Record_In_Timeline); //new BaseObject("Controller2", identity);
					MeshComponent *mc2 = new MeshComponent("../Resources/Player_hand.obj");
					mc2->AddTexture("../Resources/Player_hand_Diffuse.png", eTEX_DIFFUSE);
					mc2->AddTexture("../Resources/Player_hand_Emissive.png", eTEX_EMISSIVE);
					mc2->AddTexture("../Resources/Player_hand_Normal.png", eTEX_NORMAL);
					mc2->AddTexture("../Resources/Player_hand_Specular", eTEX_SPECULAR);
					MeshComponent *leftRaycaster = new MeshComponent("../Resources/RaycastCylinder.obj");
					leftRaycaster->AddTexture("../Resources/Teal.png", eTEX_DIFFUSE);
					leftRaycaster->SetVisible(false);
					TeleportAction* leftTele = new TeleportAction(eControllerType_Secondary);
					ControllerCollider* leftConCol = new ControllerCollider(LeftController, vec3f(-0.10f, -0.10f, -0.10f), vec3f(0.10f, 0.10f, 0.10f), true);
					LeftController->AddComponent(leftConCol);
					LeftController->AddComponent(leftRaycaster);
					LeftController->AddComponent(leftTele);
					LeftController->AddComponent(mc2);

					BaseObject* headset = Pool::Instance()->iGetObject()->Reset("headset", identity, nullptr, BaseObject_Flag_Record_In_Timeline); //new BaseObject("headset", transform);
					HeadsetFollow* hfollow = new HeadsetFollow();
					MainMenuBT* bt = new MainMenuBT();
					headset->AddComponent(bt);
					headset->AddComponent(hfollow);

					Emitter* sound = new SFXEmitter();
					((SFXEmitter*)sound)->SetEvent(AK::EVENTS::SFX_TELEPORTSOUND);
					AudioWrapper::GetInstance().AddEmitter(sound, headset->GetName().c_str());
					headset->AddComponent(sound);

					Physics::Instance()->mObjects.push_back(RightController);
					Physics::Instance()->mObjects.push_back(LeftController);

					next->AssignPlayerControls(headset, LeftController, RightController);
					next->AddObject(headset);
					next->AddObject(LeftController);
					next->AddObject(RightController);

					bool boop = true;
					int floorPos = Settings::GetInstance().GetInt("mmLevel") * -10;
					auto& levelObjects = next->GetLevelObjects();
					for (auto it = levelObjects.begin(); it != levelObjects.end(); ++it)
					{
						std::string temp = ((BaseObject*)*it)->GetName();
						if (temp == "FloorChamber" || temp == "mmStartSign" || temp == "mmExitSign" || temp == "mmExitButton" ||
							temp == "mmStartButton" || temp == "mmStartStand" || temp == "mmExitStand" ||
							temp == "mmTutButton" || temp == "mmTutSign" || temp == "mmTutStand" || temp == "mmPrevStand"
							|| temp == "mmPrevButton" || temp =="mmPrevSign")
						{
							Transform t;
							t.SetMatrix(((BaseObject*)*it)->GetTransform().GetMatrix() * matrix4::CreateNewTranslation(0, (float)floorPos, 0));
							((BaseObject*)*it)->SetTransform(t);

							if (boop)
							{
								next->GetStartPos() = vec4f(0, (float)floorPos, 0, 1);
								boop = false;
							}

							if (temp == "mmStartButton")
							{
								matrix4 mat = ((BaseObject*)*it)->GetTransform().GetMatrix();
								((ButtonCollider*)((BaseObject*)*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(mat.fourth);
								((ButtonCollider*)((BaseObject*)*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mLowerBound.mOffset = mat.fourth.y - .2f;
								((ButtonCollider*)((BaseObject*)*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mUpperBound.mOffset = mat.fourth.y - .2f;

							}
							else if (temp == "mmExitButton")
							{
								matrix4 mat = ((BaseObject*)*it)->GetTransform().GetMatrix();
								((ButtonCollider*)((BaseObject*)*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(mat.fourth);
								((ButtonCollider*)((BaseObject*)*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mLowerBound.mOffset = mat.fourth.y - .2f;
								((ButtonCollider*)((BaseObject*)*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mUpperBound.mOffset = mat.fourth.y - .2f;

							}
							else if (temp == "mmTutButton")
							{
								matrix4 mat = ((BaseObject*)*it)->GetTransform().GetMatrix();
								((ButtonCollider*)((BaseObject*)*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(mat.fourth);
								((ButtonCollider*)((BaseObject*)*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mLowerBound.mOffset = mat.fourth.y - .2f;
								((ButtonCollider*)((BaseObject*)*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mUpperBound.mOffset = mat.fourth.y - .2f;
							}
						}
						else if (floorPos == -10 && temp == "mmClosingPanel")
						{
							Transform t;
							t.SetMatrix(((BaseObject*)*it)->GetTransform().GetMatrix() * matrix4::CreateNewTranslation(2, 0, 0));
							((BaseObject*)*it)->SetTransform(t);
						}

						if ((*it)->mComponents[eCOMPONENT_COLLIDER].size() > 0)
						{
							Physics::Instance()->mObjects.push_back((*it));
						}
					}

					ParticleSystem::Instance()->Clear();

					((MeshComponent*)next->FindObjectWithName("Test2")->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)next->FindObjectWithName("Test3")->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)next->FindObjectWithName("Test4")->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);

					if (Settings::GetInstance().GetBool("CompleteLevel1"))
					{
						Particle* p1 = &Particle::Init();
						p1->SetPos(vec3f(0, 0, 0));
						p1->SetColors(vec3f(0, 1, 0), vec3f(0, .5f, .5f));
						p1->SetLife(550);
						p1->SetSize(.35f, .15f);
						ParticleEmitter* emit11 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, 2.820054f, 1));
						emit11->SetParticle(p1);
						emit11->SetTexture("../Resources/BasicRectP.png");
						((TeleportEffect*)emit11)->y1 = 8;
						((TeleportEffect*)emit11)->y2 = 12;
						((TeleportEffect*)emit11)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
						((TeleportEffect*)emit11)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit11);
						emit11->FIRE();

						p1 = &Particle::Init();
						p1->SetPos(vec3f(0, 0, 0));
						p1->SetColors(vec3f(0, .5f, .5f), vec3f(0, 1, 0));
						p1->SetLife(550);
						p1->SetSize(.15f, .05f);
						ParticleEmitter* emit12 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, 2.820054f, 1));
						emit12->SetTexture("../Resources/BasicCircleP.png");
						emit12->SetParticle(p1);
						((TeleportEffect*)emit12)->y1 = 1;
						((TeleportEffect*)emit12)->y2 = 5;
						((TeleportEffect*)emit12)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
						((TeleportEffect*)emit12)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit12);
						emit12->FIRE();
					}
					else
					{
						Particle* p1 = &Particle::Init();
						p1->SetPos(vec3f(0, 0, 0));
						p1->SetColors(vec3f(1, 0, 0), vec3f(.5f, 0, .5f));
						p1->SetLife(550);
						p1->SetSize(.35f, .15f);
						ParticleEmitter* emit11 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, 2.820054f, 1));
						emit11->SetParticle(p1);
						emit11->SetTexture("../Resources/BasicRectP.png");
						((TeleportEffect*)emit11)->y1 = 8;
						((TeleportEffect*)emit11)->y2 = 12;
						((TeleportEffect*)emit11)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
						((TeleportEffect*)emit11)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit11);
						emit11->FIRE();

						p1 = &Particle::Init();
						p1->SetPos(vec3f(0, 0, 0));
						p1->SetColors(vec3f(.5f, 0, .5f), vec3f(1, 0, 0));
						p1->SetLife(550);
						p1->SetSize(.15f, .05f);
						ParticleEmitter* emit12 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, 2.820054f, 1));
						emit12->SetTexture("../Resources/BasicCircleP.png");
						emit12->SetParticle(p1);
						((TeleportEffect*)emit12)->y1 = 1;
						((TeleportEffect*)emit12)->y2 = 5;
						((TeleportEffect*)emit12)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
						((TeleportEffect*)emit12)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit12);
						emit12->FIRE();
					}

					if (Settings::GetInstance().GetBool("CompleteLevel2"))
					{
						((MeshComponent*)next->FindObjectWithName("Test2")->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
						((MeshComponent*)next->FindObjectWithName("Test2")->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
						Particle* p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(0, 1, 0), vec3f(0, .5f, .5f));
						p2->SetLife(500);
						p2->SetSize(.35f, .15f);
						ParticleEmitter* emit21 = new TeleportEffect(-1, 150, 2, vec4f(-2.82f, -10, 0, 1));
						emit21->SetParticle(p2);
						emit21->SetTexture("../Resources/BasicRectP.png");
						((TeleportEffect*)emit21)->y1 = 8;
						((TeleportEffect*)emit21)->y2 = 12;
						((TeleportEffect*)emit21)->SetPosBounds(vec3f(0, 0, -.5f), vec3f(0, 1, .5f));
						((TeleportEffect*)emit21)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit21);
						emit21->FIRE();

						p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(0, .5f, .5f), vec3f(0, 1, 0));
						p2->SetLife(500);
						p2->SetSize(.15f, .05f);
						ParticleEmitter* emit22 = new TeleportEffect(-1, 150, 2, vec4f(-2.82f, -10, 0, 1));
						emit22->SetTexture("../Resources/BasicCircleP.png");
						emit22->SetParticle(p2);
						((TeleportEffect*)emit22)->y1 = 1;
						((TeleportEffect*)emit22)->y2 = 5;
						((TeleportEffect*)emit22)->SetPosBounds(vec3f(0, 0, -.5f), vec3f(0, 1, .5f));
						((TeleportEffect*)emit22)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit22);
						emit22->FIRE();
					}
					else if (Settings::GetInstance().GetInt("CurrentLevel") >= 1 && Settings::GetInstance().GetBool("CompleteLevel1"))
					{
						((MeshComponent*)next->FindObjectWithName("Test2")->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
						((MeshComponent*)next->FindObjectWithName("Test2")->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
						Particle* p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(1, 0, 0), vec3f(.5f, 0, .5f));
						p2->SetLife(500);
						p2->SetSize(.35f, .15f);
						ParticleEmitter* emit21 = new TeleportEffect(-1, 150, 2, vec4f(-2.82f, -10, 0, 1));
						emit21->SetParticle(p2);
						emit21->SetTexture("../Resources/BasicRectP.png");
						((TeleportEffect*)emit21)->y1 = 8;
						((TeleportEffect*)emit21)->y2 = 12;
						((TeleportEffect*)emit21)->SetPosBounds(vec3f(0, 0, -.5f), vec3f(0, 1, .5f));
						((TeleportEffect*)emit21)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit21);
						emit21->FIRE();

						p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(.5f, 0, .5f), vec3f(1, 0, 0));
						p2->SetLife(500);
						p2->SetSize(.15f, .05f);
						ParticleEmitter* emit22 = new TeleportEffect(-1, 150, 2, vec4f(-2.82f, -10, 0, 1));
						emit22->SetTexture("../Resources/BasicCircleP.png");
						emit22->SetParticle(p2);
						((TeleportEffect*)emit22)->y1 = 1;
						((TeleportEffect*)emit22)->y2 = 5;
						((TeleportEffect*)emit22)->SetPosBounds(vec3f(0, 0, -.5f), vec3f(0, 1, .5f));
						((TeleportEffect*)emit22)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit22);
						emit22->FIRE();
					}

					if (Settings::GetInstance().GetBool("CompleteLevel3"))
					{
						((MeshComponent*)next->FindObjectWithName("Test3")->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
						((MeshComponent*)next->FindObjectWithName("Test3")->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
						Particle* p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(0, 1, 0), vec3f(0, .5f, .5f));
						p2->SetLife(500);
						p2->SetSize(.35f, .15f);
						ParticleEmitter* emit21 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, -2.82f, 1));
						emit21->SetParticle(p2);
						emit21->SetTexture("../Resources/BasicRectP.png");
						((TeleportEffect*)emit21)->y1 = 8;
						((TeleportEffect*)emit21)->y2 = 12;
						((TeleportEffect*)emit21)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
						((TeleportEffect*)emit21)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit21);
						emit21->FIRE();

						p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(0, .5f, .5f), vec3f(0, 1, 0));
						p2->SetLife(500);
						p2->SetSize(.15f, .05f);
						ParticleEmitter* emit22 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, -2.82f, 1));
						emit22->SetTexture("../Resources/BasicCircleP.png");
						emit22->SetParticle(p2);
						((TeleportEffect*)emit22)->y1 = 1;
						((TeleportEffect*)emit22)->y2 = 5;
						((TeleportEffect*)emit22)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
						((TeleportEffect*)emit22)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit22);
						emit22->FIRE();
					} 
					else if (Settings::GetInstance().GetInt("CurrentLevel") >= 2 && Settings::GetInstance().GetBool("CompleteLevel2"))
					{
						((MeshComponent*)next->FindObjectWithName("Test3")->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
						((MeshComponent*)next->FindObjectWithName("Test3")->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
						Particle* p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(1, 0, 0), vec3f(.5f, 0, .5f));
						p2->SetLife(500);
						p2->SetSize(.35f, .15f);
						ParticleEmitter* emit21 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, -2.82f, 1));
						emit21->SetParticle(p2);
						emit21->SetTexture("../Resources/BasicRectP.png");
						((TeleportEffect*)emit21)->y1 = 8;
						((TeleportEffect*)emit21)->y2 = 12;
						((TeleportEffect*)emit21)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
						((TeleportEffect*)emit21)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit21);
						emit21->FIRE();

						p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(.5f, 0, .5f), vec3f(1, 0, 0));
						p2->SetLife(500);
						p2->SetSize(.15f, .05f);
						ParticleEmitter* emit22 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, -2.82f, 1));
						emit22->SetTexture("../Resources/BasicCircleP.png");
						emit22->SetParticle(p2);
						((TeleportEffect*)emit22)->y1 = 1;
						((TeleportEffect*)emit22)->y2 = 5;
						((TeleportEffect*)emit22)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
						((TeleportEffect*)emit22)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit22);
						emit22->FIRE();
					}

					if (Settings::GetInstance().GetBool("CompleteLevel4"))
					{
						((MeshComponent*)next->FindObjectWithName("Test4")->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
						((MeshComponent*)next->FindObjectWithName("Test4")->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
						Particle* p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(0, 1, 0), vec3f(0, .5f, .5f));
						p2->SetLife(500);
						p2->SetSize(.35f, .15f);
						ParticleEmitter* emit21 = new TeleportEffect(-1, 150, 2, vec4f(2.82f, -10, 0, 1));
						emit21->SetParticle(p2);
						emit21->SetTexture("../Resources/BasicRectP.png");
						((TeleportEffect*)emit21)->y1 = 8;
						((TeleportEffect*)emit21)->y2 = 12;
						((TeleportEffect*)emit21)->SetPosBounds(vec3f(0, 0, -.5f), vec3f(0, 1, .5f));
						((TeleportEffect*)emit21)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit21);
						emit21->FIRE();

						p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(0, .5f, .5f), vec3f(0, 1, 0));
						p2->SetLife(500);
						p2->SetSize(.15f, .05f);
						ParticleEmitter* emit22 = new TeleportEffect(-1, 150, 2, vec4f(2.82f, -10, 0, 1));
						emit22->SetTexture("../Resources/BasicCircleP.png");
						emit22->SetParticle(p2);
						((TeleportEffect*)emit22)->y1 = 1;
						((TeleportEffect*)emit22)->y2 = 5;
						((TeleportEffect*)emit22)->SetPosBounds(vec3f(0, 0, -.5f), vec3f(0, 1, .5f));
						((TeleportEffect*)emit22)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit22);
						emit22->FIRE();
					}
					else if (Settings::GetInstance().GetInt("CurrentLevel") >= 3 && Settings::GetInstance().GetBool("CompleteLevel3"))
					{
						((MeshComponent*)next->FindObjectWithName("Test4")->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
						((MeshComponent*)next->FindObjectWithName("Test4")->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
						Particle* p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(1, 0, 0), vec3f(.5f, 0, .5f));
						p2->SetLife(500);
						p2->SetSize(.35f, .15f);
						ParticleEmitter* emit21 = new TeleportEffect(-1, 150, 2, vec4f(2.82f, -10, 0, 1));
						emit21->SetParticle(p2);
						emit21->SetTexture("../Resources/BasicRectP.png");
						((TeleportEffect*)emit21)->y1 = 8;
						((TeleportEffect*)emit21)->y2 = 12;
						((TeleportEffect*)emit21)->SetPosBounds(vec3f(0, 0, -.5f), vec3f(0, 1, .5f));
						((TeleportEffect*)emit21)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit21);
						emit21->FIRE();

						p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(.5f, 0, .5f), vec3f(1, 0, 0));
						p2->SetLife(500);
						p2->SetSize(.15f, .05f);
						ParticleEmitter* emit22 = new TeleportEffect(-1, 150, 2, vec4f(2.82f, -10, 0, 1));
						emit22->SetTexture("../Resources/BasicCircleP.png");
						emit22->SetParticle(p2);
						((TeleportEffect*)emit22)->y1 = 1;
						((TeleportEffect*)emit22)->y2 = 5;
						((TeleportEffect*)emit22)->SetPosBounds(vec3f(0, 0, -.5f), vec3f(0, 1, .5f));
						((TeleportEffect*)emit22)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit22);
						emit22->FIRE();
					}

					if (Settings::GetInstance().GetBool("CompleteLevel5"))
					{
						((MeshComponent*)next->FindObjectWithName("Test5")->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
						((MeshComponent*)next->FindObjectWithName("Test5")->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
						Particle* p1 = &Particle::Init();
						p1->SetPos(vec3f(0, 0, 0));
						p1->SetColors(vec3f(0, 1, 0), vec3f(0, .5f, .5f));
						p1->SetLife(550);
						p1->SetSize(.35f, .15f);
						ParticleEmitter* emit11 = new TeleportEffect(-1, 150, 2, vec4f(0, -20, 2.820054f, 1));
						emit11->SetParticle(p1);
						emit11->SetTexture("../Resources/BasicRectP.png");
						((TeleportEffect*)emit11)->y1 = 8;
						((TeleportEffect*)emit11)->y2 = 12;
						((TeleportEffect*)emit11)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
						((TeleportEffect*)emit11)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit11);
						emit11->FIRE();

						p1 = &Particle::Init();
						p1->SetPos(vec3f(0, 0, 0));
						p1->SetColors(vec3f(0, .5f, .5f), vec3f(0, 1, 0));
						p1->SetLife(550);
						p1->SetSize(.15f, .05f);
						ParticleEmitter* emit12 = new TeleportEffect(-1, 150, 2, vec4f(0, -20, 2.820054f, 1));
						emit12->SetTexture("../Resources/BasicCircleP.png");
						emit12->SetParticle(p1);
						((TeleportEffect*)emit12)->y1 = 1;
						((TeleportEffect*)emit12)->y2 = 5;
						((TeleportEffect*)emit12)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
						((TeleportEffect*)emit12)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit12);
						emit12->FIRE();
					}
					else if(Settings::GetInstance().GetInt("CurrentLevel") >= 4 && Settings::GetInstance().GetBool("CompleteLevel4"))
					{
						((MeshComponent*)next->FindObjectWithName("Test5")->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
						((MeshComponent*)next->FindObjectWithName("Test5")->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
						Particle* p1 = &Particle::Init();
						p1->SetPos(vec3f(0, 0, 0));
						p1->SetColors(vec3f(1, 0, 0), vec3f(.5f, 0, .5f));
						p1->SetLife(550);
						p1->SetSize(.35f, .15f);
						ParticleEmitter* emit11 = new TeleportEffect(-1, 150, 2, vec4f(0, -20, 2.820054f, 1));
						emit11->SetParticle(p1);
						emit11->SetTexture("../Resources/BasicRectP.png");
						((TeleportEffect*)emit11)->y1 = 8;
						((TeleportEffect*)emit11)->y2 = 12;
						((TeleportEffect*)emit11)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
						((TeleportEffect*)emit11)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit11);
						emit11->FIRE();

						p1 = &Particle::Init();
						p1->SetPos(vec3f(0, 0, 0));
						p1->SetColors(vec3f(.5f, 0, .5f), vec3f(1, 0, 0));
						p1->SetLife(550);
						p1->SetSize(.15f, .05f);
						ParticleEmitter* emit12 = new TeleportEffect(-1, 150, 2, vec4f(0, -20, 2.820054f, 1));
						emit12->SetTexture("../Resources/BasicCircleP.png");
						emit12->SetParticle(p1);
						((TeleportEffect*)emit12)->y1 = 1;
						((TeleportEffect*)emit12)->y2 = 5;
						((TeleportEffect*)emit12)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
						((TeleportEffect*)emit12)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit12);
						emit12->FIRE();
					}

					Particle* start = &Particle::Init();
					start->SetPos(vec3f(0, 0, 0));
					start->SetColors(vec3f(.2f, .2f, 1), vec3f(0, 1, .2f));
					start->SetLife(500);
					start->SetSize(.35f, .15f);
					ParticleEmitter* startEmit = new TeleportEffect(400, 250, 2, vec4f(0, -10, 0, 1));
					startEmit->SetParticle(start);
					startEmit->SetTexture("../Resources/BasicRectP.png");
					((TeleportEffect*)startEmit)->y1 = 8;
					((TeleportEffect*)startEmit)->y2 = 12;
					((TeleportEffect*)startEmit)->SetPosBounds(vec3f(-1, 0, -1), vec3f(1, 1, 1));
					((TeleportEffect*)startEmit)->SetVelBounds(vec3f(.5f, 1, .5f), vec3f(.5f, 5, .5f));
					ParticleSystem::Instance()->AddEmitter(startEmit);
					startEmit->FIRE();

					start = &Particle::Init();
					start->SetPos(vec3f(0, 0, 0));
					start->SetColors(vec3f(.5f, 0, .25f), vec3f(.2f, .8f, .5f));
					start->SetLife(500);
					start->SetSize(.15f, .05f);
					ParticleEmitter* startEmit2 = new TeleportEffect(400, 150, 1, vec4f(0, -10, 0, 1));
					startEmit2->SetTexture("../Resources/BasicCircleP.png");
					startEmit2->SetParticle(start);
					((TeleportEffect*)startEmit2)->y1 = 1;
					((TeleportEffect*)startEmit2)->y2 = 5;
					((TeleportEffect*)startEmit2)->SetPosBounds(vec3f(-1, 0, -1), vec3f(1, 1, 1));
					((TeleportEffect*)startEmit2)->SetVelBounds(vec3f(.5f, 1, .5f), vec3f(.5f, 5, .5f));
					ParticleSystem::Instance()->AddEmitter(startEmit2);
					startEmit2->FIRE();

					LevelManager::GetInstance().GetCurrentLevel()->GetStartPos() = vec4f(0, -10, 0, 1);
					SystemLogger::Debug() << "Loading complete" << std::endl;
					Physics::Instance()->PhysicsLock.unlock();
					Settings::GetInstance().SetBool("LevelIsLoading", false);
					Settings::GetInstance().SetInt("CurrentLevel", 0);
				}
			}
		}
	};

}