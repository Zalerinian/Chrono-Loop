#include "../../DXTK/DirectXTex.h"
#include "../../DXTK/ddstextureloader.h"
#include <d3d11.h>
#include <memory>
#include "FileIO_Textures.h"
#include "FileIO_General.h"
#include "../Common.h"
#include "../Logger.h"
#include "../../Rendering/Renderer.h"

using namespace std;
using namespace DirectX;

namespace Epoch
{
	namespace FileIO
	{

		bool LoadTexture2D(const char * _path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* _srv, Microsoft::WRL::ComPtr<ID3D11Texture2D>* _texture)
		{
			if (_srv == nullptr && _texture == nullptr)
			{
				return false;
			}
			wchar_t *buffer = nullptr;
			MakeWide(_path, &buffer, (unsigned int)strlen(_path) + 1);
			ScratchImage scratch;
			Blob imageBlob;
			HRESULT hr = LoadFromDDSFile(buffer, 0, nullptr, scratch);
			if (FAILED(hr))
			{
				if (hr == E_OUTOFMEMORY)
				{
					SystemLogger::GetError() << "[Critial] System ran out of memory attempting to allocate storage for texture \"" << _path << "\"!" << endl;
					delete[] buffer;
					if (_srv)
					{
						(*_srv) = nullptr;
					}
					if (_texture)
					{
						(*_texture) = nullptr;
					}
					return false;
				}
				//SystemLogger::GetError() << "[Warning] Requested texture \"" << _path << "\" could not be loaded as a DDS file: 0x" << hex << hr << dec << ". Attempting as WIC file..." << endl;
				TexMetadata tMeta;
				hr = LoadFromWICFile(buffer, WIC_FLAGS_IGNORE_SRGB, &tMeta, scratch);
				delete[] buffer;
				if (FAILED(hr))
				{
					SystemLogger::GetError() << "[Error] Could not load texture \"" << _path << "\": 0x" << hex << hr << dec << endl;
					if (_srv)
					{
						(*_srv) = nullptr;
					}
					if (_texture)
					{
						(*_texture) = nullptr;
					}
					return false;
				}
				ID3D11ShaderResourceView *srv;
				ID3D11Resource *texture;
				Renderer::Instance()->GetRendererLock().lock();
				hr = CreateShaderResourceView(Renderer::Instance()->GetDevice().Get(), scratch.GetImage(0, 0, 0), 1, tMeta, &srv);
				if (FAILED(hr))
				{
					SystemLogger::GetError() << "[Error] Failed to create ShaderResourceView for texture \"" << _path << "\": 0x" << hex << hr << dec << endl;
					// We don't return false because the shader resource view is technically unnecessary, a texture2D might have been the goal all along.
				}
				hr = CreateTexture(Renderer::Instance()->GetDevice().Get(), scratch.GetImage(0, 0, 0), 1, tMeta, &texture);
				if (tMeta.format == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB || tMeta.format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB) {
					SystemLogger::Warn() << "\"" << _path << "\" is stored in sRGB!" << std::endl;
				}
				Renderer::Instance()->GetRendererLock().unlock();
				if (FAILED(hr))
				{
					SystemLogger::GetError() << "[Error] Failed to create Texture2D for \"" << _path << "\": 0x" << hex << hr << dec << endl;
					if (_srv)
					{
						(*_srv) = nullptr;
					}
					if (_texture)
					{
						(*_texture) = nullptr;
					}
					return false;
				}

#if _DEBUG
				string textureName = "Texture2D for " + string(_path);
				string viewName = "SRV for " + string(_path);
				if (texture != nullptr)
				{
					texture->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)textureName.size(), textureName.c_str());
				}
				if (srv != nullptr)
				{
					srv->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)viewName.size(), viewName.c_str());
				}
#endif
				if (srv != nullptr && _srv != nullptr)
				{
					_srv->Attach(srv);
				}
				if (texture != nullptr && _texture != nullptr)
				{
					_texture->Attach((ID3D11Texture2D*)texture);
				}
				SystemLogger::Debug() << "Successfully loaded texture \"" << _path << "\"" << std::endl;
				return true;
			}
			delete[] buffer;
			ID3D11Resource *texture;
			ID3D11ShaderResourceView *srv;
			Renderer::Instance()->GetRendererLock().lock();
			hr = CreateDDSTextureFromMemory(Renderer::Instance()->GetDevice().Get(), scratch.GetPixels(), scratch.GetPixelsSize(), &texture, &srv);
			if (FAILED(hr))
			{
				SystemLogger::GetError() << "[Error] Failed to create texture and shader resource view: 0x" << hex << hr << dec << endl;
				if (_srv)
				{
					(*_srv) = nullptr;
				}
				if (_texture)
				{
					(*_texture) = nullptr;
				}
				return false;
			}
			Renderer::Instance()->GetRendererLock().unlock();
#if _DEBUG
			string textureName = "Texture loaded from " + string(_path);
			string viewName = "Texture view for " + string(_path);
			texture->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)textureName.size(), textureName.c_str());
			srv->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)viewName.size(), viewName.c_str());
#endif

			if (_srv == nullptr)
			{
				// If both inputs are nullptr, the function returns false, so _texture may be assumed valid.
				SystemLogger::GetError() << "[Warning] The resource view is a nullptr, but the texture is valid. The resource view will be released now, but this ay not be the desired behavior." << endl;
				srv->Release();
			}

			if (_srv)
			{
				_srv->Attach(srv);
			}
			if (_texture)
			{
				_texture->Attach((ID3D11Texture2D*)texture);
			}

			return true;
		}

	}
}