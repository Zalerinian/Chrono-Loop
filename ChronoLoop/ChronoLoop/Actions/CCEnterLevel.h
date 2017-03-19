#pragma once
#include "CodeComponent.hpp"
#include "..\Common\Logger.h"
#include "..\Core\LevelManager.h"
#include "..\Messager\Messager.h"
#include "..\Core\Pool.h"
#include "..\Core\TimeManager.h"

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
			SystemLogger::GetLog() << "CCEnterLevel: LOAD LEVEL" << std::endl;
			once = false;
		}

		virtual void Update() {
			if (!once) {
				Level* next;
				LM::LevelStatus status = LevelManager::GetInstance().LoadLevelAsync("../Resources/Level1_2_6.xml", &next);
				if (status == LM::LevelStatus::Success)
				{
					TimeManager::Instance()->Destroy();
					Renderer::Instance()->ClearRenderSet();
					Physics::Instance()->PhysicsLock.lock();
					// Boop those controllers.
					LevelManager::GetInstance().GetCurrentLevel()->GetLeftController()->Destroy();
					LevelManager::GetInstance().GetCurrentLevel()->GetRightController()->Destroy();
					LevelManager::GetInstance().GetCurrentLevel()->GetHeadset()->Destroy();

					Physics::Instance()->mObjects.clear();


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

					LevelManager::GetInstance().RequestLevelChange(next);
					matrix4 cameraPos = matrix4::CreateYRotation(next->GetStartRot().y) * matrix4::CreateZRotation(next->GetStartRot().z) * matrix4::CreateXRotation(next->GetStartRot().x) * matrix4::CreateTranslation(next->GetStartPos());
					Renderer::Instance()->SetDebugCameraPosition(cameraPos);
					Transform identity, transform;
					BaseObject* RightController = Pool::Instance()->iGetObject()->Reset("RController", identity);// new BaseObject("Controller", identity);
					MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");
					MeshComponent *rightRaycaster = new MeshComponent("../Resources/BootrayCast.obj");
					rightRaycaster->AddTexture("../Resources/bootray.png", eTEX_DIFFUSE);
					mc->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
					TeleportAction *ta = new TeleportAction(eControllerType_Primary);
					TimeManipulation* tm = new TimeManipulation(eControllerType_Primary);
					ControllerCollider* rightConCol = new ControllerCollider(RightController, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), false);
					RightController->AddComponent(mc);
					RightController->AddComponent(rightRaycaster);
					RightController->AddComponent(ta);
					RightController->AddComponent(rightConCol);
					RightController->AddComponent(tm);
					RightController->AddComponent(ambient);
					ambient->Play();
					BoxSnapToControllerAction* pickup = new BoxSnapToControllerAction();
					((BoxSnapToControllerAction*)pickup)->mControllerRole = eControllerType_Primary;
					RightController->AddComponent(pickup);
					TimeManager::Instance()->AddObjectToTimeline(RightController);
					
					//pat added
					BaseObject* LeftController = Pool::Instance()->iGetObject()->Reset("LController", identity); //new BaseObject("Controller2", identity);
					MeshComponent *mc2 = new MeshComponent("../Resources/Controller.obj");
					MeshComponent *leftRaycaster = new MeshComponent("../Resources/BootrayCast.obj");
					leftRaycaster->AddTexture("../Resources/bootray.png", eTEX_DIFFUSE);
					mc2->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
					TeleportAction *ta2 = new TeleportAction(eControllerType_Secondary);
					TimeManipulation* tm2 = new TimeManipulation(eControllerType_Secondary);
					ControllerCollider* leftConCol = new ControllerCollider(LeftController, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), true);
					LeftController->AddComponent(leftConCol);
					LeftController->AddComponent(leftRaycaster);
					LeftController->AddComponent(mc2);
					LeftController->AddComponent(ta2);
					LeftController->AddComponent(tm2);
					BoxSnapToControllerAction* pickup2 = new BoxSnapToControllerAction();
					((BoxSnapToControllerAction*)pickup2)->mControllerRole = eControllerType_Secondary;
					LeftController->AddComponent(pickup2);

					BaseObject* headset = Pool::Instance()->iGetObject()->Reset("headset", transform); //new BaseObject("headset", transform);
					MeshComponent *visibleMesh2 = new MeshComponent("../Resources/Cube.obj");
					visibleMesh2->AddTexture("../Resources/cube_texture.png", eTEX_DIFFUSE);
					visibleMesh2->SetVisible(false);
					HeadsetFollow* hfollow = new HeadsetFollow();
					headset->AddComponent(hfollow);
					headset->AddComponent(visibleMesh2);
					headset->AddComponent(ears);
					TimeManager::Instance()->AddObjectToTimeline(headset);

					TimeManager::Instance()->AddObjectToTimeline(LeftController);
					//next->CallStart();
					Physics::Instance()->mObjects.push_back(RightController);
					Physics::Instance()->mObjects.push_back(LeftController);
					Physics::Instance()->mObjects.push_back(headset);
					next->Initialize(headset, LeftController, RightController);
					LevelManager::GetInstance().RequestLevelChange(next);
				


					SystemLogger::Debug() << "Loading complete" << std::endl;
					Physics::Instance()->PhysicsLock.unlock();
				}
			}
		}
	};

}