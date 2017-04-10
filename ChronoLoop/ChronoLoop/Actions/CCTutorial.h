#pragma once
#include "CodeComponent.hpp"
#include "../Common/Settings.h"
#include <vector>
#include "../Objects/BaseObject.h"
#include "../Objects/MeshComponent.h"
#include "../Objects/Component.h"
#include "../Input/VRInputManager.h"


namespace Epoch
{
	
	struct CCTutorial : public CodeComponent
	{
		std::vector<BaseObject*> boards;
		BaseObject* button, *stand;

		virtual void Start()
		{
			button = LevelManager::GetInstance().GetCurrentLevel()->FindObjectWithName("Button");
			((ButtonCollider*)button->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->Disable();
			((MeshComponent*)button->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
			((MeshComponent*)button->GetComponentIndexed(eCOMPONENT_MESH, 0))->Disable();

			stand = LevelManager::GetInstance().GetCurrentLevel()->FindObjectWithName("Buttonstand");
			((CubeCollider*)stand->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->Disable();
			((MeshComponent*)stand->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);

			Settings::GetInstance().SetInt("tutStep", 1);
			Transform identity;
			//Teleport Initialize
			BaseObject* mTeleportBoard = new BaseObject("mTeleportBoard", identity);
			MeshComponent* tm = new MeshComponent("../Resources/PlaneCorrection.obj");
			tm->AddTexture("../Resources/tutTeleport.png", eTEX_DIFFUSE);
			tm->SetVisible(false);
			mTeleportBoard->AddComponent(tm);
			boards.push_back(mTeleportBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mTeleportBoard);

			//Pick Up Initialize
			BaseObject* mPickUpBoard = new BaseObject("mPickUpBoard", identity);
			MeshComponent* pum = new MeshComponent("../Resources/PlaneCorrection.obj");
			pum->AddTexture("../Resources/tutPickUp.png", eTEX_DIFFUSE);
			pum->SetVisible(false);
			mPickUpBoard->AddComponent(pum);
			boards.push_back(mPickUpBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mPickUpBoard);

			//Pause Time Initialize
			BaseObject* mPauseTimeBoard = new BaseObject("mPauseTimeBoard", identity);
			MeshComponent* ptm = new MeshComponent("../Resources/PlaneCorrection.obj");
			ptm->AddTexture("../Resources/tutPauseTime.png", eTEX_DIFFUSE);
			ptm->SetVisible(false);
			mPauseTimeBoard->AddComponent(ptm);
			boards.push_back(mPauseTimeBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mPauseTimeBoard);

			//Rewind Initialize
			BaseObject* mRewindBoard = new BaseObject("mRewindBoard", identity);
			MeshComponent* rm = new MeshComponent("../Resources/PlaneCorrection.obj");
			rm->AddTexture("../Resources/tutRewind.png", eTEX_DIFFUSE);
			rm->SetVisible(false);
			mRewindBoard->AddComponent(rm);
			boards.push_back(mRewindBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mRewindBoard);

			//Create Clone Initialize
			BaseObject* mCreateCloneBoard = new BaseObject("mCreateCloneBoard", identity);
			MeshComponent* ccm = new MeshComponent("../Resources/PlaneCorrection.obj");
			ccm->AddTexture("../Resources/tutClone.png", eTEX_DIFFUSE);
			ccm->SetVisible(false);
			mCreateCloneBoard->AddComponent(ccm);
			boards.push_back(mCreateCloneBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mCreateCloneBoard);

			//Accept Time Initialize
			BaseObject* mAcceptBoard = new BaseObject("mAcceptBoard", identity);
			MeshComponent* am = new MeshComponent("../Resources/PlaneCorrection.obj");
			am->AddTexture("../Resources/tutAcceptTime.png", eTEX_DIFFUSE);
			am->SetVisible(false);
			mAcceptBoard->AddComponent(am);
			boards.push_back(mAcceptBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mAcceptBoard);

			//Cancel Time Initialize
			BaseObject* mCancelBoard = new BaseObject("mCancelBoard", identity);
			MeshComponent* cm = new MeshComponent("../Resources/PlaneCorrection.obj");
			cm->AddTexture("../Resources/tutCancelTime.png", eTEX_DIFFUSE);
			cm->SetVisible(false);
			mCancelBoard->AddComponent(cm);
			boards.push_back(mCancelBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mCancelBoard);

			//Delete Clone Initialize
			BaseObject* mDeleteCloneBoard = new BaseObject("mDeleteCloneBoard", identity);
			MeshComponent* dcm = new MeshComponent("../Resources/PlaneCorrection.obj");
			dcm->AddTexture("../Resources/tutDeleteClone.png", eTEX_DIFFUSE);
			dcm->SetVisible(false);
			mDeleteCloneBoard->AddComponent(dcm);
			boards.push_back(mDeleteCloneBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mDeleteCloneBoard);

			//UI Initialize
			BaseObject* mUIBoard = new BaseObject("mUIBoard", identity);
			MeshComponent* uim = new MeshComponent("../Resources/PlaneCorrection.obj");
			uim->AddTexture("../Resources/tutUI.png", eTEX_DIFFUSE);
			uim->SetVisible(false);
			mUIBoard->AddComponent(uim);
			boards.push_back(mUIBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mUIBoard);

			//Pause Menu Initialize
			//BaseObject* mPauseMenuBoard = new BaseObject("mPauseMenuBoard", identity);
			//MeshComponent* pmm = new MeshComponent("../Resources/tutPauseMenu.png");
			//pmm->SetVisible(false);
			//mPauseMenuBoard->AddComponent(tm);

			//Settings Initialize
			//Settings::GetInstance().SetBool("tutTeleport", false);
			//Settings::GetInstance().SetBool("tutPickUp", false);
			//Settings::GetInstance().SetBool("tutPause", false);
			//Settings::GetInstance().SetBool("tutRewind", false);
			//Settings::GetInstance().SetBool("tutAcceptCancelTime", false);
			//Settings::GetInstance().SetBool("tutUI", false);
			//Settings::GetInstance().SetBool("tutCreateClone", false);
			//Settings::GetInstance().SetBool("tutDeleteClone", false);
			//Settings::GetInstance().SetBool("tutPauseMenu", false);

		}

		virtual void Update()
		{
			bool thing = ((MeshComponent*)boards[7]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible();
			int t = Settings::GetInstance().GetInt("tutStep");
			switch (Settings::GetInstance().GetInt("tutStep"))
			{
			case 1: //Teleport
				if(!((MeshComponent*)boards[0]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible())
				{
					((MeshComponent*)boards[0]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);

					boards[0]->GetTransform().SetMatrix(matrix4::CreateZRotation(-1.5708f) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateTranslation(2, 1.5f, 0));
				}
				break;
			case 2://Pick Up
				if (!((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)boards[0]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					
					boards[1]->GetTransform().SetMatrix(matrix4::CreateZRotation(-1.5708f) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(0.785398f) * matrix4::CreateTranslation(-3.55f, 1.7f, 3.71f));
				}
				break;
			case 3://Pause Time
				if (!((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);

					boards[2]->GetTransform().SetMatrix(matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
				}
				break;
			case 4://Rewind
				if (!((MeshComponent*)boards[3]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)boards[3]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);

					boards[3]->GetTransform().SetMatrix(matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
				}
				break;
			case 5://Creating Clone
				if (!((MeshComponent*)boards[4]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)boards[3]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)boards[4]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((MeshComponent*)boards[8]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);

					boards[4]->GetTransform().SetMatrix(matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
					boards[8]->GetTransform().SetMatrix(matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 1.1f));
				}
				break;
			case 6://Accepting Time / Canceling Time
				if (!((MeshComponent*)boards[5]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)boards[4]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)boards[8]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)boards[5]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((MeshComponent*)boards[6]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);

					boards[5]->GetTransform().SetMatrix(matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
					boards[6]->GetTransform().SetMatrix(matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 1.1f));
				}
				break;
			case 7://Delete Clone
				if (!((MeshComponent*)boards[7]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)boards[5]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)boards[6]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)boards[7]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);

					boards[7]->GetTransform().SetMatrix(matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
				}
				break;
			case 8://End
				if (((MeshComponent*)boards[7]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible() && !LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused()) {
					((MeshComponent*)boards[7]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((ButtonCollider*)button->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->Enable();
					((MeshComponent*)button->GetComponentIndexed(eCOMPONENT_MESH, 0))->Enable();
					((MeshComponent*)button->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((CubeCollider*)stand->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->Enable();
					((MeshComponent*)stand->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
				}
				break;
			}
		}

		virtual void OnDestroy()
		{
			Settings::GetInstance().SetInt("tutStep", 0);
		}
	};

}