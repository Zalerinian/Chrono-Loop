//#include "stdafx.h"
#include "RenderShape.h"
#include "renderer.h"
#include <d3d11.h>
#include "../Common/FileIO.h"
#include "../Common/Common.h"
#include "../../DXTK/ddstextureloader.h"
#include "../../DXTK/DirectXTex.h"
#include <memory>


#if _DEBUG
#include <intrin.h>
#endif

namespace RenderEngine {

	RenderShape::RenderShape() {
		mType = RenderNodeType::Shape;
		mNext = nullptr;
		mContext.mRasterState = eRS_FILLED;
		mContext.mVertexFormat = eVERT_POSNORMTEX;
	}
	
	RenderShape::RenderShape(Mesh & _mesh) {
		this->Load(_mesh);
		mNext = nullptr;
		mType = RenderNodeType::Shape;
		mContext.mRasterState = eRS_FILLED;
		mContext.mVertexFormat = eVERT_POSNORMTEX;
	}

	RenderShape::RenderShape(const char * _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs) {
		Load(_path, _invert, _ps, _vs);
	}

	RenderShape::~RenderShape() {
		if (mVertexBuffer.get() != nullptr) {
			(*mVertexBuffer)->Release();
		}
		if (mIndexBuffer.get() != nullptr) {
			(*mIndexBuffer)->Release();
		}
	}
	
	void RenderShape::Load(Mesh & _mesh) {
#if _DEBUG
		if (_mesh.VertSize() == 0) {
			OutputDebugString(L"Attempting to load an empty mesh.\n");
			__debugbreak();
		}
#endif
		ID3D11Buffer *tBuffer;
		mIndexBuffer = nullptr;
		mVertexBuffer = nullptr;
		auto device = *Renderer::Instance()->GetDevice().get();
		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		auto verts = _mesh.GetVerts();
		vertexBufferData.pSysMem = verts;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc((UINT)_mesh.VertSize() * sizeof(VertexPosNormTex), D3D11_BIND_VERTEX_BUFFER);
		HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &tBuffer);
		mVertexBuffer = std::make_shared<ID3D11Buffer*>(tBuffer);
		//Index Buffer
		mIndexCount = (unsigned int)_mesh.VertSize();
		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = _mesh.GetIndicies();
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc((UINT)_mesh.IndicieSize() * sizeof(unsigned short), D3D11_BIND_INDEX_BUFFER);
		result = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &tBuffer);
		mIndexBuffer = std::make_shared<ID3D11Buffer*>(tBuffer);
	}

	void RenderShape::Load(const char * _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs) {
		Mesh m(_path);
		if (_invert) {
			m.Invert();
		}
		Load(m);
		SetShaders(_ps, _vs);
	}
	
	void RenderShape::SetShaders(PixelShaderFormat pf, VertexShaderFormat vf)
	{
		if (pf < ePS_BASIC || pf >= ePS_MAX) {
			throw "Invalid pixel shader enumeration.";
		}
		if (vf < eVS_BASIC || vf >= eVS_MAX) {
			throw "Invalid vertex shader enumeration.";
		}
		mContext.mVertexShaderFormat = vf;
		mContext.mPixelShaderFormat = pf;
	}

	RenderShape& RenderShape::AddTexture(const char * _path, TextureType position) {
		wchar_t *path = nullptr;
		Engine::MakeWide(_path, path, (unsigned int)strlen(_path) + 1);
		AddTexture(path, position);
		delete[] path;
		return *this;
	}

	RenderShape& RenderShape::AddTexture(const wchar_t * _path, TextureType position) {
		DirectX::ScratchImage img;
		DirectX::Blob *image = new DirectX::Blob;
		UINT flags = 0;
		DirectX::LoadFromWICFile(_path, flags, nullptr, img);
		DirectX::SaveToDDSMemory(*img.GetImage(0, 0, 0), flags, *image);
		ID3D11ShaderResourceView *srv;
		ID3D11Texture2D *tex;
		DirectX::CreateDDSTextureFromMemory((*Renderer::Instance()->GetDevice()), 
			(const uint8_t*)image->GetBufferPointer(),
			image->GetBufferSize(),
			(ID3D11Resource**)&tex,
			&srv);
		mContext.mTextures[position] = std::make_shared<ID3D11ShaderResourceView*>(srv);
		delete image;
		return *this;
	}

	void RenderShape::Render() {
		UINT stride = sizeof(VertexPosNormTex), offset = 0;
		(*Renderer::Instance()->GetContext())->IASetIndexBuffer(*mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		(*Renderer::Instance()->GetContext())->IASetVertexBuffers(0, 1, mVertexBuffer.get(), &stride, &offset);
		(*Renderer::Instance()->GetContext())->DrawIndexed(mIndexCount, 0, 0);
	}

}