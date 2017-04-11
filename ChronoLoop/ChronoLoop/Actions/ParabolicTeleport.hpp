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
#include <d3d11.h>


namespace Epoch {

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
		float t = 0;

		for (int i = 0; i < 10; i++)
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

	struct PTeleportAction : public CodeComponent {
		MeshComponent *mPlaneMesh, *mWallsMesh, *mBlockMesh, *mExitMesh, *mServerMesh;
		BaseObject *mPlaneObject, *mWallsObject, *mBlockObject, *mExitObject, *mServerObject, *mHeadset;
		ControllerType mControllerRole = eControllerType_Primary;
		Level* cLevel = nullptr;
		PTeleportAction(ControllerType _t) { mControllerRole = _t; };

		bool mCanTeleport = false;
		std::vector<vec3f> mArc;
		vec3f mVelocity, mAcceleration;
		RenderShape* mShape;

		virtual void Start() {
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			mPlaneObject = cLevel->FindObjectWithName("Floor");
			mWallsObject = cLevel->FindObjectWithName("Walls");
			mBlockObject = cLevel->FindObjectWithName("TransparentDoor1");
			mExitObject = cLevel->FindObjectWithName("TransparentDoor2");
			mServerObject = cLevel->FindObjectWithName("Servers");
			if (mPlaneObject)
			{
				mPlaneMesh = (MeshComponent*)mPlaneObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mWallsMesh = (MeshComponent*)mWallsObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mBlockMesh = (MeshComponent*)mBlockObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mExitMesh = (MeshComponent*)mExitObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
				mServerMesh = (MeshComponent*)mServerObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			}
			mHeadset = LevelManager::GetInstance().GetCurrentLevel()->GetHeadset();

			mVelocity = vec3f(0, 0, 10);
			mAcceleration = vec3f(0, -9.81f, 0);
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

			//TODO RAYMOND: Recalculate velocity, clamp to max of 45
			vec3f fwdvel, right;
			vec3f up = vec3f(0, 1, 0);
			fwdvel = mVelocity * ((up.Dot(mVelocity)) / mVelocity.SquaredMagnitude());
			float angle = acos(fwdvel.Dot(mVelocity));
			right = up.Cross(fwdvel);

			if (fwdvel.Dot(mVelocity) < 0)
				angle *= -1;

			if (angle > 45)
			{
				float ag = acos(fwdvel.Dot(mVelocity));
				mVelocity =  fwdvel * (sin((1 - 45.0f / angle) * ag) / sin(ag)) + mVelocity * (sin((45.0f / angle) * ag) / sin(ag));
				mVelocity /= mVelocity.Magnitude();
				mVelocity *= mVelocity.Magnitude();
				angle = 45;
			}

			mCanTeleport = CalculateCurve(mat.Position, mVelocity, mAcceleration, mPlaneMesh, mArc);

			//TODO RAYMOND: Rework this for arc end point
			if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Touchpad) && mCanTeleport && !Settings::GetInstance().GetBool("PauseMenuUp")) {
				if (!paused) {
					vec3f raydir = mArc[mArc.size() - 1] - mArc[0];

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
							if (Physics::Instance()->RayToTriangle(
								(tris + i)->Vertex[0],
								(tris + i)->Vertex[1],
								(tris + i)->Vertex[2],
								(tris + i)->Normal, meshPos, fwd, hitTime)) {
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
						if (Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, mArc[0], raydir, meshTime)) {
							if (meshTime < wallTime) {
								fwd *= meshTime;
								point[0] += fwd[0] * objMat.xAxis[0]; // x
								point[2] += fwd[2] * objMat.zAxis[2]; // z
																	  //VRInputManager::Instance().iGetPlayerPosition()[3][3] += forward[3]; // w
																	  //Move any held objects along with player 


								mat = VRInputManager::GetInstance().GetPlayerPosition();
								mat.Position.y = 1.5f;
								vec3f pos = mat.Position;
								vec3f up(0, 1, 0);
								mat = DirectX::XMMatrixLookAtRH(mat.Position.vector, point.vector, up.vector);
								mat = mat.Invert();
								mat.Position = pos;
								meshTime = 0, wallTime = FLT_MAX;
								for (int i = 0; i < ARRAYSIZE(meshes); ++i) {
									forward.Set(0, 0, 1, 0);
									matrix4 objMatInv = objects[i]->GetTransform().GetMatrix().Invert();
									matrix4 inverse = (mat * objMatInv);
									vec3f meshPos = inverse.Position;
									forward *= inverse;
									vec3f fwd(forward);
									Triangle *tris = meshes[i]->GetTriangles();
									size_t numTris = meshes[i]->GetTriangleCount();
									for (unsigned int i = 0; i < numTris; ++i) {
										float hitTime = FLT_MAX;
										if (Physics::Instance()->RayToTriangle(
											(tris + i)->Vertex[0],
											(tris + i)->Vertex[1],
											(tris + i)->Vertex[2],
											(tris + i)->Normal, meshPos, fwd, hitTime)) {
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
								vec3f fwd = forward;
								if (Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, mArc[0], raydir, meshTime))
								{
									if (meshTime < wallTime) {
										fwd *= meshTime;
										VRInputManager::GetInstance().GetPlayerPosition()[3][0] += fwd[0] * objMat.xAxis[0]; // x
										VRInputManager::GetInstance().GetPlayerPosition()[3][2] += fwd[2] * objMat.zAxis[2]; // z

										if (mHeadset->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1) != nullptr && dynamic_cast<SFXEmitter*>(mHeadset->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1)))
											((SFXEmitter*)mHeadset->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 1))->CallEvent(Emitter::ePlay);
									}
								}
							}
							else {
								SystemLogger::GetLog() << "[DEBUG] Can't let you do that, Starfox." << std::endl;
							}
						}
					}
				}
			}
		}
	};
}