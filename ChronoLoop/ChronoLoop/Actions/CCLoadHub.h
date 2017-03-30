#pragma once
#include "CodeComponent.hpp"
#include "..\Common\Logger.h"
#include "..\Core\LevelManager.h"
#include "..\Core\Pool.h"
#include "..\Common\Settings.h"
#include "..\Actions\MainMenuBT.h"
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

					//new stuff
					Transform identity, transform;
					BaseObject* RightController = Pool::Instance()->iGetObject()->Reset("Controller1 - 0", identity);
					MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");
					MeshComponent *rightRaycaster = new MeshComponent("../Resources/BootrayCast.obj");
					rightRaycaster->AddTexture("../Resources/bootray.png", eTEX_DIFFUSE);
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
					MeshComponent *leftRaycaster = new MeshComponent("../Resources/BootrayCast.obj");
					leftRaycaster->AddTexture("../Resources/bootray.png", eTEX_DIFFUSE);
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
						if ((*it)->mComponents[eCOMPONENT_COLLIDER].size() > 0)
						{
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