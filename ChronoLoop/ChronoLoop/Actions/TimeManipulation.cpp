#include "./Actions/TimeManipulation.h"
#include "Objects/Component.h"
#include "Objects/MeshComponent.h"
#include "Actions/CodeComponent.hpp"
#include "Objects/BaseObject.h"
#include "Levels/LevelManager.h"
#include "Input/VRInputManager.h"


TimeManipulation::TimeManipulation() {}


TimeManipulation::~TimeManipulation() {}

virtual void TimeManipulation::Start() {
	mCloneCount++;

	//TODO PAT: Replace this with ryan's GUI system
	mCountMap = RenderEngine::Renderer::Instance()->CreateBitmapForTexture(mCountTxt);
	//This draws to center
	RenderEngine::Renderer::Instance()->DrawTextToBitmap(std::to_wstring(mCloneCount), mCountMap, 0.0f, 0.0f, 1.0f, 1.0f);
}

virtual void TimeManipulation::Update() {
	if (VRInputManager::Instance().iGetController(mLeft).GetPressDown(vr::EVRButtonId::k_EButton_ApplicationMenu)) {
		int frameRewind = 30;

		if (!TimeManager::Instance()->CheckRewindAvaliable(frameRewind))
			return;
		Transform identity;
		identity.SetMatrix(Math::MatrixIdentity());

		int rand = std::rand();

		ID3D11Texture2D* clonecount;
		MeshComponent *visibleMesh = new MeshComponent("../Resources/Cube.obj");
		visibleMesh->AddTexture("../Resources/cube_texture.png", RenderEngine::eTEX_DIFFUSE);
		BaseObject* headset = new BaseObject("headset" + std::to_string(rand), identity);
		headset->AddComponent(visibleMesh);


		BaseObject* Controller1 = new BaseObject("Controller" + std::to_string(rand), identity);
		MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");
		mc->AddTexture("../Resources/vr_controller_lowpoly_texture.png", RenderEngine::eTEX_DIFFUSE);
		Controller1->AddComponent(mc);



		BaseObject* Controller2 = new BaseObject("Controller2" + std::to_string(rand), identity);
		MeshComponent *mc2 = new MeshComponent("../Resources/Controller.obj");
		mc2->AddTexture("../Resources/vr_controller_lowpoly_texture.png", RenderEngine::eTEX_DIFFUSE);
		Controller2->AddComponent(mc2);

		//Make a clone 3 seconds ago.
		TimeManager::Instance()->RewindMakeClone(TimeManager::Instance()->GetCurrentSnapFrame() - 30, headset, Controller1, Controller2);
		LevelManager::Instance()->GetLevel(0)->SetHeadsetAndControllers(headset, Controller1, Controller2);
		TimeManager::Instance()->AddObjectToTimeline(headset);
		TimeManager::Instance()->AddObjectToTimeline(Controller1);
		TimeManager::Instance()->AddObjectToTimeline(Controller2);

	}
	if (VRInputManager::Instance().iGetController(mLeft).GetPressDown(vr::EVRButtonId::k_EButton_Grip)) {
		BaseLevel* CurLev = LevelManager::Instance()->GetLevel(0);
		TimeManager::Instance()->RewindTimeline(TimeManager::Instance()->GetCurrentSnapFrame() - 30, CurLev->mHeadset->GetUniqueID(), CurLev->mController1->GetUniqueID(), CurLev->mController2->GetUniqueID());

	}

}
