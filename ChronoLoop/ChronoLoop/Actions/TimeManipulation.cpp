
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
		
	
		if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_Grip)) {
			/*int frameRewind = 30;
			if (!TimeManager::Instance()->CheckRewindAvaliable(frameRewind))
				return;

			TimeManager::Instance()->RewindTimeline(TimeManager::Instance()->GetCurrentSnapFrame() - frameRewind, Level::Instance()->iGetHeadset()->GetUniqueID(), Level::Instance()->iGetRightController()->GetUniqueID(), Level::Instance()->iGetLeftController()->GetUniqueID());
			VRInputManager::GetInstance().RewindInputTimeline(TimeManager::Instance()->GetCurrentSnapFrame(), Level::Instance()->iGetRightController()->GetUniqueID(), Level::Instance()->iGetLeftController()->GetUniqueID());*/
			Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();
			if (mPauseTime) {
				// Resume Time

				//put the original controll and headset back in control
				//Remove the clone created
				currentLevel->SetHeadsetAndControllers(mCurCloneHeadset, mCurCloneController1, mCurCloneController2);
				TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneHeadset);
				TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneController1);
				TimeManager::Instance()->UpdatePlayerObjectInTimeline(mCurCloneController2);
				TimeManager::Instance()->DeleteClone(mCurCloneHeadset->GetUniqueID());

				vec2f finalRatios(0, 0);
				mDesaturationInterpolator.Prepare(0.5f, mEffectData.ratios, finalRatios, mEffectData.ratios);
				mDesaturationInterpolator.SetActive(true);

				mPauseTime = false;
				TimeManager::Instance()->RewindTimeline(
					TimeManager::Instance()->GetCurrentSnapFrame(),
					cLevel->GetHeadset()->GetUniqueID(),
					cLevel->GetRightController()->GetUniqueID(),
					cLevel->GetLeftController()->GetUniqueID());
					
				
			} else {
				// Stop time
				//pat added
				Transform identity;
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

			if (cLevel->GetRightTimeManipulator() != nullptr || cLevel->GetLeftTimeManipulator() != nullptr) {
				right = cLevel->GetRightTimeManipulator()->isTimePaused();
				left = cLevel->GetLeftTimeManipulator()->isTimePaused();
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

				//rewind their creation time
				TimeManager::Instance()->SetCreationTimeofClone(mCurCloneHeadset->GetUniqueID(), mCurCloneController1->GetUniqueID(), mCurCloneController2->GetUniqueID());
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

				//rewind input timeline
				VRInputManager::GetInstance().RewindInputTimeline(
					TimeManager::Instance()->GetTempCurSnap(),
					cLevel->GetRightController()->GetUniqueID(),
					cLevel->GetLeftController()->GetUniqueID());

				cLevel->GetLeftTimeManipulator()->makeTimePaused(false);
				cLevel->GetRightTimeManipulator()->makeTimePaused(false);
			}

		
			else
			{
				SystemLogger::GetLog() << HotfixButtonDown << std::endl;
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
	}
	void TimeManipulation::MakeCloneBaseObjects(BaseObject * _headset, BaseObject * _controller1, BaseObject * _controller2)
	{
		Level* currentLevel = LevelManager::GetInstance().GetCurrentLevel();
		//If you change the name. Pls change it in Timemanager::findotherclones otherwise there will be problems
		mCloneCount++;
		SystemLogger::GetLog() << "[Debug] A clone is being made, please hold: " << mCloneCount << " | Is left: " << mControllerRole << std::endl;

		MeshComponent *visibleMesh = new MeshComponent("../Resources/Clone.obj");
		visibleMesh->AddTexture(TimeManager::Instance()->GetNextTexture().c_str(), eTEX_DIFFUSE);
		_headset->AddComponent(visibleMesh);

		//If you change the name. Pls change it in Timemanager::findotherclones otherwise there will be problems
		MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");
		ControllerCollider* CubeColider = new ControllerCollider(_controller1, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), true);
		mc->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
		_controller1->AddComponent(mc);
		_controller1->AddComponent(CubeColider);
		BoxSnapToControllerAction* SN1 = new BoxSnapToControllerAction();
		_controller1->AddComponent(SN1);

		//If you change the name. Pls change it in Timemanager::findotherclones otherwise there will be proble
		MeshComponent *mc2 = new MeshComponent("../Resources/Controller.obj");
		ControllerCollider* CubeColider2 = new ControllerCollider(_controller2, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), false);
		mc2->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
		_controller2->AddComponent(mc2);
		_controller2->AddComponent(CubeColider2);
		BoxSnapToControllerAction* SN2 = new BoxSnapToControllerAction();
		_controller2->AddComponent(SN2);

		//
		//KEEP THIS ORDER NO MATTER WHAT!!!! 
		//
		//Make a clone 3 seconds ago.
		TimeManager::Instance()->RewindMakeClone(TimeManager::Instance()->GetCurrentSnapFrame(), _headset, _controller1, _controller2);
		currentLevel->SetHeadsetAndControllers(_headset, _controller1, _controller2);
	
		//new Objects are added to the timeline to update the old player BaseObject pointers
		TimeManager::Instance()->UpdatePlayerObjectInTimeline(_headset);
		TimeManager::Instance()->UpdatePlayerObjectInTimeline(_controller1);
		TimeManager::Instance()->UpdatePlayerObjectInTimeline(_controller2);
		
	}

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