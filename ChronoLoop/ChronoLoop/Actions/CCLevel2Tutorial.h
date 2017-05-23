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

			Settings::GetInstance().SetInt("tutStep", 5);
			TimeManager::Instance()->SaveSettingIntToTimeline("tutStep", 5);

			//Create Clone Initialize
			Transform transformCreateBoard;
			mBoardMatrixs.push_back(matrix4::CreateNewXRotation(1.5709795f) * matrix4::CreateNewTranslation(6.03f,1.62f,-4.83f));
			transformCreateBoard.SetMatrix(mBoardMatrixs.back());
			BaseObject* mCreateCloneBoard = new BaseObject("mCreateCloneBoard", transformCreateBoard);
			MeshComponent* ccm = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			ccm->AddTexture("../Resources/tutClone.png", eTEX_DIFFUSE);
			ccm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			ccm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			ccm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			ccm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			ccm->SetVisible(false);
			mCreateCloneBoard->AddComponent(ccm);
			Emitter* cce = new SFXEmitter();
			((SFXEmitter*)cce)->SetEvent(AK::EVENTS::SFX_GENTLE_ALARM);
			mCreateCloneBoard->AddComponent(cce);
			AudioWrapper::GetInstance().AddEmitter(cce, "mCreateCloneBoard");
			boards.push_back(mCreateCloneBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mCreateCloneBoard);

			//Delete Clone Initialize
			Transform transformDeleteBoard;
			mBoardMatrixs.push_back(matrix4::CreateNewXRotation(1.5709795f) *  matrix4::CreateNewTranslation(6.03f, 1.62f, -4.83f));
			transformDeleteBoard.SetMatrix(mBoardMatrixs.back());
			BaseObject* mDeleteCloneBoard = new BaseObject("mDeleteCloneBoard", transformDeleteBoard);
			MeshComponent* dcm = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			dcm->AddTexture("../Resources/tutDeleteClone.png", eTEX_DIFFUSE);
			dcm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			dcm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			dcm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			dcm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			dcm->SetVisible(false);
			mDeleteCloneBoard->AddComponent(dcm);
			Emitter* dce = new SFXEmitter();
			((SFXEmitter*)dce)->SetEvent(AK::EVENTS::SFX_GENTLE_ALARM);
			mDeleteCloneBoard->AddComponent(dce);
			AudioWrapper::GetInstance().AddEmitter(dce, "mDeleteCloneBoard");
			boards.push_back(mDeleteCloneBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mDeleteCloneBoard);

			//UI Initialize
			Transform transformUIBoard;
			mBoardMatrixs.push_back(matrix4::CreateNewXRotation(1.5709795f) * matrix4::CreateNewYRotation(4.712385f) * matrix4::CreateNewTranslation(6.85f,1.62f,-3.95f));
			transformUIBoard.SetMatrix(mBoardMatrixs.back());
			BaseObject* mUIBoard = new BaseObject("mUIBoard", transformUIBoard);
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

			//UI Initialize
			Transform transformCloneVisualBoard;
			mBoardMatrixs.push_back(matrix4::CreateNewXRotation(1.5709795f) * matrix4::CreateNewYRotation(1.5709795f)* matrix4::CreateNewTranslation(5.1f,1.66f,-4));
			transformCloneVisualBoard.SetMatrix(mBoardMatrixs.back());
			BaseObject* mCloneVisualBoard = new BaseObject("mCloneVisualBoard", transformCloneVisualBoard);
			MeshComponent* cvm = new MeshComponent("../Resources/PlaneCorrection.obj", .9f);
			cvm->AddTexture("../Resources/tutCloneCreation.png", eTEX_DIFFUSE);
			cvm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			cvm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			cvm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			cvm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			cvm->SetVisible(false);
			mCloneVisualBoard->AddComponent(cvm);
			boards.push_back(mCloneVisualBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mCloneVisualBoard);
		}

		virtual void Update() {
			if (GetAsyncKeyState(VK_SHIFT) & 1)
				Settings::GetInstance().SetInt("tutStep", Settings::GetInstance().GetInt("tutStep") + 1);


			int tut = Settings::GetInstance().GetInt("tutStep");
			if (tut != currentTut) {
				currentTut = tut;
				switch (tut) {
				case 5:
					boardchange = true;
					mPrevBoards = mCurrentBoards;
					mCurrentBoards.clear();
					mCurrentBoards.push_back(0);
					mCurrentBoards.push_back(3);
					mCurrentBoards.push_back(2);
					break;
				case 7:
					boardchange = true;
					mPrevBoards = mCurrentBoards;
					mCurrentBoards.clear();
					mCurrentBoards.push_back(1);
					mCurrentBoards.push_back(2);
					break;
				case 8:
					boardchange = true;
					mPrevBoards = mCurrentBoards;
					mCurrentBoards.clear();
					Settings::GetInstance().SetInt("tutStep", 0);
					break;
				}
			}

			ScaleUpCurrentBoards();
			ScaleDownPrevBoards();
		}

		virtual void OnDestroy() {
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
					boards[mCurrentBoards[i]]->GetTransform().SetMatrix(matrix4::CreateNewScale(scaleUpX, 1, scaleUpY) * mBoardMatrixs[mCurrentBoards[i]]);
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
						boards[mPrevBoards[i]]->GetTransform().SetMatrix(matrix4::CreateNewScale(scaleDownX, 1, scaleDownY) * boards[mPrevBoards[i]]->GetTransform().GetMatrix());
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
