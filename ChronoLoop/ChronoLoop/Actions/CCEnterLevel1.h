#pragma once
#include "CodeComponent.hpp"
#include "..\Common\Logger.h"
#include "..\Core\LevelManager.h"
#include "..\Core\Pool.h"
#include "..\Core\TimeManager.h"
#include "..\Common\Settings.h"
#include "..\Actions\UIRewind.h"
#include "..\Actions\CCPauseToCancel.h"
#include "..\Actions\CCTeleToPlay.h"
#include "..\Actions\CCDisplayOnPause.h"
#include "..\Actions\CCLevel1Tutorial.h"
#include "..\Actions\UICreateToDeleteClone.h"
#include "..\Actions\UIClonePlusToMinus.h"
#include "..\Actions\UICloneText.h"

#include "..\Actions\BoxSnapToControllerAction.hpp"
#include "..\Actions\TeleportAction.hpp"
#include "..\Actions\PauseMenu.hpp"
#include "..\Actions\CCTimeIndicator.h"
#include "..\Rendering\Draw2D.h"
#include "..\Rendering\Renderer.h"
#include "..\Rendering\TextureManager.h"
#include "..\Objects\MeshComponent.h"
#include "..\Rendering\TextureManager.h"
#include <wrl\client.h>
#include "../../Resources/Soundbanks/Wwise_IDs.h"
#include "DirectXMath.h"


namespace Epoch
{

	struct CCEnterLevel1 : public CodeComponent
	{
		const wchar_t* _basePath = L"../Resources/Soundbanks/";
		const wchar_t* _initSB = L"Init.bnk";
		const wchar_t* _aSB = L"Test_Soundbank.bnk";


		bool once = true;
		void SetOnce(bool _set) { once = _set; };
		bool GetOnce() { return once; };
		virtual void OnTriggerEnter(Collider& _col1, Collider& _col2)
		{
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
				next->BinaryLoadLevel("../Resources/Level1.elf");
				Renderer::Instance()->ClearLights();

				// Todo: Un-hardcode this
				// use a setting string for next level path?
				//LM::LevelStatus status = LevelManager::GetInstance().LoadLevelAsync("../Resources/Level1_2_6.xml", &next);
				if (/*status == LM::LevelStatus::Success*/ true)
				{
					// Clean up the current level and request the new one be used next time.
					Physics::Instance()->PhysicsLock.lock();
					Physics::Instance()->mObjects.clear();
					LevelManager::GetInstance().RequestLevelChange(next);

					//Sound Initializing---------------------------------------------------

					Listener* ears = new Listener();
					Emitter* ambient = new AudioEmitter();
					((AudioEmitter*)ambient)->AddEvent(Emitter::EventType::ePlay, AK::EVENTS::PLAY_LEVEL2AMBIENT);
					((AudioEmitter*)ambient)->AddEvent(Emitter::EventType::ePause, AK::EVENTS::PAUSE_LEVEL2AMBIENT);
					((AudioEmitter*)ambient)->AddEvent(Emitter::EventType::eResume, AK::EVENTS::RESUME_LEVEL2AMBIENT);
					((AudioEmitter*)ambient)->AddEvent(Emitter::EventType::eStop, AK::EVENTS::STOP_LEVEL2AMBIENT);
					AudioWrapper::GetInstance().AddListener(ears, "Listener");
					AudioWrapper::GetInstance().AddEmitter(ambient, "ambiance");

					//new stuff
					Transform identity, t;
					t.SetMatrix(matrix4::CreateXRotation(DirectX::XM_PI / 2) * matrix4::CreateTranslation(0, 1.3f, 0));
					BaseObject* RightController = Pool::Instance()->iGetObject()->Reset("Controller1 - 0", t, nullptr, BaseObject_Flag_Record_In_Timeline);
					BaseObject* LeftController = Pool::Instance()->iGetObject()->Reset("Controller2 - 0", identity, nullptr, BaseObject_Flag_Record_In_Timeline);
					BaseObject* headset = Pool::Instance()->iGetObject()->Reset("Headset - 0", identity, nullptr, BaseObject_Flag_Record_In_Timeline);
					MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");
					mc->AddTexture("../Resources/Controller_Diffuse.png", eTEX_DIFFUSE);
					mc->AddTexture("../Resources/Controller_Normal", eTEX_NORMAL);
					mc->AddTexture("../Resources/Controller_Specular", eTEX_SPECULAR);
					CubeCollider* col = new CubeCollider(headset, false, false, vec3f(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, vec3f(-0.10f, -0.10f, -0.10f), vec3f(0.10f, 0.10f, 0.10f));
					headset->AddComponent(col);

					ControllerCollider* rightConCol = new ControllerCollider(RightController, vec3f(-0.10f, -0.10f, -0.10f), vec3f(0.10f, 0.10f, 0.10f), false);
					BoxSnapToControllerAction* pickup = new BoxSnapToControllerAction();
					MeshComponent *rightRaycaster = new MeshComponent("../Resources/RaycastCylinder.obj");
					rightRaycaster->AddTexture("../Resources/Teal.png", eTEX_DIFFUSE);
					rightRaycaster->SetPixelShader(ePS_PURETEXTURE);
					mc->SetPixelShader(ePS_PURETEXTURE);
					TeleportAction *ta = new TeleportAction(eControllerType_Primary);
					TimeManipulation* tm = new TimeManipulation(eControllerType_Primary);
					RightController->AddComponent(mc);
					RightController->AddComponent(rightConCol);
					RightController->AddComponent(pickup);
					RightController->AddComponent(rightRaycaster);
					RightController->AddComponent(ta);
					RightController->AddComponent(tm);

					t.SetMatrix(matrix4::CreateScale(.1f, .1f, .1f));
					BaseObject *timeDisplay = Pool::Instance()->iGetObject()->Reset("TimeIndicatorLine", t);
					MeshComponent* tdisp = new MeshComponent("../Resources/TimeIndicatorLine.obj");
					tdisp->AddTexture("../Resources/TimeIndicatorLine.png", eTEX_DIFFUSE);
					tdisp->SetPixelShader(ePS_PURETEXTURE);
					timeDisplay->AddComponent(tdisp);
					timeDisplay->SetParent(RightController);
					RightController->AddChild(timeDisplay);

					BaseObject *timeDisplayNeedle = Pool::Instance()->iGetObject()->Reset("TimeIndicatorNeedle", t);
					MeshComponent* tdispn = new MeshComponent("../Resources/TimeIndicator.obj");
					CCTimeIndicator* time = new CCTimeIndicator();
					timeDisplayNeedle->AddComponent(time);
					tdispn->AddTexture("../Resources/TimeIndicator.png", eTEX_DIFFUSE);
					tdispn->SetPixelShader(ePS_PURETEXTURE);
					timeDisplayNeedle->AddComponent(tdispn);
					timeDisplayNeedle->SetParent(RightController);
					RightController->AddChild(timeDisplayNeedle);

					t.SetMatrix(matrix4::CreateTranslation(0, .015f, .054f));
					BaseObject *rewindDisplay = Pool::Instance()->iGetObject()->Reset("RewindDisplay", t);
					MeshComponent* rewind = new MeshComponent("../Resources/UIRewind.obj");
					rewind->AddTexture("../Resources/rewind.png", eTEX_DIFFUSE);
					rewind->SetPixelShader(ePS_PURETEXTURE);
					UIRewind* spin = new UIRewind();
					CCDisplayOnPause* rdop = new CCDisplayOnPause();
					rewindDisplay->AddComponent(rdop);
					rewindDisplay->AddComponent(spin);
					rewindDisplay->AddComponent(rewind);
					rewindDisplay->SetParent(RightController);
					RightController->AddChild(rewindDisplay);

					t.SetMatrix(matrix4::CreateTranslation(-0.039f, 0.015f, 0.054f));
					BaseObject *rewindHelp = Pool::Instance()->iGetObject()->Reset("RewindHelp", t);
					MeshComponent* rhdisp = new MeshComponent("../Resources/help.obj");
					rhdisp->SetVisible(false);
					rhdisp->AddTexture("../Resources/rewindHelp.png", eTEX_DIFFUSE);
					rhdisp->SetPixelShader(ePS_PURETEXTURE);
					CCDisplayOnPause* dop = new CCDisplayOnPause();
					rewindHelp->AddComponent(dop);
					rewindHelp->AddComponent(rhdisp);
					rewindHelp->SetParent(RightController);
					RightController->AddChild(rewindHelp);

					t.SetMatrix(matrix4::CreateTranslation(0, 0.015f, 0.054f));
					BaseObject *teleportHelp = Pool::Instance()->iGetObject()->Reset("teleportHelp", t);
					MeshComponent* thdisp = new MeshComponent("../Resources/help.obj");
					thdisp->AddTexture("../Resources/teleport.png", eTEX_DIFFUSE);
					thdisp->SetPixelShader(ePS_PURETEXTURE);
					CCTeleToPlay* ttp = new CCTeleToPlay();
					teleportHelp->AddComponent(ttp);
					teleportHelp->AddComponent(thdisp);
					teleportHelp->SetParent(RightController);
					RightController->AddChild(teleportHelp);

					t.SetMatrix(matrix4::CreateTranslation(-0.03f, -0.01f, 0.089f));
					BaseObject *pauseHelp = Pool::Instance()->iGetObject()->Reset("pauseHelp", t);
					MeshComponent* phdisp = new MeshComponent("../Resources/help.obj");
					phdisp->AddTexture("../Resources/pause.png", eTEX_DIFFUSE);
					phdisp->SetPixelShader(ePS_PURETEXTURE);
					CCPauseToCancel* ptc = new CCPauseToCancel();
					pauseHelp->AddComponent(ptc);
					pauseHelp->AddComponent(phdisp);
					pauseHelp->SetParent(RightController);
					RightController->AddChild(pauseHelp);

					//pat added
					MeshComponent *mc2 = new MeshComponent("../Resources/Player_hand.obj");
					mc2->AddTexture("../Resources/Player_hand_Diffuse.png", eTEX_DIFFUSE);
					mc2->AddTexture("../Resources/Player_hand_Emissive.png", eTEX_EMISSIVE);
					mc2->AddTexture("../Resources/Player_hand_Normal.png", eTEX_NORMAL);
					mc2->AddTexture("../Resources/Player_hand_Specular", eTEX_SPECULAR);
					ControllerCollider* leftConCol = new ControllerCollider(LeftController, vec3f(-0.10f, -0.10f, -0.10f), vec3f(0.10f, 0.10f, 0.10f), true);
					BoxSnapToControllerAction* pickup2 = new BoxSnapToControllerAction();
					MeshComponent *leftRaycaster = new MeshComponent("../Resources/RaycastCylinder.obj");
					leftRaycaster->AddTexture("../Resources/Teal.png", eTEX_DIFFUSE);
					leftRaycaster->SetPixelShader(ePS_PURETEXTURE);
					mc2->SetPixelShader(ePS_PURETEXTURE);
					TeleportAction *ta2 = new TeleportAction(eControllerType_Secondary);
					LeftController->AddComponent(mc2);
					LeftController->AddComponent(leftConCol);
					LeftController->AddComponent(pickup2);
					LeftController->AddComponent(leftRaycaster);
					LeftController->AddComponent(ta2);

					MeshComponent *visibleMesh2 = new MeshComponent("../Resources/TinyCube.obj");
					visibleMesh2->AddTexture("../Resources/cube_texture.png", eTEX_DIFFUSE);
					visibleMesh2->SetVisible(false);
					CCLevel1Tutorial *tut = new CCLevel1Tutorial();
					headset->AddComponent(tut);
					headset->AddComponent(ambient);
					headset->AddComponent(visibleMesh2);
					AudioWrapper::GetInstance().STOP();
					((AudioEmitter*)ambient)->CallEvent(Emitter::EventType::ePlay);
					HeadsetFollow* hfollow = new HeadsetFollow();
					headset->AddComponent(hfollow);
					headset->AddComponent(ears);
					PauseMenu* pauseComp = new PauseMenu();
					headset->AddComponent(pauseComp);
					
					Emitter* sound = new SFXEmitter();
					((SFXEmitter*)sound)->SetEvent(AK::EVENTS::SFX_TELEPORTSOUND);
					AudioWrapper::GetInstance().AddEmitter(sound, headset->GetName().c_str());
					headset->AddComponent(sound);

					Emitter* timepause = new SFXEmitter();
					((SFXEmitter*)timepause)->SetEvent(AK::EVENTS::SFX_TIMEPAUSE);
					AudioWrapper::GetInstance().AddEmitter(timepause, headset->GetName().c_str());
					headset->AddComponent(timepause);

					Emitter* timeresume = new SFXEmitter();
					((SFXEmitter*)timeresume)->SetEvent(AK::EVENTS::SFX_TIMERESUME);
					AudioWrapper::GetInstance().AddEmitter(timeresume, headset->GetName().c_str());
					headset->AddComponent(timeresume);

					Emitter* sound1 = new SFXEmitter();
					((SFXEmitter*)sound1)->SetEvent(AK::EVENTS::SFX_PLAYERDEATH);
					AudioWrapper::GetInstance().AddEmitter(sound1, headset->GetName().c_str());
					headset->AddComponent(sound1);

					Emitter* resetlevelsound = new SFXEmitter();
					((SFXEmitter*)resetlevelsound)->SetEvent(AK::EVENTS::SFX_RESETLEVEL);
					headset->AddComponent(resetlevelsound);
					AudioWrapper::GetInstance().AddEmitter(resetlevelsound, headset->GetName().c_str());




					/*BaseObject* magicalCube = Pool::Instance()->iGetObject()->Reset("Magical Cube That Follows Me");
					MeshComponent* mcmc = new MeshComponent("../Resources/UnitCube.obj");
					mcmc->AddTexture("../Resources/cube_texture.png", eTEX_DIFFUSE);
					magicalCube->AddComponent(mcmc);
					magicalCube->AddComponent(new CCSnapToPlayerPos);
					next->AddObject(magicalCube);*/


					LevelManager::GetInstance().RequestLevelChange(next);

					ParticleSystem::Instance()->Clear();

					//Sparks////////////////////////////////////////////////////////////////////////////////////
					Particle* sp = &Particle::Init();
					sp->SetPos(vec3f(0,0,0));
					sp->SetColors(vec3f(.6f, .6f, 1), vec3f(.2f, .2f, 1));
					sp->SetLife(100);
					sp->SetSize(.05f, .03f);
					ParticleEmitter* emits = new Sparks(-1, 250, 2, vec3f(3.55f, 0.45f, 8.31f));
					emits->SetParticle(sp);
					emits->SetTexture("../Resources/BasicCircleP.png");
					ParticleSystem::Instance()->AddEmitter(emits);
					emits->FIRE();
					////////////////////////////////////////////////////////////////////////////////////////////////

					next->AssignPlayerControls(headset, LeftController, RightController);
					next->AddObject(headset);
					next->AddObject(LeftController);
					next->AddObject(RightController);
					next->AddObject(timeDisplay);
					next->AddObject(timeDisplayNeedle);
					next->AddObject(rewindDisplay);
					next->AddObject(rewindHelp);
					next->AddObject(pauseHelp);
					next->AddObject(teleportHelp);

			

					//Enter///////////////////////////////////////////////////////////////////////////////////
					Particle* start = &Particle::Init();
					start->SetPos(vec3f(0, 0, 0));
					start->SetColors(vec3f(.2f, .2f, 1), vec3f(0, 1, .2f));
					start->SetLife(500);
					start->SetSize(.35f, .15f);
					ParticleEmitter* startEmit = new TeleportEffect(400, 250, 2, vec4f(-15.78f, 0, -2.86f, 1));
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
					ParticleEmitter* startEmit2 = new TeleportEffect(400, 150, 1, vec4f(-15.78f, 0, -2.86f, 1));
					startEmit2->SetTexture("../Resources/BasicCircleP.png");
					startEmit2->SetParticle(start);
					((TeleportEffect*)startEmit2)->y1 = 1;
					((TeleportEffect*)startEmit2)->y2 = 5;
					((TeleportEffect*)startEmit2)->SetPosBounds(vec3f(-1, 0, -1), vec3f(1, 1, 1));
					((TeleportEffect*)startEmit2)->SetVelBounds(vec3f(.5f, 1, .5f), vec3f(.5f, 5, .5f));
					ParticleSystem::Instance()->AddEmitter(startEmit2);
					startEmit2->FIRE();
					/////////////////////////////////////////////////////////////////////////////////////////////

					//Exit///////////////////////////////////////////////////////////////////////////////////////
					Particle* p1 = &Particle::Init();
					p1->SetPos(vec3f(0, 0, 0));
					p1->SetColors(vec3f(0, 0, 1), vec3f(.5f, 0, .5f));
					p1->SetLife(550);
					p1->SetSize(.35f, .15f);
					ParticleEmitter* emit11 = new TeleportEffect(-1, 150, 2, vec4f(0, 0, 12.12225f, 1));
					emit11->SetParticle(p1);
					emit11->SetTexture("../Resources/BasicRectP.png");
					((TeleportEffect*)emit11)->y1 = 8;
					((TeleportEffect*)emit11)->y2 = 12;
					((TeleportEffect*)emit11)->SetPosBounds(vec3f(-1.1f, 0, 0), vec3f(1.1f, 1, 0));
					((TeleportEffect*)emit11)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
					ParticleSystem::Instance()->AddEmitter(emit11);
					emit11->FIRE();
					
					p1 = &Particle::Init();
					p1->SetPos(vec3f(0, 0, 0));
					p1->SetColors(vec3f(.5f, 0, .5f), vec3f(0, 0, 1));
					p1->SetLife(550);
					p1->SetSize(.15f, .05f);
					ParticleEmitter* emit12 = new TeleportEffect(-1, 150, 2, vec4f(0, 0, 12.12225f, 1));
					emit12->SetTexture("../Resources/BasicCircleP.png");
					emit12->SetParticle(p1);
					((TeleportEffect*)emit12)->y1 = 1;
					((TeleportEffect*)emit12)->y2 = 5;
					((TeleportEffect*)emit12)->SetPosBounds(vec3f(-1.1f, 0, 0), vec3f(1.1f, 1, 0));
					((TeleportEffect*)emit12)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
					ParticleSystem::Instance()->AddEmitter(emit12);
					emit12->FIRE();
					//////////////////////////////////////////////////////////////////////////////////////////////////

					Light* l1 = new Light();
					l1->Type = 4;
					l1->Color = vec3f(1, 1, 1);
					l1->ConeDirection = vec3f(0, -1, -.1f);
					l1->Position = vec3f(3.75f, 3.8f, 2.2f);
					l1->ConeRatio = .9f;

					Light* l2 = new Light();
					l2->Type = 2;
					l2->Position = vec3f(0, 4, 0);
					l2->Color = vec3f(.5f, .5f, 1);
					

					Light* l3 = new Light();
					l3->Type = 4;
					l3->Color = vec3f(0, 0, 1);
					l3->ConeDirection = vec3f(0, -1, 0);
					l3->Position = vec3f(-4, 3, 6.5f);
					l3->ConeRatio = .75f;

					Renderer::Instance()->SetLight(l1, 0);
					Renderer::Instance()->SetLight(l2, 1);
					Renderer::Instance()->SetLight(l3, 2);

					SystemLogger::Debug() << "Loading complete" << std::endl;
					Physics::Instance()->PhysicsLock.unlock();
					Settings::GetInstance().SetBool("LevelIsLoading", false);
					Settings::GetInstance().SetInt("CurrentLevel", 1);
				}
				//once = true;
			}
		}
	};

}
