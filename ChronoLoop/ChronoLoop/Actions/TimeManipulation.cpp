
#include "../Objects/Component.h"
#include "../Objects/MeshComponent.h"
#include "../Actions/CodeComponent.hpp"
#include "../Objects/BaseObject.h"
#include "../Core/LevelManager.h"
#include "../Input/VRInputManager.h"
#include "../Rendering/Draw2D.h"
#include "../Core/Pool.h"
#include "TimeManipulation.h"
#include "BoxSnapToControllerAction.hpp"
#include "../Objects/MeshComponent.h"
#include "../Common/Common.h"
#include "../Particles/ParticleSystem.h"
#include "../Sound/SoundEngine.h"
#include "CCCloneHeadCollider.h"
#include "CCCloneIndicator.h"

namespace Epoch
{

	TimeManipulation::TimeManipulation()
	{}


	TimeManipulation::~TimeManipulation() {}

	void TimeManipulation::Start()
	{
		mEffectData.saturationColor.Set(0.30f, 0.59f, 0.11f, 0);
		mEffectData.tintColor.Set(1, 0.85f, 1, 1);
		mEffectData.ratios.Set(0, 0);
		mEffectData.fullRatios.Set(0.7f, 0.3f);
	}

	void TimeManipulation::Update()
	{
		Level* currentLevel = LevelManager::GetInstance().GetCurrentLevel();

		if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_Grip) && !Settings::GetInstance().GetBool("PauseMenuUp") && !Settings::GetInstance().GetBool("CantPauseTime"))
		{
		

			if (mPauseTime && (Settings::GetInstance().GetInt("tutStep") == 0 || Settings::GetInstance().GetInt("tutStep") > 3)) //created clone (tut 2)
			{
				//enable pointy bits on controller
				((MeshComponent*)currentLevel->GetRightController()->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
				((MeshComponent*)currentLevel->GetLeftController()->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
				// Cancel Time
				((SFXEmitter*)currentLevel->GetHeadset()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 3))->CallEvent(Emitter::ePlay);
				if (Settings::GetInstance().GetInt("tutStep") >= 4)//accepted time (
				{
					if (Settings::GetInstance().GetBool("Level1Tutorial"))
						Settings::GetInstance().SetInt("tutStep", 6);//end
					else
						Settings::GetInstance().SetInt("tutStep", 7);//delete clone
				}
				//put the original controll and headset back in control
				//Remove the clone created
				if (mCurCloneHeadset && mCurCloneController1 && mCurCloneController2)
				{
					currentLevel->SetHeadsetAndControllers(mCurCloneHeadset, mCurCloneController1, mCurCloneController2, false);
					TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneHeadset);
					TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneController1);
					TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneController2);
					TimeManager::Instance()->DeleteClone(mCurCloneHeadset->GetUniqueID(), false);
					mCurCloneHeadset = nullptr;
					mCurCloneController1 = nullptr;
					mCurCloneController2 = nullptr;
				}

				vec2f finalRatios(0, 0);
				mDesaturationInterpolator.Prepare(0.5f, mEffectData.ratios, finalRatios, mEffectData.ratios);
				mDesaturationInterpolator.SetActive(true);

				mPauseTime = false;
				Settings::GetInstance().SetBool("IsTimePaused", false);
				TimeManager::Instance()->RewindTimeline(
					TimeManager::Instance()->GetCurrentSnapFrame(),
					currentLevel->GetHeadset()->GetUniqueID(),
					currentLevel->GetRightController()->GetUniqueID(),
					currentLevel->GetLeftController()->GetUniqueID());

				mIsBeingMade = false;
				TimeManager::Instance()->ShowTimelineColliders(false);

			}
			else if (!mPauseTime && (Settings::GetInstance().GetInt("tutStep") == 0 || Settings::GetInstance().GetInt("tutStep") >= 2))
			{
				//disable pointy bits
				((MeshComponent*)currentLevel->GetRightController()->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(true);
				((MeshComponent*)currentLevel->GetLeftController()->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(true);

				// Stop time
				if (Settings::GetInstance().GetInt("tutStep") == 2)//Paused time (tut 1)
				{
					Settings::GetInstance().SetUInt("tut1FirstPause", TimeManager::Instance()->GetCurrentSnapFrame());
					Settings::GetInstance().SetInt("tutStep", 3);//Rewind (tut 1)
				}

				((SFXEmitter*)currentLevel->GetHeadset()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 2))->CallEvent(Emitter::ePlay);

				if (Settings::GetInstance().GetInt("CurrentLevel") != 1)
				{
					Transform identity;
					memset(&identity.GetMatrix(), 0, sizeof(identity.GetMatrix()));
					mCloneCount++;
					mCurCloneHeadset = Pool::Instance()->iGetObject()->Reset("Headset - " + std::to_string(mCloneCount), identity); //new BaseObject("headset" + std::to_string(rand), identity);
					mCurCloneController1 = Pool::Instance()->iGetObject()->Reset("Controller1 - " + std::to_string(mCloneCount), identity); //new BaseObject("Controller" + std::to_string(rand), identity);
					mCurCloneController2 = Pool::Instance()->iGetObject()->Reset("Controller2 - " + std::to_string(mCloneCount), identity); //new BaseObject("Controller" + std::to_string(rand), identity);
					MakeCloneBaseObjects(mCurCloneHeadset, mCurCloneController1, mCurCloneController2);
				}

				vec2f finalRatios(0.7f, 0.3f);
				mDesaturationInterpolator.Prepare(0.5f, mEffectData.ratios, finalRatios, mEffectData.ratios);
				mDesaturationInterpolator.SetActive(true);
				TimeManager::Instance()->SetTempCurSnap();
				mPauseTime = true;
				Settings::GetInstance().SetBool("IsTimePaused", true);

				VRInputManager::GetInstance().RewindInputTimeline(
					TimeManager::Instance()->GetCurrentSnapFrame(),
					currentLevel->GetRightController()->GetUniqueID(),
					currentLevel->GetLeftController()->GetUniqueID());

				Component* comp = currentLevel->GetRightController()->GetComponentIndexed(eCOMPONENT_CODE, 0);
				if (dynamic_cast<BoxSnapToControllerAction*>(comp))
				{
					if(((BoxSnapToControllerAction*)comp)->mPickUp)
					((BoxSnapToControllerAction*)comp)->mPickUp->mShouldMove = true;
					((BoxSnapToControllerAction*)comp)->mPickUp = nullptr;
				}

				comp = currentLevel->GetLeftController()->GetComponentIndexed(eCOMPONENT_CODE, 0);
				if (dynamic_cast<BoxSnapToControllerAction*>(comp))
				{
					if (((BoxSnapToControllerAction*)comp)->mPickUp)
						((BoxSnapToControllerAction*)comp)->mPickUp->mShouldMove = true;
					((BoxSnapToControllerAction*)comp)->mPickUp = nullptr;
				}

				if (mCurCloneController1 && mCurCloneController2 && mCurCloneHeadset)
				{
					MeshComponent* mesh = (MeshComponent*)mCurCloneController2->GetComponentIndexed(eCOMPONENT_MESH, 0);
					if (mesh)
					{
						mesh->SetInMotion(true);
					}
					mesh = (MeshComponent*)mCurCloneController1->GetComponentIndexed(eCOMPONENT_MESH, 0);
					if (mesh)
					{
						mesh->SetInMotion(true);
					}
					mesh = (MeshComponent*)mCurCloneHeadset->GetComponentIndexed(eCOMPONENT_MESH, 0);
					if (mesh)
					{
						mesh->SetInMotion(true);
					}
					TimeManager::Instance()->ShowTimelineColliders(true);
				}
			}
		}

		// Update effect interpolator
		if (mDesaturationInterpolator.GetActive())
		{
			RenderShape* quad = Renderer::Instance()->GetSceneQuad();
			Renderer::Instance()->GetContext()->UpdateSubresource(quad->GetContext().mPixelCBuffers[ePB_REGISTER1].Get(), 0, NULL, &mEffectData, 0, 0);
		}
		if (mDesaturationInterpolator.Update(TimeManager::Instance()->GetDeltaTime()))
		{
			mDesaturationInterpolator.SetActive(false);
			RenderShape* quad = Renderer::Instance()->GetSceneQuad();
			Renderer::Instance()->GetContext()->UpdateSubresource(quad->GetContext().mPixelCBuffers[ePB_REGISTER1].Get(), 0, NULL, &mEffectData, 0, 0);
		}


		if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::k_EButton_SteamVR_Touchpad)
			&& (Settings::GetInstance().GetInt("tutStep") == 0 || Settings::GetInstance().GetInt("tutStep") > 3))//Created Clone (tut 2)
			{

			Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();

			// Accept timeline position
			if (mPauseTime)
			{
				//disable pointy bits on controller
				((MeshComponent*)currentLevel->GetRightController()->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
				((MeshComponent*)currentLevel->GetLeftController()->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);

				((SFXEmitter*)cLevel->GetHeadset()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 3))->CallEvent(Emitter::ePlay);
				if (Settings::GetInstance().GetInt("tutStep") >= 4)
				{
					if (Settings::GetInstance().GetBool("Level1Tutorial"))
						Settings::GetInstance().SetInt("tutStep", 6);//end
					else
						Settings::GetInstance().SetInt("tutStep", 7);//delete clone
				}
				vec2f finalRatios(0, 0);
				mDesaturationInterpolator.Prepare(0.5f, mEffectData.ratios, finalRatios, mEffectData.ratios);
				mDesaturationInterpolator.SetActive(true);

				if (mIsBeingMade &&  mNumOfConfirmedClones < cLevel->GetMaxClones() && TimeManager::Instance()->GetTempCurSnap() < TimeManager::Instance()->GetCurrentSnapFrame()-1 )
				{
		
					unsigned int curTime = TimeManager::Instance()->GetCurrentSnapFrame();
				
					//rewind first then enable clone
					TimeManager::Instance()->RewindTimeline(
						TimeManager::Instance()->GetTempCurSnap(),
						cLevel->GetHeadset()->GetUniqueID(),
						cLevel->GetRightController()->GetUniqueID(),
						cLevel->GetLeftController()->GetUniqueID());

					//rewind their creation time of the headset. This way its begining of the timeline is right after the last clones birth
					TimeManager::Instance()->SetCreationTimeofClone(cLevel->GetLeftController()->GetUniqueID(), cLevel->GetRightController()->GetUniqueID(), cLevel->GetHeadset()->GetUniqueID());
					if (mCurCloneHeadset && mCurCloneController1 && mCurCloneController2)
					{
						if (curTime > 0)
							curTime -= 1;
						//Set the final frame the clone is done at
						TimeManager::Instance()->SetClonesFinalFrame(mCurCloneHeadset->GetUniqueID(), curTime);
						TimeManager::Instance()->SetClonesFinalFrame(mCurCloneController1->GetUniqueID(), curTime);
						TimeManager::Instance()->SetClonesFinalFrame(mCurCloneController2->GetUniqueID(), curTime);

						//Update the made time of the clone
						TimeManager::Instance()->UpdateCloneMadeTime(mCurCloneHeadset->GetUniqueID(), mCurCloneController1->GetUniqueID(), mCurCloneController2->GetUniqueID());
						//add Interpolators for the clones
						TimeManager::Instance()->AddInterpolatorForClone(mCurCloneHeadset);
						TimeManager::Instance()->AddInterpolatorForClone(mCurCloneController1);
						TimeManager::Instance()->AddInterpolatorForClone(mCurCloneController2);
						TimeManager::Instance()->AssignTextureToClone(mCurCloneHeadset->GetUniqueID());
						//it is extreamly important that the objects are added after time rewinded because of the objectLifeTimeStruct and more..
						Physics::Instance()->mObjects.push_back(mCurCloneHeadset);
						Physics::Instance()->mObjects.push_back(mCurCloneController1);
						Physics::Instance()->mObjects.push_back(mCurCloneController2);
						++mNumOfConfirmedClones;


						Particle* p = &Particle::Init();
						p->SetColors(vec4f(0, .25, 1, 1), vec4f(.2f, .55f, .8f, 1));
						p->SetLife(250);
						p->SetSize(.25f, .15f);

						vec3f EPos = vec3f(mCurCloneHeadset->GetTransform().GetPosition()->x, mCurCloneHeadset->GetTransform().GetPosition()->y, mCurCloneHeadset->GetTransform().GetPosition()->z);
						ParticleEmitter* emit = new RadialEmitter(250, 250, 25, EPos);
						emit->SetParticle(p);
						emit->SetTexture("../Resources/BasicCircleP.png");
						ParticleSystem::Instance()->AddEmitter(emit);
						vec4f temp = EPos;
						AudioWrapper::GetInstance().MakeEventAtLocation(AK::EVENTS::SFX_SHORTCIRUIT, &temp);
						emit->FIRE();;

						Transform t;
						BaseObject *CloneDisplayNeedle = Pool::Instance()->iGetObject()->Reset("CloneIcon", t);
						MeshComponent* tdispn = new MeshComponent("../Resources/MiniClone.obj");
						tdispn->SetPixelShader(ePS_PURETEXTURE);
						tdispn->AddTexture(mCurrTexture.c_str(),TextureType::eTEX_DIFFUSE);
						CCCloneIndicator* time = new CCCloneIndicator(mCurCloneController1->GetUniqueID());
						CloneDisplayNeedle->AddComponent(tdispn);
						CloneDisplayNeedle->AddComponent(time);
						cLevel->GetRightController()->AddChild(CloneDisplayNeedle);
					}

				}
				else
				{
					if (mCurCloneHeadset && mCurCloneController1 && mCurCloneController2)
					{
						//switch with the headset to get our old info back and delete temp clone
						currentLevel->SetHeadsetAndControllers(mCurCloneHeadset, mCurCloneController1, mCurCloneController2, false);
						TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneHeadset);
						TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneController1);
						TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneController2);
						TimeManager::Instance()->DeleteClone(mCurCloneHeadset->GetUniqueID(), false);
					}

					//set the player headset and controllers birth back
					TimeManager::Instance()->SetCreationTimeofClone(cLevel->GetLeftController()->GetUniqueID(), cLevel->GetRightController()->GetUniqueID(), cLevel->GetHeadset()->GetUniqueID());
					//Update the made time of the old headset and controllers
					TimeManager::Instance()->UpdateCloneMadeTime(cLevel->GetLeftController()->GetUniqueID(), cLevel->GetRightController()->GetUniqueID(), cLevel->GetHeadset()->GetUniqueID());

					//rewind time after that was done to correctly update the player
					TimeManager::Instance()->RewindTimeline(
						TimeManager::Instance()->GetTempCurSnap(),
						cLevel->GetHeadset()->GetUniqueID(),
						cLevel->GetRightController()->GetUniqueID(),
						cLevel->GetLeftController()->GetUniqueID());

				}

				//rewind input timeline
				VRInputManager::GetInstance().RewindInputTimeline(
					TimeManager::Instance()->GetTempCurSnap(),
					cLevel->GetRightController()->GetUniqueID(),
					cLevel->GetLeftController()->GetUniqueID());

				mPauseTime = false;
				Settings::GetInstance().SetBool("IsTimePaused", false);
				mIsBeingMade = false;
				TimeManager::Instance()->ShowTimelineColliders(false);
			}

		}
		if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::k_EButton_SteamVR_Trigger))
		{
			//toggle to have clone turn on or off
			if (mPauseTime && (Settings::GetInstance().GetInt("tutStep") == 0 || Settings::GetInstance().GetInt("tutStep") >= 4))//rewound time (tut 1)
			{
				if (mCurCloneController1 && mCurCloneController2 && mCurCloneHeadset)
				{
					bool FirstFrame = TimeManager::Instance()->GetCurrentSnapFrame() < TimeManager::Instance()->GetTempCurSnap();
				
						if (LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->RaycastCloneCheck() == false)
						{
							if(!FirstFrame)
							mIsBeingMade = !mIsBeingMade;
						}

						if (!FirstFrame && mIsBeingMade)
						{
							if (Settings::GetInstance().GetInt("tutStep") == 4)//rewound time (tut 1)
								Settings::GetInstance().SetInt("tutStep", 6);//pick up box (tut 1)

							((MeshComponent*)mCurCloneHeadset->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetAlpha(1);
							((MeshComponent*)mCurCloneController1->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetAlpha(1);
							((MeshComponent*)mCurCloneController2->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetAlpha(1);
							SystemLogger::GetLog() << "Opaque" << std::endl;
						}
						else
						{
							((MeshComponent*)mCurCloneHeadset->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetAlpha(.5f);
							((MeshComponent*)mCurCloneController1->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetAlpha(.5f);
							((MeshComponent*)mCurCloneController2->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetAlpha(.5f);
							SystemLogger::GetLog() << "Transparent" << std::endl;
						}
					}
					else
					{
						LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->RaycastCloneCheck();
					}
				}
			
		}

	}
	void TimeManipulation::MakeCloneBaseObjects(BaseObject * _headset, BaseObject * _controller1, BaseObject * _controller2)
	{
		mIsBeingMade = false;
		Level* currentLevel = LevelManager::GetInstance().GetCurrentLevel();
		//If you change the name. Pls change it in Timemanager::findotherclones otherwise there will be problems
		SystemLogger::GetLog() << "[Debug] A clone is being made, please hold: " << mCloneCount << " | Is left: " << mControllerRole << std::endl;

		PSTransparentScanline_Data _data;
		_data.alpha = 1;
		_data.ScanlineData.x = 0;
		_data.ScanlineData.y = 0.2f;
		_data.ScanlineData.z = 0;
		_data.ScanlineData.w = 0.8f;
		mCurrTexture = TimeManager::Instance()->GetNextTexture();
		MeshComponent *visibleMesh = new MeshComponent("../Resources/Clone.obj", .5f);
		visibleMesh->SetPixelShader(ePS_TRANSPARENT);
		visibleMesh->AddTexture(mCurrTexture.c_str(), eTEX_DIFFUSE);

		/*SphereCollider* col = new SphereCollider(_headset, false, false, vec3f(), 100.0f, .02f, .2f, .2f, 0, 1, "../Resources/Clone.obj");
		CCCloneHeadCollider* headCol = new CCCloneHeadCollider();*/
		//visibleMesh->AddTexture("../Resources/Multiscan.png", eTEX_CUSTOM1);
		//visibleMesh->AddTexture("../Resources/Scanline.png", eTEX_CUSTOM2);

		//visibleMesh->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
		//D3D11_SUBRESOURCE_DATA initialData;
		//initialData.pSysMem = &_data;
		//CD3D11_BUFFER_DESC bufferDesc(sizeof(PSTransparentScanline_Data), D3D11_BIND_CONSTANT_BUFFER);
		//Renderer::Instance()->GetDevice()->CreateBuffer(&bufferDesc, &initialData, visibleMesh->GetShape()->GetContext().mPixelCBuffers[ePB_CUSTOM1].GetAddressOf());
		//SetD3DName(visibleMesh->GetShape()->GetContext().mPixelCBuffers[ePB_CUSTOM1].Get(), "Headset scanline buffer");
		_headset->AddComponent(visibleMesh);
		/*_headset->AddComponent(col);
		_headset->AddComponent(headCol);*/

		//If you change the name. Pls change it in Timemanager::findotherclones otherwise there will be problems
		MeshComponent *mc = new MeshComponent("../Resources/Player_hand.obj",.5f);
		mc->AddTexture("../Resources/Player_hand_Diffuse.png", eTEX_DIFFUSE);
		mc->AddTexture("../Resources/Player_hand_Emissive.png", eTEX_EMISSIVE);
		mc->AddTexture("../Resources/Player_hand_Normal.png", eTEX_NORMAL);
		mc->AddTexture("../Resources/Player_hand_Specular", eTEX_SPECULAR);
		ControllerCollider* CubeColider = new ControllerCollider(_controller1, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), true);
		mc->SetPixelShader(ePS_TRANSPARENT);
		//mc->AddTexture("../Resources/Multiscan.png", eTEX_CUSTOM1);
		//mc->AddTexture("../Resources/Scanline.png", eTEX_CUSTOM2);

		//mc->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
		//initialData.pSysMem = &_data;
		//Renderer::Instance()->GetDevice()->CreateBuffer(&bufferDesc, &initialData, mc->GetShape()->GetContext().mPixelCBuffers[ePB_CUSTOM1].GetAddressOf());
		//SetD3DName(mc->GetShape()->GetContext().mPixelCBuffers[ePB_CUSTOM1].Get(), "Controller1 scanline buffer");
		_controller1->AddComponent(mc);
		_controller1->AddComponent(CubeColider);
		BoxSnapToControllerAction* SN1 = new BoxSnapToControllerAction();
		_controller1->AddComponent(SN1);

		//If you change the name. Pls change it in Timemanager::findotherclones otherwise there will be proble
		MeshComponent *mc2 = new MeshComponent("../Resources/Player_hand.obj",.5f);
		mc2->AddTexture("../Resources/Player_hand_Diffuse.png", eTEX_DIFFUSE);
		mc2->AddTexture("../Resources/Player_hand_Emissive.png", eTEX_EMISSIVE);
		mc2->AddTexture("../Resources/Player_hand_Normal.png", eTEX_NORMAL);
		mc2->AddTexture("../Resources/Player_hand_Specular", eTEX_SPECULAR);
		ControllerCollider* CubeColider2 = new ControllerCollider(_controller2, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), false);
		mc2->SetPixelShader(ePS_TRANSPARENT);
		//mc2->AddTexture("../Resources/Multiscan.png", eTEX_CUSTOM1);
		//mc2->AddTexture("../Resources/Scanline.png", eTEX_CUSTOM2);

		//mc2->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
		//initialData.pSysMem = &_data;
		//Renderer::Instance()->GetDevice()->CreateBuffer(&bufferDesc, &initialData, mc2->GetShape()->GetContext().mPixelCBuffers[ePB_CUSTOM1].GetAddressOf());
		//SetD3DName(mc->GetShape()->GetContext().mPixelCBuffers[ePB_CUSTOM1].Get(), "Controller2 scanline buffer");
		_controller2->AddComponent(mc2);
		_controller2->AddComponent(CubeColider2);
		BoxSnapToControllerAction* SN2 = new BoxSnapToControllerAction();
		_controller2->AddComponent(SN2);

		//
		//KEEP THIS ORDER NO MATTER WHAT!!!! 
		//
		//Make a clone 3 seconds ago.
		TimeManager::Instance()->RewindMakeClone(TimeManager::Instance()->GetCurrentSnapFrame(), _headset, _controller1, _controller2);
		currentLevel->SetHeadsetAndControllers(_headset, _controller1, _controller2, true);

		//new Objects are added to the timeline to update the old player BaseObject pointers
		TimeManager::Instance()->UpdatePlayerObjectInTimeline(_headset);
		TimeManager::Instance()->UpdatePlayerObjectInTimeline(_controller1);
		TimeManager::Instance()->UpdatePlayerObjectInTimeline(_controller2);


		TimeManager::Instance()->SetupClonePairs(_headset->GetUniqueID(), _controller1->GetUniqueID(), _controller2->GetUniqueID());
	}

	bool TimeManipulation::RaycastCloneCheck()
	{
		if (!VRInputManager::GetInstance().GetController(mControllerRole).GetValid())
		{
			return false;
		}
		matrix4 mat = VRInputManager::GetInstance().GetController(mControllerRole).GetPosition();
		mObject->GetTransform().SetMatrix(mat);

		std::vector<BaseObject*> clones = TimeManager::Instance()->GetClonesVec();
		for (int i = 0; i < clones.size(); ++i)
		{
			if (clones[i]->GetUniqueID() == mCurCloneHeadset->GetUniqueID() || clones[i]->GetUniqueID() == mCurCloneController1->GetUniqueID() ||
				clones[i]->GetUniqueID() == mCurCloneController2->GetUniqueID())
				continue;
			MeshComponent* mesh = (MeshComponent*)clones[i]->GetComponentIndexed(eCOMPONENT_MESH, 0);
			vec4f forward;
			forward.Set(0, 0, 1, 0);
			matrix4 inverse = (mat * clones[i]->GetTransform().GetMatrix().Invert());
			vec3f meshPos = inverse.Position;
			forward *= inverse;
			vec3f fwd = forward;
			Triangle *tris = mesh->GetTriangles();
			size_t numTris = mesh->GetTriangleCount();
			for (unsigned int j = 0; j < numTris; ++j)
			{
				float hitTime;
				if (Physics::Instance()->RayToTriangle((tris + j)->Vertex[0], (tris + j)->Vertex[1], (tris + j)->Vertex[2], (tris + j)->Normal, meshPos, fwd, hitTime))
				{
					TimeManager::Instance()->DeleteClone(clones[i]->GetUniqueID(), true);
					--mNumOfConfirmedClones;

					if (Settings::GetInstance().GetInt("tutStep") == 7)//deleted clone (tut 2)
						Settings::GetInstance().SetInt("tutStep", 8);//finished level 2 tutorial

					return true;
				}
			}
		}
		return false;
	}

} // Epoch Namespace