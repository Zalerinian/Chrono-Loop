//#include "stdafx.h"
#include "ShaderManager.h"
#include <d3d11.h>
#include "../Common/FileIO.h"
#include "renderer.h"
#include <memory>

namespace RenderEngine {

	ShaderManager* ShaderManager::sInstance = nullptr;
	
	ShaderManager::ShaderManager() {
		// Create Pixel Shaders
		char *buffer;
		int byteSize = 0;
		ID3D11PixelShader *ps;
		if (!FileIO::LoadBytes("BasicPixel.cso", &buffer, byteSize)) {
			// TODO: Put an actual error here.
			throw "Something catastrophic has occurred.";
		}
		(*Renderer::Instance()->GetDevice())->CreatePixelShader(buffer, byteSize, nullptr, &ps);
		mPixelShaders[ePS_BASIC] = std::make_shared<ID3D11PixelShader*>(ps);
		delete[] buffer;

		if (!FileIO::LoadBytes("TexturedPixel.cso", &buffer, byteSize)) {
			// TODO: Put an actual error here.
			throw "Something catastrophic has occurred.";
		}
		(*Renderer::Instance()->GetDevice())->CreatePixelShader(buffer, byteSize, nullptr, &ps);
		mPixelShaders[ePS_TEXTURED] = std::make_shared<ID3D11PixelShader*>(ps);
		delete[] buffer;
		

		// Create Vertex Shaders.
		ID3D11VertexShader *vs;
		if (!FileIO::LoadBytes("BasicVertex.cso", &buffer, byteSize)) {
			// TODO: Put an actual error here.
			throw "Something catastrophic has occurred.";
		}
		(*Renderer::Instance()->GetDevice())->CreateVertexShader(buffer, byteSize, nullptr, &vs);
		mVertexShaders[eVS_BASIC] = std::make_shared<ID3D11VertexShader*>(vs);
		delete[] buffer;

		if (!FileIO::LoadBytes("TexturedVertex.cso", &buffer, byteSize)) {
			// TODO: Put an actual error here.
			throw "Something catastrophic has occurred.";
		}
		(*Renderer::Instance()->GetDevice())->CreateVertexShader(buffer, byteSize, nullptr, &vs);
		mVertexShaders[eVS_TEXTURED] = std::make_shared<ID3D11VertexShader*>(vs);
		delete[] buffer;
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
		return mPixelShaders[f];
	}
	std::shared_ptr<ID3D11VertexShader*> ShaderManager::GetVertexShader(VertexShaderFormat f) {
		if (f >= eVS_BASIC && f < eVS_MAX) {
			return mVertexShaders[f];
		}
		return mVertexShaders[f];
	}
}
