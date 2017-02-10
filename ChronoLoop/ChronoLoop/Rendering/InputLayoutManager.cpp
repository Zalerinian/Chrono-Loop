#include "stdafx.h"
#include <d3d11.h>
#include "InputLayoutManager.h"
#include "renderer.h"
#include <fstream>


namespace RenderEngine {

	InputLayoutManager* InputLayoutManager::sInstance = nullptr;

	InputLayoutManager::InputLayoutManager()
	{
		this->Initialize();
	}
	
	void InputLayoutManager::Initialize()
	{
		ID3D11Device* DEVICE = *Renderer::Instance()->mDevice.get();
		char* buffer = nullptr;
		int bufferSize = 0;
		D3D11_INPUT_ELEMENT_DESC PosDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		LoadShader("VertexLayout_POS_VS.cso", &buffer, bufferSize);
		DEVICE->CreateInputLayout(PosDesc, ARRAYSIZE(PosDesc), buffer, bufferSize, &mInputLayouts[eVERT_POS]);
		delete[] buffer;
		D3D11_INPUT_ELEMENT_DESC PosColorDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		LoadShader("VertexLayout_POSCOLOR_VS.cso", &buffer, bufferSize);
		DEVICE->CreateInputLayout(PosColorDesc, ARRAYSIZE(PosColorDesc), buffer, bufferSize, &mInputLayouts[eVERT_POSCOLOR]);
		delete[] buffer;
		D3D11_INPUT_ELEMENT_DESC PosTexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		LoadShader("VertexLayout_POSTEX_VS.cso", &buffer, bufferSize);
		DEVICE->CreateInputLayout(PosTexDesc, ARRAYSIZE(PosTexDesc), buffer, bufferSize, &mInputLayouts[eVERT_POSTEX]);
		delete[] buffer;
		D3D11_INPUT_ELEMENT_DESC PosNormTexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		LoadShader("VertexLayout_POSNORMTEX_VS.cso", &buffer, bufferSize);
		DEVICE->CreateInputLayout(PosNormTexDesc, ARRAYSIZE(PosNormTexDesc), buffer, bufferSize, &mInputLayouts[eVERT_POSNORMTEX]);
		delete[] buffer;
		D3D11_INPUT_ELEMENT_DESC PosNormTanTexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 1, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		LoadShader("VertexLayout_POSNORMTANTEX_VS.cso", &buffer, bufferSize);
		DEVICE->CreateInputLayout(PosNormTanTexDesc, ARRAYSIZE(PosNormTanTexDesc), buffer, bufferSize, &mInputLayouts[eVERT_POSNORMTANTEX]);
		delete[] buffer;
		D3D11_INPUT_ELEMENT_DESC PosBoneWeightDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		LoadShader("VertexLayout_POSBONEWEIGHT_VS.cso", &buffer, bufferSize);
		DEVICE->CreateInputLayout(PosBoneWeightDesc, ARRAYSIZE(PosBoneWeightDesc), buffer, bufferSize, &mInputLayouts[eVERT_POSBONEWEIGHT]);
		delete[] buffer;
		D3D11_INPUT_ELEMENT_DESC PosBoneWeightNormTexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BNEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		LoadShader("VertexLayout_POSBONEWEIGHTNORMTEX_VS.cso", &buffer, bufferSize);
		DEVICE->CreateInputLayout(PosBoneWeightNormTexDesc, ARRAYSIZE(PosBoneWeightNormTexDesc), buffer, bufferSize, &mInputLayouts[eVERT_POSBONEWEIGHTNORMTEX]);
		delete[] buffer;
		D3D11_INPUT_ELEMENT_DESC PosBoneWeightNormTanTexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BNEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 1, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		LoadShader("VertexLayout_POSBONEWEIGHTNORMTANTEX_VS.cso", &buffer, bufferSize);
		DEVICE->CreateInputLayout(PosBoneWeightNormTanTexDesc, ARRAYSIZE(PosBoneWeightNormTanTexDesc), buffer, bufferSize, &mInputLayouts[eVERT_POSBONEWEIGHTNORMTANTEX]);
		delete[] buffer;
	}
	
	InputLayoutManager::~InputLayoutManager()
	{
		for (int i = 0; i < VertFormat::eVERT_MAX; i++)
		{
			mInputLayouts[i]->Release();
		}
	}
	
	InputLayoutManager & InputLayoutManager::Instance()
	{
		if (!sInstance)
			sInstance = new InputLayoutManager();
		return *sInstance;
	}
	
	void InputLayoutManager::DestroyInstance()
	{
		if (sInstance)
			delete sInstance;
	}
	void InputLayoutManager::LoadShader(char * _fileName, char **_output, int& _outputSize)
	{
		std::ifstream load;
		load.open(_fileName, std::ios_base::binary);
		if (!load.is_open()) {
			_output = nullptr;
			_outputSize = 0;
			return;
		}
		load.seekg(0, std::ios_base::end);
		_outputSize = int(load.tellg());
		*_output = new char[_outputSize];
		load.seekg(0, std::ios_base::beg);
		load.read(*_output, _outputSize);
		load.close();
	}
}
