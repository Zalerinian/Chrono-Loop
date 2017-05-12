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

		bool mCanTeleport = false, mInitial = false;
		std::vector<vec3f> mArc;
		vec3f mVelocity, mAcceleration;
		BaseObject* mTPLoc, *mCSLoc, *mMSLoc;
		MeshComponent* mTPMesh, *mCSMesh, *mMidMesh;

		bool CheckMesh(MeshComponent* _plane, vec3f _start, vec3f _end, vec3f& _hit)
		{
			Triangle* tris = _plane->GetTriangles();
			int count = _plane->GetTriangleCount();

			for (int i = 0; i < count; i++)
			{
				bool hit = Physics::Instance()->Linecast((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, _start, _end, _hit);

				if (hit)
					return true;

			}

			return false;
		}
		bool ChecktoFloor(MeshComponent* _plane, vec3f _start, vec3f _ray, vec3f& _hit)
		{
			Triangle* tris = _plane->GetTriangles();
			int count = _plane->GetTriangleCount();

			float t;
			for (int i = 0; i < count; i++)
			{
				bool hit = Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, _start, _ray, t);

				if (hit)
				{
					_hit = _start + (_ray * t);
					return true;
				}

			}

			return false;
		}

		vec3f ParabolicCurve(vec3f _p, vec3f _v, vec3f _a, float _t)
		{
			return _p + _v * _t + _a * .5f * _t * _t;
		}
		vec3f DerivedCurve(vec3f _v, vec3f _a, float _t)
		{
			return _v + _a * _t;
		}

		bool CalculateCurve(vec3f _p, vec3f _v, vec3f _a, MeshComponent* _plane, MeshComponent* _walls, std::vector<vec3f>& _arc)
		{
			_arc.clear();

			vec3f lastpos = _p;
			_arc.push_back(_p);
			float t = 0;

			for (int i = 0; i < 25; i++)
			{
				t += .5f / DerivedCurve(_v, _a, t).Magnitude();

				vec3f nextpos = ParabolicCurve(_p, _v, _a, t);

				vec3f hit;

				for (int e = 0; e < mEnvironmentObjects.size(); e++)
				{
					for (int m = 0; m < mEnvironmentObjects[e]->GetComponentCount(ComponentType::eCOMPONENT_MESH); m++)
					{
						if (CheckMesh((MeshComponent*)mEnvironmentObjects[e]->GetComponents(ComponentType::eCOMPONENT_MESH)[m], lastpos, nextpos, hit))
						{
							/*vec3f floorhit;

							if (ChecktoFloor(_plane, hit, vec3f(0, -1, 0), floorhit))
							{
								_arc.push_back(floorhit);
							}
							else*/
							_arc.push_back(hit);

							return true;
						}
					}
				}
				for (int p = 0; p < mPlaneObjects.size(); p++)
				{
					for (int m = 0; m < mPlaneObjects[p]->GetComponentCount(ComponentType::eCOMPONENT_MESH); m++)
					{
						if (CheckMesh((MeshComponent*)mPlaneObjects[p]->GetComponents(ComponentType::eCOMPONENT_MESH)[m], lastpos, nextpos, hit))
						{
							//if it hits the plane
							_arc.push_back(hit);

							return true;
						}
					}
				}

				_arc.push_back(nextpos);

				lastpos = nextpos;
			}

			return false;
		}

		virtual void Start() {
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			interp = cLevel->playerInterp;

			mPlaneObjects = cLevel->FindAllObjectsByPattern("Floor");
			mEnvironmentObjects = cLevel->FindAllObjectsByPattern("Env");

			mHeadset = LevelManager::GetInstance().GetCurrentLevel()->GetHeadset();
			endPos = VRInputManager::GetInstance().GetPlayerPosition();
			interp->SetActive(false);

			mVelocity = vec3f(0, 0, 20);
			mAcceleration = vec3f(0, -9.81f, 0);
			mInitial = false;

			matrix4 scaleM;
			scaleM.first = vec4f(.05f, 0, 0, 0);
			scaleM.second = vec4f(0, .05f, 0, 0);
			scaleM.third = vec4f(0, 0, .05f, 0);
			scaleM.fourth = vec4f(0, 0, 0, 1);

			mTPLoc = new BaseObject("TeleportSpot");
			mCSLoc = new BaseObject("ArcStart");
			mMSLoc = new BaseObject("ArcMid");

			//TODO : Make mesh and load them here as well as textures
			mCSMesh = new MeshComponent("../Resources/ControllerTP.obj");
			mCSMesh->AddTexture("../Resources/cube_texture.png", TextureType::eTEX_DIFFUSE);
			mCSLoc->AddComponent(mCSMesh);
			mMidMesh = new MeshComponent("../Resources/ControllerTP.obj");
			mMidMesh->AddTexture("../Resources/cube_texture.png", TextureType::eTEX_DIFFUSE);
			mMSLoc->AddComponent(mMidMesh);
			mTPMesh = new MeshComponent("../Resources/TeleportMarker.obj");
			mTPMesh->AddTexture("../Resources/cube_texture.png", TextureType::eTEX_DIFFUSE);
			mTPLoc->AddComponent(mTPMesh);

			matrix4 temp;
			temp = mCSMesh->GetTransform().GetMatrix() * scaleM;
			mCSMesh->GetTransform().SetMatrix(temp);

			temp = mMidMesh->GetTransform().GetMatrix() * scaleM;
			mMidMesh->GetTransform().SetMatrix(temp);

			temp = mTPMesh->GetTransform().GetMatrix();
			mTPMesh->GetTransform().SetMatrix(temp);

			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mTPLoc);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mCSLoc);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mMSLoc);

			mTPMesh->SetVisible(false);
			mCSMesh->SetVisible(false);
			mMidMesh->SetVisible(false);

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

			matrix4 tmp = mat;
			vec3f fwdvel, right;
			vec4f t = mVelocity;
			t.w = 1;

			tmp.first.w = 0;
			tmp.second.w = 0;
			tmp.third.w = 0;
			tmp.fourth = vec4f();

			t *= tmp;

			vec4f up = vec3f(0, 1, 0);
			fwdvel = t * ((up.Dot(t)) / t.SquaredMagnitude());
			float angle = acos(fwdvel.Dot(t));
			right = up.Cross(fwdvel);

			if (right.Dot(fwdvel.Cross(t)) > 0)
				angle *= -1;

			if (angle > 45)
			{
				float ag = acos(fwdvel.Dot(t));
				t = fwdvel * (sin((1 - (45.0f / angle)) * ag) / sin(ag)) + t * (sin((45.0f / angle) * ag) / sin(ag));
				t /= t.Magnitude();
				t *= mVelocity.Magnitude();
				angle = 45;
			}

			mCanTeleport = CalculateCurve(mat.Position, t, mAcceleration, nullptr, nullptr, mArc);

			if (mCanTeleport)
			{
				matrix4 scaleM;
				scaleM.first = vec4f(.15f, 0, 0, 0);
				scaleM.second = vec4f(0, .15f, 0, 0);
				scaleM.third = vec4f(0, 0, .15f, 0);
				scaleM.fourth = vec4f(0, 0, 0, 1);

				mTPMesh->SetVisible(true);
				mCSMesh->SetVisible(true);
				mMidMesh->SetVisible(true);

				matrix4 m;
				m = mTPMesh->GetTransform().GetMatrix();
				m.fourth = mArc[mArc.size() - 1];
				mTPMesh->GetTransform().SetMatrix(m);

				m = mat * scaleM;
				m.fourth = mat.fourth;
				mCSMesh->GetTransform().SetMatrix(m);

				m = mat * scaleM;
				m.fourth = mArc[(mArc.size() / 2 - 1) < 0 ? 0 : (mArc.size() / 2 - 1)];
				mMidMesh->GetTransform().SetMatrix(m);
			}
			else
			{
				mTPMesh->SetVisible(false);
				mCSMesh->SetVisible(false);
				mMidMesh->SetVisible(false);
			}

			if (!interp->GetActive() && !Settings::GetInstance().GetBool("CantTeleport")) {
				if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Touchpad) && mCanTeleport && !Settings::GetInstance().GetBool("PauseMenuUp")) {
					if (!paused) {
						//SystemLogger::Debug() << "Touchpad Pressed" << std::endl;
						vec4f raydir = (mArc[mArc.size() - 1] - mArc[0]);
						raydir.w = 0;

						vec4f forward(0, 0, 1, 0);
						float controllerTime = 0, wallTime = FLT_MAX;
						size_t Esize = mEnvironmentObjects.size();
						for (int i = 0; i < Esize; ++i) {
							forward.Set(0, 0, 1, 0);
							matrix4 objMat = mEnvironmentObjects[i]->GetTransform().GetMatrix();
							matrix4 objMatInv = mEnvironmentObjects[i]->GetTransform().GetMatrix().Invert();
							matrix4 inverse = (mat * objMatInv);
							vec3f meshPos = inverse.Position;
							forward = raydir;
							//forward *= inverse;
							vec3f fwd(forward);
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
								forward = raydir;
								//forward *= inverse;
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
			}
			else if (interp->Update(TimeManager::Instance()->GetDeltaTime()))
				interp->SetActive(false);
		}
	};
}
