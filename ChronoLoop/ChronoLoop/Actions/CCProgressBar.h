#pragma once
#include "..\Actions\CodeComponent.hpp"
#include "..\Objects\MeshComponent.h"
#include "..\Objects\BaseObject.h"
#include "..\Rendering\Draw2D.h"
#include "..\Core\Pool.h"
#include "..\Core\LevelManager.h"

namespace Epoch
{
	struct CCProgressBar : public CodeComponent
	{
		BaseObject* pBackground, *pProgressBar;
		MeshComponent* mcBackground, *mcProgressBar;
		float tempProgress,curProgress, finalProgress;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texBackground, texProgressBar;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtvBackground, rtvProgressBar;
		Transform identity;
		float transparentColor[4] = { 0,0,0,0 };

		CCProgressBar()
		{
			SetUpThisObjectForMe(&pBackground, (MeshComponent**)&mcBackground, std::string("Progress Bar - Base"), identity);
			TextureManager::Instance()->iGetTexture2D("memory:Progress Bar - Base", nullptr, &texBackground);
			pBackground->AddComponent(mcBackground);

			SetUpThisObjectForMe(&pProgressBar, (MeshComponent**)&mcProgressBar, std::string("Progress Bar - Loading Bar"), identity);
			TextureManager::Instance()->iGetTexture2D("memory:Progress Bar - Loading Bar", nullptr, &texProgressBar);
			pProgressBar->AddComponent(mcProgressBar);

			pBackground->AddChild(pProgressBar);
			pProgressBar->SetParent(pBackground);

			pBackground->GetTransform().SetMatrix(matrix4::CreateNewScale(20, 1, 4));
			pProgressBar->GetTransform().SetMatrix(matrix4::CreateNewTranslation(0, 0.01f, 0));

			Renderer::Instance()->GetDevice()->CreateRenderTargetView((ID3D11Resource*)texBackground.Get(), NULL, rtvBackground.GetAddressOf());
			Renderer::Instance()->GetDevice()->CreateRenderTargetView((ID3D11Resource*)texProgressBar.Get(), NULL, rtvProgressBar.GetAddressOf());

		}
		virtual void Start()
		{
			mcBackground->SetPixelShader(ePS_PURETEXTURE);
			mcProgressBar->SetPixelShader(ePS_PURETEXTURE);
		}
		virtual void Update()
		{

			if (curProgress != tempProgress) 
			{
				Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvProgressBar.Get(), transparentColor);
				if (curProgress < finalProgress - (finalProgress / 20)) {
					pProgressBar->GetTransform().SetMatrix(matrix4::CreateNewScale((curProgress / finalProgress) * 0.85f, 1, 0.85f) * matrix4::CreateNewTranslation(0, 0.001f, 0));
					tempProgress = curProgress;
					D2D1::ColorF tempColor = { 1,0,0,1 };
					Draw::Instance().DrawRectangleToBitmap(
						0, 0, 256.0f, 256.0f, tempColor,
						Draw::Instance().GetBitmap(texProgressBar.Get()));
				}
				else
				{
					D2D1::ColorF tempColor = { 0,1,0,1 };
					Draw::Instance().DrawRectangleToBitmap(
						0, 0, 256.0f, 256.0f, tempColor,
						Draw::Instance().GetBitmap(texProgressBar.Get()));
				}
			}
		}
		virtual void OnEnable()
		{
			mcBackground->SetVisible(true);
			mcProgressBar->SetVisible(true);
			Renderer::Instance()->GetContext()->ClearRenderTargetView(rtvBackground.Get(), transparentColor);
			Draw::Instance().DrawRectangleToBitmap(
				0, 0, 256.0f, 256.0f,
				(D2D1::ColorF::Black, (UINT32)1.0f),
				Draw::Instance().GetBitmap(texBackground.Get()));
		}
		virtual void OnDisable()
		{
			mcBackground->SetVisible(false);
			mcProgressBar->SetVisible(false);
		}
		virtual void OnDestroy()
		{
			rtvBackground.Reset();
			rtvProgressBar.Reset();
			//(rtvBackground)->Release();
		}
		void SetCurProgress(float _set) { curProgress = _set; }
		float GetCurProgress() { return curProgress; }
		void SetFinalProgress(float _set) { finalProgress = _set; }
		float GetFinalProgress() { return finalProgress; }

		BaseObject* GetBackground() { return pBackground; }
		BaseObject* GetProgressBar() { return pProgressBar; }
		
		//Just a helper fucntion, plz don't use 
		void SetUpThisObjectForMe(BaseObject** _obj, MeshComponent** _mc, std::string _name, Transform _t, unsigned int _width = 256, unsigned int _height = 256)
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
		//Just a helper fucntion, plz don't use
		Microsoft::WRL::ComPtr<ID3D11Texture2D> GetTexture(BaseObject* _obj, unsigned int _width, unsigned int _height)
		{
			Microsoft::WRL::ComPtr<ID3D11Texture2D> screenTex;
			D3D11_TEXTURE2D_DESC bitmapDesc = GimmeDatShitmapDescBoi(_width, _height);
			HRESULT hr = Renderer::Instance()->GetDevice()->CreateTexture2D(&bitmapDesc, nullptr, screenTex.GetAddressOf());
			return screenTex;
		}
		//Just a helper fucntion, plz don't use
		D3D11_TEXTURE2D_DESC GimmeDatShitmapDescBoi(unsigned int _width, unsigned int _height)
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
	};
}