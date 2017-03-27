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
		virtual void Update()
		{
			if (GetAsyncKeyState(VK_SHIFT) & 1)
			{
				Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;

				TextureManager::Instance()->iGetTexture2D("memory:Clone Display", nullptr, &tex);

				Font* font = new Font();
				font->mFontName = L"Comic Sans MS";
				font->mFontSize = 100;
				Draw::Instance().DrawTextToBitmap(0, 0, 256, 256, *font, L"0", Draw::Instance().GetBitmap(tex.Get()));
			}
		}
	};

}