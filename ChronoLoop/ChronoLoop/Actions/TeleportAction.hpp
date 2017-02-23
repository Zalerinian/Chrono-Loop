#pragma once
#include "../Objects/BaseObject.h"
#include "../Common/Logger.h"
#include "CodeComponent.hpp"
#include "../Objects/MeshComponent.h"
#include "../Physics/Physics.h"
#include "../Input/VRInputManager.h"
#include "../Levels/LevelManager.h"
#include "../HeadsetFollow.hpp"


struct TeleportAction : public CodeComponent {
	MeshComponent *mPlaneMesh;
	bool left;
	TeleportAction(bool _left) { left = _left; };

	virtual void Start() {
		mPlaneMesh = (MeshComponent*)BaseObject::GetObjectByName("plane")->GetComponentIndexed(eCOMPONENT_MESH, 0);
	}

	virtual void Update() {
		if (!VRInputManager::Instance().iIsInitialized()) {
			return;
		}
		// I'm lazy so, let's just set this thing's position to the controller's position.
		matrix4 mat = VRInputManager::Instance().iGetController(left).GetPosition();
		mObject->GetTransform().SetMatrix(mat);

		if (VRInputManager::Instance().iGetController(left).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) {
			vec4f forward(0, 0, 1, 0);
			forward *= mObject->GetTransform().GetMatrix();
			Triangle *tris = mPlaneMesh->GetTriangles();
			size_t numTris = mPlaneMesh->GetTriangleCount();
			vec4f position = mat.fourth;
			for (unsigned int i = 0; i < numTris; ++i) {
				float time = 0;
				if (Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, position, forward, time)) {
					forward *= time;
					VRInputManager::Instance().iGetPlayerPosition()[3][0] += forward[0]; // x
					VRInputManager::Instance().iGetPlayerPosition()[3][2] += forward[2]; // z
					//VRInputManager::Instance().iGetPlayerPosition()[3][3] += forward[3]; // w
				}
			}
		}

		if (VRInputManager::Instance().iGetController(left).GetPressDown(vr::EVRButtonId::k_EButton_Grip))
		{
			int frameRewind = 30;

			if (!TimeManager::Instance()->CheckRewindAvaliable(frameRewind))
				return;
			Transform identity;
			identity.SetMatrix(Math::MatrixIdentity());

			int rand = std::rand();

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
		if (VRInputManager::Instance().iGetController(left).GetPressDown(vr::EVRButtonId::k_EButton_ApplicationMenu)) {
			BaseLevel* CurLev = LevelManager::Instance()->GetLevel(0);
			TimeManager::Instance()->RewindTimeline(TimeManager::Instance()->GetCurrentSnapFrame() - 30, CurLev->mHeadset->GetUniqueID(), CurLev->mController1->GetUniqueID(), CurLev->mController2->GetUniqueID());

		}

	}

};
