//#include "stdafx.h"
#include "ShaderManager.h"
#include <d3d11.h>
#include "../Common/FileIO.h"
#include "Renderer.h"
#include <memory>
#include "../Common/Logger.h"
#include "../Common/Common.h"

namespace Epoch {

	ShaderManager* ShaderManager::sInstance = nullptr;

	ShaderManager::ShaderManager() {
		// Create Pixel Shaders
		char *buffer;
		int byteSize = 0;
		ID3D11PixelShader *ps;
		if (!FileIO::LoadBytes("BasicPixel.cso", &buffer, byteSize)) {
			SystemLogger::GetError() << "[Error] An error has occurred when trying to read BasicPixel.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_BASIC] = std::make_shared<ID3D11PixelShader*>(nullptr);
		} else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, &ps);
			SetD3DName(ps, "BasicPixel.cso");
			mPixelShaders[ePS_BASIC] = std::make_shared<ID3D11PixelShader*>(ps);
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("TexturedPixel.cso", &buffer, byteSize)) {
			SystemLogger::GetError() << "[Error] An error has occurred when trying to read TexturedPixel.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_TEXTURED] = std::make_shared<ID3D11PixelShader*>(nullptr);
		} else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, &ps);
			SetD3DName(ps, "TexturedPixel.cso");
			mPixelShaders[ePS_TEXTURED] = std::make_shared<ID3D11PixelShader*>(ps);
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("PSTimeManipulation.cso", &buffer, byteSize)) {
			SystemLogger::GetError() << "[Error] An error has occurred when trying to read PSTimeManipulation.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_POSTPROCESS] = std::make_shared<ID3D11PixelShader*>(nullptr);
		} else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, &ps);
			SetD3DName(ps, "PSTimeManipulation.cso");
			mPixelShaders[ePS_POSTPROCESS] = std::make_shared<ID3D11PixelShader*>(ps);
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("PSPureTexture.cso", &buffer, byteSize)) {
			SystemLogger::GetError() << "[Error] An error has occurred when trying to read PSPureTexture.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_PURETEXTURE] = std::make_shared<ID3D11PixelShader*>(nullptr);
		} else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, &ps);
			SetD3DName(ps, "PSPureTexture.cso");
			mPixelShaders[ePS_PURETEXTURE] = std::make_shared<ID3D11PixelShader*>(ps);
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("PSTransparent.cso", &buffer, byteSize)) {
			SystemLogger::GetError() << "[Error] An error has occurred when trying to read PSTransparent.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_TRANSPARENT] = std::make_shared<ID3D11PixelShader*>(nullptr);
		}
		else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, &ps);
			SetD3DName(ps, "PSTransparent.cso");
			mPixelShaders[ePS_TRANSPARENT] = std::make_shared<ID3D11PixelShader*>(ps);
			delete[] buffer;
		}
		
		if (!FileIO::LoadBytes("PSAnimatedMultiscan.cso", &buffer, byteSize)) {
			SystemLogger::GetError() << "[Error] An error has occurred when trying to read PSAnimatedMultiscan.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_TRANSPARENT_SCANLINE] = std::make_shared<ID3D11PixelShader*>(nullptr);
		}
		else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, &ps);
			SetD3DName(ps, "PSAnimatedMultiscan.cso");
			mPixelShaders[ePS_TRANSPARENT_SCANLINE] = std::make_shared<ID3D11PixelShader*>(ps);
			delete[] buffer;
		}




		// Create Vertex Shaders.
		ID3D11VertexShader *vs;
		if (!FileIO::LoadBytes("BasicVertex.cso", &buffer, byteSize)) {
			SystemLogger::GetError() << "[Error] An error has occurred when trying to read BasicVertex.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mVertexShaders[eVS_BASIC] = std::make_shared<ID3D11VertexShader*>(nullptr);
		} else {
			Renderer::Instance()->GetDevice()->CreateVertexShader(buffer, byteSize, nullptr, &vs);
			SetD3DName(vs, "BasicVertex.cso");
			mVertexShaders[eVS_BASIC] = std::make_shared<ID3D11VertexShader*>(vs);
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("TexturedVertex.cso", &buffer, byteSize)) {
			SystemLogger::GetError() << "[Error] An error has occurred when trying to read TexturedVertex.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mVertexShaders[eVS_TEXTURED] = std::make_shared<ID3D11VertexShader*>(nullptr);
		} else {
			Renderer::Instance()->GetDevice()->CreateVertexShader(buffer, byteSize, nullptr, &vs);
			SetD3DName(vs, "TexturedVertex.cso");
			mVertexShaders[eVS_TEXTURED] = std::make_shared<ID3D11VertexShader*>(vs);
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("NDCVertex.cso", &buffer, byteSize)) {
			SystemLogger::GetError() << "[Error] An error has occurred when trying to read NDCVertex.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mVertexShaders[eVS_NDC] = std::make_shared<ID3D11VertexShader*>(nullptr);
		} else {
			Renderer::Instance()->GetDevice()->CreateVertexShader(buffer, byteSize, nullptr, &vs);
			SetD3DName(vs, "NDCVertex.cso");
			mVertexShaders[eVS_NDC] = std::make_shared<ID3D11VertexShader*>(vs);
			delete[] buffer;
		}






		// Create geometry shaders
		ID3D11GeometryShader *gs;
		if (!FileIO::LoadBytes("GSDuplicateMesh.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read GSDuplucateMesh.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mGeoShaders[eGS_PosNormTex] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreateGeometryShader(buffer, byteSize, nullptr, &gs);
			SetD3DName(gs, "GSDuplicateMesh.cso");
			mGeoShaders[eGS_PosNormTex].Attach(gs);
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("GSNDC.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read GSNDC.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mGeoShaders[eGS_PosNormTex_NDC] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreateGeometryShader(buffer, byteSize, nullptr, &gs);
			SetD3DName(gs, "GSNDC.cso");
			mGeoShaders[eGS_PosNormTex_NDC].Attach(gs);
			delete[] buffer;
		}
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

	void ShaderManager::ApplyVShader(VertexShaderFormat f) {
		Renderer::Instance()->GetContext()->VSSetShader(*mVertexShaders[f], nullptr, 0);
	}

	void ShaderManager::ApplyPShader(PixelShaderFormat f) {
		Renderer::Instance()->GetContext()->PSSetShader(*mPixelShaders[f], nullptr, 0);
	}

	void ShaderManager::ApplyGShader(GeometryShaderFormat _f) {
		Renderer::Instance()->GetContext()->GSSetShader(mGeoShaders[_f].Get(), nullptr, 0);
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
