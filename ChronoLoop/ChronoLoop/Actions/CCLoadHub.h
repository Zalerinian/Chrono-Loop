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
			if(_col2.mColliderType == Collider::eCOLLIDER_Controller)
				once = false;
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
				next->LoadLevel("../Resources/mainMenu.xml");
				// Todo: Un-hardcode this
				// use a setting string for next level path?
				//LM::LevelStatus status = LevelManager::GetInstance().LoadLevelAsync("../Resources/Level1_2_6.xml", &next);
				if (/*status == LM::LevelStatus::Success*/ true)
				{
					// Clean up the current level and request the new one be used next time.
					Listener* l =  ((Listener*)LevelManager::GetInstance().GetCurrentLevel()->GetHeadset()->GetComponentIndexed(eCOMPONENT_AUDIOLISTENER, 0));
					Emitter* e = ((Emitter*)LevelManager::GetInstance().GetCurrentLevel()->GetHeadset()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0));

					std::vector<Component*>& lcoms = LevelManager::GetInstance().GetCurrentLevel()->GetHeadset()->GetComponents(eCOMPONENT_AUDIOLISTENER);
					lcoms.erase(lcoms.begin());
					std::vector<Component*>& ecoms = LevelManager::GetInstance().GetCurrentLevel()->GetHeadset()->GetComponents(eCOMPONENT_AUDIOEMITTER);
					ecoms.erase(ecoms.begin());
					AudioWrapper::GetInstance().RemoveListener(l);
					AudioWrapper::GetInstance().RemoveEmitter(e);

					Physics::Instance()->PhysicsLock.lock();
					TimeManager::Instance()->Destroy();
					Physics::Instance()->mObjects.clear();
					LevelManager::GetInstance().RequestLevelChange(next);
					

					TimeManager::Instance();

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
					BaseObject* RightController = Pool::Instance()->iGetObject()->Reset("Controller1 - 0", identity);
					MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");
					MeshComponent *rightRaycaster = new MeshComponent("../Resources/RaycastCylinder.obj");
					rightRaycaster->AddTexture("../Resources/Scanline.png", eTEX_DIFFUSE);
					mc->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
					MainMenuBT *bt = new MainMenuBT(eControllerType_Primary);
					ControllerCollider* rightConCol = new ControllerCollider(RightController, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), false);
					RightController->AddComponent(mc);
					RightController->AddComponent(rightRaycaster);
					RightController->AddComponent(bt);
					RightController->AddComponent(rightConCol);
					TimeManager::Instance()->AddObjectToTimeline(RightController);

					BaseObject* LeftController = Pool::Instance()->iGetObject()->Reset("Controller2 - 0", identity); //new BaseObject("Controller2", identity);
					MeshComponent *mc2 = new MeshComponent("../Resources/Controller.obj");
					MeshComponent *leftRaycaster = new MeshComponent("../Resources/RaycastCylinder.obj");
					leftRaycaster->AddTexture("../Resources/Scanline.png", eTEX_DIFFUSE);
					mc2->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
					MainMenuBT *bt2 = new MainMenuBT(eControllerType_Secondary);
					ControllerCollider* leftConCol = new ControllerCollider(LeftController, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), true);
					LeftController->AddComponent(leftConCol);
					LeftController->AddComponent(leftRaycaster);
					LeftController->AddComponent(mc2);
					LeftController->AddComponent(bt2);
					TimeManager::Instance()->AddObjectToTimeline(LeftController);

					BaseObject* headset = Pool::Instance()->iGetObject()->Reset("headset", identity); //new BaseObject("headset", transform);
					HeadsetFollow* hfollow = new HeadsetFollow();
					headset->AddComponent(hfollow);
					TimeManager::Instance()->AddObjectToTimeline(headset);
					//TODO PAT: UNcomment this when raymond gets sounds
					Emitter* sound = new SFXEmitter();
					((SFXEmitter*)sound)->SetEvent(AK::EVENTS::SFX_TELEPORTSOUND);
					AudioWrapper::GetInstance().AddEmitter(sound, headset->GetName().c_str());
					headset->AddComponent(sound);

					Transform cubeScale;
					cubeScale.SetMatrix(matrix4::CreateScale(0.01f, 0.01f, 0.01f));
					BaseObject* mmCube = Pool::Instance()->iGetObject()->Reset("mmCube", cubeScale);// new BaseObject("walls", PlaneTransform);
					MeshComponent *mmCubeMesh = new MeshComponent("../Resources/Cube.obj");
					mmCubeMesh->AddTexture("../Resources/cube_texture.png", eTEX_DIFFUSE);
					mmCube->AddComponent(mmCubeMesh);

					Physics::Instance()->mObjects.push_back(RightController);
					Physics::Instance()->mObjects.push_back(LeftController);

					next->AssignPlayerControls(headset, LeftController, RightController);
					next->AddObject(headset);
					next->AddObject(LeftController);
					next->AddObject(RightController);

					auto& levelObjects = next->GetLevelObjects();
					for (auto it = levelObjects.begin(); it != levelObjects.end(); ++it)
					{
						std::string temp = ((BaseObject*)*it)->GetName();
						if (temp == "mmChamber" || temp == "mmStartSign" || temp == "mmExitSign" || temp == "mmExitButton" || temp == "mmStartButton" || temp == "mmStartStand" || temp == "mmExitStand")
						{
							Transform t;
							t.SetMatrix(((BaseObject*)*it)->GetTransform().GetMatrix() * matrix4::CreateTranslation(0, -10, 0));
							((BaseObject*)*it)->SetTransform(t);
							next->GetStartPos() = vec4f(0, -10, 0, 1);

							if (temp == "mmStartButton")
							{
								matrix4 mat = mObject->GetTransform().GetMatrix();
								((CCStartButton*)mObject->GetComponentIndexed(eCOMPONENT_CODE, 0))->levels = 1;
								((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(mat.fourth);
								((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mLowerBound.mOffset = mat.fourth.y - .2f;
								((ButtonCollider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mUpperBound.mOffset = mat.fourth.y + .2f;
							}
							else if(temp == "mmExitButton")
							{
								matrix4 mat = ((BaseObject*)*it)->GetTransform().GetMatrix();
								((ButtonCollider*)((BaseObject*)*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(((BaseObject*)*it)->GetTransform().GetMatrix().fourth);
								((ButtonCollider*)((BaseObject*)*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mLowerBound.mOffset = ((BaseObject*)*it)->GetTransform().GetMatrix().fourth.y - .2f;
								((ButtonCollider*)((BaseObject*)*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mUpperBound.mOffset = ((BaseObject*)*it)->GetTransform().GetMatrix().fourth.y - .2f;
							}
						}

						if ((*it)->mComponents[eCOMPONENT_COLLIDER].size() > 0)
						{
							Physics::Instance()->mObjects.push_back((*it));
						}
					}

					ParticleSystem::Instance()->Clear();

					if (Settings::GetInstance().GetBool("PlayingLevel1"))
					{
						Particle* p1 = &Particle::Init();
						p1->SetPos(vec3f(0, 0, 0));
						p1->SetColors(vec3f(0, 1, 0), vec3f(0, .5f, .5f));
						p1->SetLife(550);
						p1->SetSize(.35f, .15f);
						ParticleEmitter* emit11 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, 2.611548f, 1));
						emit11->SetParticle(p1);
						emit11->SetTexture("../Resources/BasicRectP.png");
						((TeleportEffect*)emit11)->y1 = 8;
						((TeleportEffect*)emit11)->y2 = 12;
						((TeleportEffect*)emit11)->SetPosBounds(vec3f(-.37f, 0, 0), vec3f(.37f, 1, 0));
						((TeleportEffect*)emit11)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit11);
						emit11->FIRE();

						p1 = &Particle::Init();
						p1->SetPos(vec3f(0, 0, 0));
						p1->SetColors(vec3f(0, .5f, .5f), vec3f(0, 1, 0));
						p1->SetLife(550);
						p1->SetSize(.15f, .05f);
						ParticleEmitter* emit12 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, 2.611548f, 1));
						emit12->SetTexture("../Resources/BasicCircleP.png");
						emit12->SetParticle(p1);
						((TeleportEffect*)emit12)->y1 = 1;
						((TeleportEffect*)emit12)->y2 = 5;
						((TeleportEffect*)emit12)->SetPosBounds(vec3f(-.37f, 0, 0), vec3f(.37f, 1, 0));
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
						ParticleEmitter* emit11 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, 2.611548f, 1));
						emit11->SetParticle(p1);
						emit11->SetTexture("../Resources/BasicRectP.png");
						((TeleportEffect*)emit11)->y1 = 8;
						((TeleportEffect*)emit11)->y2 = 12;
						((TeleportEffect*)emit11)->SetPosBounds(vec3f(-.37f, 0, 0), vec3f(.37f, 1, 0));
						((TeleportEffect*)emit11)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit11);
						emit11->FIRE();

						p1 = &Particle::Init();
						p1->SetPos(vec3f(0, 0, 0));
						p1->SetColors(vec3f(.5f, 0, .5f), vec3f(1, 0, 0));
						p1->SetLife(550);
						p1->SetSize(.15f, .05f);
						ParticleEmitter* emit12 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, 2.611548f, 1));
						emit12->SetTexture("../Resources/BasicCircleP.png");
						emit12->SetParticle(p1);
						((TeleportEffect*)emit12)->y1 = 1;
						((TeleportEffect*)emit12)->y2 = 5;
						((TeleportEffect*)emit12)->SetPosBounds(vec3f(-.37f, 0, 0), vec3f(.37f, 1, 0));
						((TeleportEffect*)emit12)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit12);
						emit12->FIRE();
					}
					
					if (Settings::GetInstance().GetBool("PlayingLevel2"))
					{
						Particle* p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(0, 1, 0), vec3f(0, .5f, .5f));
						p2->SetLife(500);
						p2->SetSize(.35f, .15f);
						ParticleEmitter* emit21 = new TeleportEffect(-1, 150, 2, vec4f(-2.61, -10, 0, 1));
						emit21->SetParticle(p2);
						emit21->SetTexture("../Resources/BasicRectP.png");
						((TeleportEffect*)emit21)->y1 = 8;
						((TeleportEffect*)emit21)->y2 = 12;
						((TeleportEffect*)emit21)->SetPosBounds(vec3f(0, 0, -.37f), vec3f(0, 1, .37f));
						((TeleportEffect*)emit21)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit21);
						emit21->FIRE();

						p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(0, .5f, .5f), vec3f(0, 1, 0));
						p2->SetLife(500);
						p2->SetSize(.15f, .05f);
						ParticleEmitter* emit22 = new TeleportEffect(-1, 150, 2, vec4f(-2.61, -10, 0, 1));
						emit22->SetTexture("../Resources/BasicCircleP.png");
						emit22->SetParticle(p2);
						((TeleportEffect*)emit22)->y1 = 1;
						((TeleportEffect*)emit22)->y2 = 5;
						((TeleportEffect*)emit22)->SetPosBounds(vec3f(0, 0, -.37f), vec3f(0, 1, .37f));
						((TeleportEffect*)emit22)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit22);
						emit22->FIRE();
					}
					else
					{
						Particle* p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(1, 0, 0), vec3f(.5f, 0, .5f));
						p2->SetLife(500);
						p2->SetSize(.35f, .15f);
						ParticleEmitter* emit21 = new TeleportEffect(-1, 150, 2, vec4f(-2.61, -10, 0, 1));
						emit21->SetParticle(p2);
						emit21->SetTexture("../Resources/BasicRectP.png");
						((TeleportEffect*)emit21)->y1 = 8;
						((TeleportEffect*)emit21)->y2 = 12;
						((TeleportEffect*)emit21)->SetPosBounds(vec3f(0, 0, -.37f), vec3f(0, 1, .37f));
						((TeleportEffect*)emit21)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit21);
						emit21->FIRE();

						p2 = &Particle::Init();
						p2->SetPos(vec3f(0, 0, 0));
						p2->SetColors(vec3f(.5f, 0, .5f), vec3f(1, 0, 0));
						p2->SetLife(500);
						p2->SetSize(.15f, .05f);
						ParticleEmitter* emit22 = new TeleportEffect(-1, 150, 2, vec4f(-2.61, -10, 0, 1));
						emit22->SetTexture("../Resources/BasicCircleP.png");
						emit22->SetParticle(p2);
						((TeleportEffect*)emit22)->y1 = 1;
						((TeleportEffect*)emit22)->y2 = 5;
						((TeleportEffect*)emit22)->SetPosBounds(vec3f(0, 0, -.37f), vec3f(0, 1, .37f));
						((TeleportEffect*)emit22)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
						ParticleSystem::Instance()->AddEmitter(emit22);
						emit22->FIRE();
					}

					Light* l1 = new Light();
					l1->Type = 4;
					l1->Color = vec3f(1, 1, 1);
					l1->ConeDirection = vec3f(0, -1, 0);
					l1->Position = vec3f(0, 4, 0);
					l1->ConeRatio = .85f;

					Light* l2 = new Light();
					l2->Type = 4;
					l2->Color = vec3f(0, 0, 1);
					l2->ConeDirection = vec3f(0, -1, 0);
					l2->Position = vec3f(3.972854, 5, 0);
					l2->ConeRatio = .9f;

					Light* l3 = new Light();
					l3->Type = 4;
					l3->Color = vec3f(0, 1, 0);
					l3->ConeDirection = vec3f(0, -1, 0);
					l3->Position = vec3f(0, 5, -3.872531);
					l3->ConeRatio = .9f;

					Renderer::Instance()->SetLight(l1, 0);
					Renderer::Instance()->SetLight(l2, 1);
					Renderer::Instance()->SetLight(l3, 2);

					LevelManager::GetInstance().GetCurrentLevel()->GetStartPos() = vec4f(0, -10, 0, 1);
					SystemLogger::Debug() << "Loading complete" << std::endl;
					Physics::Instance()->PhysicsLock.unlock();
					Settings::GetInstance().SetBool("LevelIsLoading", false);
				}
			}
		}
	};

}