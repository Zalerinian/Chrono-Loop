
#include "../Objects/Component.h"
#include "../Objects/MeshComponent.h"
#include "../Actions/CodeComponent.hpp"
#include "../Objects/BaseObject.h"
#include "../Core/Level.h"
#include "../Input/VRInputManager.h"
#include "../Rendering/TextureManager.h"
#include "TimeManipulation.h"


TimeManipulation::TimeManipulation() {}


TimeManipulation::~TimeManipulation() {}

unsigned int TimeManipulation::mCloneCount = 0;


void TimeManipulation::Start() {
	mCloneCount++;

	//TODO PAT: Replace this with ryan's GUI system
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
	hr = ((*RenderEngine::Renderer::Instance()->iGetDevice())->CreateTexture2D(&txtdec, NULL, &mCountTxt));
	mCountMap = RenderEngine::Renderer::Instance()->CreateBitmapForTexture(mCountTxt);
	//This draws to center
	RenderEngine::Renderer::Instance()->DrawTextToBitmap(std::to_wstring(mCloneCount), mCountMap, 0.0f, 0.0f, 1.0f, 1.0f);
}

void TimeManipulation::Update() {
	//std::string textureName = std::string("Clone") + std::to_string(mCloneCount);
	//if (!mTexturedApplied) {
	//	TextureManager::Instance()->iAddTexture2D(textureName, mCountTxt, nullptr);
	//	//((MeshComponent*)BaseObject::GetObjectByName("plane")->GetComponentIndexed(eCOMPONENT_MESH, 0))->AddTexture(textureName.c_str(), eTEX_CUSTOM1);
	//	mTexturedApplied = true;
	//}
	if (VRInputManager::Instance().iGetController(mLeft).GetPressDown(vr::EVRButtonId::k_EButton_ApplicationMenu)) {
		int frameRewind = 30;

		if (!TimeManager::Instance()->CheckRewindAvaliable(frameRewind))
			return;
		Transform identity;
		identity.SetMatrix(Math::MatrixIdentity());

		int rand = std::rand();


		MeshComponent *visibleMesh = new MeshComponent("../Resources/Cube.obj");
		//visibleMesh->AddTexture(textureName.c_str(), RenderEngine::eTEX_DIFFUSE);

		BaseObject* headset = new BaseObject("headset" + std::to_string(rand), identity);
		headset->AddComponent(visibleMesh);


		BaseObject* Controller1 = new BaseObject("Controller" + std::to_string(rand), identity);
		MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");
		mc->AddTexture("../Resources/vr_controller_lowpoly_texture.png", RenderEngine::eTEX_DIFFUSE);
		Controller1->AddComponent(mc);



		BaseObject* Controller2 = new BaseObject("Controller2" + std::to_string(rand), identity);
		MeshComponent *mc2 = new MeshComponent("../Resources/Controller.obj");
		mc2->AddTexture("../Resources/vr_controller_lowpoly_texture.png", RenderEngine::eTEX_DIFFUSE);
		Controller2->AddComponent(mc2);

		//Make a clone 3 seconds ago.
		TimeManager::Instance()->RewindMakeClone(TimeManager::Instance()->GetCurrentSnapFrame() - 30, headset, Controller1, Controller2);
		Level::Instance()->iSetHeadsetAndControllers(headset, Controller1, Controller2);
		TimeManager::Instance()->AddObjectToTimeline(headset);
		TimeManager::Instance()->AddObjectToTimeline(Controller1);
		TimeManager::Instance()->AddObjectToTimeline(Controller2);

	}
	if (VRInputManager::Instance().iGetController(mLeft).GetPressDown(vr::EVRButtonId::k_EButton_Grip)) {
		Level* CurLev = Level::Instance();
		TimeManager::Instance()->RewindTimeline(TimeManager::Instance()->GetCurrentSnapFrame() - 30, Level::Instance()->iGetHeadset()->GetUniqueID(), Level::Instance()->iGetLeftController()->GetUniqueID(), Level::Instance()->iGetRightController()->GetUniqueID());
	}

}
