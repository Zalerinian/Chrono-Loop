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
		MeshComponent* mDisplayBack = nullptr, *mTextMesh = nullptr;
		PSAnimatedMultiscan_Data mScanlineData;

		virtual void Start()
		{
			mScanlineData.DiffuseAlpha = 0.9f;
			mScanlineData.MultiscanAlpha = 0.9f;
			mScanlineData.ScanlineAlpha = 0.9f;

			desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;
			TextureManager::Instance()->iGetTexture2D("memory:Clone Display", nullptr, &tex);
			HRESULT hr = Renderer::Instance()->GetDevice()->CreateRenderTargetView(tex.Get(), &desc, rtv.GetAddressOf());
			mDisplayBack = (MeshComponent*)LevelManager::GetInstance().GetCurrentLevel()->FindObjectWithName("cloneDisplayBack")->GetComponentIndexed(eCOMPONENT_MESH, 0);
			mTextMesh = (MeshComponent*)mObject->GetComponentIndexed(eCOMPONENT_MESH, 0);

			mDisplayBack->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			mDisplayBack->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			mDisplayBack->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			mDisplayBack->SetPixelShader(ePS_TRANSPARENT_SCANLINE);

			mTextMesh->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			mTextMesh->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			mTextMesh->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			mTextMesh->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			mTextMesh->SetBlended(true);
		}

		virtual void Update()
		{
			mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
			mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
			if (mScanlineData.ScanlineVOffset > 2.5f) {
				mScanlineData.ScanlineVOffset = -0.5f;
			}
			mDisplayBack->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);
			mTextMesh->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);
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