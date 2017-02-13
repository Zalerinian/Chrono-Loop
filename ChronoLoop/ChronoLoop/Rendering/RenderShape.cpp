//#include "stdafx.h"
#include "RenderShape.h"
#include "renderer.h"
#include <d3d11.h>
#include "../Common/FileIO.h"
#include "../Common/Common.h"
#include "../../DXTK/ddstextureloader.h"
#include "../../DXTK/DirectXTex.h"
#include <memory>

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
	
	void RenderShape::Load(Mesh & _mesh) {
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

	void RenderShape::AddTexture(const char * _path, TextureType position) {
		wchar_t *path = nullptr;
		Engine::MakeWide(_path, path, (unsigned int)strlen(_path) + 1);
		AddTexture(path, position);
		delete[] path;
	}

	void RenderShape::AddTexture(const wchar_t * _path, TextureType position) {
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
	}

	void RenderShape::Render() {
		UINT stride = sizeof(VertexPosNormTex), offset = 0;
		(*Renderer::Instance()->GetContext())->IASetIndexBuffer(*mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		(*Renderer::Instance()->GetContext())->IASetVertexBuffers(0, 1, mVertexBuffer.get(), &stride, &offset);
		(*Renderer::Instance()->GetContext())->DrawIndexed(mIndexCount, 0, 0);
	}

}