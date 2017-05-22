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
		if (!FileIO::LoadBytes("BasicPixel.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read BasicPixel.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_BASIC] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, mPixelShaders[ePS_BASIC].GetAddressOf());
			SetD3DName(mPixelShaders[ePS_BASIC].Get(), "BasicPixel.cso");
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("TexturedPixel.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read TexturedPixel.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_TEXTURED] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, mPixelShaders[ePS_TEXTURED].GetAddressOf());
			SetD3DName(mPixelShaders[ePS_TEXTURED].Get(), "TexturedPixel.cso");
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("PSTimeManipulation.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read PSTimeManipulation.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_POSTPROCESS] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, mPixelShaders[ePS_POSTPROCESS].GetAddressOf());
			SetD3DName(mPixelShaders[ePS_POSTPROCESS].Get(), "PSTimeManipulation.cso");
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("PSPureTexture.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read PSPureTexture.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_PURETEXTURE] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, mPixelShaders[ePS_PURETEXTURE].GetAddressOf());
			SetD3DName(mPixelShaders[ePS_PURETEXTURE].Get(), "PSPureTexture.cso");
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("PSTransparent.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read PSTransparent.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_TRANSPARENT] = nullptr;
		}
		else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, mPixelShaders[ePS_TRANSPARENT].GetAddressOf());
			SetD3DName(mPixelShaders[ePS_TRANSPARENT].Get(), "PSTransparent.cso");
			delete[] buffer;
		}
		
		if (!FileIO::LoadBytes("PSAnimatedMultiscan.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read PSAnimatedMultiscan.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_TRANSPARENT_SCANLINE] = nullptr;
		}
		else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, mPixelShaders[ePS_TRANSPARENT_SCANLINE].GetAddressOf());
			SetD3DName(mPixelShaders[ePS_TRANSPARENT_SCANLINE].Get(), "PSAnimatedMultiscan.cso");
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("PSBlur.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read PSBlur.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_BLUR] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, mPixelShaders[ePS_BLUR].GetAddressOf());
			SetD3DName(mPixelShaders[ePS_BLUR].Get(), "PSBlur.cso");
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("PSBloom.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read PSBloom.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_BLOOM] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, mPixelShaders[ePS_BLOOM].GetAddressOf());
			SetD3DName(mPixelShaders[ePS_BLOOM].Get(), "PSBloom.cso");
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("PSCombiner.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read PSCombiner.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_DEFERRED] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, mPixelShaders[ePS_DEFERRED].GetAddressOf());
			SetD3DName(mPixelShaders[ePS_DEFERRED].Get(), "PSCombiner.cso");
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("PSSolidColor.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read PSSolidColor.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_SOLIDCOLOR] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, mPixelShaders[ePS_SOLIDCOLOR].GetAddressOf());
			SetD3DName(mPixelShaders[ePS_SOLIDCOLOR].Get(), "PSSolidColor.cso");
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("PSLighting.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read PSLighting.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mPixelShaders[ePS_LIGHTING] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, byteSize, nullptr, mPixelShaders[ePS_LIGHTING].GetAddressOf());
			SetD3DName(mPixelShaders[ePS_LIGHTING].Get(), "PSLighting.cso");
			delete[] buffer;
		}




		// Create Vertex Shaders.
		if (!FileIO::LoadBytes("BasicVertex.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read BasicVertex.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mVertexShaders[eVS_BASIC] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreateVertexShader(buffer, byteSize, nullptr, mVertexShaders[eVS_BASIC].GetAddressOf());
			SetD3DName(mVertexShaders[eVS_BASIC].Get(), "BasicVertex.cso");
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("TexturedVertex.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read TexturedVertex.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mVertexShaders[eVS_TEXTURED] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreateVertexShader(buffer, byteSize, nullptr, mVertexShaders[eVS_TEXTURED].GetAddressOf());
			SetD3DName(mVertexShaders[eVS_TEXTURED].Get(), "TexturedVertex.cso");
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("NDCVertex.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read NDCVertex.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mVertexShaders[eVS_NDC] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreateVertexShader(buffer, byteSize, nullptr, mVertexShaders[eVS_NDC].GetAddressOf());
			SetD3DName(mVertexShaders[eVS_NDC].Get(), "NDCVertex.cso");
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("VSBlur.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read VSBlur.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mVertexShaders[eVS_BLUR] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreateVertexShader(buffer, byteSize, nullptr, mVertexShaders[eVS_BLUR].GetAddressOf());
			SetD3DName(mVertexShaders[eVS_BLUR].Get(), "VSBlur.cso");
			delete[] buffer;
		}




		// Create geometry shaders
		mGeoShaders[eGS_None] = nullptr;

		if (!FileIO::LoadBytes("GSDuplicateMesh.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read GSDuplucateMesh.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mGeoShaders[eGS_PosNormTex] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreateGeometryShader(buffer, byteSize, nullptr, mGeoShaders[eGS_PosNormTex].GetAddressOf());
			SetD3DName(mGeoShaders[eGS_PosNormTex].Get(), "GSDuplicateMesh.cso");
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("GSNDC.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read GSNDC.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mGeoShaders[eGS_PosNormTex_NDC] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreateGeometryShader(buffer, byteSize, nullptr, mGeoShaders[eGS_PosNormTex_NDC].GetAddressOf());
			SetD3DName(mGeoShaders[eGS_PosNormTex_NDC].Get(), "GSNDC.cso");
			delete[] buffer;
		}

		if (!FileIO::LoadBytes("GSAnimatedQuad.cso", &buffer, byteSize)) {
			SystemLogger::Error() << "An error has occurred when trying to read GSAnimatedQuad.cso. Chances are the file is missing or has been renamed. The shader will be null, and may result in a crash." << std::endl;
			mGeoShaders[eGS_PosNormTex_AnimQuad] = nullptr;
		} else {
			Renderer::Instance()->GetDevice()->CreateGeometryShader(buffer, byteSize, nullptr, mGeoShaders[eGS_PosNormTex_AnimQuad].GetAddressOf());
			SetD3DName(mGeoShaders[eGS_PosNormTex_AnimQuad].Get(), "GSAnimatedQuad.cso");
			delete[] buffer;
		}

	}

	ShaderManager::~ShaderManager() {}

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
		Renderer::Instance()->GetContext()->VSSetShader(mVertexShaders[f].Get(), nullptr, 0);
	}

	void ShaderManager::ApplyPShader(PixelShaderFormat f) {
		Renderer::Instance()->GetContext()->PSSetShader(mPixelShaders[f].Get(), nullptr, 0);
	}

	void ShaderManager::ApplyGShader(GeometryShaderFormat _f) {
		Renderer::Instance()->GetContext()->GSSetShader(mGeoShaders[_f].Get(), nullptr, 0);
	}

	Microsoft::WRL::ComPtr<ID3D11PixelShader>& ShaderManager::GetPixelShader(PixelShaderFormat f) {
		return mPixelShaders[f];
	}

	Microsoft::WRL::ComPtr<ID3D11VertexShader>& ShaderManager::GetVertexShader(VertexShaderFormat f) {
		return mVertexShaders[f];
	}
}
