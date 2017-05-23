#pragma once
#include "CodeComponent.hpp"
#include "../Common/Settings.h"
#include <vector>
#include "../Objects/BaseObject.h"
#include "../Objects/MeshComponent.h"
#include "../Objects/Component.h"
#include "../Actions/CCProgressBar.h"
#include "../Input/VRInputManager.h"
#include "../Sound/SoundEngine.h"


namespace Epoch
{

	struct CCLevel1Tutorial : public CodeComponent
	{
		std::vector<BaseObject*> boards;
		std::vector<int>mPrevBoards, mCurrentBoards;
		std::vector<matrix4>mBoardMatrixs;
		float scaleUpX, scaleUpY, scaleDownX, scaleDownY;
		float tempScaleX, tempScaleY;
		bool scalingDone, boardchange = false, once = true;
		int currentTut = -1, timeToRewind = 0;
		unsigned int tStart, tEnd;
		PSAnimatedMultiscan_Data mScanlineData;
		CCProgressBar* pb;

		virtual void Start()
		{
			mScanlineData.DiffuseAlpha = 0.9f;
			mScanlineData.MultiscanAlpha = 0.9f;
			mScanlineData.ScanlineAlpha = 0.9f;
			scalingDone = false;

			Settings::GetInstance().SetInt("tutStep", 1);
			TimeManager::Instance()->SaveSettingIntToTimeline("tutStep", 1);
			Settings::GetInstance().SetBool("Level1Tutorial", true);
			//Teleport Initialize 0
			Transform Transform1;
			mBoardMatrixs.push_back(matrix4::CreateNewXRotation(1.5708f) * matrix4::CreateNewYRotation(-1.5708f) * matrix4::CreateNewTranslation(-14.3f, 1.5f, -2.83f));
			Transform1.SetMatrix(mBoardMatrixs.back());
			BaseObject* mTeleportBoard = new BaseObject("mTeleportBoard", Transform1);
			MeshComponent* tm = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			tm->AddTexture("../Resources/tutTeleport.png", eTEX_DIFFUSE);
			tm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			tm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			tm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			tm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			tm->SetVisible(false);
			mTeleportBoard->AddComponent(tm);
			Emitter* te = new SFXEmitter();
			((SFXEmitter*)te)->SetEvent(AK::EVENTS::SFX_GENTLE_ALARM);
			mTeleportBoard->AddComponent(te);
			AudioWrapper::GetInstance().AddEmitter(te, "mTeleportBoard");
			boards.push_back(mTeleportBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mTeleportBoard);

			//Pick Up Initialize 1
			Transform transformPickup;
			mBoardMatrixs.push_back(matrix4::CreateNewXRotation(1.5708f) * matrix4::CreateNewYRotation(-1.5708f) * matrix4::CreateNewTranslation(5.97f, 2.09f, -.57f));
			transformPickup.SetMatrix(mBoardMatrixs.back());
			BaseObject* mPickUpBoard = new BaseObject("mPickUpBoard", transformPickup);
			MeshComponent* pum = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			pum->AddTexture("../Resources/tutPickUp.png", eTEX_DIFFUSE);
			pum->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			pum->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			pum->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			pum->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			pum->SetVisible(false);
			mPickUpBoard->AddComponent(pum);
			Emitter* pe = new SFXEmitter();
			((SFXEmitter*)pe)->SetEvent(AK::EVENTS::SFX_GENTLE_ALARM);
			mPickUpBoard->AddComponent(pe);
			AudioWrapper::GetInstance().AddEmitter(pe, "mPickUpBoard");
			boards.push_back(mPickUpBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mPickUpBoard);

			//Pause Time Initialize 2
			Transform transformPauseBoard;
			mBoardMatrixs.push_back(matrix4::CreateNewXRotation(1.5708f) * matrix4::CreateNewYRotation(-1.5708f) * matrix4::CreateNewTranslation(-9.68f, 1.5f, -3.39f));
			transformPauseBoard.SetMatrix(mBoardMatrixs.back());
			BaseObject* mPauseTimeBoard = new BaseObject("mPauseTimeBoard", transformPauseBoard);
			MeshComponent* ptm = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			ptm->AddTexture("../Resources/tutPauseTime.png", eTEX_DIFFUSE);
			ptm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			ptm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			ptm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			ptm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			ptm->SetVisible(false);
			mPauseTimeBoard->AddComponent(ptm);
			Emitter* pte = new SFXEmitter();
			((SFXEmitter*)pte)->SetEvent(AK::EVENTS::SFX_GENTLE_ALARM);
			mPauseTimeBoard->AddComponent(pte);
			AudioWrapper::GetInstance().AddEmitter(pte, "mPauseTimeBoard");
			boards.push_back(mPauseTimeBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mPauseTimeBoard);

			//Rewind Initialize 3
			Transform transformRewind;
			mBoardMatrixs.push_back(matrix4::CreateNewXRotation(1.5708f) * matrix4::CreateNewYRotation(-1.5708f) * matrix4::CreateNewTranslation(-9.68f, 1.5f, -3.39f));
			transformRewind.SetMatrix(mBoardMatrixs.back());
			BaseObject* mRewindBoard = new BaseObject("mRewindBoard", transformRewind);
			MeshComponent* rm = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			rm->AddTexture("../Resources/tutRewind.png", eTEX_DIFFUSE);
			rm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			rm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			rm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			rm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			rm->SetVisible(false);
			pb = new CCProgressBar();
			Settings::GetInstance().SetFloat("TutorialRewind - CurProgress", 0);
			pb->SetCurProgress(0);
			mRewindBoard->AddComponent(pb);
			mRewindBoard->AddComponent(rm);
			Emitter* re = new SFXEmitter();
			((SFXEmitter*)re)->SetEvent(AK::EVENTS::SFX_GENTLE_ALARM);
			mRewindBoard->AddComponent(re);
			AudioWrapper::GetInstance().AddEmitter(re, "mRewindBoard");
			boards.push_back(mRewindBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mRewindBoard);

			//Accept Time Initialize 4
			Transform transformAcceptTime;
			mBoardMatrixs.push_back(matrix4::CreateNewXRotation(1.5708f) * matrix4::CreateNewYRotation(-1.5708f) * matrix4::CreateNewTranslation(-9.68f, 1.5f, -3.39f));
			transformAcceptTime.SetMatrix(mBoardMatrixs.back());
			BaseObject* mAcceptBoard = new BaseObject("mAcceptBoard", transformAcceptTime);
			MeshComponent* am = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			am->AddTexture("../Resources/tutAcceptTime.png", eTEX_DIFFUSE);
			am->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			am->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			am->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			am->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			am->SetVisible(false);
			mAcceptBoard->AddComponent(am);
			Emitter* ae = new SFXEmitter();
			((SFXEmitter*)ae)->SetEvent(AK::EVENTS::SFX_GENTLE_ALARM);
			mAcceptBoard->AddComponent(ae);
			AudioWrapper::GetInstance().AddEmitter(ae, "mAcceptBoard");
			boards.push_back(mAcceptBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mAcceptBoard);

			//Cancel Time Initialize 5
			Transform transformCancelTime;
			mBoardMatrixs.push_back(matrix4::CreateNewXRotation(1.5708f) * matrix4::CreateNewYRotation(-1.5708f) * matrix4::CreateNewTranslation(-9.68f, 1.5f, -2.24f));
			transformCancelTime.SetMatrix(mBoardMatrixs.back());
			BaseObject* mCancelBoard = new BaseObject("mCancelBoard", transformCancelTime);
			MeshComponent* cm = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			cm->AddTexture("../Resources/tutCancelTime.png", eTEX_DIFFUSE);
			cm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			cm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			cm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			cm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			cm->SetVisible(false);
			mCancelBoard->AddComponent(cm);
			boards.push_back(mCancelBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mCancelBoard);

			//Device Board 6
			Transform transformDeviceboard;
			mBoardMatrixs.push_back(matrix4::CreateNewXRotation(1.5708f) * matrix4::CreateNewYRotation(-1.5708f) * matrix4::CreateNewTranslation(-9.68f, 1.5f, -2.24f));
			transformDeviceboard.SetMatrix(mBoardMatrixs.back());
			BaseObject* mDeviceBoard = new BaseObject("mDeviceBoard", transformDeviceboard);
			MeshComponent* dm = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			dm->AddTexture("../Resources/tutDevice.png", eTEX_DIFFUSE);
			dm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			dm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			dm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			dm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			dm->SetVisible(false);
			mDeviceBoard->AddComponent(dm);
			boards.push_back(mDeviceBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mDeviceBoard);

			pb->OnDisable();
		}

		virtual void Update()
		{
			if (GetAsyncKeyState(VK_SHIFT) & 1)
				Settings::GetInstance().SetInt("tutStep", Settings::GetInstance().GetInt("tutStep") + 1);

			int tut = Settings::GetInstance().GetInt("tutStep");
			if (tut != currentTut)
			{
				currentTut = tut;
				switch (tut)
				{
				case 1:
					boardchange = true;
					mCurrentBoards.push_back(0);
					break;
				case 2:
					boardchange = true;
					mPrevBoards = mCurrentBoards;
					mCurrentBoards.clear();
					mCurrentBoards.push_back(2);
					mCurrentBoards.push_back(6);
					break;
				case 3:
					boardchange = true;
					mPrevBoards = mCurrentBoards;
					mPrevBoards.pop_back();
					mCurrentBoards.clear();
					mCurrentBoards.push_back(3);
					mCurrentBoards.push_back(6);

					if(once)
					{
						tStart = Settings::GetInstance().GetUInt("tut1ButtonPress");
						tEnd = Settings::GetInstance().GetUInt("tut1FirstPause");
						timeToRewind = Settings::GetInstance().GetUInt("tut1FirstPause") - Settings::GetInstance().GetUInt("tut1ButtonPress");
						//SystemLogger::GetLog() << "pb End Progress: " << timeToRewind << std::endl;
						Settings::GetInstance().SetUInt("TutorialRewind - FinalProgress", timeToRewind);
						pb->SetFinalProgress((float)timeToRewind);
						pb->GetBackground()->GetTransform().SetMatrix(pb->GetBackground()->GetTransform().GetMatrix() * matrix4::CreateNewXRotation(1.5708f) * matrix4::CreateNewYRotation(-1.5708f) * matrix4::CreateNewTranslation(-9.95f, 2.2f, -2.86f));
						pb->OnEnable();
						once = false;
					}
					break;
				case 4:
					boardchange = true;
					mPrevBoards = mCurrentBoards;
					mCurrentBoards.clear();
					mCurrentBoards.push_back(4);
					mCurrentBoards.push_back(5);
					pb->OnDisable();
					break;
				case 6:
					boardchange = true;
					mPrevBoards = mCurrentBoards;
					mCurrentBoards.clear();
					mCurrentBoards.push_back(1);
					break;
				case 8:
					boardchange = true;
					mPrevBoards = mCurrentBoards;
					Settings::GetInstance().SetInt("tutStep", 0);
					mCurrentBoards.clear();
					break;
				}
			}

			if (tut == 3)
			{
				//SystemLogger::GetLog() << "pb Current Progress: " << Settings::GetInstance().GetUInt("TutorialRewind - CurProgress") << std::endl;
				pb->SetCurProgress((float)Settings::GetInstance().GetUInt("TutorialRewind - CurProgress"));
			}

			ScaleUpCurrentBoards();
			ScaleDownPrevBoards();
		}

		virtual void OnDestroy()
		{
			Settings::GetInstance().SetBool("Level1Tutorial", false);
			Settings::GetInstance().SetInt("tutStep", 0);
		}
		void SetAllBoardsInvisable()
		{
			for (unsigned int i = 0; i < boards.size(); i++)
			{
				((MeshComponent*)boards[i]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
			}
		}

		void ScaleUpCurrentBoards()
		{

			for (unsigned int i = 0; i < mCurrentBoards.size(); i++)
			{
				if (!((MeshComponent*)boards[mCurrentBoards[i]]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible())
				{
					((MeshComponent*)boards[mCurrentBoards[i]]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					if (boards[mCurrentBoards[i]]->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))
						((SFXEmitter*)boards[mCurrentBoards[i]]->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();

					scaleUpX = 0.05f;
					scaleUpY = 0;
				}
			}
			if (scaleUpY < 1.0f)
				scaleUpY += 0.1f;
			else if (scaleUpX < 1.0f)
				scaleUpX += 0.05f;

			for (unsigned int i = 0; i < mCurrentBoards.size(); i++)
			{
				if (scaleUpX < 1 || scaleUpY < 1)
				{
					vec4f pos = *boards[mCurrentBoards[i]]->GetTransform().GetPosition();
					boards[mCurrentBoards[i]]->GetTransform().SetMatrix(matrix4::CreateNewScale(scaleUpX, 1, scaleUpY) * mBoardMatrixs[mCurrentBoards[i]]);
				}
			}

			mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
			mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
			if (mScanlineData.ScanlineVOffset > 2.5f)
			{
				mScanlineData.ScanlineVOffset = -0.5f;
			}
			for (unsigned int i = 0; i < mCurrentBoards.size(); i++)
			{
				((MeshComponent*)boards[mCurrentBoards[i]]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);
			}
		}


		void ScaleDownPrevBoards()
		{

			if (mPrevBoards.size() > 0)
			{
				if (boardchange)
				{
					scaleDownX = 1.0f;
					scaleDownY = 1.0f;
					scalingDone = false;
					boardchange = false;
				}
				if (!scalingDone)
				{
					if (scaleDownX >= 0.05f)
						scaleDownX -= 0.05f;
					else if (scaleDownY >= 0.0f)
						scaleDownY -= 0.1f;
					for (size_t i = 0; i < mPrevBoards.size(); i++)
					{
						boards[mPrevBoards[i]]->GetTransform().SetMatrix(matrix4::CreateNewScale(scaleDownX, 1, scaleDownY) * boards[mPrevBoards[i]]->GetTransform().GetMatrix());
						if (scaleDownY <= 0.0f)
						{
							((MeshComponent*)boards[mPrevBoards[i]]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
							scalingDone = true;
						}
					}
				}
			}

		}
	};

}