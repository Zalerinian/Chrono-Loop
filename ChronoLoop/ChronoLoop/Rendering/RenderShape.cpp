//#include "stdafx.h"
#include "RenderShape.h"
#include "renderer.h"
#include "Mesh.h"
#include <d3d11.h>
#include "../Common/FileIO.h"

namespace RenderEngine {

	RenderShape::RenderShape() {
		mNext = nullptr;
	}
	
	RenderShape::RenderShape(Mesh & _mesh) {
		this->Load(_mesh);
		mNext = nullptr;
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

}