#pragma once
#include "CodeComponent.hpp"
#include "..\Common\Logger.h"
#include "..\Core\LevelManager.h"
#include "..\Messager\Messager.h"
#include "..\Core\Pool.h"
#include "..\Core\TimeManager.h"
#include "..\Common\Settings.h"

namespace Epoch 
{

	struct CCEnterLevel : public CodeComponent 
	{
		const wchar_t* _basePath = L"../Resources/Soundbanks/";
		const wchar_t* _initSB = L"Init.bnk";
		const wchar_t* _aSB = L"Test_Soundbank.bnk";

		bool once = true;
		virtual void OnTriggerEnter(Collider& _col1, Collider& _col2) 
		{
			once = false;
		}

		virtual void Update() {
			if (!once) {
				Settings::GetInstance().SetBool("LevelIsLoading", true);
				Level* next = new Level;
				next->LoadLevel("../Resources/Level1_2_6.xml");
				// Todo: Un-hardcode this
				// use a setting string for next level path?
				//LM::LevelStatus status = LevelManager::GetInstance().LoadLevelAsync("../Resources/Level1_2_6.xml", &next);
				if (/*status == LM::LevelStatus::Success*/ true)
				{
					// Clean up the current level and request the new one be used next time.
					Physics::Instance()->PhysicsLock.lock();
					TimeManager::Instance()->Destroy();
					Physics::Instance()->mObjects.clear();
					Renderer::Instance()->ClearRenderSet();
					LevelManager::GetInstance().RequestLevelChange(next);

					//Sound Initializing---------------------------------------------------
					TimeManager::Instance();

					Listener* ears = new Listener();
					Emitter* ambient = new Emitter();
					ambient->AddSoundEvent(Emitter::sfxTypes::ePlayLoop, AK::EVENTS::PLAY_TEST2);
					ambient->AddSoundEvent(Emitter::sfxTypes::ePauseLoop, AK::EVENTS::PAUSE_TEST2);
					ambient->AddSoundEvent(Emitter::sfxTypes::eResumeLoop, AK::EVENTS::RESUME_TEST2);
					ambient->AddSoundEvent(Emitter::sfxTypes::eStopLoop, AK::EVENTS::STOP_TEST2);
					Messager::Instance().SendInMessage(new Message(msgTypes::mSound, soundMsg::ADD_Listener, 0, false, (void*)new m_Listener(ears, "Listener")));
					Messager::Instance().SendInMessage(new Message(msgTypes::mSound, soundMsg::ADD_Emitter, 0, false, (void*)new m_Emitter(ambient, "ambiance")));


					//BaseObject* RightController = Pool::Instance()->iGetObject()->Reset("RController");
					//MeshComponent *mc = (new MeshComponent("../Resources/Controller.obj"))->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
					//MeshComponent *rightRaycaster = (new MeshComponent("../Resources/BootrayCast.obj"))->AddTexture("../Resources/bootray.png", eTEX_DIFFUSE);
					//TeleportAction *ta = new TeleportAction(eControllerType_Primary);
					//TimeManipulation* tm = new TimeManipulation(eControllerType_Primary);
					//ControllerCollider* rightConCol = new ControllerCollider(RightController, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), false);
					//BoxSnapToControllerAction* pickup = new BoxSnapToControllerAction();
					//RightController->AddComponent(mc)->AddComponent(rightRaycaster)->AddComponent(ta)->AddComponent(rightConCol)->AddComponent(tm)->AddComponent(ambient)->AddComponent(pickup);
					//ambient->Play();
					//pickup->mControllerRole = eControllerType_Primary;
					//
					////pat added
					//BaseObject* LeftController = Pool::Instance()->iGetObject()->Reset("LController");
					//MeshComponent *mc2 = (new MeshComponent("../Resources/Controller.obj"))->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
					//MeshComponent *leftRaycaster = (new MeshComponent("../Resources/BootrayCast.obj"))->AddTexture("../Resources/bootray.png", eTEX_DIFFUSE);
					//TeleportAction *ta2 = new TeleportAction(eControllerType_Secondary);
					//TimeManipulation* tm2 = new TimeManipulation(eControllerType_Secondary);
					//ControllerCollider* leftConCol = new ControllerCollider(LeftController, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), true);
					//BoxSnapToControllerAction* pickup2 = new BoxSnapToControllerAction();
					//LeftController->AddComponent(leftConCol)->AddComponent(leftRaycaster)->AddComponent(mc2)->AddComponent(ta2)->AddComponent(tm2)->AddComponent(pickup2);
					//pickup2->mControllerRole = eControllerType_Secondary;

					//BaseObject* headset = Pool::Instance()->iGetObject()->Reset("headset");
					//HeadsetFollow* hfollow = new HeadsetFollow();
					//headset->AddComponent(hfollow)->AddComponent(ears);

					//new stuff
					Transform identity;
					BaseObject* RightController = Pool::Instance()->iGetObject()->Reset("Controller1 - 0", identity);// new BaseObject("Controller", identity);
					BaseObject* LeftController = Pool::Instance()->iGetObject()->Reset("Controller2 - 0", identity); //new BaseObject("Controller2", identity);
					BaseObject* headset = Pool::Instance()->iGetObject()->Reset("Headset - 0", identity); //new BaseObject("headset", transform);
					MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");
					ControllerCollider* rightConCol = new ControllerCollider(RightController, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), false);
					BoxSnapToControllerAction* pickup = new BoxSnapToControllerAction();
					((BoxSnapToControllerAction*)pickup)->mControllerRole = eControllerType_Primary;
					MeshComponent *rightRaycaster = new MeshComponent("../Resources/BootrayCast.obj");
					rightRaycaster->AddTexture("../Resources/bootray.png", eTEX_DIFFUSE);
					mc->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
					TeleportAction *ta = new TeleportAction(eControllerType_Primary);
					TimeManipulation* tm = new TimeManipulation(eControllerType_Primary);
					RightController->AddComponent(mc);
					RightController->AddComponent(rightConCol);
					RightController->AddComponent(pickup);
					RightController->AddComponent(rightRaycaster);
					RightController->AddComponent(ta);
					RightController->AddComponent(tm);
					RightController->AddComponent(ambient);
					ambient->Play();
					TimeManager::Instance()->AddObjectToTimeline(RightController);

					//Transform identity;
					//BaseObject *RightTimeIndicator = Pool::Instance()->iGetObject()->Reset("RTimeIndicator", identity);
					//RightTimeIndicator->SetParent(RightController);
					//MeshComponent *TimeIndicatorLine = new MeshComponent("../Resources/TimeIndicatorLine.obj");
					//MeshComponent *TimeIndicator = new MeshComponent("../Resources/TimeIndicator.obj");
					//TimeIndicatorLine->AddTexture("../Resources/TimeIndicatorLine.png", eTEX_DIFFUSE);
					//TimeIndicator->AddTexture("../Resources/TimeIndicator.png", eTEX_DIFFUSE);
					////TimeIndicator->GetTransform().TranslateLocal(0, 0.27277f, -0.25699f); <-- Beaks here don't know why
					//TimeLineIndicator *tli = new TimeLineIndicator(TimeIndicator, TimeIndicatorLine);
					//tli->mParent = &RightController->GetTransform().GetMatrix();
					//RightTimeIndicator->AddComponent(TimeIndicatorLine);
					//RightTimeIndicator->AddComponent(TimeIndicator);
					//RightTimeIndicator->AddComponent(tli);


					//pat added
					MeshComponent *mc2 = new MeshComponent("../Resources/Controller.obj");
					ControllerCollider* leftConCol = new ControllerCollider(LeftController, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), true);
					BoxSnapToControllerAction* pickup2 = new BoxSnapToControllerAction();
					((BoxSnapToControllerAction*)pickup2)->mControllerRole = eControllerType_Secondary;
					MeshComponent *leftRaycaster = new MeshComponent("../Resources/BootrayCast.obj");
					leftRaycaster->AddTexture("../Resources/bootray.png", eTEX_DIFFUSE);
					mc2->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
					TeleportAction *ta2 = new TeleportAction(eControllerType_Secondary);
					TimeManipulation* tm2 = new TimeManipulation(eControllerType_Secondary);
					LeftController->AddComponent(mc2);
					LeftController->AddComponent(leftConCol);
					LeftController->AddComponent(pickup2);
					LeftController->AddComponent(leftRaycaster);
					LeftController->AddComponent(ta2);
					LeftController->AddComponent(tm2);
					TimeManager::Instance()->AddObjectToTimeline(LeftController);

					MeshComponent *visibleMesh2 = new MeshComponent("../Resources/TinyCube.obj");
					visibleMesh2->AddTexture("../Resources/cube_texture.png", eTEX_DIFFUSE);
					visibleMesh2->SetVisible(false);
					headset->AddComponent(visibleMesh2);

					HeadsetFollow* hfollow = new HeadsetFollow();
					headset->AddComponent(hfollow);
					headset->AddComponent(ears);
					TimeManager::Instance()->AddObjectToTimeline(headset);

					Physics::Instance()->mObjects.push_back(RightController);
					Physics::Instance()->mObjects.push_back(LeftController);
					Physics::Instance()->mObjects.push_back(headset);
					LevelManager::GetInstance().RequestLevelChange(next);


					//Enter effect
					Particle* p = &Particle::Init();
					p->SetPos(vec4f(8.88f, 0, -4.1f, 1));
					p->SetColors(vec4f(.2f, .2f, 1, 0), vec4f(0, 1, .2f, 0));
					p->SetLife(200);
					p->SetSize(1.25f / 2.0f, .15f / .2f);
					ParticleEmitter* emit = new IDC(500, 250, 2, vec4f(8.88f, 0, -4.1f, 1));
					emit->SetParticle(p);
					emit->SetTexture("../Resources/BasicRectP.png");
					((IDC*)emit)->y1 = 8;
					((IDC*)emit)->y2 = 12;
					ParticleSystem::Instance()->AddEmitter(emit);
					emit->FIRE();

					p = &Particle::Init();
					p->SetPos(vec4f(8.88f, 0, -4.1f, 1));
					p->SetColors(vec4f(.5f, 0, .25f, 0), vec4f(.2f, .8f, .5f, 0));
					p->SetLife(1000);
					p->SetSize(.25f, .05f);
					emit = new IDC(500, 150, 1, vec4f(8.88f, 0, -4.1f, 1));
					emit->SetTexture("../Resources/BasicCircleP.png");
					emit->SetParticle(p);
					((IDC*)emit)->y1 = 1;
					((IDC*)emit)->y2 = 5;
					ParticleSystem::Instance()->AddEmitter(emit);
					emit->FIRE();

					//auto objects = LevelManager::GetInstance().GetCurrentLevel()->GetLevelObjects();
					//for(auto it = objects.begin(); it != objects.end(); ++it)
					//{
					//	std::vector<Component*> zits = (*it)->GetComponents(eCOMPONENT_MESH);
					//	for(auto cit = zits.begin(); cit != zits.end(); ++cit)
					//	{
					//		((MeshComponent*)(*cit))->SetVisible(false);
					//		((MeshComponent*)(*cit))->SetVisible(true);
					//	}
					//}


					next->AssignPlayerControls(headset, LeftController, RightController);
					next->AddObject(headset);
					next->AddObject(LeftController);
					next->AddObject(RightController);

					auto& levelObjects = next->GetLevelObjects();
					for (auto it = levelObjects.begin(); it != levelObjects.end(); ++it) {
						if ((*it)->mComponents[eCOMPONENT_COLLIDER].size() > 0) {
							Physics::Instance()->mObjects.push_back((*it));
							if (((Collider*)(*it)->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->mShouldMove || (*it)->GetName() == "Door1" || (*it)->GetName() == "Door2")
							{
								if ((*it)->GetUniqueID() == next->GetLeftController()->GetUniqueID() ||
									(*it)->GetUniqueID() == next->GetRightController()->GetUniqueID() ||
									(*it)->GetUniqueID() == next->GetHeadset()->GetUniqueID())
									continue;

								TimeManager::Instance()->AddObjectToTimeline(*it);
							}
						}
					}


					SystemLogger::Debug() << "Loading complete" << std::endl;
					Physics::Instance()->PhysicsLock.unlock();
					Settings::GetInstance().SetBool("LevelIsLoading", false);
				}
			}
		}
	};

}