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
#include "../Common/Settings.h"
#include "../Actions/HeadsetFollow.hpp"
#include "BoxSnapToControllerAction.hpp"
#include "../Common/Interpolator.h"
#include <d3d11.h>

namespace Epoch {

	struct Level3TeleportAction : public CodeComponent {
		matrix4 endPos;
		Interpolator<matrix4>* interp;
		MeshComponent *mUpperPlaneMesh, *mGlassPlaneMesh, *mLowerPlaneMesh, *mDoorMesh, *mBox1Mesh, *mBox2Mesh, *mBox3Mesh, *mPillar1Mesh, *mPillar2Mesh, *mPillar3Mesh, *mPillar4Mesh, *mElevatorMesh, *mUpperWallMesh, *mLowerWallMesh;
		BaseObject*mUpperPlaneObject, *mGlassPlaneObject, *mLowerPlaneObject, *mDoorObject, *mBox1Object, *mBox2Object, *mBox3Object, *mPillar1Object, *mPillar2Object, *mPillar3Object, *mPillar4Object, *mHeadset, *mElevatorObject, *mUpperWallObject, *mLowerWallObject;
		ControllerType mControllerRole = eControllerType_Primary;
		Level* cLevel = nullptr;
		Level3TeleportAction(ControllerType _t) { mControllerRole = _t; };

		virtual void Start() {
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			interp = cLevel->playerInterp;
			mUpperPlaneObject = cLevel->FindObjectWithName("UpperFloor");
			mGlassPlaneObject = cLevel->FindObjectWithName("TransparentGlassFloor");
			mLowerPlaneObject = cLevel->FindObjectWithName("LowerFloor");
			mUpperWallObject = cLevel->FindObjectWithName("UpperWall");
			mLowerWallObject = cLevel->FindObjectWithName("LowerWall");
			mDoorObject = cLevel->FindObjectWithName("TransparentDoor1");
			mBox1Object = cLevel->FindObjectWithName("Box1");
			mBox2Object = cLevel->FindObjectWithName("Box2");
			mBox3Object = cLevel->FindObjectWithName("Box3");
			mPillar1Object = cLevel->FindObjectWithName("Pillar1");
			mPillar2Object = cLevel->FindObjectWithName("Pillar2");
			mPillar3Object = cLevel->FindObjectWithName("Pillar3");
			mPillar4Object = cLevel->FindObjectWithName("Pillar4");
			mElevatorObject = cLevel->FindObjectWithName("L3Elevator");

			mUpperPlaneMesh = (MeshComponent*)mUpperPlaneObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mGlassPlaneMesh = (MeshComponent*)mGlassPlaneObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mLowerPlaneMesh = (MeshComponent*)mLowerPlaneObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mUpperWallMesh = (MeshComponent*)mUpperWallObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mLowerWallMesh = (MeshComponent*)mLowerWallObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mDoorMesh = (MeshComponent*)mDoorObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mBox1Mesh = (MeshComponent*)mBox1Object->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mBox2Mesh = (MeshComponent*)mBox2Object->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mBox3Mesh = (MeshComponent*)mBox3Object->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mPillar1Mesh = (MeshComponent*)mPillar1Object->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mPillar2Mesh = (MeshComponent*)mPillar2Object->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mPillar3Mesh = (MeshComponent*)mPillar3Object->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mPillar4Mesh = (MeshComponent*)mPillar4Object->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mElevatorMesh = (MeshComponent*)mElevatorObject->GetComponentIndexed(eCOMPONENT_MESH, 0);

			mHeadset = cLevel->GetHeadset();
			endPos = VRInputManager::GetInstance().GetPlayerPosition();
			interp->SetActive(false);
		}

		virtual void Update() {
			if (!VRInputManager::GetInstance().IsVREnabled()) {
				if ((GetAsyncKeyState(VK_SHIFT) & 1) && dynamic_cast<SFXEmitter*>(mHeadset->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1)))
					((SFXEmitter*)mHeadset->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1))->CallEvent(Emitter::ePlay);
				return;
			}

			// I'm lazy so, let's just set this thing's position to the controller's position.
			matrix4 mat = VRInputManager::GetInstance().GetController(mControllerRole).GetPosition();
			mObject->GetTransform().SetMatrix(mat);
			bool paused = false;

			if (cLevel->GetTimeManipulator() != nullptr) {
				paused = cLevel->GetTimeManipulator()->isTimePaused();

			}

			if (!interp->GetActive() && !Settings::GetInstance().GetBool("CantTeleport")) {
				if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Touchpad) && !Settings::GetInstance().GetBool("PauseMenuUp")) {
					if (!paused) {
						//SystemLogger::Debug() << "Touchpad Pressed" << std::endl;
						vec4f forward(0, 0, 1, 0);
						MeshComponent* meshes[] = { mDoorMesh, mBox1Mesh, mBox2Mesh, mBox3Mesh, mPillar1Mesh, mPillar2Mesh, mPillar3Mesh, mPillar4Mesh, mUpperWallMesh, mLowerWallMesh };
						BaseObject* objects[] = { mDoorObject, mBox1Object, mBox2Object, mBox3Object, mPillar1Object, mPillar2Object, mPillar3Object, mPillar4Object, mUpperWallObject, mLowerWallObject };
						float wallTime = FLT_MAX;
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
							for (unsigned int j = 0; j < numTris; ++j) {
								float hitTime = FLT_MAX;
								if (Physics::Instance()->RayToTriangle(
									(tris + j)->Vertex[0],
									(tris + j)->Vertex[1],
									(tris + j)->Vertex[2],
									(tris + j)->Normal, meshPos, fwd, hitTime)) {
									if (hitTime < wallTime) {
										wallTime = hitTime;
									}
								}
							}
						}

						MeshComponent* FloorMesh;
						BaseObject* FloorObject;
						if(mHeadset->GetTransform().GetMatrix().Position.y > 0)
						{
							FloorMesh = mGlassPlaneMesh;
							FloorObject = mGlassPlaneObject;
						}
						else
						{
							FloorMesh = mLowerPlaneMesh;
							FloorObject = mLowerPlaneObject;
						}

						MeshComponent* TeleMeshes[] = { FloorMesh, mUpperPlaneMesh, mElevatorMesh };
						BaseObject* TeleObjects[] = { FloorObject, mUpperPlaneObject, mElevatorObject };
						vec3f point = VRInputManager::GetInstance().GetPlayerPosition().fourth;
						for (int i = 0; i < ARRAYSIZE(TeleMeshes); ++i) {
							forward.Set(0, 0, 1, 0);
							matrix4 objMat = TeleObjects[i]->GetTransform().GetMatrix();
							matrix4 objMatInv = TeleObjects[i]->GetTransform().GetMatrix().Invert();
							matrix4 inverse = (mat * objMatInv);
							vec3f meshPos = inverse.Position;
							forward *= inverse;
							vec3f fwd(forward);
							Triangle *tris = TeleMeshes[i]->GetTriangles();
							size_t numTris = TeleMeshes[i]->GetTriangleCount();
							float controllerTime = FLT_MAX;
							for (unsigned int j = 0; j < numTris; ++j) {
								float hitTime = FLT_MAX;
								if (Physics::Instance()->RayToTriangle((tris + j)->Vertex[0], (tris + j)->Vertex[1], (tris + j)->Vertex[2], (tris + j)->Normal, meshPos, fwd, hitTime)) {
									if (hitTime < controllerTime) {
										controllerTime = hitTime;
									}
								}
							}

							if (controllerTime < wallTime) {
								fwd *= controllerTime;
								point[0] += fwd[0] * objMat.xAxis[0]; // x
								point[2] += fwd[2] * objMat.zAxis[2]; // z
																	  //VRInputManager::Instance().iGetPlayerPosition()[3][3] += forward[3]; // w
																	  //Move any held objects along with player 

								mat = VRInputManager::GetInstance().GetPlayerPosition();
								mat.Position.y = VRInputManager::GetInstance().GetPlayerPosition().Position.y + 1;
								vec3f pos = mat.Position;
								vec3f up(0, 1, 0);
								mat = DirectX::XMMatrixLookAtRH(mat.Position.vector, point.vector, up.vector);
								mat = mat.Invert();
								mat.Position = pos;
								controllerTime = 0, wallTime = FLT_MAX;
								for (int j = 0; j < ARRAYSIZE(meshes); ++j) {
									forward.Set(0, 0, 1, 0);
									matrix4 objMatInv = objects[j]->GetTransform().GetMatrix().Invert();
									matrix4 inverse = (mat * objMatInv);
									vec3f meshPos = inverse.Position;
									forward *= inverse;
									vec3f fwd(forward);
									Triangle *tris = meshes[j]->GetTriangles();
									size_t numTris = meshes[j]->GetTriangleCount();
									for (unsigned int k = 0; k < numTris; ++k) {
										float hitTime = FLT_MAX;
										if (Physics::Instance()->RayToTriangle(
											(tris + k)->Vertex[0],
											(tris + k)->Vertex[1],
											(tris + k)->Vertex[2],
											(tris + k)->Normal, meshPos, fwd, hitTime)) {
											if (hitTime < wallTime) {
												wallTime = hitTime;
											}
										}
									}
								}

								inverse = (mat * objMat.Invert());
								vec3f position = inverse.Position;
								forward.Set(0, 0, 1, 0);
								forward *= inverse;
								vec3f agentFwd = forward;
								float agentTime = 0;
								for (unsigned int x = 0; x < numTris; ++x) {
									if (Physics::Instance()->RayToTriangle((tris + x)->Vertex[0], (tris + x)->Vertex[1], (tris + x)->Vertex[2], (tris + x)->Normal, position, agentFwd, agentTime)) {
										if (agentTime < wallTime) {
											endPos = VRInputManager::GetInstance().GetPlayerPosition();
											endPos[3][0] += fwd[0] * objMat.xAxis[0]; // x
											endPos[3][2] += fwd[2] * objMat.zAxis[2]; // z
											interp->Prepare(.1f, VRInputManager::GetInstance().GetPlayerPosition(), endPos, VRInputManager::GetInstance().GetPlayerPosition());
											interp->SetActive(true);
											SystemLogger::Debug() << "Successful raycast" << std::endl;

											if (Settings::GetInstance().GetInt("tutStep") == 1)//Teleported
												Settings::GetInstance().SetInt("tutStep", 2);//Pick up object

											if (dynamic_cast<SFXEmitter*>(mHeadset->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1)))
												((SFXEmitter*)mHeadset->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1))->CallEvent(Emitter::ePlay);
											break;
										} else {
											SystemLogger::GetLog() << "[DEBUG] Can't let you do that, Starfox." << std::endl;
										}
									}
								}
							} 
						}
					}
				}
			} else if (interp->Update(TimeManager::Instance()->GetDeltaTime()))
				interp->SetActive(false);
		}
	};
}
