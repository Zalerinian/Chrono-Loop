#pragma once
#include "Actions/CodeComponent.hpp"
#include "Objects/MeshComponent.h"
#include "Objects/TransparentMeshComponent.h"
#include "Objects/BaseObject.h"
#include "Core/LevelManager.h"
#include "Core/Pool.h"
#include "Rendering/Draw2D.h"
#include "Input/VRInputManager.h"
//vrim->inst->Getpos
#include <list>


namespace Epoch
{
	//struct PauseMenuOption
	//{
	//	BaseObject *mCur;
	//	MENU_NAME mMenuName;
	//	PauseMenuOption()
	//	{
	//		mCur = nullptr;
	//		mMenuName = NOTHING;
	//	}
	//	PauseMenuOption(BaseObject* _cur, MENU_NAME _mn)
	//	{
	//		mCur = _cur;
	//		mMenuName = _mn;
	//	}
	//};
	enum MENU_NAME
	{
		NOTHING = -1,
		PAUSEMENU_ON,
		PAUSEMENU_OFF,
		MAIN_MENU,
		RESUME,
		SETTINGS,
		HUBWORLD,
		AUDIO,
		MISC,
		MAX
	};
	struct ActivePanel
	{
		std::list<BaseObject*> mCurOptions;
		MENU_NAME mCurMenu;
		ActivePanel() {}
		ActivePanel(std::list<BaseObject*> _options)
		{
			SetOptions(_options);
			mCurMenu = PAUSEMENU_ON;
		}
		void SetOptions(std::list<BaseObject*> _set)
		{
			mCurOptions.clear();
			for (auto iter = _set.begin(); iter != _set.end(); iter++)
				mCurOptions.push_back((*iter));
		}
		void SetCurrentMenu(MENU_NAME _set) {mCurMenu = _set;}
	};
	struct PauseMenu : public CodeComponent
	{
	private:

		std::vector<BaseObject*> mAllMenuOptions;
		ActivePanel mActivePanel;
		bool PauseMenuisUp = false;
		Transform identity;

		BaseObject *pPauseMenuBase = nullptr;
		MeshComponent *mcPauseMenuBase = nullptr;

		BaseObject *pMainPanel = nullptr, *pSettingsPanel = nullptr; //Children of the Entire Pause Menu
		MeshComponent *mcMainPanel = nullptr, *mcSettingsPanel = nullptr;

		BaseObject *pResume = nullptr, *pSettings = nullptr, *pHubworld = nullptr, *pAudio = nullptr, *pMisc = nullptr;//Children of Panels
		MeshComponent *mcResume = nullptr, *mcSettings = nullptr, *mcHubworld = nullptr, *mcAudio = nullptr, *mcMisc = nullptr;

		std::unordered_map<MENU_NAME, MeshComponent*> mMeshComps;
		std::unordered_map<BaseObject*, ID3D11Texture2D*> mPauseMenuTextures;
		D2D1::ColorF wut = { 1,0,0,1 };
		Font* mainFont = new Font(L"Agency FB", 50, wut);
		float transparentColor[4] = { 0,0,0,0 };
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texPauseMenuBase, texMainPanel,texSettingsPanel, texResume,texSettings,texHubworld,texAudio,texMisc;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtvPauseMenuBase, rtvMainPanel, rtvSettingsPanel, rtvResume, rtvSettings, rtvHubworld, rtvAudio, rtvMisc;
	public:
		//Accessors
		ActivePanel* GetActivePanel() { return &mActivePanel; }
		bool isPauseMenuOn() { return PauseMenuisUp; }

		//Mutators
		void SetPauseMenuToOn(bool _set) { PauseMenuisUp = _set; }

		//
		virtual void Start()
		{
			
			//Pause Menu Base Initialize
			SetUpThisObjectForMe(&pPauseMenuBase, (MeshComponent**)&mcPauseMenuBase, std::string("PauseMenu - Base"), (identity));

			pPauseMenuBase->AddComponent(mcPauseMenuBase);
			TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Base", nullptr, &texPauseMenuBase);

			mMeshComps.insert({ PAUSEMENU_ON,mcPauseMenuBase });

			//PANEL SET UP
				//Main Pause Menu Panel Initialize
				//identity.SetMatrix(matrix4::CreateTranslation(0, 0, 0.9f));
				SetUpThisObjectForMe(&pMainPanel, &mcMainPanel, std::string("PauseMenu - Main Menu"), identity);
				TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Main Menu", nullptr, &texMainPanel);

				pMainPanel->AddComponent(mcMainPanel);
				pMainPanel->SetParent(pPauseMenuBase);
				mMeshComps.insert({ MAIN_MENU,mcMainPanel });

				//Settings Panel Initialize
				SetUpThisObjectForMe(&pSettingsPanel, &mcSettingsPanel, std::string("PauseMenu - Settings Panel"), identity);
				TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Settings Panel", nullptr, &texSettingsPanel);

				pSettingsPanel->AddComponent(mcSettingsPanel);
				pSettingsPanel->SetParent(pPauseMenuBase);
				mMeshComps.insert({ SETTINGS,mcSettingsPanel });

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
					mMeshComps.insert({ MAIN_MENU,mcResume });

					//Settings Option Initialize
					SetUpThisObjectForMe(&pSettings, &mcSettings, std::string("PauseMenu - Settings Option"), identity);
					TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Settings Option", nullptr, &texSettings);

					pSettings->AddComponent(mcSettings);
					pSettings->SetParent(pMainPanel);
					mMeshComps.insert({ MAIN_MENU,mcSettings });

					//Hubworld Option Initialize
					SetUpThisObjectForMe(&pHubworld, &mcHubworld, std::string("PauseMenu - Hubworld Option"), identity);
					TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Hubworld Option", nullptr, &texHubworld);

					pHubworld->AddComponent(mcHubworld);
					pHubworld->SetParent(pMainPanel);

					mMeshComps.insert({ MAIN_MENU,mcHubworld });

					//Setting Children for Main Panel
					pMainPanel->AddChild(pResume);
					pMainPanel->AddChild(pSettings);
					pMainPanel->AddChild(pHubworld);
				//Settings Panel Options
					//Audio Option Initialize
					//identity.SetMatrix(matrix4::CreateTranslation(0, 0, 0.8f));
					SetUpThisObjectForMe(&pAudio, &mcAudio, std::string("PauseMenu - Audio Option"), identity);
					TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Audio Option", nullptr, &texAudio);

					pAudio->AddComponent(mcAudio);
					pAudio->SetParent(pSettingsPanel);
					mMeshComps.insert({ SETTINGS,mcAudio });


					//Misc Option Initialize
					//identity.SetMatrix(matrix4::CreateTranslation(0, 0, 0.8f));
					SetUpThisObjectForMe(&pMisc, &mcMisc, std::string("PauseMenu - Misc Option"), identity);
					TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Misc Option", nullptr, &texMisc);

					pMisc->AddComponent(mcMisc);
					pMisc->SetParent(pSettingsPanel);
					mMeshComps.insert({ SETTINGS,mcMisc});


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
					Renderer::Instance()->GetDevice()->CreateRenderTargetView((ID3D11Resource*)texAudio.Get(), NULL, rtvAudio.GetAddressOf());
					Renderer::Instance()->GetDevice()->CreateRenderTargetView((ID3D11Resource*)texMisc.Get(), NULL, rtvMisc.GetAddressOf());

			//Active Panel Start Up
				mActivePanel.SetOptions(pMainPanel->GetChildren());
				mActivePanel.SetCurrentMenu(PAUSEMENU_ON);
			//Panel Start Up
				SwitchPanel(&mActivePanel);
				//OnDisable();  
			OnEnable();
			
		}
		virtual void Update()
		{
	
			Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvPauseMenuBase.Get(), transparentColor);
			Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvMainPanel.Get(), transparentColor);
			Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvSettingsPanel.Get(), transparentColor);
			Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvResume.Get(), transparentColor);
			Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvSettings.Get(), transparentColor);
			//Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvHubworld.Get(), transparentColor);
			//Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvAudio.Get(), transparentColor);
			//Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvMisc.Get(), transparentColor);

			D2D1::ColorF tempColor = { 0,0,1,0.5f };
			Draw::Instance().DrawRectangleToBitmap(
				0, 0, 256.0f,256.0f, 
				tempColor,
				Draw::Instance().GetBitmap(texPauseMenuBase.Get()));
			//Draw::Instance().DrawTextToBitmap(
			//	0, 0, 256.0f, 256.0f,
			//	*mainFont, L"Fuck you",
			//	Draw::Instance().GetBitmap(texPauseMenuBase.Get()));

			Draw::Instance().DrawRectangleToBitmap(
				0, 0, 256.0f, 256.0f,
				(D2D1::ColorF::Black, 1.0f),
				Draw::Instance().GetBitmap(texMainPanel.Get()));

			//Draw::Instance().DrawRectangleToBitmap(
			//	0, 0, 256.0f, 256.0f,
			//	(D2D1::ColorF::Black, 0.8f),
			//	Draw::Instance().GetBitmap(texSettingsPanel.Get()));

			Draw::Instance().DrawTextToBitmap(
				0, 0, 256.0f, 256.0f,
				*mainFont, L"Resume",
				Draw::Instance().GetBitmap(texResume.Get()));
			tempColor = { 0,1,0,0.5f };

			Draw::Instance().DrawRectangleToBitmap(
				0, 0, 256.0f, 256.0f,
				tempColor,
				Draw::Instance().GetBitmap(texResume.Get()));
			//Draw::Instance().DrawTextToBitmap(
			//	0, 0, 256.0f, 256.0f,
			//	*mainFont, L"Settings",
			//	Draw::Instance().GetBitmap(texSettings.Get()));

			//Draw::Instance().DrawTextToBitmap(
			//	0, 0, 256.0f, 256.0f,
			//	*mainFont, L"Hubworld",
			//	Draw::Instance().GetBitmap(texHubworld.Get()));

			//Draw::Instance().DrawTextToBitmap(
			//	0, 0, 256.0f, 256.0f,
			//	*mainFont, L"Audio",
			//	Draw::Instance().GetBitmap(texAudio.Get()));

			//Draw::Instance().DrawTextToBitmap(
			//	0, 0, 256.0f, 256.0f,
			//	*mainFont, L"Misc",
			//	Draw::Instance().GetBitmap(texMisc.Get()));
			//OnEnable();
			//if (PauseMenuisUp) {
			//	PauseMenuisUp = false;
			//	OnDisable();
			//}
			//else {
			//	PauseMenuisUp = true;
			//	OnEnable();
			//}
		}
		virtual void OnEnable()
		{
			mActivePanel.SetOptions(pMainPanel->GetChildren());
			mActivePanel.SetCurrentMenu(PAUSEMENU_ON);
			SwitchPanel(&mActivePanel);
			PauseMenuisUp = true;
			Transform tempT;
			matrix4 playerPos = VRInputManager::GetInstance().GetPlayerView();
			vec4f playerRot = vec4f(0, 0, 1, 0) *  playerPos;
			
			tempT.SetMatrix(playerPos.CreateXRotation(1.39626) * playerPos.CreateScale(20,20,20) * (playerPos) * playerPos.CreateTranslation(playerRot));// * playerPos.CreateTranslation(0, 5.0f, 5.0f)));// *playerPos.CreateTranslation(playerRot));
			pPauseMenuBase->SetTransform(tempT);
			tempT.SetMatrix(playerPos.CreateScale(0.85f, 0.85f, 0.85f) * playerPos.CreateTranslation(0, 0.001f, 0));
			pMainPanel->SetTransform(tempT);
			tempT.SetMatrix(playerPos.CreateScale(0.85f, 0.85f, 0.85f) * playerPos.CreateTranslation(0, 0.001f, 0));
			pSettingsPanel->SetTransform(tempT);
			tempT.SetMatrix(playerPos.CreateScale(0.85f, 0.85f, 0.85f) * playerPos.CreateTranslation(0, 0.001f, 0));
			pResume->SetTransform(tempT);
		}
		virtual void OnDisable()
		{
			mActivePanel.SetCurrentMenu(PAUSEMENU_OFF);
			SwitchPanel(&mActivePanel);
			PauseMenuisUp = false;
		}
		void SwitchPanel(ActivePanel* _activepanel)
		{
			switch (_activepanel->mCurMenu)
			{
			case PAUSEMENU_ON:
				{
					mcPauseMenuBase->SetVisible(true);
					_activepanel->SetCurrentMenu(MAIN_MENU);
				}
				break;
			case MAIN_MENU:
				{
				SetVisiblity(&pMainPanel, true);
				SetVisiblity(&pSettingsPanel, false);
					/*for (auto it : mMeshComps)
					{
						if (it.first == MAIN_MENU)
							it.second->SetVisible(true);
						else
							it.second->SetVisible(false);
					}*/
				}
				break;
			case RESUME:
				{

				}
				break;
			case SETTINGS:
				{
				SetVisiblity(&pMainPanel, false);
				SetVisiblity(&pSettingsPanel, true);
					//for (auto it : mMeshComps)
					//{
					//	if (it.first == SETTINGS)
					//		it.second->SetVisible(true);
					//	else
					//		it.second->SetVisible(false);
					//}
				}
				break;
			case HUBWORLD:
				{

				}
				break;
			case AUDIO:
				{

				}
				break;
			case MISC:
				{

				}
				break;
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
			GetTexture(*_obj, _width, _height);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(*_obj);
		}
		Microsoft::WRL::ComPtr<ID3D11Texture2D> GetTexture(BaseObject* _obj,unsigned int _width,unsigned int _height)
		{
			for (auto it : mPauseMenuTextures)
			{
				if (it.first == _obj)
					return it.second;
			}
			Microsoft::WRL::ComPtr<ID3D11Texture2D> screenTex;
			
			D3D11_TEXTURE2D_DESC bitmapDesc = GimmeDatShitmapDescBoi(_width, _height);
			HRESULT hr = Renderer::Instance()->GetDevice()->CreateTexture2D(&bitmapDesc, nullptr, screenTex.GetAddressOf());
			mPauseMenuTextures.insert({ _obj,screenTex.Get() });
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
		}
	};
}