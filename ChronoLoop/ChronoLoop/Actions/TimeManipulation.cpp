
#include "../Objects/Component.h"
#include "../Objects/MeshComponent.h"
#include "../Actions/CodeComponent.hpp"
#include "../Objects/BaseObject.h"
#include "../Core/Level.h"
#include "../Input/VRInputManager.h"
#include "../Rendering/TextureManager.h"
#include "../Rendering/Draw2D.h"
#include "../Core/Pool.h"
#include "TimeManipulation.h"

namespace Epoch
{

	TimeManipulation::TimeManipulation() {}


	TimeManipulation::~TimeManipulation() {}

	unsigned int TimeManipulation::mCloneCount = 0;


	void TimeManipulation::Start()
	{
		//TODO PAT: Replace this with ryan's GUI system
		return;
		D3D11_TEXTURE2D_DESC txtdec;
		txtdec.Width = 800;
		txtdec.Height = 600;
		txtdec.MipLevels = 1;
		txtdec.ArraySize = 1;
		txtdec.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		txtdec.SampleDesc.Count = 1;
		txtdec.SampleDesc.Quality = 0;
		txtdec.Usage = D3D11_USAGE_DEFAULT;
		txtdec.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		txtdec.CPUAccessFlags = 0;
		txtdec.MiscFlags = 0;
		HRESULT hr;
		hr = Renderer::Instance()->iGetDevice().Get()->CreateTexture2D(&txtdec, NULL, &mCountTxt);
		mCountMap = Draw::Instance().CreateBitmapForTexture(mCountTxt);
		//This draws to center
		Font* tempFont = new Font(L"Times New Roman", 25, (D2D1::ColorF(D2D1::ColorF::White, 1.0f)), DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		Draw::Instance().DrawTextToBitmap(0.0f, 0.0f, 1.0f, 1.0f, *tempFont, std::to_wstring(mCloneCount), mCountMap);
	}

	void TimeManipulation::Update()
	{
		//std::string textureName = std::string("Clone") + std::to_string(mCloneCount);
		//if (!mTexturedApplied) {
		//	TextureManager::Instance()->iAddTexture2D(textureName, mCountTxt, nullptr);
		//	//((MeshComponent*)BaseObject::GetObjectByName("plane")->GetComponentIndexed(eCOMPONENT_MESH, 0))->AddTexture(textureName.c_str(), eTEX_CUSTOM1);
		//	mTexturedApplied = true;
		//}
		if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_ApplicationMenu))
		{
			int frameRewind = 30;

			if (!TimeManager::Instance()->CheckRewindAvaliable(frameRewind))
				return;

			SystemLogger::GetLog() << "[Debug] A clone is being made, please hold: " << mCloneCount << " | Is left: " << mControllerRole << std::endl;

			Transform identity;

			BaseObject* headset = Pool::Instance()->iGetObject()->Reset("headset - " + std::to_string(mCloneCount), identity); //new BaseObject("headset" + std::to_string(rand), identity);
			MeshComponent *visibleMesh = new MeshComponent("../Resources/Cube.obj");
			visibleMesh->AddTexture("../Resources/cube_texture.png", eTEX_DIFFUSE);
			headset->AddComponent(visibleMesh);


			BaseObject* Controller1 = Pool::Instance()->iGetObject()->Reset("Controller - " + std::to_string(mCloneCount), identity); //new BaseObject("Controller" + std::to_string(rand), identity);
			MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");
			ControllerCollider* CubeColider = new ControllerCollider(Controller1, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), true);
			mc->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
			Controller1->AddComponent(CubeColider);
			Controller1->AddComponent(mc);


			BaseObject* Controller2 = Pool::Instance()->iGetObject()->Reset("Controller2 - " + std::to_string(mCloneCount)); //new BaseObject("Controller2" + std::to_string(rand), identity);
			MeshComponent *mc2 = new MeshComponent("../Resources/Controller.obj");
			ControllerCollider* CubeColider2 = new ControllerCollider(Controller2, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), false);
			mc2->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
			Controller2->AddComponent(CubeColider2);
			Controller2->AddComponent(mc2);

			//Make a clone 3 seconds ago.
			TimeManager::Instance()->RewindMakeClone(TimeManager::Instance()->GetCurrentSnapFrame() - 30, headset, Controller1, Controller2);
			Level::Instance()->iSetHeadsetAndControllers(headset, Controller1, Controller2, CubeColider, CubeColider2);
			//it is extreamly important that the objects are added after time rewinded because of the objectLifeTimeStruct and more..
			Physics::Instance()->mObjects.push_back(headset);
			Physics::Instance()->mObjects.push_back(Controller1);
			Physics::Instance()->mObjects.push_back(Controller2);
			TimeManager::Instance()->AddPlayerObjectToTimeline(headset);
			TimeManager::Instance()->AddPlayerObjectToTimeline(Controller1);
			TimeManager::Instance()->AddPlayerObjectToTimeline(Controller2);
			mCloneCount++;
		}
		if (VRInputManager::GetInstance().GetController(mControllerRole).GetPressDown(vr::EVRButtonId::k_EButton_Grip))
		{
			TimeManager::Instance()->RewindTimeline(TimeManager::Instance()->GetCurrentSnapFrame() - 30, Level::Instance()->iGetHeadset()->GetUniqueID(), Level::Instance()->iGetLeftController()->GetUniqueID(), Level::Instance()->iGetRightController()->GetUniqueID());
		}

	}

} // Epoch Namespace