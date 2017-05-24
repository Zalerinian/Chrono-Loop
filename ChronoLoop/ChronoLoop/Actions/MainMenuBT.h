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
		MainMenuBT() {};

		MeshComponent *mChamberMesh;
		BaseObject *mChamberObject, *mTeleportBoard/*, *mCubeObject*/;
		ControllerType mControllerRole = eControllerType_Primary;
		Level* cLevel = nullptr;
		bool mBooped = false;
		float scaleX, scaleY;
		float tempScaleX, tempScaleY;
		bool scalingDone;
		PSAnimatedMultiscan_Data mScanlineData;

		//bool AudioToggle = false;

		virtual void Start()
		{
			cLevel = LevelManager::GetInstance().GetCurrentLevel();

			mChamberObject = cLevel->FindObjectWithName("FloorChamber");
			mChamberMesh = (MeshComponent*)mChamberObject->GetComponentIndexed(eCOMPONENT_MESH, 0);

			mScanlineData.DiffuseAlpha = 0.9f;
			mScanlineData.MultiscanAlpha = 0.9f;
			mScanlineData.ScanlineAlpha = 0.9f;
			Transform identity;
			mTeleportBoard = new BaseObject("mTeleportBoard", identity);
			MeshComponent* tm = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			tm->AddTexture("../Resources/tutTeleport.png", eTEX_DIFFUSE);
			tm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			tm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			tm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			tm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			tm->SetVisible(false);
			mTeleportBoard->AddComponent(tm);
			Emitter* te = new SFXEmitter();
			((SFXEmitter*)te)->SetEvent(AK::EVENTS::SFX_COMMUNICATION_CHANNEL);
			mTeleportBoard->AddComponent(te);
			AudioWrapper::GetInstance().AddEmitter(te, "mTeleportBoard");
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mTeleportBoard);
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

			if (mChamberMesh->GetTransform().GetPosition()->y < -9.99f) {
				Settings::GetInstance().SetBool("CantTeleport", false);
				if (!((MeshComponent*)mTeleportBoard->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)mTeleportBoard->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((SFXEmitter*)mTeleportBoard->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();

					scaleX = 0.05f;
					scaleY = 0;
				}

				tempScaleX = scaleX;
				tempScaleY = scaleY;
				if (scaleY < 1.0f)
					scaleY += 0.1f;
				else if (scaleX < 1.0f)
					scaleX += 0.05f;

				if (tempScaleX != scaleX || tempScaleY != scaleY)
					mTeleportBoard->GetTransform().SetMatrix(matrix4::CreateNewScale(scaleX, 1, scaleY) * matrix4::CreateNewXRotation(1.5708f) * matrix4::CreateNewYRotation(3.92699f) * matrix4::CreateNewTranslation(0.84f, -8.45f, .84f));

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)mTeleportBoard->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);
			}

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

