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
		float scaleX, scaleY;
		float tempScaleX, tempScaleY;
		bool scalingDone;
		CCProgressBar* pb;

		PSAnimatedMultiscan_Data mScanlineData;

		virtual void Start() {
			mScanlineData.DiffuseAlpha = 0.9f;
			mScanlineData.MultiscanAlpha = 0.9f;
			mScanlineData.ScanlineAlpha = 0.9f;

			scalingDone = false;

			Settings::GetInstance().SetInt("tutStep", 1);
			Transform identity;
			//Teleport Initialize
			BaseObject* mTeleportBoard = new BaseObject("mTeleportBoard", identity);
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
			boards.push_back(mTeleportBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mTeleportBoard);

			//Pick Up Initialize
			BaseObject* mPickUpBoard = new BaseObject("mPickUpBoard", identity);
			MeshComponent* pum = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			pum->AddTexture("../Resources/tutPickUp.png", eTEX_DIFFUSE);
			pum->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			pum->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			pum->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			pum->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			pum->SetVisible(false);
			mPickUpBoard->AddComponent(pum);
			Emitter* pe = new SFXEmitter();
			((SFXEmitter*)pe)->SetEvent(AK::EVENTS::SFX_COMMUNICATION_CHANNEL);
			mPickUpBoard->AddComponent(pe);
			AudioWrapper::GetInstance().AddEmitter(pe, "mPickUpBoard");
			boards.push_back(mPickUpBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mPickUpBoard);

			//Pause Time Initialize
			BaseObject* mPauseTimeBoard = new BaseObject("mPauseTimeBoard", identity);
			MeshComponent* ptm = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			ptm->AddTexture("../Resources/tutPauseTime.png", eTEX_DIFFUSE);
			ptm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			ptm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			ptm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			ptm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			ptm->SetVisible(false);
			mPauseTimeBoard->AddComponent(ptm);
			Emitter* pte = new SFXEmitter();
			((SFXEmitter*)pte)->SetEvent(AK::EVENTS::SFX_COMMUNICATION_CHANNEL);
			mPauseTimeBoard->AddComponent(pte);
			AudioWrapper::GetInstance().AddEmitter(pte, "mPauseTimeBoard");
			boards.push_back(mPauseTimeBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mPauseTimeBoard);

			//Rewind Initialize
			BaseObject* mRewindBoard = new BaseObject("mRewindBoard", identity);
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
			((SFXEmitter*)re)->SetEvent(AK::EVENTS::SFX_COMMUNICATION_CHANNEL);
			mRewindBoard->AddComponent(re);
			AudioWrapper::GetInstance().AddEmitter(re, "mRewindBoard");
			boards.push_back(mRewindBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mRewindBoard);

			//Accept Time Initialize
			BaseObject* mAcceptBoard = new BaseObject("mAcceptBoard", identity);
			MeshComponent* am = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			am->AddTexture("../Resources/tutAcceptTime.png", eTEX_DIFFUSE);
			am->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			am->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			am->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			am->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			am->SetVisible(false);
			mAcceptBoard->AddComponent(am);
			Emitter* ae = new SFXEmitter();
			((SFXEmitter*)ae)->SetEvent(AK::EVENTS::SFX_COMMUNICATION_CHANNEL);
			mAcceptBoard->AddComponent(ae);
			AudioWrapper::GetInstance().AddEmitter(ae, "mAcceptBoard");
			boards.push_back(mAcceptBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mAcceptBoard);

			//Cancel Time Initialize
			BaseObject* mCancelBoard = new BaseObject("mCancelBoard", identity);
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

			BaseObject* mDeviceBoard = new BaseObject("mDeviceBoard", identity);
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

			bool thing = ((MeshComponent*)boards[6]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible();
			int t = Settings::GetInstance().GetInt("tutStep");
			switch (Settings::GetInstance().GetInt("tutStep")) {
			case 1: //Teleport
				if (!((MeshComponent*)boards[0]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)boards[0]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((SFXEmitter*)boards[0]->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();

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
					boards[0]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.78736f) * matrix4::CreateTranslation(1.08f, 1.5f, -8.48f));

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)boards[0]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);

				break;
			case 2://Pick Up
				if (!((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((SFXEmitter*)boards[1]->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();

					scalingDone = false;
					scaleX = 1.0f;
					scaleY = 1.0f;
				}
				tempScaleX = scaleX;
				tempScaleY = scaleY;
				if (!scalingDone) {
					if (scaleX >= 0.05f)
						scaleX -= 0.05f;
					else if (scaleY >= 0.0f)
						scaleY -= 0.1f;
					boards[0]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.78736f) * matrix4::CreateTranslation(1.08f, 1.5f, -8.48f));
					if (scaleY <= 0.0f) {
						((MeshComponent*)boards[0]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						scalingDone = true;
					}
				} else {
					if (scaleY < 1.0f)
						scaleY += 0.05f;
					else if (scaleX < 1.0f)
						scaleX += 0.05f;
					if (tempScaleX != scaleX || tempScaleY != scaleY)
						boards[1]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(5.97f, 2.09f, -.57f));
				}

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);

				break;
			case 3://Pause Time
				if (!((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((MeshComponent*)boards[6]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((SFXEmitter*)boards[2]->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();

					if (TimeManager::Instance()->GetCurrentSnapFrame() < 60)
						Settings::GetInstance().SetFloat("TutorialRewind - FinalProgress", TimeManager::Instance()->GetCurrentSnapFrame());
					else
						Settings::GetInstance().SetFloat("TutorialRewind - FinalProgress", 60);
					scalingDone = false;
					scaleX = 1.0f;
					scaleY = 1.0f;
				}
				tempScaleX = scaleX;
				tempScaleY = scaleY;
				if (!scalingDone) {
					if (scaleX >= 0.05f)
						scaleX -= 0.05f;
					else if (scaleY >= 0.0f)
						scaleY -= 0.1f;

					boards[1]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(5.97f, 2.09f, -.57f));
					if (scaleY <= 0.0f) {
						((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						scalingDone = true;
					}
				} else {
					if (scaleY < 1.0f)
						scaleY += 0.05f;
					else if (scaleX < 1.0f)
						scaleX += 0.05f;
					if (tempScaleX != scaleX || tempScaleY != scaleY) {
						boards[2]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY)  * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(4.26f, 1.95f, 3));
						boards[6]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY)  * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(3.2f, 1.95f, 3));
					}
				}

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);
				((MeshComponent*)boards[6]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);

				break;
			case 4://Rewind
				if (!((MeshComponent*)boards[3]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)boards[3]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((SFXEmitter*)boards[3]->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();

					Settings::GetInstance().SetBool("Level1Tutorial", true);
					scalingDone = false;
					scaleX = 1.0f;
					scaleY = 1.0f;
					pb->GetBackground()->GetTransform().SetMatrix(pb->GetBackground()->GetTransform().GetMatrix() * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(3.71f, 1.46f, 3));
					pb->OnEnable();
				}
				tempScaleX = scaleX;
				tempScaleY = scaleY;
				if (!scalingDone) {
					if (scaleX >= 0.05f)
						scaleX -= 0.05f;
					else if (scaleY >= 0.0f)
						scaleY -= 0.1f;
					boards[2]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(4.26f, 1.95f, 3));
					boards[6]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(3.2f, 1.95f, 3));
					if (scaleY <= 0.0f) {
						((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						((MeshComponent*)boards[6]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						scalingDone = true;
					}
				} else {
					if (scaleY < 1.0f)
						scaleY += 0.05f;
					else if (scaleX < 1.0f)
						scaleX += 0.05f;
					if (tempScaleX != scaleX || tempScaleY != scaleY) {
						//pb->GetBackground()->GetTransform().SetMatrix(pb->GetBackground()->GetTransform().GetMatrix() * matrix4::CreateScale(scaleX, 1, scaleY)); 
						boards[3]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY)  * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(3.71f, 2.18f, 3));
					}
				}

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)boards[3]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);
				pb->SetCurProgress(Settings::GetInstance().GetFloat("TutorialRewind - CurProgress"));
				break;
			case 6://Accepting Time / Canceling Time
				if (!((MeshComponent*)boards[4]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)boards[4]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((MeshComponent*)boards[5]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((SFXEmitter*)boards[4]->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();

					//boards[4]->GetTransform().SetMatrix(matrix4::CreateScale(1, 1, 1) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(4.26f, 1.95f, 3));
					//boards[5]->GetTransform().SetMatrix(matrix4::CreateScale(1, 1, 1) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(3.2f, 1.95f, 3));
					scalingDone = false;
					scaleX = 1.0f;
					scaleY = 1.0f;
					pb->OnDisable();
				}
				tempScaleX = scaleX;
				tempScaleY = scaleY;
				if (!scalingDone) {
					if (scaleX >= 0.05f)
						scaleX -= 0.05f;
					else if (scaleY >= 0.0f)
						scaleY -= 0.1f;
					boards[3]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY)  * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(3.71f, 2.18f, 3));
					if (scaleY <= 0.0f) {
						((MeshComponent*)boards[3]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						scalingDone = true;
					}
				} else {
					if (scaleY < 1.0f)
						scaleY += 0.05f;
					else if (scaleX < 1.0f)
						scaleX += 0.05f;
					if (tempScaleX != scaleX || tempScaleY != scaleY) {
						boards[4]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(4.26f, 1.95f, 3));
						boards[5]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(3.2f, 1.95f, 3));
					}
				}

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)boards[4]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);
				((MeshComponent*)boards[5]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);

				break;
			case 8://End
				if (((MeshComponent*)boards[4]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible() && !LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused()) {
					scalingDone = false;
				}
				if (!scalingDone) {
					if (scaleX >= 0.05f)
						scaleX -= 0.05f;
					else if (scaleY >= 0.0f)
						scaleY -= 0.1f;
					boards[4]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * boards[6]->GetTransform().GetMatrix());
					boards[5]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * boards[6]->GetTransform().GetMatrix());
					if (scaleY <= 0.0f) {
						((MeshComponent*)boards[4]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						((MeshComponent*)boards[5]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						scalingDone = true;
					}
				}
				break;
			}
		}

		virtual void OnDestroy() {
			Settings::GetInstance().SetBool("Level1Tutorial", false);
			Settings::GetInstance().SetInt("tutStep", 0);
		}
	};

}