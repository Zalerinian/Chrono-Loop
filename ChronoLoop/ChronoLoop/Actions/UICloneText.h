#pragma once
#include "CodeComponent.hpp"
#include "..\Rendering\Draw2D.h"
#include "..\Rendering\Renderer.h"
#include "..\Rendering\TextureManager.h"
#include <wrl\client.h>


namespace Epoch
{

	struct UICloneText : public CodeComponent
	{
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
		int prev = -1;

		virtual void Start()
		{
			desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;
			TextureManager::Instance()->iGetTexture2D("memory:Clone Display", nullptr, &tex);
			HRESULT hr = Renderer::Instance()->GetDevice()->CreateRenderTargetView(tex.Get(), &desc, rtv.GetAddressOf());
		}

		virtual void Update()
		{
			if (prev != LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->GetNumOfConfirmedClones())
			{
				prev = LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->GetNumOfConfirmedClones();
				
				float color[4] = { 0,0,0,0 };
				Renderer::Instance()->GetContext()->ClearRenderTargetView(rtv.Get(), color);

				Font* font = new Font();
				font->mFontName = L"Agency FB";
				font->mFontSize = 125;
				std::wstring num = std::to_wstring(prev) + L"/" + std::to_wstring(LevelManager::GetInstance().GetCurrentLevel()->GetMaxClones());
				Draw::Instance().DrawTextToBitmap(0, 0, 256, 256, *font, num, Draw::Instance().GetBitmap(tex.Get()));
			}
		}
	};

}