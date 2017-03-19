
#include "../Objects/Component.h"
#include "../Objects/MeshComponent.h"
#include "../Actions/CodeComponent.hpp"
#include "../Objects/BaseObject.h"
#include "../Core/LevelManager.h"
#include "../Input/VRInputManager.h"
#include "../Rendering/TextureManager.h"
#include "../Rendering/Draw2D.h"
#include "../Core/Pool.h"
#include "TimeManipulation.h"
#include "BoxSnapToControllerAction.hpp"

namespace Epoch
{

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
		
		if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_ApplicationMenu)) {
			int frameRewind = 30;

			if (!TimeManager::Instance()->CheckRewindAvaliable(frameRewind))
				return;

			SystemLogger::GetLog() << "[Debug] A clone is being made, please hold: " << mCloneCount << " | Is left: " << mControllerRole << std::endl;

			Transform identity;

			
			//If you change the name. Pls change it in Timemanager::findotherclones otherwise there will be problems
			BaseObject* headset = Pool::Instance()->iGetObject()->Reset("Headset - " + std::to_string(mCloneCount),  identity ); //new BaseObject("headset" + std::to_string(rand), identity);
			MeshComponent *visibleMesh = new MeshComponent("../Resources/Clone.obj");

			visibleMesh->AddTexture(TimeManager::Instance()->GetNextTexture().c_str(), eTEX_DIFFUSE);
			headset->AddComponent(visibleMesh);

			//If you change the name. Pls change it in Timemanager::findotherclones otherwise there will be problems
			BaseObject* Controller1 = Pool::Instance()->iGetObject()->Reset("Controller1 - " + std::to_string(mCloneCount), identity); //new BaseObject("Controller" + std::to_string(rand), identity);
			MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");
			ControllerCollider* CubeColider = new ControllerCollider(Controller1, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), true);
			mc->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
			Controller1->AddComponent(CubeColider);
			BoxSnapToControllerAction* SN1 = new BoxSnapToControllerAction();
			Controller1->AddComponent(mc);
			Controller1->AddComponent(SN1);

			//If you change the name. Pls change it in Timemanager::findotherclones otherwise there will be problems
			BaseObject* Controller2 = Pool::Instance()->iGetObject()->Reset("Controller2 - " + std::to_string(mCloneCount), identity); //new BaseObject("Controller" + std::to_string(rand), identity);
			MeshComponent *mc2 = new MeshComponent("../Resources/Controller.obj");
			ControllerCollider* CubeColider2 = new ControllerCollider(Controller2, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), false);
			mc2->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
			Controller2->AddComponent(CubeColider2);
			BoxSnapToControllerAction* SN2 = new BoxSnapToControllerAction();
			Controller2->AddComponent(mc2);
			Controller2->AddComponent(SN2);



			//
			//KEEP THIS ORDER NO MATTER WHAT!!!! 
			//
			//Make a clone 3 seconds ago.
			TimeManager::Instance()->RewindMakeClone(TimeManager::Instance()->GetCurrentSnapFrame() - frameRewind, headset, Controller1, Controller2);
			currentLevel->SetHeadsetAndControllers(headset, Controller1, Controller2, CubeColider, CubeColider2);
			//it is extreamly important that the objects are added after time rewinded because of the objectLifeTimeStruct and more..
			Physics::Instance()->mObjects.push_back(headset);
			Physics::Instance()->mObjects.push_back(Controller1);
			Physics::Instance()->mObjects.push_back(Controller2);
			//new Objects are added to the timeline to update the old player BaseObject pointers
			TimeManager::Instance()->UpdatePlayerObjectInTimeline(headset);
			TimeManager::Instance()->UpdatePlayerObjectInTimeline(Controller1);
			TimeManager::Instance()->UpdatePlayerObjectInTimeline(Controller2);
			//Rewind InputTime
			VRInputManager::GetInstance().RewindInputTimeline(TimeManager::Instance()->GetCurrentSnapFrame(), currentLevel->GetRightController()->GetUniqueID(), currentLevel->GetLeftController()->GetUniqueID());
			//add Interpolators for the clones
			TimeManager::Instance()->AddInterpolatorForClone(headset);
			TimeManager::Instance()->AddInterpolatorForClone(Controller1);
			TimeManager::Instance()->AddInterpolatorForClone(Controller2);
			
			TimeManager::Instance()->AssignTextureToClone(headset->GetUniqueId());
			

			mCloneCount++;
		}

		if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_Grip)) {
			/*int frameRewind = 30;
			if (!TimeManager::Instance()->CheckRewindAvaliable(frameRewind))
				return;

			TimeManager::Instance()->RewindTimeline(TimeManager::Instance()->GetCurrentSnapFrame() - frameRewind, Level::Instance()->iGetHeadset()->GetUniqueID(), Level::Instance()->iGetRightController()->GetUniqueID(), Level::Instance()->iGetLeftController()->GetUniqueID());
			VRInputManager::GetInstance().RewindInputTimeline(TimeManager::Instance()->GetCurrentSnapFrame(), Level::Instance()->iGetRightController()->GetUniqueID(), Level::Instance()->iGetLeftController()->GetUniqueID());*/
			Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();
			if (mPauseTime) {
				// Resume Time
				vec2f finalRatios(0, 0);
				mDesaturationInterpolator.Prepare(0.5f, mEffectData.ratios, finalRatios, mEffectData.ratios);
				mDesaturationInterpolator.SetActive(true);

				mPauseTime = false;
				TimeManager::Instance()->RewindTimeline(
					TimeManager::Instance()->GetCurrentSnapFrame(),
					cLevel->GetHeadset()->GetUniqueID(),
					cLevel->GetRightController()->GetUniqueID(),
					cLevel->GetLeftController()->GetUniqueID()
				);
			} else {
				// Stop time
				vec2f finalRatios(0.7, 0.3);
				mDesaturationInterpolator.Prepare(0.5f, mEffectData.ratios, finalRatios, mEffectData.ratios);
				mDesaturationInterpolator.SetActive(true);
				TimeManager::Instance()->SetTempCurSnap();
				mPauseTime = true;
			}
		}

		// Update effect interpolator
		if (mDesaturationInterpolator.GetActive()) {
			RenderShape* quad = Renderer::Instance()->GetSceneQuad();
			Renderer::Instance()->GetContext()->UpdateSubresource(quad->GetContext().mPixelCBuffers[ePB_SLOT2].Get(), 0, NULL, &mEffectData, 0, 0);
		}
		if (mDesaturationInterpolator.Update(TimeManager::Instance()->GetDeltaTime())) {
			mDesaturationInterpolator.SetActive(false);
			RenderShape* quad = Renderer::Instance()->GetSceneQuad();
			Renderer::Instance()->GetContext()->UpdateSubresource(quad->GetContext().mPixelCBuffers[ePB_SLOT2].Get(), 0, NULL, &mEffectData, 0, 0);
		}


		if (VRInputManager::GetInstance().GetController(mControllerRole).GetPress(vr::k_EButton_SteamVR_Touchpad)) {
			bool right = false;
			bool left = false;
			Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();

			if (cLevel->GetRightTimeManinpulator() != nullptr || cLevel->GetLeftTimeManinpulator() != nullptr) {
				right = cLevel->GetRightTimeManinpulator()->isTimePaused();
				left = cLevel->GetLeftTimeManinpulator()->isTimePaused();
			}

			// Accept timeline position
			if (left || right) {
				vec2f finalRatios(0, 0);
				mDesaturationInterpolator.Prepare(0.5f, mEffectData.ratios, finalRatios, mEffectData.ratios);
				mDesaturationInterpolator.SetActive(true);
				TimeManager::Instance()->RewindTimeline(
					TimeManager::Instance()->GetTempCurSnap(),
					cLevel->GetHeadset()->GetUniqueID(),
					cLevel->GetRightController()->GetUniqueID(),
					cLevel->GetLeftController()->GetUniqueID());

				VRInputManager::GetInstance().RewindInputTimeline(
					TimeManager::Instance()->GetTempCurSnap(),
					cLevel->GetRightController()->GetUniqueID(),
					cLevel->GetLeftController()->GetUniqueID());

				cLevel->GetLeftTimeManinpulator()->makeTimePaused(false);
				cLevel->GetRightTimeManinpulator()->makeTimePaused(false);
			}

			if(GetAsyncKeyState(VK_END) & 1)
			{
				HotfixButtonDown++;
				if (HotfixButtonDown > 169) {
					HotfixButtonDown = 0;
					TimeManager::Instance()->HotfixResetTimeline();
				}
			}
			else
			{
				HotfixButtonDown = 0;
			}
		}
		else
		{
			HotfixButtonDown = 0;
		}
	};

	void TimeManipulation::RaycastCloneCheck() {
		matrix4 mat = VRInputManager::GetInstance().GetController(mControllerRole).GetPosition();
		mObject->GetTransform().SetMatrix(mat);

			std::vector<BaseObject*> clones = TimeManager::Instance()->GetClonesVec();
		
			for (int i = 0; i < clones.size(); ++i) {
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
							TimeManager::Instance()->DeleteClone(clones[i]->GetUniqueId());
							return;
					}
				}
			}
		}
	
} // Epoch Namespace