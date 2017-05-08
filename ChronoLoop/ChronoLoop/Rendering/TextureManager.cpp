#include "TextureManager.h"
#include "../Common/FileIO/FileIO_Textures.h"
#include "../Common/Common.h"
#include "Renderer.h"
#include <d3d11.h>
#include <memory>

namespace Epoch {

	TextureManager* TextureManager::sInstance = nullptr;

	TextureManager::TextureManager() {}

	TextureManager::~TextureManager() {
		//for (auto it = mTextureMap2D.begin(); it != mTextureMap2D.end(); ++it) {
		//	(*std::get<0>(it->second))->Release();
		//	(*std::get<1>(it->second))->Release();
		//}
		//for (auto it = mTextureMap3D.begin(); it != mTextureMap3D.end(); ++it) {
		//	(*std::get<0>(it->second))->Release();
		//	(*std::get<1>(it->second))->Release();
		//}
	}

	TextureManager* TextureManager::Instance() {
		if (nullptr == sInstance) {
			sInstance = new TextureManager;
		}
		return sInstance;
	}

	void TextureManager::DestroyInstance() {
		if (nullptr != sInstance) {
			delete sInstance;
		}
		sInstance = nullptr;
	}

	TextureManager::TextureStatus TextureManager::iAddTexture2D(std::string& _name, Microsoft::WRL::ComPtr<ID3D11Texture2D>& _tex, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* _srv) {
		if (_tex == nullptr) {
			return TextureStatus::eError;
		}
		std::string typedName = "memory:" + _name;
		if(mTextureMap2D[typedName].first.Get() != nullptr)
		{
			_name = typedName;
			if (_srv) {
				(*_srv) = mTextureMap2D[_name].first;
			}
			return TextureStatus::eSuccess;
		}
		D3D11_TEXTURE2D_DESC texDesc;
		_tex->GetDesc(&texDesc);
		CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(_tex.Get(), D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D, texDesc.Format);
		ID3D11ShaderResourceView* srv;
		Renderer::Instance()->GetDevice()->CreateShaderResourceView(_tex.Get(), &srvDesc, &srv);
		_name = typedName;
		SetD3DName(srv, (std::string("Internal SRV for ") + _name).c_str());
		mTextureMap2D[_name] = std::make_pair<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, Microsoft::WRL::ComPtr<ID3D11Texture2D>>(
				std::forward<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>(srv),
				std::forward<Microsoft::WRL::ComPtr<ID3D11Texture2D>>(_tex)
			);
		_tex = mTextureMap2D[_name].second;
		if (_srv) {
			(*_srv) = mTextureMap2D[_name].first;
		}
		return TextureStatus::eSuccess;
	}

	TextureManager::TextureStatus TextureManager::iGetTexture2D(const char * _path, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* _srv, Microsoft::WRL::ComPtr<ID3D11Texture2D>* _texture) {
		if (_srv == nullptr && _texture == nullptr) {
			return TextureStatus::eError;
		}
		if (mTextureMap2D[_path].first.Get() != nullptr) {
			if (_srv) {
				*_srv = mTextureMap2D[_path].first;
			}
			if (_texture) {
				*_texture = mTextureMap2D[_path].second;
			}
			return TextureStatus::eSuccess;
		}

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> text2d;
		if (strncmp(_path, "memory:", 7) == 0) {
			// We're looking for a file added via memory, but we don't have it, so return nullptr's.
			if (_srv) {
				(*_srv) = nullptr;
			}
			if (_texture) {
				(*_texture) = nullptr;
			}
			return TextureStatus::eError;
		} else if (!FileIO::LoadTexture2D(_path, &srv, &text2d)) {
			if (_srv) {
				(*_srv) = nullptr;
			}
			if (_texture) {
				(*_texture) = nullptr;
			}
			return TextureStatus::eError;
		}
		mTextureMap2D[_path] = std::make_pair<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, Microsoft::WRL::ComPtr<ID3D11Texture2D>>(
			std::forward<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>(srv),
			std::forward<Microsoft::WRL::ComPtr<ID3D11Texture2D>>(text2d)
		);
		if (_srv) {
			(*_srv) = mTextureMap2D[_path].first;
		}
		if (_texture) {
			(*_texture) = mTextureMap2D[_path].second;
		}
		return TextureStatus::eSuccess;
	}

}