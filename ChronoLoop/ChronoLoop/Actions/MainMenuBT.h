#pragma once
#include "../Objects/BaseObject.h"
#include "../Common/Logger.h"
#include "CodeComponent.hpp"
#include "../Objects/MeshComponent.h"
#include "../Physics/Physics.h"
#include "../Input/VRInputManager.h"
#include "../Core/Level.h"
#include "../Actions/HeadsetFollow.hpp"
#include "..\Common\Interpolator.h"

namespace Epoch
{

	struct MainMenuBT : public CodeComponent
	{
		MainMenuBT(ControllerType _t) { mControllerRole = _t; };

		Interpolator<matrix4>* mChamberInterp = new Interpolator<matrix4>(InterpolatorType::I_Matrix4);
		Interpolator<matrix4>* mPlayerInterp = new Interpolator<matrix4>(InterpolatorType::I_Matrix4);
		MeshComponent *mChamberMesh, *mStartMesh, *mExitMesh, *mFloorMesh, *mRoomMesh;
		BaseObject *mChamberObject, *mStartObject, *mExitObject, *mFloorObject, *mRoomObject, *mCubeObject;
		ControllerType mControllerRole = eControllerType_Primary;
		bool mBooped = false;

		float tTime;

		virtual void Start()
		{
			tTime = 0;

			mChamberObject = Level::Instance()->iFindObjectWithName("mmChamber");
			mStartObject = Level::Instance()->iFindObjectWithName("mmStart");
			mExitObject = Level::Instance()->iFindObjectWithName("mmExit");
			mFloorObject = Level::Instance()->iFindObjectWithName("mmFloor");
			mRoomObject = Level::Instance()->iFindObjectWithName("mmRoom");
			mCubeObject = Level::Instance()->iFindObjectWithName("mmCube");

			mChamberMesh = (MeshComponent*)mChamberObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mStartMesh = (MeshComponent*)mStartObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mExitMesh = (MeshComponent*)mExitObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mFloorMesh = (MeshComponent*)mFloorObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mRoomMesh = (MeshComponent*)mRoomObject->GetComponentIndexed(eCOMPONENT_MESH, 0);

		}

		virtual void Update()
		{
			if (!VRInputManager::GetInstance().IsVREnabled())
			{
				return;
			}
			// I'm lazy so, let's just set this thing's position to the controller's position.
			matrix4 mat = VRInputManager::GetInstance().GetController(mControllerRole).GetPosition();
			mObject->GetTransform().SetMatrix(mat);


			// Here lies the code to make a tiny little cube move to where you raycast (currently only works on the star and exit planes).
			MeshComponent* meshes[] = { mStartMesh, mExitMesh };
			BaseObject* objects[] = { mStartObject, mExitObject };
			vec4f awdforward(0, 0, 1, 0);
			for (int i = 0; i < ARRAYSIZE(objects); ++i) {
				awdforward.Set(0, 0, 1, 0);
				matrix4 inverse = (mat * objects[i]->GetTransform().GetMatrix().Invert());
				vec4f meshPos = inverse.Position;
				awdforward *= inverse;
				Triangle *tris = meshes[i]->GetTriangles();
				size_t numTris = meshes[i]->GetTriangleCount();
				for (unsigned int j = 0; j < numTris; ++j) {
					float hitTime;
					if (Physics::Instance()->RayToTriangle((tris + j)->Vertex[0], (tris + j)->Vertex[1], (tris + j)->Vertex[2], (tris + j)->Normal, meshPos, awdforward, hitTime)) {
						mCubeObject->GetTransform().SetMatrix(matrix4::CreateScale(0.1f, 0.1f, 0.1f) * mat);
						mCubeObject->GetTransform().GetMatrix().Position.x += (vec4f(0, 0, 1, 0) * mat).x * hitTime;
						mCubeObject->GetTransform().GetMatrix().Position.y += (vec4f(0, 0, 1, 0) * mat).y * hitTime;
						mCubeObject->GetTransform().GetMatrix().Position.z += (vec4f(0, 0, 1, 0) * mat).z * hitTime;
					}
				}
			}




			if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Trigger))
			{
				MeshComponent* meshes[] = { mStartMesh, mExitMesh };
				BaseObject* objects[] = { mStartObject, mExitObject };
				for (int i = 0; i < ARRAYSIZE(meshes); ++i)
				{
					vec4f forward(0, 0, 1, 0);
					matrix4 inverse = (mat * objects[i]->GetTransform().GetMatrix().Invert());
					vec4f meshPos = inverse.Position;
					forward *= inverse;
					Triangle *tris = meshes[i]->GetTriangles();
					size_t numTris = meshes[i]->GetTriangleCount();
					for (unsigned int j = 0; j < numTris; ++j)
					{
						float hitTime;
						if (Level::Instance()->mmflip && Physics::Instance()->RayToTriangle((tris + j)->Vertex[0], (tris + j)->Vertex[1], (tris + j)->Vertex[2], (tris + j)->Normal, meshPos, forward, hitTime))
						{
							if (i == 0)
							{
								matrix4 mat = mChamberObject->GetTransform().GetMatrix();
								mChamberInterp->SetEdit(mChamberObject->GetTransform().GetMatrix());
								mChamberInterp->SetStart(mat);
								mat *= matrix4::CreateTranslation(0, -10, 0);
								mChamberInterp->SetEnd(mat);
								mChamberInterp->SetActive(true);

								mat = VRInputManager::GetInstance().GetPlayerPosition();
								mPlayerInterp->SetEdit(VRInputManager::GetInstance().GetPlayerPosition());
								mPlayerInterp->SetStart(mat);
								mat *= matrix4::CreateTranslation(0, -10, 0);
								mPlayerInterp->SetEnd(mat);
								mPlayerInterp->SetActive(true);
								mBooped = true;
								Level::Instance()->mmflip = false;
							}
							else if (i == 1)
								Level::Instance()->ChronoLoop = false;
						}
					}
				}
			}
			else if (mChamberMesh->GetTransform().GetPosition()->y < -9.9f && VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Touchpad))
			{
				vec4f forward(0, 0, 1, 0);
				forward *= mObject->GetTransform().GetMatrix();
				MeshComponent* meshes[] = { mRoomMesh, mChamberMesh };
				BaseObject* objects[] = { mRoomObject, mChamberObject };
				float meshTime = 0, wallTime = FLT_MAX;
				for (int i = 0; i < ARRAYSIZE(meshes); ++i)
				{
					vec4f meshPos = (mat * objects[i]->GetTransform().GetMatrix().Invert()).Position;
					Triangle *tris = meshes[i]->GetTriangles();
					size_t numTris = meshes[i]->GetTriangleCount();
					for (unsigned int i = 0; i < numTris; ++i)
					{
						float hitTime = FLT_MAX;
						Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, meshPos, forward, hitTime);
						if (hitTime < wallTime)
						{
							wallTime = hitTime;
						}
					}
				}

				Triangle *tris = mFloorMesh->GetTriangles();
				size_t numTris = mFloorMesh->GetTriangleCount();
				vec4f position = (mat * mFloorObject->GetTransform().GetMatrix().Invert()).Position;
				for (unsigned int i = 0; i < numTris; ++i)
				{
					if (Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, position, forward, meshTime))
					{
						if (meshTime < wallTime)
						{
							forward *= meshTime;
							VRInputManager::GetInstance().GetPlayerPosition()[3][0] += forward[0]; // x
							VRInputManager::GetInstance().GetPlayerPosition()[3][2] += forward[2]; // z
																								   //VRInputManager::Instance().iGetPlayerPosition()[3][3] += forward[3]; // w
						}
						else
						{
							SystemLogger::GetLog() << "[DEBUG] Can't let you do that, Starfox." << std::endl;
						}
					}
				}
			}

			if (mBooped) 
			{
				SystemLogger::Debug() << tTime << "  |  " << TimeManager::Instance()->GetDeltaTime() << std::endl;
				tTime += TimeManager::Instance()->GetDeltaTime();
				if (tTime <= 15) {
					mChamberInterp->Update(tTime / 15.0f);
					mPlayerInterp->Update(tTime / 15.0f);
				}
			}
		}

		virtual void OnDestroy()
		{
			delete mChamberInterp;
			delete mPlayerInterp;
		}
	};

}

