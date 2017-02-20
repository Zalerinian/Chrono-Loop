#pragma once
#include "../Objects/BaseObject.h"
#include "../Common/Logger.h"
#include "CodeComponent.hpp"
#include "../Objects/MeshComponent.h"
#include "../Physics/Physics.h"
#include "../Input/VRInputManager.h"

struct TeleportAction : public CodeComponent {
	MeshComponent *mPlaneMesh;

	virtual void Start() {
		mPlaneMesh = (MeshComponent*)BaseObject::GetObjectByName("plane")->GetComponentIndexed(eCOMPONENT_MESH, 0);
	}

	virtual void Update() {
		// I'm lazy so, let's just set this thing's position to the controller's position.
		mObject->GetTransform().SetMatrix(Math::FromMatrix(VRInputManager::Instance().iGetController(true).GetPose().mDeviceToAbsoluteTracking));

		if (VRInputManager::Instance().iGetController(true).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) {
			matrix4 mat = mObject->GetTransform().GetMatrix();
			vec4f forward(0, 0, 1, 0);
			forward *= mat;
			Triangle *tris = mPlaneMesh->GetTriangles();
			size_t numTris = mPlaneMesh->GetTriangleCount();
			vec4f position = (mat).fourth;
			for (unsigned int i = 0; i < numTris; ++i) {
				float time = 0;
				if (Physics::Instance()->RayToTriangle(tris->Vertex[0], tris->Vertex[1], tris->Vertex[2], tris->Normal, position, forward, time)) {
					forward *= time;
					VRInputManager::Instance().iGetPlayerPosition()[4][0] += forward[0]; // x
					VRInputManager::Instance().iGetPlayerPosition()[3][2] += forward[2]; // z
					//VRInputManager::Instance().iGetPlayerPosition()[3][3] += forward[3]; // w
					int i = 0;
				}
			}
		}
	}

};