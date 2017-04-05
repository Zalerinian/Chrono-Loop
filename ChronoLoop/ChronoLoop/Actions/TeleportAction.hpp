#pragma once
#include "../Objects/BaseObject.h"
#include "../Common/Logger.h"
#include "CodeComponent.hpp"
#include "../Objects/MeshComponent.h"
#include "../Physics/Physics.h"
#include "../Input/VRInputManager.h"
#include "../Core/Level.h"
#include "../Core/TimeManager.h"
#include "../Core/LevelManager.h"
#include "../Actions/HeadsetFollow.hpp"

namespace Epoch {

	struct TeleportAction : public CodeComponent {
		MeshComponent *mPlaneMesh, *mWallsMesh, *mBlockMesh, *mExitMesh, *mServerMesh;
		BaseObject *mPlaneObject, *mWallsObject, *mBlockObject, *mExitObject, *mServerObject;
		ControllerType mControllerRole = eControllerType_Primary;
		Level* cLevel = nullptr;
		TeleportAction(ControllerType _t) { mControllerRole = _t; };

		virtual void Start() {
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			mPlaneObject  = cLevel->FindObjectWithName("Floor");
			mWallsObject  = cLevel->FindObjectWithName("Walls");
			mBlockObject  = cLevel->FindObjectWithName("TransparentDoor1");
			mExitObject   = cLevel->FindObjectWithName("TransparentDoor2");
			mServerObject = cLevel->FindObjectWithName("Servers");
			if(mPlaneObject)
			{
				mPlaneMesh    = (MeshComponent*)mPlaneObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mWallsMesh    = (MeshComponent*)mWallsObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mBlockMesh    = (MeshComponent*)mBlockObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mExitMesh     = (MeshComponent*)mExitObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mServerMesh = (MeshComponent*)mServerObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			}
		}

		virtual void Update() {
			if (!VRInputManager::GetInstance().IsVREnabled()) {
				return;
			}

			// I'm lazy so, let's just set this thing's position to the controller's position.
			matrix4 mat = VRInputManager::GetInstance().GetController(mControllerRole).GetPosition();
			mObject->GetTransform().SetMatrix(mat);
			bool paused = false;

			if (cLevel->GetTimeManipulator() != nullptr) {
				paused = cLevel->GetTimeManipulator()->isTimePaused();
				
			}
			
			if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Touchpad)) {
				if (!paused) {
					vec4f forward(0, 0, 1, 0);
					MeshComponent* meshes[] = { mWallsMesh, mBlockMesh, mExitMesh, mServerMesh };
					BaseObject* objects[] = { mWallsObject, mBlockObject, mExitObject, mServerObject };
					float meshTime = 0, wallTime = FLT_MAX;
					for (int i = 0; i < ARRAYSIZE(meshes); ++i) {
						forward.Set(0, 0, 1, 0);
						matrix4 objMat = objects[i]->GetTransform().GetMatrix();
						matrix4 objMatInv = objects[i]->GetTransform().GetMatrix().Invert();
						matrix4 inverse = (mat * objMatInv);
						vec3f meshPos = inverse.Position;
						forward *= inverse;
						vec3f fwd(forward);
						Triangle *tris = meshes[i]->GetTriangles();
						size_t numTris = meshes[i]->GetTriangleCount();
						for (unsigned int i = 0; i < numTris; ++i) {
							float hitTime = FLT_MAX;
							if (Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, meshPos, fwd, hitTime)) {
								if (hitTime < wallTime) {
									wallTime = hitTime;
								}
							}
						}
					}

					Triangle *tris = mPlaneMesh->GetTriangles();
					size_t numTris = mPlaneMesh->GetTriangleCount();
					matrix4 objMat = mPlaneObject->GetTransform().GetMatrix();
					matrix4 inverse = (mat * objMat.Invert());
					vec3f position = inverse.Position;
					forward.Set(0, 0, 1, 0);
					forward *= inverse;
					vec3f fwd = forward;
					for (unsigned int i = 0; i < numTris; ++i) {
						if (Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, position, fwd, meshTime)) {
							if (meshTime < wallTime) {
								forward *= meshTime;
								VRInputManager::GetInstance().GetPlayerPosition()[3][0] += fwd[0] * objMat.xAxis[0]; // x
								VRInputManager::GetInstance().GetPlayerPosition()[3][2] += fwd[2] * objMat.zAxis[2]; // z
																									   //VRInputManager::Instance().iGetPlayerPosition()[3][3] += forward[3]; // w
							} else {
								SystemLogger::GetLog() << "[DEBUG] Can't let you do that, Starfox." << std::endl;
							}
						}
					}
				}
			}
		}
	};
}