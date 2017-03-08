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
		BaseObject *mChamberObject, *mStartObject, *mExitObject, *mFloorObject, *mRoomObject;
		ControllerType mControllerRole = eControllerType_Primary;

		float tTime;

		virtual void Start()
		{
			tTime = 0;

			mChamberObject = Level::Instance()->iFindObjectWithName("mmChamber");
			mStartObject = Level::Instance()->iFindObjectWithName("mmStart");
			mExitObject = Level::Instance()->iFindObjectWithName("mmExit");
			mFloorObject = Level::Instance()->iFindObjectWithName("mmFloor");
			mRoomObject = Level::Instance()->iFindObjectWithName("mmRoom");

			mChamberMesh = (MeshComponent*)mExitObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mStartMesh = (MeshComponent*)mExitObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mExitMesh = (MeshComponent*)mExitObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mFloorMesh = (MeshComponent*)mExitObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mRoomMesh = (MeshComponent*)mExitObject->GetComponentIndexed(eCOMPONENT_MESH, 0);

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

			if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Trigger))
			{
				vec4f forward(0, 0, 1, 0);
				forward *= mObject->GetTransform().GetMatrix();
				MeshComponent* meshes[] = { mStartMesh, mExitMesh };
				BaseObject* objects[] = { mStartObject, mExitObject };
				for (int i = 0; i < ARRAYSIZE(meshes); ++i)
				{
					vec4f meshPos = (mat * objects[i]->GetTransform().GetMatrix().Invert()).Position;
					Triangle *tris = meshes[i]->GetTriangles();
					size_t numTris = meshes[i]->GetTriangleCount();
					for (unsigned int i = 0; i < numTris; ++i)
					{
						float hitTime;
						if (Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, meshPos, forward, hitTime))
						{
							if (i == 0)
							{
								matrix4 mat = mChamberObject->GetTransform().GetMatrix();
								mChamberInterp->SetEdit(mChamberObject->GetTransform().GetMatrix());
								mChamberInterp->SetStart(mat);
								mat += matrix4::CreateTranslation(0, -10, 0);
								mChamberInterp->SetEnd(mat);
								mChamberInterp->SetActive(true);

								mat = VRInputManager::GetInstance().GetPlayerPosition();
								mChamberInterp->SetEdit(VRInputManager::GetInstance().GetPlayerPosition());
								mChamberInterp->SetStart(mat);
								mat += matrix4::CreateTranslation(0, -10, 0);
								mChamberInterp->SetEnd(mat);
								mChamberInterp->SetActive(true);
							}
							else if (i == 1)
								Level::Instance()->ChronoLoop = false;
						}
					}
				}
			}
			else if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Touchpad))
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
						if (meshTime < wallTime && mChamberMesh->GetTransform().GetPosition()->y < -9.9f)
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

			tTime += TimeManager::Instance()->GetDeltaTime();
			if (tTime >= 10)
			{
				mChamberInterp->Update(tTime / 10);
				mPlayerInterp->Update(tTime / 10);
			}
		}

		virtual void OnDestroy()
		{
			delete mChamberInterp;
			delete mPlayerInterp;
		}
	};

}
