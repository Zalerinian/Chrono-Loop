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

	struct TeleportAction : public CodeComponent {
		matrix4 endPos;
		Interpolator<matrix4>* interp;
		std::vector<BaseObject*> mPlaneObjects;
		std::vector<BaseObject*> mEnvironmentObjects;
		BaseObject *mHeadset;
		ControllerType mControllerRole = eControllerType_Primary;
		Level* cLevel = nullptr;
		TeleportAction(ControllerType _t) { mControllerRole = _t; };

		virtual void Start() {
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			interp = cLevel->playerInterp;

			mPlaneObjects = cLevel->FindAllObjectsByPattern("Floor");
			mEnvironmentObjects = cLevel->FindAllObjectsByPattern("Env");

			mHeadset = LevelManager::GetInstance().GetCurrentLevel()->GetHeadset();
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
						float controllerTime = 0, wallTime = FLT_MAX;
						size_t Esize = mEnvironmentObjects.size();
						for (int i = 0; i < Esize; ++i) {
							forward.Set(0, 0, 1, 0);
							matrix4 objMat = mEnvironmentObjects[i]->GetTransform().GetMatrix();
							matrix4 objMatInv = mEnvironmentObjects[i]->GetTransform().GetMatrix().Invert();
							matrix4 inverse = (mat * objMatInv);
							vec3f meshPos = inverse.Position;
							forward *= inverse;
							vec3f fwd(forward);
							if (fwd * vec3f(0, 1, 0) > 0)
								return;

							int mSize = mEnvironmentObjects[i]->GetComponentCount(eCOMPONENT_MESH);
							for (int k = 0; k < mSize; ++k)
							{
								Triangle *tris = ((MeshComponent*)mEnvironmentObjects[i]->GetComponentIndexed(eCOMPONENT_MESH, k))->GetTriangles();
								size_t numTris = ((MeshComponent*)mEnvironmentObjects[i]->GetComponentIndexed(eCOMPONENT_MESH, k))->GetTriangleCount();
								for (unsigned int j = 0; j < numTris; ++j)
								{
									float hitTime = FLT_MAX;
									if (Physics::Instance()->RayToTriangle(
										(tris + j)->Vertex[0],
										(tris + j)->Vertex[1],
										(tris + j)->Vertex[2],
										(tris + j)->Normal, meshPos, fwd, hitTime))
									{
										if (hitTime < wallTime)
										{
											wallTime = hitTime;
										}
									}
								}
							}
						}

						size_t size = mPlaneObjects.size();
						for (int m = 0; m < size; ++m)
						{
							int mmsize = mPlaneObjects[m]->GetComponentCount(eCOMPONENT_MESH);
							for (int mm = 0; mm < mmsize; ++mm)
							{

								Triangle *tris = ((MeshComponent*)mPlaneObjects[m]->GetComponentIndexed(eCOMPONENT_MESH, mm))->GetTriangles();
								size_t numTris = ((MeshComponent*)mPlaneObjects[m]->GetComponentIndexed(eCOMPONENT_MESH, mm))->GetTriangleCount();
								matrix4 objMat = mPlaneObjects[m]->GetTransform().GetMatrix();
								matrix4 inverse = (mat * objMat.Invert());
								vec3f position = inverse.Position;
								vec3f point = VRInputManager::GetInstance().GetPlayerPosition().fourth;
								forward.Set(0, 0, 1, 0);
								forward *= inverse;
								vec3f fwd = forward;

								for (unsigned int i = 0; i < numTris; ++i)
								{
									if (Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, position, fwd, controllerTime))
									{
										if (controllerTime < wallTime)
										{
											fwd *= controllerTime;
											point[0] += fwd[0] * objMat.xAxis[0]; // x
											point[2] += fwd[2] * objMat.zAxis[2]; // z
											//VRInputManager::Instance().iGetPlayerPosition()[3][3] += forward[3]; // w
											//Move any held objects along with player 


											mat = VRInputManager::GetInstance().GetPlayerPosition();
											mat.Position.y = 2;
											vec3f pos = mat.Position;
											vec3f up(0, 1, 0);
											mat = DirectX::XMMatrixLookAtRH(mat.Position.vector, point.vector, up.vector);
											mat = mat.Invert();
											mat.Position = pos;
											controllerTime = 0, wallTime = FLT_MAX;
											size_t Esize = mEnvironmentObjects.size();
											for (int j = 0; j < Esize; ++j)
											{
												forward.Set(0, 0, 1, 0);
												matrix4 objMatInv = mEnvironmentObjects[j]->GetTransform().GetMatrix().Invert();
												matrix4 inverse = (mat * objMatInv);
												vec3f meshPos = inverse.Position;
												forward *= inverse;
												vec3f fwd(forward);

												int asize = mEnvironmentObjects[j]->GetComponentCount(eCOMPONENT_MESH);
												for (int a = 0; a < asize; ++a)
												{
													Triangle *tris = ((MeshComponent*)mEnvironmentObjects[j]->GetComponentIndexed(eCOMPONENT_MESH, a))->GetTriangles();
													size_t numTris = ((MeshComponent*)mEnvironmentObjects[j]->GetComponentIndexed(eCOMPONENT_MESH, a))->GetTriangleCount();
													for (unsigned int k = 0; k < numTris; ++k)
													{
														float hitTime = FLT_MAX;
														if (Physics::Instance()->RayToTriangle(
															(tris + k)->Vertex[0],
															(tris + k)->Vertex[1],
															(tris + k)->Vertex[2],
															(tris + k)->Normal, meshPos, fwd, hitTime))
														{
															if (hitTime < wallTime)
															{
																wallTime = hitTime;
															}
														}
													}
												}
											}


											inverse = (mat * objMat.Invert());
											position = inverse.Position;
											forward.Set(0, 0, 1, 0);
											forward *= inverse;
											vec3f agentFwd = forward;
											float agentTime = 0;
											numTris = ((MeshComponent*)mPlaneObjects[m]->GetComponentIndexed(eCOMPONENT_MESH, mm))->GetTriangleCount();
											for (unsigned int x = 0; x < numTris; ++x)
											{
												if (Physics::Instance()->RayToTriangle((tris + x)->Vertex[0], (tris + x)->Vertex[1], (tris + x)->Vertex[2], (tris + x)->Normal, position, agentFwd, agentTime))
												{
													if (agentTime < wallTime)
													{
														endPos = VRInputManager::GetInstance().GetPlayerPosition();
														endPos[3][0] += fwd[0] * objMat.xAxis[0]; // x
														endPos[3][2] += fwd[2] * objMat.zAxis[2]; // z
														interp->Prepare(.1f, VRInputManager::GetInstance().GetPlayerPosition(), endPos, VRInputManager::GetInstance().GetPlayerPosition());
														interp->SetActive(true);
														//SystemLogger::Debug() << "Successful raycast" << std::endl;

														if (dynamic_cast<SFXEmitter*>(mHeadset->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1)))
															((SFXEmitter*)mHeadset->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1))->CallEvent(Emitter::ePlay);
														return;
													}
													else
													{
														SystemLogger::GetLog() << "[DEBUG] Can't let you do that, Starfox." << std::endl;
													}
												}
											}
										}
										else
										{
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
