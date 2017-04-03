#pragma once
#include "Actions/CodeComponent.hpp"
#include "Objects/MeshComponent.h"
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
		std::unordered_map<MENU_NAME, BaseObject*> mPanels;
		MeshComponent *mcMainPanel = nullptr, *mcSettingsPanel = nullptr;


		BaseObject *pResume = nullptr, *pSettings = nullptr, *pHubworld = nullptr, *pAudio = nullptr, *pMisc = nullptr;//Children of Panels
		MeshComponent *mcResume = nullptr, *mcSettings = nullptr, *mcHubworld = nullptr, *mcAudio = nullptr, *mcMisc = nullptr;

		std::unordered_map<MENU_NAME, MeshComponent*> mMeshComps;
		std::unordered_map<BaseObject*, ID3D11Texture2D*> mPauseMenuTextures;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texPauseMenuBase, texMainPanel,texSettingsPanel, texResume,texSettings,texHubworld,texAudio,texMisc;

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
			SetUpThisObjectForMe(&pPauseMenuBase, &mcPauseMenuBase, std::string("PauseMenu - Base"),identity);

			#pragma region Commented Out
			//pPauseMenuBase = Pool::Instance()->iGetObject()->Reset("PauseMenu - Base", identity);
			//mcPauseMenuBase = new MeshComponent("../Resources/help.obj");

			//std::string tempStr1("PauseMenu - Base");
			//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv1;
			//TextureManager::Instance()->iAddTexture2D(tempStr1, GetTexture(pPauseMenuBase), &srv1);

			//mcPauseMenuBase->AddTexture(tempStr1.c_str(), eTEX_DIFFUSE);
			//mcPauseMenuBase->GetContext().mTextures[eTEX_DIFFUSE] = srv1;
			#pragma endregion
			Font* tempFont = new Font(L"Agency FB", 50, (D2D1::ColorF::White, 1.0f));
			pPauseMenuBase->AddComponent(mcPauseMenuBase);
			TextureManager::Instance()->iGetTexture2D("memory:PauseMenu - Base", nullptr, &texPauseMenuBase);
			//Draw::Instance().DrawRectangleToBitmap(
			//	0, 0, 256.0f,256.0f, 
			//	(D2D1::ColorF::Blue, 1.0f), 
			//	Draw::Instance().GetBitmap(texPauseMenuBase.Get()));
			Draw::Instance().DrawTextToBitmap(
				0, 0, 256, 256,
				*tempFont, L"Fuck you",
				Draw::Instance().GetBitmap(texPauseMenuBase.Get()));
			mMeshComps.insert({ PAUSEMENU_ON,mcPauseMenuBase });

			//PANEL SET UP
				//Main Pause Menu Panel Initialize
				SetUpThisObjectForMe(&pMainPanel, &mcMainPanel, std::string("PauseMenu - Main Menu"), identity);

				#pragma region Commented Out
			//pMainPanel = Pool::Instance()->iGetObject()->Reset("PauseMenu - Main Panel", identity);
			//mcMainPanel = new MeshComponent("../Resources/help.obj");

			//std::string tempStr2("PauseMenu - Main Panel");
			//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv2;
			//TextureManager::Instance()->iAddTexture2D(tempStr2, GetTexture(pMainPanel), &srv2);

			//mcMainPanel->AddTexture(tempStr2.c_str(), eTEX_DIFFUSE);
			//mcMainPanel->GetContext().mTextures[eTEX_DIFFUSE] = srv2;
			#pragma endregion

				pMainPanel->AddComponent(mcMainPanel);
				pMainPanel->SetParent(pPauseMenuBase);

				mPanels.insert({ MAIN_MENU, pMainPanel });
				mMeshComps.insert({ MAIN_MENU,mcMainPanel });

				Draw::Instance().DrawRectangleToBitmap(
					0, 0, 256.0f, 256.0f,
					(D2D1::ColorF::Black, 0.8f),
					Draw::Instance().GetBitmap(mPauseMenuTextures.at(pMainPanel)));

				//Settings Panel Initialize
				SetUpThisObjectForMe(&pSettingsPanel, &mcSettingsPanel, std::string("PauseMenu - Settings Panel"), identity);
				
				#pragma region Commented Out
			//pSettingsPanel = Pool::Instance()->iGetObject()->Reset("PauseMenu - Settings Panel", identity);
			//mcSettingsPanel = new MeshComponent("../Resources/help.obj");

			//std::string tempStr3("PauseMenu - Settings Panel");
			//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv3;
			//TextureManager::Instance()->iAddTexture2D(tempStr3, GetTexture(pSettingsPanel), &srv3);

			//mcSettingsPanel->AddTexture(tempStr3.c_str(), eTEX_DIFFUSE);
			//mcSettingsPanel->GetContext().mTextures[eTEX_DIFFUSE] = srv3;
			#pragma endregion

				pSettingsPanel->AddComponent(mcSettingsPanel);
				pSettingsPanel->SetParent(pPauseMenuBase);
				
				mPanels.insert({SETTINGS,pSettingsPanel });
				mMeshComps.insert({ SETTINGS,mcSettingsPanel });

				Draw::Instance().DrawRectangleToBitmap(
					0, 0, 256, 256,
					(D2D1::ColorF::Black, 0.8f),
					Draw::Instance().GetBitmap(mPauseMenuTextures.at(pSettingsPanel)));

				//Setting Children of Pause Menu Base
				pPauseMenuBase->AddChild(pMainPanel);
				pPauseMenuBase->AddChild(pSettingsPanel);
			//OPTIONS SET UP
				//Main Panel Options
					//Resume Option Initialize
					SetUpThisObjectForMe(&pResume, &mcResume, std::string("PauseMenu - Resume Option"), identity);

					pResume->AddComponent(mcResume);
					pResume->SetParent(pMainPanel);
					
					mMeshComps.insert({ MAIN_MENU,mcResume });
					//Font* tempFont = new Font(L"Agency FB", 50, (D2D1::ColorF::White, 1.0f));
					Draw::Instance().DrawTextToBitmap(
						0, 0, 256, 256,
						*tempFont, L"Resume",
						Draw::Instance().GetBitmap(mPauseMenuTextures.at(pResume)));
						
					//Settings Option Initialize
					SetUpThisObjectForMe(&pSettings, &mcSettings, std::string("PauseMenu - Settings Option"), identity);

					pSettings->AddComponent(mcSettings);
					pSettings->SetParent(pMainPanel);
					mMeshComps.insert({ MAIN_MENU,mcSettings });

					Draw::Instance().DrawTextToBitmap(
						0, 0, 256, 256,
						*tempFont, L"Settings",
						Draw::Instance().GetBitmap(mPauseMenuTextures.at(pSettings)));
					//Hubworld Option Initialize
					SetUpThisObjectForMe(&pHubworld, &mcHubworld, std::string("PauseMenu - Hubworld Option"), identity);

					pHubworld->AddComponent(mcHubworld);
					pHubworld->SetParent(pMainPanel);

					mMeshComps.insert({ MAIN_MENU,mcHubworld });
					Draw::Instance().DrawTextToBitmap(
						0, 0, 256, 256,
						*tempFont, L"Hubworld",
						Draw::Instance().GetBitmap(mPauseMenuTextures.at(pHubworld)));

					//Setting Children for Main Panel
					pMainPanel->AddChild(pResume);
					pMainPanel->AddChild(pSettings);
					pMainPanel->AddChild(pHubworld);
				//Settings Panel Options
					//Audio Option Initialize
					SetUpThisObjectForMe(&pAudio, &mcAudio, std::string("PauseMenu - Audio Option"), identity);

					pAudio->AddComponent(mcAudio);
					pAudio->SetParent(pSettingsPanel);
					mMeshComps.insert({ SETTINGS,mcAudio });

					Draw::Instance().DrawTextToBitmap(
						0, 0, 256, 256,
						*tempFont, L"Audio",
						Draw::Instance().GetBitmap(mPauseMenuTextures.at(pAudio)));
					//Misc Option Initialize
					SetUpThisObjectForMe(&pMisc, &mcMisc, std::string("PauseMenu - Misc Option"), identity);

					pMisc->AddComponent(mcMisc);
					pMisc->SetParent(pSettingsPanel);
					mMeshComps.insert({ SETTINGS,mcMisc});

					Draw::Instance().DrawTextToBitmap(
						0, 0, 256, 256,
						*tempFont, L"Misc",
						Draw::Instance().GetBitmap(mPauseMenuTextures.at(pMisc)));

					//Setting Children for Settings Panel
					pSettingsPanel->AddChild(pAudio);
					pSettingsPanel->AddChild(pMisc);

			//Active Panel Start Up
				mActivePanel.SetOptions(mPanels.at(MAIN_MENU)->GetChildren());
				mActivePanel.SetCurrentMenu(PAUSEMENU_ON);
			//Panel Start Up
				SwitchPanel(&mActivePanel);
				//OnDisable();  
			OnEnable();
			
		}
		virtual void Update()
		{
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
			mActivePanel.SetOptions(mPanels.at(MAIN_MENU)->GetChildren());
			mActivePanel.SetCurrentMenu(PAUSEMENU_ON);
			SwitchPanel(&mActivePanel);
			PauseMenuisUp = true;
			Transform tempT;
			matrix4 playerPos = VRInputManager::GetInstance().GetPlayerView();
			//vec4f playerRot = vec4f(0, 0, 1, 0) *  playerPos;
			
			tempT.SetMatrix(matrix4::CreateScale(10,10,10) * (playerPos));// * playerPos.CreateTranslation(0, 5.0f, 5.0f)));// *playerPos.CreateTranslation(playerRot));
			pPauseMenuBase->SetTransform(tempT);
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
				SetVisiblity(&mPanels.at(MAIN_MENU), true);
				SetVisiblity(&mPanels.at(SETTINGS), false);
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
				SetVisiblity(&mPanels.at(MAIN_MENU), false);
				SetVisiblity(&mPanels.at(SETTINGS), true);
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