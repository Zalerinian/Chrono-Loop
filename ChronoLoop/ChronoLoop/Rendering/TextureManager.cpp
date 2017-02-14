#include "TextureManager.h"
#include "../Common/FileIO/FileIO_Textures.h"
#include <d3d11.h>
#include <memory>

namespace RenderEngine {

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
		if (!FileIO::LoadTexture2D(_path, &srv, &text2d)) {
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
			(*_srv) = srv;
		}
		if (_texture) {
			(*_texture) = text2d;
		}
		return TextureStatus::eSuccess;
	}

}