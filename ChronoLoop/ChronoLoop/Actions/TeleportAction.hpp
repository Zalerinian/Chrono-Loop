#pragma once
#include "../Objects/BaseObject.h"
#include "../Common/Logger.h"
#include "CodeComponent.hpp"
#include "../Objects/MeshComponent.h"
#include "../Physics/Physics.h"
#include "../Input/VRInputManager.h"
#include "../Levels/LevelManager.h"

struct TeleportAction : public CodeComponent {
	MeshComponent *mPlaneMesh;
	

	virtual void Start() {
		mPlaneMesh = (MeshComponent*)BaseObject::GetObjectByName("plane")->GetComponentIndexed(eCOMPONENT_MESH, 0);
	}

	virtual void Update() {
		if (!VRInputManager::Instance().iIsInitialized()) {
			return;
		}
		// I'm lazy so, let's just set this thing's position to the controller's position.
		matrix4 mat = VRInputManager::Instance().iGetController(true).GetPosition();
		mObject->GetTransform().SetMatrix(mat);

		if (VRInputManager::Instance().iGetController(true).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) {
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

		if (VRInputManager::Instance().iGetController(true).GetPressDown(vr::EVRButtonId::k_EButton_DPad_Left))
		{
			Transform identity;
			identity.SetMatrix(Math::MatrixIdentity());

			MeshComponent *visibleMesh = new MeshComponent("../Resources/Cube.obj");
			visibleMesh->AddTexture("../Resources/cube_texture.png", RenderEngine::eTEX_DIFFUSE);

			BaseObject* headset = new BaseObject("headset", identity);
			headset->AddComponent(visibleMesh);
			headset->AddComponent(visibleMesh);
			TimeManager::Instance()->AddObjectToTimeline(headset);

			BaseObject* Controller1 = new BaseObject("Controller", identity);
			MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");
			mc->AddTexture("../Resources/vr_controller_lowpoly_texture.png", RenderEngine::eTEX_DIFFUSE);
			Controller1->AddComponent(mc);

		

			BaseObject* Controller2 = new BaseObject("Controller", identity);
			MeshComponent *mc2 = new MeshComponent("../Resources/Controller.obj");
			mc2->AddTexture("../Resources/vr_controller_lowpoly_texture.png", RenderEngine::eTEX_DIFFUSE);
			Controller2->AddComponent(mc2);

			TimeManager::Instance()->AddObjectToTimeline(headset);
			TimeManager::Instance()->AddObjectToTimeline(Controller1);
			TimeManager::Instance()->AddObjectToTimeline(Controller2);

			LevelManager::Instance()->GetLevel(0)->SetHeadsetAndControllers(headset, Controller1, Controller2);
			//Make a clone 3 seconds ago.
			TimeManager::Instance()->RewindMakeClone(TimeManager::Instance()->GetCurrentSnapFrame() - 30, headset, Controller1, Controller2);
			
		}
	}

};
