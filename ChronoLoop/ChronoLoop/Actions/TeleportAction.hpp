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

namespace Epoch
{
	struct TeleportAction : public CodeComponent
	{
		matrix4 endPos;
		Interpolator<matrix4>* interp, mRcolliderInterp;
		std::vector<BaseObject*> mPlaneObjects;
		std::vector<BaseObject*> mEnvironmentObjects;
		BaseObject *mHeadset;
		ControllerType mControllerRole = eControllerType_Primary;
		Level* cLevel = nullptr;
		TeleportAction(ControllerType _t) { mControllerRole = _t; };

		bool mCanTeleport = false, mInitial = false;
		std::vector<vec3f> mArc;
		vec3f mVelocity, mAcceleration;
		BaseObject* mTPLoc, *mCSLoc, *mMSLoc[5];
		MeshComponent* mTPMesh, *mCSMesh, *mMidMesh[5];
		float ang = 0.05f;
		ParticleEmitter* mTPParticles;

		matrix4 GetEndPos() { return endPos; }
		RenderShape* mParabola;
		GhostList<matrix4>::GhostNode* mPGhost;
		Mesh* mArcMesh;

		bool CheckMesh(MeshComponent* _plane, vec3f _ts, vec3f _te, vec3f _start, vec3f _end, vec3f& _hit)
		{
			Triangle* tris = _plane->GetTriangles();
			int count = _plane->GetTriangleCount();

			bool didHit = false;
			float time = FLT_MAX;
			for (int i = 0; i < count; i++)
			{
				float t = FLT_MAX;
				bool hit = Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, _ts, (_te - _ts), t);

				if (hit)
				{
					if (t < time)
					{
						time = t;
						didHit = true;
					}
				}

			}
			_hit = _start + (_end - _start) * time;

			vec3f sd = _end - _start;
			vec3f nd = _hit - _start;

			if (sd.Dot(nd) > 1 || sd.Dot(nd) < 0)
				didHit = false;

			return didHit;
		}
		bool ChecktoFloor(MeshComponent* _plane, vec3f _start, vec3f _ray, vec3f& _hit)
		{
			bool h = false;
			float t = FLT_MAX;

			for (int p = 0; p < mPlaneObjects.size(); p++)
			{
				vec4f ray = _ray;
				vec4f thit = _start;
				matrix4 tm = _plane->GetTransform().GetMatrix() * mPlaneObjects[p]->GetTransform().GetMatrix().Invert();
				ray *= tm;
				thit *= tm;

				vec3f st(thit);
				vec3f ra(ray);

				for (int m = 0; m < mPlaneObjects[p]->GetComponentCount(ComponentType::eCOMPONENT_MESH); m++)
				{
					Triangle* tris = ((MeshComponent*)mPlaneObjects[p]->GetComponents(ComponentType::eCOMPONENT_MESH)[m])->GetTriangles();
					int count = ((MeshComponent*)mPlaneObjects[p]->GetComponents(ComponentType::eCOMPONENT_MESH)[m])->GetTriangleCount();


					for (int i = 0; i < count; i++)
					{
						float time = FLT_MAX;
						bool hit = Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, st, ra, time);

						if (hit)
						{
							_hit = _start + (_ray * t);
							return true;
						}
					}

				}
			}

			_hit = _start + (_ray * t);

			return h;
		}
		void GenerateMesh(vec3f& _vel, matrix4 _m)
		{
			//verts and uvs
			std::vector<VertexPosNormTanTex> mVerts;
			std::vector<unsigned int> mIndices;

			vec3f right = _vel.Cross(vec3f(0, 1, 0)).Normalize();
			//Maybe over extend, may be some index issue
			mVerts.reserve(50);
			mVerts.resize(50);
			int ind = 0, numrv = 0;
			for (int i = 0; i < 25; i++)
			{
				VertexPosNormTanTex t1, t2;
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
			matrix4 cm = VRInputManager::GetInstance().GetController(mControllerRole).GetPosition();

			vec3f vel = vec3f(0, 0, 10);
			float t = 0;
			vec3f lastpos = ParabolicCurve(vec3f(), vel, _a, t);
			vec3f initial = lastpos;
			_arc.push_back(lastpos);

			for (int i = 0; i < 25; i++)
			{
				t += .5f / DerivedCurve(vel, _a, t).Magnitude();

				vec3f nextpos = ParabolicCurve(initial, vel, _a, t);

				vec3f hit;

				for (int e = 0; e < mEnvironmentObjects.size(); e++)
				{
					vec4f tl = lastpos, tn = nextpos;
					matrix4 tm = cm * mEnvironmentObjects[e]->GetTransform().GetMatrix().Invert();
					tl *= tm;
					tn *= tm;
					for (int m = 0; m < mEnvironmentObjects[e]->GetComponentCount(ComponentType::eCOMPONENT_MESH); m++)
					{
						if (CheckMesh((MeshComponent*)mEnvironmentObjects[e]->GetComponents(ComponentType::eCOMPONENT_MESH)[m], tl, tn, lastpos, nextpos, hit))
						{
							vec3f floorhit;

							if (ChecktoFloor((MeshComponent*)mEnvironmentObjects[e]->GetComponents(ComponentType::eCOMPONENT_MESH)[m], hit, vec3f(0, -1, 0), floorhit))
							{
								_arc.push_back(floorhit);

							}
							else
								_arc.push_back(hit);

							return true;
						}
					}
				}
				for (int p = 0; p < mPlaneObjects.size(); p++)
				{
					vec4f tl = lastpos, tn = nextpos;
					matrix4 tm = cm * mPlaneObjects[p]->GetTransform().GetMatrix().Invert();
					tl *= tm;
					tn *= tm;
					for (int m = 0; m < mPlaneObjects[p]->GetComponentCount(ComponentType::eCOMPONENT_MESH); m++)
					{
						if (CheckMesh((MeshComponent*)mPlaneObjects[p]->GetComponents(ComponentType::eCOMPONENT_MESH)[m], tl, tn, lastpos, nextpos, hit))
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

		virtual void Start()
		{
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
			for (int i = 0; i < 5; i++)
				mMSLoc[i] = new BaseObject("ArcMid" + i);

			mCSMesh = new MeshComponent("../Resources/ControllerTP.obj");
			mCSMesh->AddTexture("../Resources/Marker.png", TextureType::eTEX_DIFFUSE);
			mCSLoc->AddComponent(mCSMesh);
			mCSMesh->SetAlpha(.45);
			for (int i = 0; i < 5; i++)
			{
				mMidMesh[i] = new MeshComponent("../Resources/ArcMarker.obj");
				mMidMesh[i]->AddTexture("../Resources/Marker.png", TextureType::eTEX_DIFFUSE);
				mMSLoc[i]->AddComponent(mMidMesh[i]);
				mMidMesh[i]->SetAlpha(.45);
			}
			mTPMesh = new MeshComponent("../Resources/TeleportMarker.obj");
			mTPMesh->AddTexture("../Resources/Marker.png", TextureType::eTEX_DIFFUSE);
			mTPLoc->AddComponent(mTPMesh);
			mTPMesh->SetAlpha(.45);

			matrix4 temp;
			temp = mCSMesh->GetTransform().GetMatrix() * scaleM;
			mCSMesh->GetTransform().SetMatrix(temp);

			scaleM.first = vec4f(.25f, 0, 0, 0);
			scaleM.second = vec4f(0, .25f, 0, 0);
			scaleM.third = vec4f(0, 0, .25f, 0);
			scaleM.fourth = vec4f(0, 0, 0, 1);

			for (int i = 0; i < 5; i++)
			{
				temp = mMidMesh[i]->GetTransform().GetMatrix() * scaleM;
				mMidMesh[i]->GetTransform().SetMatrix(temp);
			}

			scaleM.first = vec4f(.35f, 0, 0, 0);
			scaleM.second = vec4f(0, .35f, 0, 0);
			scaleM.third = vec4f(0, 0, .35f, 0);
			scaleM.fourth = vec4f(0, 0, 0, 1);

			temp = mTPMesh->GetTransform().GetMatrix() * scaleM;
			mTPMesh->GetTransform().SetMatrix(temp);

			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mTPLoc);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mCSLoc);
			for (int i = 0; i < 5; i++)
				LevelManager::GetInstance().GetCurrentLevel()->AddObject(mMSLoc[i]);

			mTPMesh->SetVisible(false);
			mCSMesh->SetVisible(false);
			for (int i = 0; i < 5; i++)
				mMidMesh[i]->SetVisible(false);

			mTPParticles = new TeleportEffect(-1, 500, 15, vec3f());
			Particle* p = &Particle::Init();
			p->SetColors(vec4f(0, .8, 1, 1), vec4f(0, .8, 1, 1));
			p->SetLife(400);
			p->SetSize(.015, .005);
			mTPParticles->SetParticle(p);
			mTPParticles->SetPosBounds(vec3f(-.25, 0, -.25), vec3f(.25, 0, .25));
			mTPParticles->SetVelBounds(vec3f(0, .8, 0), vec3f(0, 2.5, 0));
			mTPParticles->SetTexture("../Resources/BasicSquareP.png");

			ParticleSystem::Instance()->AddEmitter(mTPParticles);
		}

		virtual void Update()
		{
			if (!VRInputManager::GetInstance().IsVREnabled())
			{
				if (/*(GetAsyncKeyState(VK_SHIFT) & 1) &&*/ dynamic_cast<SFXEmitter*>(mHeadset->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1)))
					((SFXEmitter*)mHeadset->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1))->CallEvent(Emitter::ePlay);
				return;
			}

			// I'm lazy so, let's just set this thing's position to the controller's position.
			matrix4 mat = VRInputManager::GetInstance().GetController(mControllerRole).GetPosition();
			mObject->GetTransform().SetMatrix(mat);
			bool paused = false;

			if (cLevel->GetTimeManipulator() != nullptr)
			{
				paused = cLevel->GetTimeManipulator()->isTimePaused();

			}

			mCanTeleport = CalculateCurve(mat.Position, mVelocity, mAcceleration, nullptr, nullptr, mArc);

			if (mCanTeleport)
			{
				/*GenerateMesh(vec3f(0, 0, 10), mat);
				mPGhost->data.Position = vec4f();*/

				matrix4 scaleM;
				scaleM.first = vec4f(.05f, 0, 0, 0);
				scaleM.second = vec4f(0, .05f, 0, 0);
				scaleM.third = vec4f(0, 0, .05f, 0);
				scaleM.fourth = vec4f(0, 0, 0, 1);

				matrix4 r = matrix4::CreateNewYRotation(ang);

				matrix4 m;
				m = mTPMesh->GetTransform().GetMatrix() * r;
				m.fourth = vec4f(mArc[mArc.size() - 1]) * mat;
				mTPMesh->GetTransform().SetMatrix(m);

				m = mat * scaleM * r;
				m.fourth = mat.fourth;
				mCSMesh->GetTransform().SetMatrix(m);

				#pragma region Arc Points

				int index = 0;
				scaleM.first = vec4f(.025f, 0, 0, 0);
				scaleM.second = vec4f(0, .025f, 0, 0);
				scaleM.third = vec4f(0, 0, .025f, 0);
				scaleM.fourth = vec4f(0, 0, 0, 1);
				r = matrix4::CreateNewXRotation(ang);
				m = mat * scaleM * r;

				index = (((mArc.size() / 5) == 0 ? 1 : mArc.size() / 5) - 1) * 1;
				m.fourth = vec4f(mArc[index]) * mat;
				mMidMesh[0]->GetTransform().SetMatrix(m);

				scaleM.first = vec4f(.05f, 0, 0, 0);
				scaleM.second = vec4f(0, .05f, 0, 0);
				scaleM.third = vec4f(0, 0, .05f, 0);
				scaleM.fourth = vec4f(0, 0, 0, 1);

				m = mat * scaleM * r;
				index = (((mArc.size() / 5) == 0 ? 1 : mArc.size() / 5) - 1) * 2;
				m.fourth = vec4f(mArc[index]) * mat;
				mMidMesh[1]->GetTransform().SetMatrix(m);

				//middle
				scaleM.first = vec4f(.07f, 0, 0, 0);
				scaleM.second = vec4f(0, .07f, 0, 0);
				scaleM.third = vec4f(0, 0, .07f, 0);
				scaleM.fourth = vec4f(0, 0, 0, 1);

				m = mat * scaleM * r;
				index = (((mArc.size() / 5) == 0 ? 1 : mArc.size() / 5) - 1) * 3;
				m.fourth = vec4f(mArc[index]) * mat;
				mMidMesh[2]->GetTransform().SetMatrix(m);

				scaleM.first = vec4f(.09f, 0, 0, 0);
				scaleM.second = vec4f(0, .09f, 0, 0);
				scaleM.third = vec4f(0, 0, .09f, 0);
				scaleM.fourth = vec4f(0, 0, 0, 1);

				m = mat * scaleM * r;
				index = (((mArc.size() / 5) == 0 ? 1 : mArc.size() / 5) - 1) * 4;
				m.fourth = vec4f(mArc[index]) * mat;
				mMidMesh[3]->GetTransform().SetMatrix(m);

				scaleM.first = vec4f(.15f, 0, 0, 0);
				scaleM.second = vec4f(0, .15f, 0, 0);
				scaleM.third = vec4f(0, 0, .15f, 0);
				scaleM.fourth = vec4f(0, 0, 0, 1);

				m = mat * scaleM * r;
				index = (((mArc.size() / 5) == 0 ? 1 : mArc.size() / 5) - 1) * 5;
				if (index == mArc.size() - 1)
					index = (((mArc.size() / 5) == 0 ? 1 : mArc.size() / 5) - 1) * 4;
				m.fourth = vec4f(mArc[index]) * mat;
				mMidMesh[4]->GetTransform().SetMatrix(m);

			#pragma endregion

				mat = VRInputManager::GetInstance().GetController(mControllerRole).GetPosition();
			}

			if (!interp->GetActive() && !Settings::GetInstance().GetBool("CantTeleport"))
			{
				if (VRInputManager::GetInstance().GetController(mControllerRole).GetPress(vr::EVRButtonId::k_EButton_SteamVR_Touchpad) && mCanTeleport)
				{
					mTPMesh->SetVisible(true);
					mCSMesh->SetVisible(true);
					for (int i = 0; i < 5; i++)
						mMidMesh[i]->SetVisible(true);
					if (ParticleSystem::Instance()->DoesExist(mTPParticles))
					{
						mTPParticles->SetPos(vec4f(mArc[mArc.size() - 1]) * mat);
						mTPParticles->FIRE();
					}
				}
				else
				{
					mTPMesh->SetVisible(false);
					mCSMesh->SetVisible(false);
					for (int i = 0; i < 5; i++)
						mMidMesh[i]->SetVisible(false);
					if (ParticleSystem::Instance()->DoesExist(mTPParticles))
						mTPParticles->CeaseFire();
				}
				if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressUp(vr::EVRButtonId::k_EButton_SteamVR_Touchpad) && mCanTeleport && !Settings::GetInstance().GetBool("PauseMenuUp"))
				{
					if (!paused)
					{

						//SystemLogger::Debug() << "Touchpad Pressed" << std::endl;
						vec4f raydir = (mArc[mArc.size() - 1] - mArc[0]);
						raydir.w = 0;
						//raydir = raydir.Normalize();

						vec4f forward(0, 0, 1, 0);
						float controllerTime = 0, wallTime = FLT_MAX;
						size_t Esize = mEnvironmentObjects.size();
						for (int i = 0; i < Esize; ++i)
						{
							forward.Set(0, 0, 1, 0);
							matrix4 objMat = mEnvironmentObjects[i]->GetTransform().GetMatrix();
							matrix4 objMatInv = mEnvironmentObjects[i]->GetTransform().GetMatrix().Invert();
							matrix4 inverse = (mat * objMatInv);
							vec3f meshPos = inverse.Position;
							forward = raydir;
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
								forward = raydir;
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
											mat.Position.y += 2;
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
