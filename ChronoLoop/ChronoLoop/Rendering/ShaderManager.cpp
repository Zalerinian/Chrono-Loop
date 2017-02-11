//#include "stdafx.h"
#include "ShaderManager.h"
#include <d3d11.h>
#include "InputLayoutManager.h"
#include "../Common/FileIO.h"

namespace RenderEngine {

	ShaderManager* ShaderManager::sInstance = nullptr;
	
	ShaderManager::ShaderManager() {
		// Create Pixel Shaders
		char *buffer;
		int byteSize = 0;
		if (!FileIO::LoadBytes("BasicPixel.cso", &buffer, byteSize)) {
			// TODO: Put an actual error here.
		}
		// Create the shader
		// Store it as a shared_ptr
		delete[] buffer;

		//Load a new shader



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
