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
#include <chrono>

namespace Epoch {

	struct TeleportAction : public CodeComponent {
		matrix4 endPos;
		Interpolator<matrix4>* interp;
		MeshComponent *mPlaneMesh, *mWallsMesh, *mBlockMesh, *mExitMesh, *mServerMesh, *mTWall1Mesh, *mTWall2Mesh, *mTWindowMesh;
		BaseObject *mPlaneObject, *mWallsObject, *mBlockObject, *mExitObject, *mServerObject, *mHeadset, *mTWall1, *mTWall2, *mTWindow;
		ControllerType mControllerRole = eControllerType_Primary;
		Level* cLevel = nullptr;
		TeleportAction(ControllerType _t) { mControllerRole = _t; };

		bool mCanTeleport = false, mInitial = false;
		std::vector<vec3f> mArc; 
		vec3f mVelocity, mAcceleration;
		BaseObject* mTPLoc, *mCSLoc;
		MeshComponent* mTPMesh, *mCSMesh;

		RenderShape* mParabola;
		GhostList<matrix4>::GhostNode* mPGhost;
		Mesh* mArcMesh;

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

		void GenerateMesh(vec3f& _vel, matrix4 _m)
		{
			//verts and uvs
			std::vector<VertexPosNormTex> mVerts;
			std::vector<unsigned int> mIndices;

			vec3f right = _vel.Cross(vec3f(0, 1, 0)).Normalize();
			mVerts.reserve(50);
			mVerts.resize(50);
			int ind = 0, numrv = 0;
			for (int i = 0; i < 25; i++)
			{
				VertexPosNormTex t1, t2;
				if (i < mArc.size())
				{
					t1.Position = mArc[i] - right * (.2f / 2);
					t2.Position = mArc[i] + right * (.2f / 2);
					t1.Position.w = 1;
					t2.Position.w = 1;

					/*t1.Position *= _m;
					t2.Position *= _m;
					*/
					//Figure out uv offset
					float uvoff = fmodf((std::chrono::steady_clock::now().time_since_epoch().count()) / 1000.0f / 1000.0f / 1000.0f, 1);
					if (i == mArc.size() - 1 && i > 1)
					{
						float dlast, dcur;
						dlast = (mArc[i - 2] - mArc[i - 1]).Magnitude();
						dcur = (mArc[i] - mArc[i - 1]).Magnitude();
						uvoff += 1 - dcur / dlast;
					}
					t1.UV = vec3f(0, i - uvoff, 0);
					t2.UV = vec3f(1, i - uvoff, 0);
					mVerts[ind] = t1;
					mVerts[ind + 1] = t2;
					ind += 2;
					numrv += 2;
				}
			}

			//indexs
			mIndices.reserve((mVerts.size() / 2 - 1) * 12);
			mIndices.resize((mVerts.size() / 2 - 1) * 12);
			ind = 0;
			for (int i = 0; i < numrv / 2 - 1; i++)
			{
				int p1 = 2 * i, p2 = 2 * i + 1, p3 = 2 * i + 2, p4 = 2 * i + 3;

				/*mIndices.push_back(p1);
				mIndices.push_back(p2);
				mIndices.push_back(p3);
				mIndices.push_back(p3);
				mIndices.push_back(p2);
				mIndices.push_back(p4);

				mIndices.push_back(p3);
				mIndices.push_back(p2);
				mIndices.push_back(p1);
				mIndices.push_back(p4);
				mIndices.push_back(p2);
				mIndices.push_back(p3);*/

				mIndices[ind] = (p1);
				mIndices[ind + 1] = (p2);
				mIndices[ind + 2] = (p3);
				mIndices[ind + 3] = (p3);
				mIndices[ind + 4] = (p2);
				mIndices[ind + 5] = (p4);

				mIndices[ind + 6] = (p3);
				mIndices[ind + 7] = (p2);
				mIndices[ind + 8] = (p1);
				mIndices[ind + 9] = (p4);
				mIndices[ind + 10] = (p2);
				mIndices[ind + 11] = (p3);
				ind += 12;
			}

			if (!mInitial)
			{
				//Make the mesh
				mArcMesh = new Mesh();
				mArcMesh->Load(mVerts, mIndices);
				//Make and push in rendershape, save the ghost node
				mParabola = new RenderShape(*mArcMesh);
				mParabola->SetShaders(PixelShaderFormat::ePS_PURETEXTURE, VertexShaderFormat::eVS_TEXTURED);
				mParabola->SetGeometryShader(GeometryShaderFormat::eGS_PosNormTex);
				mParabola->AddTexture(L"../Resources/cube_texture.png", TextureType::eTEX_DIFFUSE);
				mPGhost = Renderer::Instance()->AddOpaqueNode(*mParabola);
				mInitial = true;
			}
			else
			{
				mArcMesh->Update(mVerts, mIndices);
				mParabola->UpdateBufferData(mArcMesh);
			}
		}

		vec3f ParabolicCurve(vec3f _p, vec3f _v, vec3f _a, float _t)
		{
			return _p + _v * _t + _a * .5f * _t * _t;
		}
		vec3f DerivedCurve(vec3f _v, vec3f _a, float _t)
		{
			return _v + _a * _t;
		}

		bool CalculateCurve(vec3f _p, vec3f _v, vec3f _a, MeshComponent* _plane, std::vector<vec3f>& _arc)
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
				if (CheckMesh(_plane, lastpos, nextpos, hit))
				{
					//if it hits the plane
					_arc.push_back(hit);

					return true;
				}
				else
					_arc.push_back(nextpos);

				lastpos = nextpos;
			}

			return false;
		}


		virtual void Start() {
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			interp = cLevel->playerInterp;
			mPlaneObject = cLevel->FindObjectWithName("Floor");
			mWallsObject = cLevel->FindObjectWithName("Walls");
			mBlockObject = cLevel->FindObjectWithName("TransparentDoor1");
			mExitObject = cLevel->FindObjectWithName("TransparentDoor2");
			mTWall1 = cLevel->FindObjectWithName("TransparentWall1");
			mTWall2 = cLevel->FindObjectWithName("TransparentWall2");
			mTWindow = cLevel->FindObjectWithName("TransparentWindow");
			mServerObject = cLevel->FindObjectWithName("Servers");

			if (mPlaneObject) {
				mPlaneMesh = (MeshComponent*)mPlaneObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mWallsMesh = (MeshComponent*)mWallsObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mBlockMesh = (MeshComponent*)mBlockObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mExitMesh = (MeshComponent*)mExitObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mServerMesh = (MeshComponent*)mServerObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mTWall1Mesh = (MeshComponent*)mTWall1->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mTWall2Mesh = (MeshComponent*)mTWall2->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mTWindowMesh = (MeshComponent*)mTWindow->GetComponentIndexed(eCOMPONENT_MESH, 0);
			}
			mHeadset = LevelManager::GetInstance().GetCurrentLevel()->GetHeadset();
			endPos = VRInputManager::GetInstance().GetPlayerPosition();
			interp->SetActive(false);

			mVelocity = vec3f(0, 0, 20);
			mAcceleration = vec3f(0, -9.81f, 0);
			mInitial = false;

			mTPLoc = new BaseObject("TeleportSpot");
			mCSLoc = new BaseObject("ArcStart");

			//TODO : Make mesh and load them here as well as textures

			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mTPLoc);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mCSLoc);
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

			vec3f fwdvel, right;
			vec4f t = mVelocity;
			t.w = 1;
			t *= mat;

			vec4f up = vec3f(0, 1, 0);
			up *= mat;
			up.Normalize();
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

			mCanTeleport = CalculateCurve(mat.Position, t, mAcceleration, mPlaneMesh, mArc);

			if (mCanTeleport)
			{
				GenerateMesh(vec3f(t.x, t.y, t.z), mat);
				mPGhost->data.Position = vec4f();

				mTPMesh->SetVisible(true);
				mCSMesh->SetVisible(true);

				matrix4 m;
				m = mTPMesh->GetTransform().GetMatrix();
				m.fourth = mArc[mArc.size() - 1];
				mTPMesh->GetTransform().SetMatrix(m);

				m = mCSMesh->GetTransform().GetMatrix();
				m.fourth = mat.fourth;
				mCSMesh->GetTransform().SetMatrix(m);
			}
			else
			{
				mTPMesh->SetVisible(false);
				mCSMesh->SetVisible(false);
			}

			if (!interp->GetActive()) {
				if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Touchpad) && !Settings::GetInstance().GetBool("PauseMenuUp")) {
					if (!paused) {
						
						vec3f raydir = mArc[mArc.size() - 1] - mArc[0];

						SystemLogger::Debug() << "Touchpad Pressed" << std::endl;
						vec4f forward(0, 0, 1, 0);
						MeshComponent* meshes[] = { mWallsMesh, mBlockMesh, mExitMesh, mServerMesh, mTWall1Mesh, mTWall2Mesh, mTWindowMesh };
						BaseObject* objects[] = { mWallsObject, mBlockObject, mExitObject, mServerObject, mTWall1, mTWall2, mTWindow };
						float controllerTime = 0, wallTime = FLT_MAX;
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

						Triangle *tris = mPlaneMesh->GetTriangles();
						size_t numTris = mPlaneMesh->GetTriangleCount();
						matrix4 objMat = mPlaneObject->GetTransform().GetMatrix();
						matrix4 inverse = (mat * objMat.Invert());
						vec3f position = inverse.Position;
						vec3f point = VRInputManager::GetInstance().GetPlayerPosition().fourth;
						forward.Set(0, 0, 1, 0);
						forward *= inverse;
						vec3f fwd = forward;
						for (unsigned int i = 0; i < numTris; ++i) {
							if (Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, mArc[0], raydir, controllerTime)) {
								if (controllerTime < wallTime) {
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
												(tris + k)->Normal, mArc[0], raydir, hitTime)) {
												if (hitTime < wallTime) {
													wallTime = hitTime;
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
									numTris = mPlaneMesh->GetTriangleCount();
									for (unsigned int i = 0; i < numTris; ++i) {
										if (Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, position, agentFwd, agentTime)) {
											if (agentTime < wallTime) {
												endPos = VRInputManager::GetInstance().GetPlayerPosition();
												endPos[3][0] += raydir[0] * objMat.xAxis[0]; // x
												endPos[3][2] += raydir[2] * objMat.zAxis[2]; // z
												interp->Prepare(.1f, VRInputManager::GetInstance().GetPlayerPosition(), endPos, VRInputManager::GetInstance().GetPlayerPosition());
												interp->SetActive(true);
												SystemLogger::Debug() << "Successful raycast" << std::endl;

												if (Settings::GetInstance().GetInt("tutStep") == 1)//Teleported
													Settings::GetInstance().SetInt("tutStep", 2);//Pick up object

												if (dynamic_cast<SFXEmitter*>(mHeadset->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1)))
													((SFXEmitter*)mHeadset->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1))->CallEvent(Emitter::ePlay);
											} else {
												SystemLogger::GetLog() << "[DEBUG] Can't let you do that, Starfox." << std::endl;
											}
										}
									}
								} else {
									SystemLogger::GetLog() << "[DEBUG] Can't let you do that, Starfox." << std::endl;
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
