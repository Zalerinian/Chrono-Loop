#pragma once
#include "CodeComponent.hpp"
#include "../Common/Settings.h"
#include <vector>
#include "../Objects/BaseObject.h"
#include "../Objects/MeshComponent.h"
#include "../Objects/Component.h"
#include "../Actions/CCProgressBar.h"
#include "../Input/VRInputManager.h"

namespace Epoch {

	struct CCLevel1Tutorial : public CodeComponent {
		std::vector<BaseObject*> boards;
		std::vector<int>mPrevBoards, mCurrentBoards;
		std::vector<matrix4>mBoardMatrixs;
		float scaleUpX, scaleUpY, scaleDownX, scaleDownY;
		float tempScaleX, tempScaleY;
		bool scalingDone, boardchange = false;
		int currentTut = -1;
		CCProgressBar* pb;

		PSAnimatedMultiscan_Data mScanlineData;

		virtual void Start() {
			mScanlineData.DiffuseAlpha = 0.9f;
			mScanlineData.MultiscanAlpha = 0.9f;
			mScanlineData.ScanlineAlpha = 0.9f;

			scalingDone = false;

			Settings::GetInstance().SetInt("tutStep", 1);
			TimeManager::Instance()->SaveSettingIntToTimeline("tutStep", 1);
			Settings::GetInstance().SetBool("Level1Tutorial", true);
			//Teleport Initialize
			Transform Transform1;
			mBoardMatrixs.push_back(matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.78736f) * matrix4::CreateTranslation(1.08f, 1.5f, -8.48f));
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

			//Pick Up Initialize
			Transform transformPickup;
			mBoardMatrixs.push_back(matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(5.97f, 2.09f, -.57f));
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

			//Pause Time Initialize
			Transform transformPauseBoard;
			mBoardMatrixs.push_back(matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(4.26f, 1.95f, 3));
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

			//Rewind Initialize
			Transform transformRewind;
			mBoardMatrixs.push_back(matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(3.71f, 2.18f, 3));
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
			Settings::GetInstance().SetFloat("TutorialRewind - FinalProgress", 50);
			pb->SetFinalProgress(50);
			pb->SetCurProgress(0);
			mRewindBoard->AddComponent(pb);
			mRewindBoard->AddComponent(rm);
			Emitter* re = new SFXEmitter();
			((SFXEmitter*)re)->SetEvent(AK::EVENTS::SFX_GENTLE_ALARM);
			mRewindBoard->AddComponent(re);
			AudioWrapper::GetInstance().AddEmitter(re, "mRewindBoard");
			boards.push_back(mRewindBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mRewindBoard);

			//Accept Time Initialize
			Transform transformAcceptTime;
			mBoardMatrixs.push_back(matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(4.26f, 1.95f, 3));
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

			//Cancel Time Initialize
			Transform transformCancelTime;
			mBoardMatrixs.push_back(matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(3.2f, 1.95f, 3));
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

			Transform transformDeviceboard;
			mBoardMatrixs.push_back(matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(3.2f, 1.95f, 3));
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

		virtual void Update() {
			if (GetAsyncKeyState(VK_SHIFT) & 1)
				Settings::GetInstance().SetInt("tutStep", Settings::GetInstance().GetInt("tutStep") + 1);

			
			int tut = 4;// Settings::GetInstance().GetInt("tutStep");
			if (tut != currentTut) {
				currentTut = tut;
				switch (tut) {
				case 1:
					boardchange = true;
					mPrevBoards = mCurrentBoards;
					mCurrentBoards.clear();
					mCurrentBoards.push_back(0);
					break;
				case 2:
					boardchange = true;
					mPrevBoards = mCurrentBoards;
					mCurrentBoards.clear();
					mCurrentBoards.push_back(1);
					break;
				case 3:
					boardchange = true;
					mPrevBoards = mCurrentBoards;
					mCurrentBoards.clear();
					mCurrentBoards.push_back(2);
					mCurrentBoards.push_back(6);
					break;
				case 4:
					boardchange = true;
					mPrevBoards = mCurrentBoards;
					mCurrentBoards.clear();
					mCurrentBoards.push_back(3);
					pb->GetBackground()->GetTransform().SetMatrix(pb->GetProgressBar()->GetTransform().GetMatrix() * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(3.71f, 1.46f, 3));
					pb->OnEnable();
					break;
				case 6:
					boardchange = true;
					mPrevBoards = mCurrentBoards;
					mCurrentBoards.clear();
					mCurrentBoards.push_back(4);
					mCurrentBoards.push_back(5);
					pb->OnDisable();

					break;
				case 8:
					boardchange = true;
					mPrevBoards = mCurrentBoards;
					mCurrentBoards.clear();
					//SetAllBoardsInvisable();
					break;

				}
			}
			if(tut == 4)
			{
				pb->SetCurProgress(Settings::GetInstance().GetFloat("TutorialRewind - CurProgress"));
			}
			ScaleUpCurrentBoards();
			ScaleDownPrevBoards();

		}

		virtual void OnDestroy() {
			Settings::GetInstance().SetBool("Level1Tutorial", false);
			Settings::GetInstance().SetInt("tutStep", 0);
		}
		void SetAllBoardsInvisable() {
			for (unsigned int i = 0; i < boards.size(); i++) {
				((MeshComponent*)boards[i]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
			}
		}

		void ScaleUpCurrentBoards() {

			for (unsigned int i = 0; i < mCurrentBoards.size(); i++) {
				if (!((MeshComponent*)boards[mCurrentBoards[i]]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
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

			for (unsigned int i = 0; i < mCurrentBoards.size(); i++) {
				if (scaleUpX < 1 || scaleUpY < 1) {
					vec4f pos = *boards[mCurrentBoards[i]]->GetTransform().GetPosition();
					boards[mCurrentBoards[i]]->GetTransform().SetMatrix(matrix4::CreateScale(scaleUpX, 1, scaleUpY) * mBoardMatrixs[mCurrentBoards[i]]);
				}
			}

			mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
			mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
			if (mScanlineData.ScanlineVOffset > 2.5f) {
				mScanlineData.ScanlineVOffset = -0.5f;
			}
			for (unsigned int i = 0; i < mCurrentBoards.size(); i++) {
				((MeshComponent*)boards[mCurrentBoards[i]]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);
			}
		}


		void ScaleDownPrevBoards() {

			if (mPrevBoards.size() > 0) {
				if (boardchange) {
					scaleDownX = 1.0f;
					scaleDownY = 1.0f;
					scalingDone = false;
					boardchange = false;
				}
				if (!scalingDone) {
					if (scaleDownX >= 0.05f)
						scaleDownX -= 0.05f;
					else if (scaleDownY >= 0.0f)
						scaleDownY -= 0.1f;
					for (size_t i = 0; i < mPrevBoards.size(); i++) {
						boards[mPrevBoards[i]]->GetTransform().SetMatrix(matrix4::CreateScale(scaleDownX, 1, scaleDownY) * boards[mPrevBoards[i]]->GetTransform().GetMatrix());
						if (scaleDownY <= 0.0f) {
							((MeshComponent*)boards[mPrevBoards[i]]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
							scalingDone = true;
						}
					}
				}
			}

		}
	};

}