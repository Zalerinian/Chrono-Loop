#include "TextureManager.h"
#include "../Common/FileIO/FileIO_Textures.h"
#include "Renderer.h"
#include <d3d11.h>
#include <memory>

namespace Epoch {

	TextureManager* TextureManager::sInstance = nullptr;

	TextureManager::TextureManager() {}

	TextureManager::~TextureManager() {
		for (auto it = mTextureMap2D.begin(); it != mTextureMap2D.end(); ++it) {
			(*std::get<0>(it->second))->Release();
			(*std::get<1>(it->second))->Release();
		}
		for (auto it = mTextureMap3D.begin(); it != mTextureMap3D.end(); ++it) {
			(*std::get<0>(it->second))->Release();
			(*std::get<1>(it->second))->Release();
		}
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

	TextureManager::TextureStatus TextureManager::iAddTexture2D(std::string& _name, ID3D11Texture2D * _tex, std::shared_ptr<ID3D11ShaderResourceView*>* _srv) {
		if (_tex == nullptr) {
			return TextureStatus::eError;
		}
		std::string typedName = "memory:" + _name;
		if(mTextureMap2D[typedName].first.get() != nullptr)
		{
			_name = typedName;
			return TextureStatus::eSuccess;
		}
		D3D11_TEXTURE2D_DESC texDesc;
		_tex->GetDesc(&texDesc);
		CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(_tex, D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D, texDesc.Format);
		ID3D11ShaderResourceView* srv;
		Renderer::Instance()->GetDevice()->CreateShaderResourceView(_tex, &srvDesc, &srv);
		_name = typedName;
		mTextureMap2D[_name] = std::make_pair<std::shared_ptr<ID3D11ShaderResourceView*>, std::shared_ptr<ID3D11Texture2D*>>(
				std::make_shared<ID3D11ShaderResourceView*>(srv),
				std::make_shared<ID3D11Texture2D*>(_tex)
			);
		if (_srv) {
			(*_srv) = mTextureMap2D[_name].first;
		}
		Renderer::Instance()->GetContext()->PSSetShaderResources(3, 1, &srv);
		return TextureStatus::eSuccess;
	}

	TextureManager::TextureStatus TextureManager::iGetTexture2D(const char * _path, std::shared_ptr<ID3D11ShaderResourceView*>* _srv, std::shared_ptr<ID3D11Texture2D*>* _texture) {
		if (_srv == nullptr && _texture == nullptr) {
			return TextureStatus::eError;
		}
		if (mTextureMap2D[_path].first.get() != nullptr) {
			if (_srv) {
				*_srv = mTextureMap2D[_path].first;
			}
			if (_texture) {
				*_texture = mTextureMap2D[_path].second;
			}
			return TextureStatus::eSuccess;
		}

		std::shared_ptr<ID3D11ShaderResourceView*> srv;
		std::shared_ptr<ID3D11Texture2D*> text2d;
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
		mTextureMap2D[_path] = std::make_pair<std::shared_ptr<ID3D11ShaderResourceView*>, std::shared_ptr<ID3D11Texture2D*>>(
			std::forward<std::shared_ptr<ID3D11ShaderResourceView*>>(srv),
			std::forward<std::shared_ptr<ID3D11Texture2D*>>(text2d)
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