//#include "stdafx.h"
#include "ShaderManager.h"
#include <d3d11.h>
#include "InputLayoutManager.h"

namespace RenderEngine {

	ShaderManager* ShaderManager::sInstance = nullptr;
	
	ShaderManager::ShaderManager() {
		// Create Pixel Shaders



		// Create Vertex Shaders, which also needs input layouts.
	}

	ShaderManager::~ShaderManager() {
		for (int i = 0; i < ePS_MAX; ++i) {
			(*mPixelShaders[i])->Release();
		}
		for (int i = 0; i < eVS_MAX; ++i) {
			(*mVertexShaders[i])->Release();
		}
	}

	ShaderManager * ShaderManager::Instance() {
		if (nullptr == sInstance) {
			sInstance = new ShaderManager;
		}
		return sInstance;
	}
	
	void ShaderManager::DestroyInstance() {
		delete sInstance;
		sInstance = nullptr;
	}
	std::shared_ptr<ID3D11PixelShader*> ShaderManager::GetPixelShader(PixelShaderFormat f) {
		if (f >= ePS_BASIC && f < ePS_MAX) {
			return mPixelShaders[f];
		}
		return std::shared_ptr<ID3D11PixelShader*>();
	}
	std::shared_ptr<ID3D11VertexShader*> ShaderManager::GetVertexShader(VertexShaderFormat f) {
		if (f >= eVS_BASIC && f < eVS_MAX) {
			return mVertexShaders[f];
		}
		return std::shared_ptr<ID3D11VertexShader*>();
	}
}
