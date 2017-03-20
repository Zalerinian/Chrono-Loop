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
				Level* next = new Level;
				Settings::GetInstance().SetBool("LevelIsLoading", true);
				// Todo: Un-hardcode this
				// use a setting string for next level path?
				LM::LevelStatus status = LevelManager::GetInstance().LoadLevelAsync("../Resources/Level1_2_6.xml", &next);
				if (status == LM::LevelStatus::Success)
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


					BaseObject* RightController = Pool::Instance()->iGetObject()->Reset("RController");
					MeshComponent *mc = (new MeshComponent("../Resources/Controller.obj"))->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
					MeshComponent *rightRaycaster = (new MeshComponent("../Resources/BootrayCast.obj"))->AddTexture("../Resources/bootray.png", eTEX_DIFFUSE);
					TeleportAction *ta = new TeleportAction(eControllerType_Primary);
					TimeManipulation* tm = new TimeManipulation(eControllerType_Primary);
					ControllerCollider* rightConCol = new ControllerCollider(RightController, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), false);
					BoxSnapToControllerAction* pickup = new BoxSnapToControllerAction();
					RightController->AddComponent(mc)->AddComponent(rightRaycaster)->AddComponent(ta)->AddComponent(rightConCol)->AddComponent(tm)->AddComponent(ambient)->AddComponent(pickup);
					ambient->Play();
					pickup->mControllerRole = eControllerType_Primary;
					
					//pat added
					BaseObject* LeftController = Pool::Instance()->iGetObject()->Reset("LController");
					MeshComponent *mc2 = (new MeshComponent("../Resources/Controller.obj"))->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
					MeshComponent *leftRaycaster = (new MeshComponent("../Resources/BootrayCast.obj"))->AddTexture("../Resources/bootray.png", eTEX_DIFFUSE);
					TeleportAction *ta2 = new TeleportAction(eControllerType_Secondary);
					TimeManipulation* tm2 = new TimeManipulation(eControllerType_Secondary);
					ControllerCollider* leftConCol = new ControllerCollider(LeftController, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), true);
					BoxSnapToControllerAction* pickup2 = new BoxSnapToControllerAction();
					LeftController->AddComponent(leftConCol)->AddComponent(leftRaycaster)->AddComponent(mc2)->AddComponent(ta2)->AddComponent(tm2)->AddComponent(pickup2);
					pickup2->mControllerRole = eControllerType_Secondary;

					BaseObject* headset = Pool::Instance()->iGetObject()->Reset("headset");
					HeadsetFollow* hfollow = new HeadsetFollow();
					headset->AddComponent(hfollow)->AddComponent(ears);

					Physics::Instance()->mObjects.push_back(RightController);
					Physics::Instance()->mObjects.push_back(LeftController);
					Physics::Instance()->mObjects.push_back(headset);
					LevelManager::GetInstance().RequestLevelChange(next);

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


					TimeManager::Instance()->AddObjectToTimeline(headset);
					TimeManager::Instance()->AddObjectToTimeline(LeftController);
					TimeManager::Instance()->AddObjectToTimeline(RightController);
					next->AssignPlayerControls(headset, LeftController, RightController);

					auto& levelObjects = next->GetLevelObjects();
					for (auto it = levelObjects.begin(); it != levelObjects.end(); ++it) {
						if ((*it)->mComponents[eCOMPONENT_COLLIDER].size() > 0) {
							Physics::Instance()->mObjects.push_back((*it));
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