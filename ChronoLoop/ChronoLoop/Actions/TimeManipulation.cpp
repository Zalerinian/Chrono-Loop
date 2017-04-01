
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
#include "../Objects/TransparentMeshComponent.h"
#include "../Common/Common.h"
#include "../Messager/Messager.h"
#include "../Particles/ParticleSystem.h"

namespace Epoch
{
	BaseObject* TimeManipulation::mCurCloneController1 = nullptr;
	BaseObject* TimeManipulation::mCurCloneController2 = nullptr;
	BaseObject* TimeManipulation::mCurCloneHeadset = nullptr;
	bool TimeManipulation::mIsBeingMade = false;
	unsigned short TimeManipulation::mNumOfConfirmedClones = 0;

	TimeManipulation::TimeManipulation() {
	}


	TimeManipulation::~TimeManipulation() {}

	

	unsigned int TimeManipulation::mCloneCount = 0;
	unsigned short TimeManipulation::mCurrTexture = 0;

	void TimeManipulation::Start()
	{
		mEffectData.saturationColor.Set(0.30f, 0.59f, 0.11f, 0);
		mEffectData.tintColor.Set(1, 0.85f, 1, 1);
		mEffectData.ratios.Set(0, 0);
		mEffectData.fullRatios.Set(0.7f, 0.3f);
	}

	void TimeManipulation::Update() {
		Level* currentLevel = LevelManager::GetInstance().GetCurrentLevel();
		

		if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_Grip)) {
			
			Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();
			if (mPauseTime) {
				// Resume Time

				//put the original controll and headset back in control
				//Remove the clone created
				if (mCurCloneHeadset && mCurCloneController1 && mCurCloneController2)
				{
					currentLevel->SetHeadsetAndControllers(mCurCloneHeadset, mCurCloneController1, mCurCloneController2,false);
					TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneHeadset);
					TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneController1);
					TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneController2);
					TimeManager::Instance()->DeleteClone(mCurCloneHeadset->GetUniqueID(),false);
					mCurCloneHeadset = nullptr;
					mCurCloneController1 = nullptr;
					mCurCloneController2 = nullptr;
				}

				vec2f finalRatios(0, 0);
				mDesaturationInterpolator.Prepare(0.5f, mEffectData.ratios, finalRatios, mEffectData.ratios);
				mDesaturationInterpolator.SetActive(true);

				mPauseTime = false;
				TimeManager::Instance()->RewindTimeline(
					TimeManager::Instance()->GetCurrentSnapFrame(),
					cLevel->GetHeadset()->GetUniqueID(),
					cLevel->GetRightController()->GetUniqueID(),
					cLevel->GetLeftController()->GetUniqueID());
					
				mIsBeingMade = false;
				
			} else {
				// Stop time
				
				Transform identity;
				memset(&identity.GetMatrix(), 0, sizeof(identity.GetMatrix()));
				mCloneCount++;
				mCurCloneHeadset = Pool::Instance()->iGetObject()->Reset("Headset - " + std::to_string(mCloneCount), identity); //new BaseObject("headset" + std::to_string(rand), identity);
				mCurCloneController1 = Pool::Instance()->iGetObject()->Reset("Controller1 - " + std::to_string(mCloneCount), identity); //new BaseObject("Controller" + std::to_string(rand), identity);
				mCurCloneController2 = Pool::Instance()->iGetObject()->Reset("Controller2 - " + std::to_string(mCloneCount), identity); //new BaseObject("Controller" + std::to_string(rand), identity);
				MakeCloneBaseObjects(mCurCloneHeadset, mCurCloneController1, mCurCloneController2);

				vec2f finalRatios(0.7f, 0.3f);
				mDesaturationInterpolator.Prepare(0.5f, mEffectData.ratios, finalRatios, mEffectData.ratios);
				mDesaturationInterpolator.SetActive(true);
				TimeManager::Instance()->SetTempCurSnap();
				mPauseTime = true;
			}
		}

		// Update effect interpolator
		if (mDesaturationInterpolator.GetActive()) {
			RenderShape* quad = Renderer::Instance()->GetSceneQuad();
			Renderer::Instance()->GetContext()->UpdateSubresource(quad->GetContext().mPixelCBuffers[ePB_REGISTER1].Get(), 0, NULL, &mEffectData, 0, 0);
		}
		if (mDesaturationInterpolator.Update(TimeManager::Instance()->GetDeltaTime())) {
			mDesaturationInterpolator.SetActive(false);
			RenderShape* quad = Renderer::Instance()->GetSceneQuad();
			Renderer::Instance()->GetContext()->UpdateSubresource(quad->GetContext().mPixelCBuffers[ePB_REGISTER1].Get(), 0, NULL, &mEffectData, 0, 0);
		}


		if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::k_EButton_SteamVR_Touchpad)) {
			bool right = false;
			bool left = false;
			Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();

			if (cLevel->GetRightTimeManipulator() != nullptr || cLevel->GetLeftTimeManipulator() != nullptr) {
				right = cLevel->GetRightTimeManipulator()->isTimePaused();
				left = cLevel->GetLeftTimeManipulator()->isTimePaused();
			}

			// Accept timeline position
			if (left || right) {
			
				vec2f finalRatios(0, 0);
				mDesaturationInterpolator.Prepare(0.5f, mEffectData.ratios, finalRatios, mEffectData.ratios);
				mDesaturationInterpolator.SetActive(true);
		

				if (mIsBeingMade)
				{
					//rewind first then enable clone
					TimeManager::Instance()->RewindTimeline(
						TimeManager::Instance()->GetTempCurSnap(),
						cLevel->GetHeadset()->GetUniqueID(),
						cLevel->GetRightController()->GetUniqueID(),
						cLevel->GetLeftController()->GetUniqueID());

					//rewind their creation time of the headset. This way its begining of the timeline is right after the last clones birth
					TimeManager::Instance()->SetCreationTimeofClone(cLevel->GetLeftController()->GetUniqueID(), cLevel->GetRightController()->GetUniqueID(), cLevel->GetHeadset()->GetUniqueID());
					//Update the made time of the clone
					TimeManager::Instance()->UpdateCloneMadeTime(mCurCloneHeadset->GetUniqueID(), mCurCloneController1->GetUniqueID(), mCurCloneController2->GetUniqueID());
					//add Interpolators for the clones
					TimeManager::Instance()->AddInterpolatorForClone(mCurCloneHeadset);
					TimeManager::Instance()->AddInterpolatorForClone(mCurCloneController1);
					TimeManager::Instance()->AddInterpolatorForClone(mCurCloneController2);
					TimeManager::Instance()->AssignTextureToClone(mCurCloneHeadset->GetUniqueId());
					//it is extreamly important that the objects are added after time rewinded because of the objectLifeTimeStruct and more..
					Physics::Instance()->mObjects.push_back(mCurCloneHeadset);
					Physics::Instance()->mObjects.push_back(mCurCloneController1);
					Physics::Instance()->mObjects.push_back(mCurCloneController2);
					++mNumOfConfirmedClones;

					//TODO: MAKE CLONE PARTICLE
					Particle* p = &Particle::Init();
					p->SetColors(vec4f(0, .25, 1,1), vec4f(.2f, .55f, .8f,1));
					p->SetLife(250);
					p->SetSize(.25f, .15f);

					vec3f EPos = vec3f(mCurCloneHeadset->GetTransform().GetPosition()->x,mCurCloneHeadset->GetTransform().GetPosition()->y, mCurCloneHeadset->GetTransform().GetPosition()->z);
					ParticleEmitter* emit = new RadialEmitter(250, 250, 25, EPos);
					emit->SetParticle(p);
					emit->SetTexture("../Resources/BasicCircleP.png");
					ParticleSystem::Instance()->AddEmitter(emit);
					vec4f temp = EPos;
					AudioWrapper::GetInstance().MakeEventAtLocation(AK::EVENTS::SFX_SHORTCIRUIT, &temp);
					emit->FIRE();
				}
				else
				{
					if (mCurCloneHeadset && mCurCloneController1 && mCurCloneController2)
					{
						//switch with the headset to get our old info back and delete temp clone
						currentLevel->SetHeadsetAndControllers(mCurCloneHeadset, mCurCloneController1, mCurCloneController2,false);
						TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneHeadset);
						TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneController1);
						TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneController2);
						TimeManager::Instance()->DeleteClone(mCurCloneHeadset->GetUniqueId(),false);

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
				}

				//rewind input timeline
				VRInputManager::GetInstance().RewindInputTimeline(
					TimeManager::Instance()->GetTempCurSnap(),
					cLevel->GetRightController()->GetUniqueID(),
					cLevel->GetLeftController()->GetUniqueID());

				cLevel->GetLeftTimeManipulator()->makeTimePaused(false);
				cLevel->GetRightTimeManipulator()->makeTimePaused(false);
				mIsBeingMade = false;
			}

		
			else
			{
				HotfixButtonDown++;
				if (HotfixButtonDown > 100) {
					HotfixButtonDown = 0;
					TimeManager::Instance()->HotfixResetTimeline();
				}
			}
		}
		else
		{
			HotfixButtonDown = 0;
		}
		if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::k_EButton_SteamVR_Trigger))
		{
			//toggle to have clone turn on or off
			if (mPauseTime)
			{
				if (LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator()->RaycastCloneCheck() == false && 
				LevelManager::GetInstance().GetCurrentLevel()->GetLeftTimeManipulator()->RaycastCloneCheck() == false)
				{
				mIsBeingMade = !mIsBeingMade;	
				}

				if(mCurCloneController1 && mCurCloneController2 && mCurCloneHeadset)
				{
					if(mIsBeingMade)
					{
						((TransparentMeshComponent*)mCurCloneHeadset->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetAlpha(1);
						((TransparentMeshComponent*)mCurCloneController1->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetAlpha(1);
						((TransparentMeshComponent*)mCurCloneController2->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetAlpha(1);
						SystemLogger::GetLog() << "Opaque" << std::endl;
					}
					else
					{
						((TransparentMeshComponent*)mCurCloneHeadset->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetAlpha(.35f);
						((TransparentMeshComponent*)mCurCloneController1->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetAlpha(.35f);
						((TransparentMeshComponent*)mCurCloneController2->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetAlpha(.35f);
						SystemLogger::GetLog() << "Transparent" << std::endl;
					}
				}
			}
		}
	}
	void TimeManipulation::MakeCloneBaseObjects(BaseObject * _headset, BaseObject * _controller1, BaseObject * _controller2)
	{
		Level* currentLevel = LevelManager::GetInstance().GetCurrentLevel();
		//If you change the name. Pls change it in Timemanager::findotherclones otherwise there will be problems
		SystemLogger::GetLog() << "[Debug] A clone is being made, please hold: " << mCloneCount << " | Is left: " << mControllerRole << std::endl;

		PSTransparentScanline_Data _data;
		_data.alpha = 1;
		_data.ScanlineData.x = 0;
		_data.ScanlineData.y = 0.2f;
		_data.ScanlineData.z = 0;
		_data.ScanlineData.w = 0.8f;
		TransparentMeshComponent *visibleMesh = new TransparentMeshComponent("../Resources/Clone.obj",.35f);
		visibleMesh->AddTexture(TimeManager::Instance()->GetNextTexture().c_str(), eTEX_DIFFUSE);
		//visibleMesh->AddTexture("../Resources/Multiscan.png", eTEX_CUSTOM1);
		//visibleMesh->AddTexture("../Resources/Scanline.png", eTEX_CUSTOM2);

		//visibleMesh->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
		//D3D11_SUBRESOURCE_DATA initialData;
		//initialData.pSysMem = &_data;
		//CD3D11_BUFFER_DESC bufferDesc(sizeof(PSTransparentScanline_Data), D3D11_BIND_CONSTANT_BUFFER);
		//Renderer::Instance()->GetDevice()->CreateBuffer(&bufferDesc, &initialData, visibleMesh->GetShape()->GetContext().mPixelCBuffers[ePB_CUSTOM1].GetAddressOf());
		//SetD3DName(visibleMesh->GetShape()->GetContext().mPixelCBuffers[ePB_CUSTOM1].Get(), "Headset scanline buffer");
		_headset->AddComponent(visibleMesh);

		//If you change the name. Pls change it in Timemanager::findotherclones otherwise there will be problems
		TransparentMeshComponent *mc = new TransparentMeshComponent("../Resources/Controller.obj",.35f);
		ControllerCollider* CubeColider = new ControllerCollider(_controller1, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), true);
		mc->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
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
		TransparentMeshComponent *mc2 = new TransparentMeshComponent("../Resources/Controller.obj",.35f);
		ControllerCollider* CubeColider2 = new ControllerCollider(_controller2, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), false);
		mc2->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
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
		currentLevel->SetHeadsetAndControllers(_headset, _controller1, _controller2,true);
	
		//new Objects are added to the timeline to update the old player BaseObject pointers
		TimeManager::Instance()->UpdatePlayerObjectInTimeline(_headset);
		TimeManager::Instance()->UpdatePlayerObjectInTimeline(_controller1);
		TimeManager::Instance()->UpdatePlayerObjectInTimeline(_controller2);
		
		TimeManager::Instance()->SetupClonePairs(_headset->GetUniqueID(),_controller1->GetUniqueID(), _controller2->GetUniqueID());
	}

	bool TimeManipulation::RaycastCloneCheck() {
		if(!VRInputManager::GetInstance().GetController(mControllerRole).GetValid())
		{
			return false;
		}
		matrix4 mat = VRInputManager::GetInstance().GetController(mControllerRole).GetPosition();
		mObject->GetTransform().SetMatrix(mat);

			std::vector<BaseObject*> clones = TimeManager::Instance()->GetClonesVec();
			for (int i = 0; i < clones.size(); ++i) {
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
				for (unsigned int j = 0; j < numTris; ++j) {
					float hitTime;
					if (Physics::Instance()->RayToTriangle((tris + j)->Vertex[0], (tris + j)->Vertex[1], (tris + j)->Vertex[2], (tris + j)->Normal, meshPos, fwd, hitTime)) { 
							TimeManager::Instance()->DeleteClone(clones[i]->GetUniqueId(),true);
							--mNumOfConfirmedClones;
							return true;
					}
				}
			}
			return false;
		}
	
} // Epoch Namespace