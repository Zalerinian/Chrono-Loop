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

		bool mCanTeleport = false, mInitial = false;
		std::vector<vec3f> mArc;
		vec3f mVelocity, mAcceleration;
		BaseObject* mTPLoc, *mCSLoc, *mMSLoc;
		MeshComponent* mTPMesh, *mCSMesh, *mMidMesh;

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
		void GenerateMesh(vec3f& _vel, matrix4 _m)
		{
			//verts and uvs
			std::vector<VertexPosNormTex> mVerts;
			std::vector<unsigned int> mIndices;

			vec3f right = _vel.Cross(vec3f(0, 1, 0)).Normalize();
			//Maybe over extend, may be some index issue
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
			for (int i = 0; i < mVerts.size() / 2 - 1; i++)
			{
				int p1 = 2 * i, p2 = 2 * i + 1, p3 = 2 * i + 2, p4 = 2 * i + 3;

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

				if (CheckMesh(_walls, lastpos, nextpos, hit))
				{
					vec3f floorhit;

					if (ChecktoFloor(_plane, hit, vec3f(0, -1, 0), floorhit))
					{
						_arc.push_back(floorhit);
					}
					else
						_arc.push_back(hit);

					return true;
				}
				if (CheckMesh(mPillar1Mesh, lastpos, nextpos, hit))
				{
					vec3f floorhit;

					if (ChecktoFloor(_plane, hit, vec3f(0, -1, 0), floorhit))
					{
						_arc.push_back(floorhit);
					}
					else
						_arc.push_back(hit);

					return true;
				}
				if (CheckMesh(mPillar2Mesh, lastpos, nextpos, hit))
				{
					vec3f floorhit;

					if (ChecktoFloor(_plane, hit, vec3f(0, -1, 0), floorhit))
					{
						_arc.push_back(floorhit);
					}
					else
						_arc.push_back(hit);

					return true;
				}
				if (CheckMesh(mPillar3Mesh, lastpos, nextpos, hit))
				{
					vec3f floorhit;

					if (ChecktoFloor(_plane, hit, vec3f(0, -1, 0), floorhit))
					{
						_arc.push_back(floorhit);
					}
					else
						_arc.push_back(hit);

					return true;
				}
				if (CheckMesh(mPillar4Mesh, lastpos, nextpos, hit))
				{
					vec3f floorhit;

					if (ChecktoFloor(_plane, hit, vec3f(0, -1, 0), floorhit))
					{
						_arc.push_back(floorhit);
					}
					else
						_arc.push_back(hit);

					return true;
				}

				if (CheckMesh(mUpperPlaneMesh, lastpos, nextpos, hit))
				{
					_arc.push_back(hit);

					return true;
				}
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

			if (mHeadset->GetTransform().GetMatrix().Position.y > 0)
				mCanTeleport = CalculateCurve(mat.Position, t, mAcceleration, mGlassPlaneMesh, mUpperWallMesh, mArc);
			else
				mCanTeleport = CalculateCurve(mat.Position, t, mAcceleration, mLowerPlaneMesh, mLowerWallMesh, mArc);


			if (mCanTeleport)
			{
				//GenerateMesh(vec3f(t.x, t.y, t.z), mat);
				//mPGhost->data.Position = vec4f();

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
				m.fourth.y += .35f;
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

						vec4f raydir = (mArc[mArc.size() - 1] - mArc[0]);
						raydir.w = 0;

						//SystemLogger::Debug() << "Touchpad Pressed" << std::endl;
						vec4f forward(0, 0, 1, 0);
						MeshComponent* meshes[] = { mDoorMesh, mBox1Mesh, mBox2Mesh, mBox3Mesh, mPillar1Mesh, mPillar2Mesh, mPillar3Mesh, mPillar4Mesh, mUpperWallMesh, mLowerWallMesh };
						BaseObject* objects[] = { mDoorObject, mBox1Object, mBox2Object, mBox3Object, mPillar1Object, mPillar2Object, mPillar3Object, mPillar4Object, mUpperWallObject, mLowerWallObject };
						float wallTime = FLT_MAX;
						for (int i = 0; i < ARRAYSIZE(meshes); ++i) {
							forward.Set(0, 0, 1, 0);
							forward = raydir;
							matrix4 objMat = objects[i]->GetTransform().GetMatrix();
							matrix4 objMatInv = objects[i]->GetTransform().GetMatrix().Invert();
							matrix4 inverse = (mat * objMatInv);
							vec3f meshPos = inverse.Position;
							//forward *= inverse;
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
						if (mHeadset->GetTransform().GetMatrix().Position.y > 0)
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
							forward = raydir;
							matrix4 objMat = TeleObjects[i]->GetTransform().GetMatrix();
							matrix4 objMatInv = TeleObjects[i]->GetTransform().GetMatrix().Invert();
							matrix4 inverse = (mat * objMatInv);
							vec3f meshPos = inverse.Position;
							//forward *= inverse;
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
										}
										else {
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
