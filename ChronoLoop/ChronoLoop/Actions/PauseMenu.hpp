
#pragma once
#include "CodeComponent.hpp"
#include "../Objects/MeshComponent.h"
#include "../Core/LevelManager.h"
#include "../Common/Settings.h"
#include "../Core/Pool.h"
#include "../Rendering/Draw2D.h"
#include "../Input/VRInputManager.h"
#include "../Input/KeyboardInput.h"
#include "../Core/TimeManager.h"
#include "../Input/CommandConsole.h"
#include <list>


namespace Epoch
{

	enum MENU_NAME
	{
		NOTHING = -1,
		PAUSEMENU_ON,
		PAUSEMENU_OFF,
		MAIN_MENU,
		RESUME,
		SETTINGS,
		HUBWORLD,
		RESTART,
		AUDIO,
		MISC,
		MAX
	};
	struct PauseMenu : public CodeComponent
	{
	private:

		std::vector<BaseObject*> mAllMenuOptions;
		MENU_NAME mActiveMenu;
		//ActivePanel mActivePanel;
		bool PauseMenuisUp = false;
		bool test1 = false, doneBeingOff = false;
		Transform identity;
		Level* cLevel;

		BaseObject *pPauseMenuBase = nullptr;
		MeshComponent *mcPauseMenuBase = nullptr;

		BaseObject *pMainPanel = nullptr, *pSettingsPanel = nullptr; //Children of the Entire Pause Menu
		MeshComponent *mcMainPanel = nullptr, *mcSettingsPanel = nullptr;

		BaseObject *pResume = nullptr, *pSettings = nullptr, *pHubworld = nullptr,*pRestartLevel = nullptr, *pAudio = nullptr, *pMisc = nullptr;//Children of Panels
		MeshComponent *mcResume = nullptr, *mcSettings = nullptr, *mcHubworld = nullptr, *mcRestartLevel = nullptr,*mcAudio = nullptr, *mcMisc = nullptr;


		D2D1::ColorF wut = { 1,1,1,1 };
		Font mainFont = Font(L"Agency FB", 75, wut);

		float transparentColor[4] = { 0,0,0,0 };

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texPauseMenuBase, texMainPanel,texSettingsPanel, texResume,texSettings,texHubworld, texRestartLevel,texAudio,texMisc;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtvPauseMenuBase, rtvMainPanel, rtvSettingsPanel, rtvResume, rtvSettings, rtvHubworld, rtvRestartLevel, rtvAudio, rtvMisc;

		matrix4 playerPos;
		vec4f playerRot;
		float scaleX = 0.0f, scaleY = 0.0f;
	public:
		//Accessors
		bool isPauseMenuOn() { return PauseMenuisUp; }

		//Mutators
		void SetPauseMenuToOn(bool _set) { PauseMenuisUp = _set; }

		virtual void Start()
		{
			//CommandConsole::Instance().AddCommand(L"/TEST1", Test1);
			Settings::GetInstance().SetBool("PauseMenuUp", PauseMenuisUp);
			cLevel = LevelManager::GetInstance().GetCurrentLevel();
			//Pause Menu Base Initialize
			SetUpThisObjectForMe(&pPauseMenuBase, (MeshComponent**)&mcPauseMenuBase, std::string("PauseMenu - Base"), (identity));

			pPauseMenuBase->AddComponent(mcPauseMenuBase);
			TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Base", nullptr, &texPauseMenuBase);


			//PANEL SET UP
				//Main Pause Menu Panel Initialize
				//identity.SetMatrix(matrix4::CreateNewTranslation(0, 0, 0.9f));
				SetUpThisObjectForMe(&pMainPanel, &mcMainPanel, std::string("PauseMenu - Main Menu"), identity);
				TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Main Menu", nullptr, &texMainPanel);

				pMainPanel->AddComponent(mcMainPanel);
				pMainPanel->SetParent(pPauseMenuBase);

				//Settings Panel Initialize
				SetUpThisObjectForMe(&pSettingsPanel, &mcSettingsPanel, std::string("PauseMenu - Settings Panel"), identity);
				TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Settings Panel", nullptr, &texSettingsPanel);

				pSettingsPanel->AddComponent(mcSettingsPanel);
				pSettingsPanel->SetParent(pPauseMenuBase);

				//Setting Children of Pause Menu Base
				pPauseMenuBase->AddChild(pMainPanel);
				pPauseMenuBase->AddChild(pSettingsPanel);
			//OPTIONS SET UP
				//Main Panel Options
					//Resume Option Initialize
					SetUpThisObjectForMe(&pResume, &mcResume, std::string("PauseMenu - Resume Option"), identity);
					TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Resume Option", nullptr, &texResume);

					pResume->AddComponent(mcResume);
					pResume->SetParent(pMainPanel);

					//Settings Option Initialize
					SetUpThisObjectForMe(&pSettings, &mcSettings, std::string("PauseMenu - Settings Option"), identity);
					TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Settings Option", nullptr, &texSettings);

					pSettings->AddComponent(mcSettings);
					pSettings->SetParent(pMainPanel);

					//Hubworld Option Initialize
					SetUpThisObjectForMe(&pHubworld, &mcHubworld, std::string("PauseMenu - Hubworld Option"), identity);
					TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Hubworld Option", nullptr, &texHubworld);

					pHubworld->AddComponent(mcHubworld);
					pHubworld->SetParent(pMainPanel);
					//Restart Level Option Initialize
					SetUpThisObjectForMe(&pRestartLevel, &mcRestartLevel, std::string("PauseMenu - Restart Level Option"), identity);
					TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Restart Level Option", nullptr, &texRestartLevel);

					pRestartLevel->AddComponent(mcRestartLevel);
					pRestartLevel->SetParent(pMainPanel);

					//Setting Children for Main Panel
					pMainPanel->AddChild(pResume);
					pMainPanel->AddChild(pSettings);
					pMainPanel->AddChild(pHubworld);
					pMainPanel->AddChild(pRestartLevel);
				//Settings Panel Options
					//Audio Option Initialize
					//identity.SetMatrix(matrix4::CreateNewTranslation(0, 0, 0.8f));
					SetUpThisObjectForMe(&pAudio, &mcAudio, std::string("PauseMenu - Audio Option"), identity);
					TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Audio Option", nullptr, &texAudio);

					pAudio->AddComponent(mcAudio);
					pAudio->SetParent(pSettingsPanel);

					//Misc Option Initialize
					//identity.SetMatrix(matrix4::CreateNewTranslation(0, 0, 0.8f));
					SetUpThisObjectForMe(&pMisc, &mcMisc, std::string("PauseMenu - Misc Option"), identity);
					TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Misc Option", nullptr, &texMisc);

					pMisc->AddComponent(mcMisc);
					pMisc->SetParent(pSettingsPanel);


					//Setting Children for Settings Panel
					pSettingsPanel->AddChild(pAudio);
					pSettingsPanel->AddChild(pMisc);
			//RTV Creation
					Renderer::Instance()->GetDevice()->CreateRenderTargetView((ID3D11Resource*)texPauseMenuBase.Get(), NULL, rtvPauseMenuBase.GetAddressOf());
					Renderer::Instance()->GetDevice()->CreateRenderTargetView((ID3D11Resource*)texMainPanel.Get(), NULL, rtvMainPanel.GetAddressOf());
					Renderer::Instance()->GetDevice()->CreateRenderTargetView((ID3D11Resource*)texSettingsPanel.Get(), NULL, rtvSettingsPanel.GetAddressOf());
					Renderer::Instance()->GetDevice()->CreateRenderTargetView((ID3D11Resource*)texResume.Get(), NULL, rtvResume.GetAddressOf());
					Renderer::Instance()->GetDevice()->CreateRenderTargetView((ID3D11Resource*)texSettings.Get(), NULL, rtvSettings.GetAddressOf());
					Renderer::Instance()->GetDevice()->CreateRenderTargetView((ID3D11Resource*)texHubworld.Get(), NULL, rtvHubworld.GetAddressOf());
					Renderer::Instance()->GetDevice()->CreateRenderTargetView((ID3D11Resource*)texRestartLevel.Get(), NULL, rtvRestartLevel.GetAddressOf());
					Renderer::Instance()->GetDevice()->CreateRenderTargetView((ID3D11Resource*)texAudio.Get(), NULL, rtvAudio.GetAddressOf());
					Renderer::Instance()->GetDevice()->CreateRenderTargetView((ID3D11Resource*)texMisc.Get(), NULL, rtvMisc.GetAddressOf());
			//Lighting Stuff
					mcPauseMenuBase->SetPixelShader(ePS_PURETEXTURE);
					mcMainPanel->SetPixelShader(ePS_PURETEXTURE);
					mcSettingsPanel->SetPixelShader(ePS_PURETEXTURE);
					mcResume->SetPixelShader(ePS_PURETEXTURE);
					mcSettings->SetPixelShader(ePS_PURETEXTURE);
					mcHubworld->SetPixelShader(ePS_PURETEXTURE);
					mcRestartLevel->SetPixelShader(ePS_PURETEXTURE);
					mcAudio->SetPixelShader(ePS_PURETEXTURE);
					mcMisc->SetPixelShader(ePS_PURETEXTURE);
			//Draw in front
					mcPauseMenuBase->SetTopmost(true);
					mcMainPanel->SetTopmost(true);
					mcSettingsPanel->SetTopmost(true);
					mcResume->SetTopmost(true);
					mcSettings->SetTopmost(true);
					mcHubworld->SetTopmost(true);
					mcRestartLevel->SetTopmost(true);
					mcAudio->SetTopmost(true);
					mcMisc->SetTopmost(true);
			//Active Panel Start Up
				mActiveMenu = PAUSEMENU_ON;
			//Panel Start Up
				SwitchPanel(&mActiveMenu);
				OnDisable();  
				//OnEnable();
			
		}
		virtual void Update()
		{
			if(Settings::GetInstance().GetBool("DidRealStuffInPauseMenu"))
			{
				Settings::GetInstance().SetBool("DidRealStuffInPauseMenu", false);
				OnDisable();
			}
			if ((!PauseMenuisUp && scaleY > 0.0f)) {
				OnEnable();
				PauseMenuisUp = false;
				doneBeingOff = false;
			}
			else if ((PauseMenuisUp && scaleX <= 20.0f))
				OnEnable();

			if((VRInputManager::GetInstance().GetController(eControllerType_Primary).GetPressDown(vr::EVRButtonId::k_EButton_ApplicationMenu) == true || 
				VRInputManager::GetInstance().GetController(eControllerType_Secondary).GetPressDown(vr::EVRButtonId::k_EButton_ApplicationMenu) == true /*|| 
				(GetAsyncKeyState(Keys::M) & 0x1)*/) && !LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused())
			{
				if (PauseMenuisUp) 
					OnDisable();
				else if (!PauseMenuisUp)
					OnEnable();
			}
			if (PauseMenuisUp) {
				Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvPauseMenuBase.Get(), transparentColor);
				Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvMainPanel.Get(), transparentColor);
				Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvSettingsPanel.Get(), transparentColor);
				Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvResume.Get(), transparentColor);
				Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvSettings.Get(), transparentColor);
				Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvHubworld.Get(), transparentColor);
				Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvRestartLevel.Get(), transparentColor);
				Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvAudio.Get(), transparentColor);
				Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvMisc.Get(), transparentColor);

				D2D1::ColorF tempColor = { 0,0,1,0.5f };
				//Pause Base Rectangle
				Draw::Instance().DrawRectangleToBitmap(
					0, 0, 256.0f, 256.0f,
					tempColor,
					Draw::Instance().GetBitmap(texPauseMenuBase.Get()));
				
				//Pause Base's Children
					//Main Panel Rectangle
					Draw::Instance().DrawRectangleToBitmap(
						0, 0, 256.0f, 256.0f,
						(D2D1::ColorF::Black, (UINT32)1.0f),
						Draw::Instance().GetBitmap(texMainPanel.Get()));
					//Main Panel Text
					mainFont.mColor = D2D1::ColorF::WhiteSmoke;
					mainFont.mFontSize = 35;
					Draw::Instance().DrawTextToBitmap(
						85.33f, 0, 170.67f, 50.0f,
						mainFont, L"Main",
						Draw::Instance().GetBitmap(texMainPanel.Get()));

					//Settings Panel Rectangle
					Draw::Instance().DrawRectangleToBitmap(
						0, 0, 256.0f, 256.0f,
						(D2D1::ColorF::Black, (UINT32)0.8f),
						Draw::Instance().GetBitmap(texSettingsPanel.Get()));
					//Settings Panel Text
					Draw::Instance().DrawTextToBitmap(
						80.0f, 0, 176.0f, 50.0f,
						mainFont, L"Settings",
						Draw::Instance().GetBitmap(texSettingsPanel.Get()));
					
					//Main Panel's Children
						mainFont.mColor = D2D1::ColorF::WhiteSmoke;
						mainFont.mFontSize = 75;
						tempColor = { 0,0,0.9f,0.5f };

						//Resume Option Rectangle 
						Draw::Instance().DrawRectangleToBitmap(
							0, 0, 256.0f, 256.0f,
							tempColor,
							Draw::Instance().GetBitmap(texResume.Get()));
						//Resume Option Text
						Draw::Instance().DrawTextToBitmap(
							0, 0, 256.0f, 256.0f,
							mainFont, L"Resume",
							Draw::Instance().GetBitmap(texResume.Get()));

						//Settings Option Rectangle
						Draw::Instance().DrawRectangleToBitmap(
							0, 0, 256.0f, 256.0f,
							tempColor,
							Draw::Instance().GetBitmap(texSettings.Get()));
						//Settings Option Text
						Draw::Instance().DrawTextToBitmap(
							0, 0, 256.0f, 256.0f,
							mainFont, L"Settings",
							Draw::Instance().GetBitmap(texSettings.Get()));

						//Hubworld Option Rectangle 
						Draw::Instance().DrawRectangleToBitmap(
							0, 0, 256.0f, 256.0f,
							tempColor,
							Draw::Instance().GetBitmap(texHubworld.Get()));
						//Hubworld Option Text
						Draw::Instance().DrawTextToBitmap(
							0, 0, 256.0f, 256.0f,
							mainFont, L"Hubworld",
							Draw::Instance().GetBitmap(texHubworld.Get()));

						//Restart Level Option Rectangle 
						Draw::Instance().DrawRectangleToBitmap(
							0, 0, 256.0f, 256.0f,
							tempColor,
							Draw::Instance().GetBitmap(texRestartLevel.Get()));
						//Restart Level Option Text
						Draw::Instance().DrawTextToBitmap(
							0, 0, 256.0f, 256.0f,
							mainFont, L"Restart Level",
							Draw::Instance().GetBitmap(texRestartLevel.Get()));
					//Setting Panel's Children

						//Audio Option Rectangle 
						Draw::Instance().DrawRectangleToBitmap(
							0, 0, 256.0f, 256.0f,
							tempColor,
							Draw::Instance().GetBitmap(texAudio.Get()));
						//Audio Option Text 
						Draw::Instance().DrawTextToBitmap(
							0, 0, 256.0f, 256.0f,
							mainFont, L"Does Nothing Atm",
							Draw::Instance().GetBitmap(texAudio.Get()));
						
						//Back Option Rectangle 
						Draw::Instance().DrawRectangleToBitmap(
							0, 0, 256.0f, 256.0f,
							tempColor,
							Draw::Instance().GetBitmap(texMisc.Get()));
						//Back Option Text
						Draw::Instance().DrawTextToBitmap(
							0, 0, 256.0f, 256.0f,
							mainFont, L"Back",
							Draw::Instance().GetBitmap(texMisc.Get()));

				if (VRInputManager::GetInstance().GetController(eControllerType_Primary).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Trigger))
				{
					if (RaycastToMenu(&pResume, &mcResume,true)) {
						mActiveMenu = RESUME;
						SwitchPanel(&mActiveMenu);
					} else if (RaycastToMenu(&pSettings, &mcSettings, true)) {
						mActiveMenu = SETTINGS;
						SwitchPanel(&mActiveMenu);
					} else if (RaycastToMenu(&pHubworld, &mcHubworld, true)) {
						mActiveMenu = HUBWORLD;
						SwitchPanel(&mActiveMenu);
					} else if (RaycastToMenu(&pRestartLevel, &mcRestartLevel, true)) {
						mActiveMenu = RESTART;
						SwitchPanel(&mActiveMenu);
					} else if (RaycastToMenu(&pAudio, &mcAudio, true)) {

					} else if (RaycastToMenu(&pMisc, &mcMisc, true)) {
						mActiveMenu = MAIN_MENU;
						SwitchPanel(&mActiveMenu);
					}
				}
				if (VRInputManager::GetInstance().GetController(eControllerType_Secondary).GetPressDown(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) 
				{
					if (RaycastToMenu(&pResume, &mcResume,false)) {
						mActiveMenu = RESUME;
						SwitchPanel(&mActiveMenu);
					} else if (RaycastToMenu(&pSettings, &mcSettings, false)) {
						mActiveMenu = SETTINGS;
						SwitchPanel(&mActiveMenu);
					}
					else if (RaycastToMenu(&pHubworld, &mcHubworld, false)) {
						mActiveMenu = HUBWORLD;
						SwitchPanel(&mActiveMenu);
					} else if (RaycastToMenu(&pRestartLevel, &mcRestartLevel, false)) {
						mActiveMenu = RESTART;
						SwitchPanel(&mActiveMenu);
					} else if (RaycastToMenu(&pAudio, &mcAudio, false)) {

					} else if (RaycastToMenu(&pMisc, &mcMisc, false)) {
						mActiveMenu = MAIN_MENU;
						SwitchPanel(&mActiveMenu);
					}
				}
				if (scaleY < 18.0f) {
					scaleX = 0.1f;
					scaleY += 2.0f;
				}
				else if (scaleX < 20.0f)
					scaleX += 1.0f;
			}
			else if(!doneBeingOff)
			{
				if (scaleX > 0.1f){
					scaleX -= 1.0f;
					if (scaleX < 0.1f)
						scaleX = 0.1f;
				}
				else if (scaleY > 0.0f) {
					scaleY -= 2.0f;
				}
				else if (scaleY <= 0.0f) {
					scaleX = 0.0f;
					//scaleY = 0.0f;
					OnDisable();
					doneBeingOff = true;
				}
			}
		}
		virtual void OnEnable()
		{
			//disable pointy bits
			((MeshComponent*)LevelManager::GetInstance().GetCurrentLevel()->GetRightController()->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(true);
			((MeshComponent*)LevelManager::GetInstance().GetCurrentLevel()->GetLeftController()->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(true);

			mActiveMenu = PAUSEMENU_ON;
			SwitchPanel(&mActiveMenu);
			PauseMenuisUp = true;
			Settings::GetInstance().SetBool("PauseMenuUp", PauseMenuisUp);
			Transform tempT;
			if (scaleY <= 0.0f)
			{
				scaleY = 0.0f;
				scaleX = 0.0f;
				playerPos = VRInputManager::GetInstance().GetPlayerView();
				playerRot = vec4f(0, 0, 1.5f, 0) *  playerPos;
			}
			
			

			tempT.SetMatrix(playerPos.CreateNewXRotation(1.5708f) * playerPos.CreateNewScale(scaleX,scaleY,1) * (playerPos) * playerPos.CreateNewTranslation(playerRot));// * playerPos.CreateNewTranslation(0, 5.0f, 5.0f)));// *playerPos.CreateNewTranslation(playerRot));
			pPauseMenuBase->SetTransform(tempT);

			tempT.SetMatrix(playerPos.CreateNewScale(0.85f, 1, 0.85f) * playerPos.CreateNewTranslation(0, 0.001f, 0));
			pMainPanel->SetTransform(tempT);
			tempT.SetMatrix(playerPos.CreateNewScale(0.85f, 1, 0.85f) * playerPos.CreateNewTranslation(0, 0.001f, 0));
			pSettingsPanel->SetTransform(tempT);

			tempT.SetMatrix(playerPos.CreateNewScale(0.3f, 1, 0.3f) * playerPos.CreateNewTranslation(-0.0075f, 0.001f, -0.0075f));
			pResume->SetTransform(tempT);
			tempT.SetMatrix(playerPos.CreateNewScale(0.3f, 1, 0.3f) * playerPos.CreateNewTranslation(0.0075f, 0.001f, -0.0075f));
			pSettings->SetTransform(tempT);
			tempT.SetMatrix(playerPos.CreateNewScale(0.3f, 1, 0.3f) * playerPos.CreateNewTranslation(-0.0075f, 0.001f, 0.0075f));
			pHubworld->SetTransform(tempT);			
			tempT.SetMatrix(playerPos.CreateNewScale(0.3f, 1, 0.3f) * playerPos.CreateNewTranslation(0.0075f, 0.001f, 0.0075f));
			pRestartLevel->SetTransform(tempT);

			tempT.SetMatrix(playerPos.CreateNewScale(0.4f, 1, 0.2f) * playerPos.CreateNewTranslation(0, 0.001f, 0));
			pAudio->SetTransform(tempT);
			tempT.SetMatrix(playerPos.CreateNewScale(0.3f, 1, 0.3f) * playerPos.CreateNewTranslation(0.015f, 0.001f, 0.015f));
			pMisc->SetTransform(tempT);
			((MeshComponent*)cLevel->GetLeftController()->GetComponentIndexed(Epoch::eCOMPONENT_MESH, 0))->SetTopmost(true);
			((MeshComponent*)cLevel->GetRightController()->GetComponentIndexed(Epoch::eCOMPONENT_MESH, 0))->SetTopmost(true);
			((MeshComponent*)cLevel->GetLeftController()->GetComponentIndexed(Epoch::eCOMPONENT_MESH, 1))->SetTopmost(true);
			((MeshComponent*)cLevel->GetRightController()->GetComponentIndexed(Epoch::eCOMPONENT_MESH, 1))->SetTopmost(true);
		}
		virtual void OnDisable()
		{
			if (scaleY <= 0.0f)
			{
				mActiveMenu = PAUSEMENU_OFF;
				SwitchPanel(&mActiveMenu);
			}
			PauseMenuisUp = false;
			Settings::GetInstance().SetBool("PauseMenuUp", PauseMenuisUp);
			((MeshComponent*)cLevel->GetLeftController()->GetComponentIndexed(Epoch::eCOMPONENT_MESH, 0))->SetTopmost(false);
			((MeshComponent*)cLevel->GetRightController()->GetComponentIndexed(Epoch::eCOMPONENT_MESH, 0))->SetTopmost(false);
			((MeshComponent*)cLevel->GetLeftController()->GetComponentIndexed(Epoch::eCOMPONENT_MESH, 1))->SetTopmost(false);
			((MeshComponent*)cLevel->GetRightController()->GetComponentIndexed(Epoch::eCOMPONENT_MESH, 1))->SetTopmost(false);

			//disable pointy bits
			((MeshComponent*)LevelManager::GetInstance().GetCurrentLevel()->GetRightController()->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
			((MeshComponent*)LevelManager::GetInstance().GetCurrentLevel()->GetLeftController()->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
		}
		void SwitchPanel(MENU_NAME* _activemenu)
		{
			switch (*_activemenu)
			{
			case PAUSEMENU_ON:
				{
					mcPauseMenuBase->SetVisible(true);
					*_activemenu = MAIN_MENU;
				}
			case MAIN_MENU:
				{
				SetVisiblity(&pMainPanel, true);
				SetVisiblity(&pSettingsPanel, false);
				}
				break;
			case RESUME:
				{
					OnDisable();
				}
				break;
			case SETTINGS:
				{
					SetVisiblity(&pMainPanel, false);
					SetVisiblity(&pSettingsPanel, true);
				}
				break;
			case HUBWORLD:
				{
				//disable pointy bits
				((MeshComponent*)LevelManager::GetInstance().GetCurrentLevel()->GetRightController()->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
				((MeshComponent*)LevelManager::GetInstance().GetCurrentLevel()->GetLeftController()->GetComponentIndexed(eCOMPONENT_MESH, 1))->SetVisible(false);
					LevelManager::GetInstance().GetCurrentLevel()->LoadLevelCmnd(nullptr, L"HUB");
					CommandConsole::Instance().Toggle();
				}
				break;
			case RESTART:
				{
					OnDisable();
					TimeManager::Instance()->ResetTimeLineandLevel();
				}
				break;
			case AUDIO:
				{
					
				}
				break;
			//case MISC:
			//	{
			//		
			//	}
			//	break;
			case PAUSEMENU_OFF:
				{
					SetVisiblity(&pPauseMenuBase, false);
				}
			break;
			default:
				{
					
				}
				break;
			}
		}
		void SetUpThisObjectForMe(BaseObject** _obj, MeshComponent** _mc, std::string _name,Transform _t,unsigned int _width = 256, unsigned int _height = 256)
		{
			*_obj = Pool::Instance()->iGetObject()->Reset(_name, _t);
			*_mc = new MeshComponent("../Resources/UIClone.obj");
			Microsoft::WRL::ComPtr<ID3D11Texture2D> temp = GetTexture(*_obj, _width, _height);
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
			TextureManager::Instance()->iAddTexture2D(_name, temp, &srv);
			(*_mc)->AddTexture(_name.c_str(), eTEX_DIFFUSE);
			//GetTexture(*_obj, _width, _height);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(*_obj);
		}
		Microsoft::WRL::ComPtr<ID3D11Texture2D> GetTexture(BaseObject* _obj,unsigned int _width,unsigned int _height)
		{
			Microsoft::WRL::ComPtr<ID3D11Texture2D> screenTex;
			D3D11_TEXTURE2D_DESC bitmapDesc = GimmeDatShitmapDescBoi(_width, _height);
			HRESULT hr = Renderer::Instance()->GetDevice()->CreateTexture2D(&bitmapDesc, nullptr, screenTex.GetAddressOf());
			return screenTex;
		}
		D3D11_TEXTURE2D_DESC GimmeDatShitmapDescBoi(unsigned int _width,unsigned int _height)
		{
			D3D11_TEXTURE2D_DESC bitmapDesc;
			bitmapDesc.Width = _width;
			bitmapDesc.Height = _height;
			bitmapDesc.MipLevels = 1;
			bitmapDesc.ArraySize = 1;
			bitmapDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			bitmapDesc.SampleDesc.Count = 1;
			bitmapDesc.SampleDesc.Quality = 0;
			bitmapDesc.Usage = D3D11_USAGE_DEFAULT;
			bitmapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			bitmapDesc.CPUAccessFlags = 0;
			bitmapDesc.MiscFlags = 0;
			return bitmapDesc;
		}
		void SetVisiblity(BaseObject** _obj, bool _isVisible)
		{
			for(auto it : (*_obj)->GetChildren())
			{
				BaseObject* temp = (&*it);
				SetVisiblity(&temp, _isVisible);
				//delete temp;
			}
			std::vector<Component*> CodeComps = (*_obj)->GetComponents(Epoch::ComponentType::eCOMPONENT_MESH);
			if (CodeComps.size() > 0) {
				for (size_t x = 0; x < CodeComps.size(); ++x)
				{
					if (dynamic_cast<MeshComponent*>(CodeComps[x])) {
						((MeshComponent*)CodeComps[x])->SetVisible(_isVisible);
						break;
					}
				}
			}
			CodeComps.clear();
		}

		bool RaycastToMenu(BaseObject** _obj, MeshComponent** _mc, bool _isLeft) {
			if (!(*_mc)->IsVisible())
				return false;
			float meshTime = FLT_MAX;
			Triangle* tris = (*_mc)->GetTriangles();
			size_t numTris = (*_mc)->GetTriangleCount();
			matrix4 objMat = (*_obj)->GetWorld();
			if(_isLeft)
			{
				vec4f forwardP;
				forwardP.Set(0, 0, 1, 0);
				matrix4 mat = (VRInputManager::GetInstance().GetController(eControllerType_Primary).GetPosition());
				matrix4 inverseP = (mat * objMat.Invert());
				vec3f meshPosP = inverseP.Position;
				forwardP *= inverseP;
				vec3f fwdP(forwardP);
				for (unsigned int i = 0; i < numTris; ++i) {
						if (Physics::Instance()->RayToTriangle(
							(tris + i)->Vertex[0],
							(tris + i)->Vertex[1],
							(tris + i)->Vertex[2],
							(tris + i)->Normal, meshPosP, fwdP, meshTime))
							return true;
				}
			}
			else 
			{
				vec4f forwardS;
				forwardS.Set(0, 0, 1, 0);
				matrix4 mat = (VRInputManager::GetInstance().GetController(eControllerType_Secondary).GetPosition());
				matrix4 inverseS = (mat * objMat.Invert());
				vec3f meshPosS = inverseS.Position;
				forwardS *= inverseS;
				vec3f fwdS(forwardS);
				for (unsigned int i = 0; i < numTris; ++i) {
					if (Physics::Instance()->RayToTriangle(
						(tris + i)->Vertex[0],
						(tris + i)->Vertex[1],
						(tris + i)->Vertex[2],
						(tris + i)->Normal, meshPosS, fwdS, meshTime))
						return true;
				}
			}
			return false;
		}
		//static void Test1(void* _self, std::wstring _ifOn)
		//{
		//	if (_ifOn == L"ON")
		//		test1 = true;
		//	else if (_ifOn == L"OFF")
		//		test1 = false;
		//}
	};
}