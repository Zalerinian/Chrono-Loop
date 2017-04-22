#pragma once
#include "CodeComponent.hpp"
#include "../Common/Settings.h"
#include <vector>
#include "../Objects/BaseObject.h"
#include "../Objects/MeshComponent.h"
#include "../Objects/Component.h"
#include "../Actions/CCProgressBar.h"
#include "../Input/VRInputManager.h"
#include "../../Resources/Soundbanks/Wwise_IDs.h"
#include "../Sound/SoundEngine.h"

namespace Epoch {

	struct CCLevel2Tutorial : public CodeComponent {
		std::vector<BaseObject*> boards;
		float scaleX, scaleY;
		float tempScaleX, tempScaleY;
		bool scalingDone;
		CCProgessBar* pb;

		PSAnimatedMultiscan_Data mScanlineData;

		virtual void Start() {
			mScanlineData.DiffuseAlpha = 0.9f;
			mScanlineData.MultiscanAlpha = 0.9f;
			mScanlineData.ScanlineAlpha = 0.9f;
			scalingDone = false;

			Settings::GetInstance().SetInt("tutStep", 5);
			Transform identity;

			//Create Clone Initialize
			BaseObject* mCreateCloneBoard = new BaseObject("mCreateCloneBoard", identity);
			MeshComponent* ccm = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			ccm->AddTexture("../Resources/tutClone.png", eTEX_DIFFUSE);
			ccm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			ccm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			ccm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			ccm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			ccm->SetVisible(false);
			mCreateCloneBoard->AddComponent(ccm);
			Emitter* cce = new SFXEmitter();
			((SFXEmitter*)cce)->SetEvent(AK::EVENTS::SFX_COMMUNICATION_CHANNEL);
			mCreateCloneBoard->AddComponent(cce);
			AudioWrapper::GetInstance().AddEmitter(cce, "mCreateCloneBoard");
			boards.push_back(mCreateCloneBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mCreateCloneBoard);

			//Delete Clone Initialize
			BaseObject* mDeleteCloneBoard = new BaseObject("mDeleteCloneBoard", identity);
			MeshComponent* dcm = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			dcm->AddTexture("../Resources/tutDeleteClone.png", eTEX_DIFFUSE);
			dcm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			dcm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			dcm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			dcm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			dcm->SetVisible(false);
			mDeleteCloneBoard->AddComponent(dcm);
			Emitter* dce = new SFXEmitter();
			((SFXEmitter*)dce)->SetEvent(AK::EVENTS::SFX_COMMUNICATION_CHANNEL);
			mDeleteCloneBoard->AddComponent(dce);
			AudioWrapper::GetInstance().AddEmitter(dce, "mDeleteCloneBoard");
			boards.push_back(mDeleteCloneBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mDeleteCloneBoard);

			//UI Initialize
			BaseObject* mUIBoard = new BaseObject("mUIBoard", identity);
			MeshComponent* uim = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			uim->AddTexture("../Resources/tutUI.png", eTEX_DIFFUSE);
			uim->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			uim->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			uim->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			uim->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			uim->SetVisible(false);
			mUIBoard->AddComponent(uim);
			boards.push_back(mUIBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mUIBoard);
		}

		virtual void Update() {
			if (GetAsyncKeyState(VK_SHIFT) & 1)
				Settings::GetInstance().SetInt("tutStep", Settings::GetInstance().GetInt("tutStep") + 1);

			switch (Settings::GetInstance().GetInt("tutStep")) {
			case 5: //Teleport
				if (!((MeshComponent*)boards[0]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)boards[0]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
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
					boards[0]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(1.43f, 1.5f, 8.9f));
					boards[2]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(2.52f, 1.5f, 8.9f));

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)boards[0]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);
				((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);

				break;
			case 7://Pick Up
				if (!((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((SFXEmitter*)boards[1]->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();

					scalingDone = false;
					scaleX = 1.0f;
					scaleY = 1.0f;
					//boards[1]->GetTransform().SetMatrix(matrix4::CreateScale(0,1,0) * matrix4::CreateZRotation(-1.5708f) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(0.785398f) * matrix4::CreateTranslation(-3.55f, 1.7f, 3.71f));
				}
				tempScaleX = scaleX;
				tempScaleY = scaleY;
				if (!scalingDone) {
					if (scaleX >= 0.05f)
						scaleX -= 0.05f;
					else if (scaleY >= 0.0f)
						scaleY -= 0.1f;
					boards[0]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(1.43f, 1.5f, 8.9f));
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
						boards[1]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(3.14159f) * matrix4::CreateTranslation(1.43f, 1.5f, 8.9f));
				}

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);

				break;
			case 8://End
				if (((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible() && !LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused()) {
					scalingDone = false;
				}
				if (!scalingDone) {
					if (scaleX >= 0.05f)
						scaleX -= 0.05f;
					else if (scaleY >= 0.0f)
						scaleY -= 0.1f;
					boards[1]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * boards[1]->GetTransform().GetMatrix());
					boards[2]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * boards[2]->GetTransform().GetMatrix());
					if (scaleY <= 0.0f) {
						((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						scalingDone = true;
					}
				}
				break;
			}
		}

		virtual void OnDestroy() {
			Settings::GetInstance().SetInt("tutStep", 0);
		}
	};

}
