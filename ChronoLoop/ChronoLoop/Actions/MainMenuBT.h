#pragma once
#include "../Objects/BaseObject.h"
#include "../Common/Logger.h"
#include "CodeComponent.hpp"
#include "CCMazeHelper.h"

#include "../Objects/MeshComponent.h"
#include "../Physics/Physics.h"
#include "../Input/VRInputManager.h"
#include "../Core/Level.h"
#include "../Actions/HeadsetFollow.hpp"
#include "..\Common\Interpolator.h"
#include "../Core/LevelManager.h"
#include "../Common/EasingFunctions.h"

namespace Epoch
{

	struct MainMenuBT : public CodeComponent
	{
		MainMenuBT(ControllerType _t) { mControllerRole = _t; };

		Interpolator<matrix4>* interp;
		matrix4 endPos;

		MeshComponent *mChamberMesh, /**mStartMesh, *mExitMesh,*/ *mFloorMesh, *mRoomMesh;
		BaseObject *mChamberObject, *mStartObject, *mExitObject, *mFloorObject, *mRoomObject/*, *mCubeObject*/;
		ControllerType mControllerRole = eControllerType_Primary;
		Level* cLevel = nullptr;
		bool mBooped = false;
		//bool AudioToggle = false;

		virtual void Start()
		{
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			interp = cLevel->playerInterp;
			endPos = VRInputManager::GetInstance().GetPlayerPosition();
			interp->SetActive(false);

			mChamberObject = cLevel->FindObjectWithName("mmChamber");
			mFloorObject = cLevel->FindObjectWithName("mmFloor");
			mRoomObject = cLevel->FindObjectWithName("RoomFloor");
			//mCubeObject = cLevel->FindObjectWithName("mmCube");
			//mCubeObject->GetTransform().GetMatrix().Position.Set(0, -30000, 0, 1);

			mChamberMesh = (MeshComponent*)mChamberObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			//mStartMesh = (MeshComponent*)mStartObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			//mExitMesh = (MeshComponent*)mExitObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
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

			if (!interp->GetActive()) {
				if (mChamberMesh->GetTransform().GetPosition()->y < -9.9f && VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Touchpad)) {
					
					if (!Settings::GetInstance().GetBool("mmStartAtBottom"))
						Settings::GetInstance().SetBool("mmStartAtBottom", true);

					vec4f forward(0, 0, 1, 0);
					forward *= mObject->GetTransform().GetMatrix();
					vec3f fwd = forward;
					MeshComponent* meshes[] = { mRoomMesh, mChamberMesh };
					BaseObject* objects[] = { mRoomObject, mChamberObject };
					float meshTime = 0, wallTime = FLT_MAX;
					for (int i = 0; i < ARRAYSIZE(meshes); ++i) {
						vec3f meshPos = (mat * objects[i]->GetTransform().GetMatrix().Invert()).Position;
						Triangle *tris = meshes[i]->GetTriangles();
						size_t numTris = meshes[i]->GetTriangleCount();
						for (unsigned int i = 0; i < numTris; ++i) {
							float hitTime = FLT_MAX;
							Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, meshPos, fwd, hitTime);
							if (hitTime < wallTime) {
								wallTime = hitTime;
							}
						}
					}

					Triangle *tris = mFloorMesh->GetTriangles();
					size_t numTris = mFloorMesh->GetTriangleCount();
					vec3f position = (mat * mFloorObject->GetTransform().GetMatrix().Invert()).Position;
					for (unsigned int i = 0; i < numTris; ++i) {
						if (Physics::Instance()->RayToTriangle((tris + i)->Vertex[0], (tris + i)->Vertex[1], (tris + i)->Vertex[2], (tris + i)->Normal, position, fwd, meshTime)) {
							if (meshTime < wallTime) {
								forward *= meshTime;
								endPos = VRInputManager::GetInstance().GetPlayerPosition();
								endPos[3][0] += forward[0]; // x
								endPos[3][2] += forward[2]; // z
								interp->Prepare(.1f, VRInputManager::GetInstance().GetPlayerPosition(), endPos, VRInputManager::GetInstance().GetPlayerPosition());
								interp->SetActive(true);
							} else {
								SystemLogger::Debug() << "Can't let you do that, Starfox." << std::endl;
							}
						}
					}
				}
			}
			else if (interp->Update(TimeManager::Instance()->GetDeltaTime()))
				interp->SetActive(false);

			//if (mBooped)
			//{
			//	mChamberInterp->Update(TimeManager::Instance()->GetDeltaTime());
			//	bool complete = mPlayerInterp->Update(TimeManager::Instance()->GetDeltaTime());
			//	if (complete)
			//	{
			//		((Emitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 0))->Stop(0);
			//		mBooped = false;
			//	}
			//}
		}

		//virtual void OnDestroy()
		//{
		//	//TODO: This is being destroyed at some point before this so wtf, figure this out i guess
		//	//((Emitter*)mChamberObject->GetComponentIndexed(ComponentType::eCOMPONENT_AUDIOEMITTER, 0))->Stop(1);
		//
		//	//SUPER FUCKING JANK END ME
		//	AudioWrapper::GetInstance().MakeEventAtListener(AK::EVENTS::STOP_TEST1);
		//
		//	delete mChamberInterp;
		//	delete mPlayerInterp;
		//}
	};

}

