//#include "stdafx.h"
#include "RenderShape.h"
#include "Renderer.h"
#include <d3d11.h>
#include "../Common/Breakpoint.h"
#include "../Common/Common.h"
#include "../Common/Logger.h"
#include "TextureManager.h"
#include "VertexBufferManager.h"
#include "IndexBufferManager.h"
#include <memory>


namespace RenderEngine {

	RenderShape::RenderShape() {
		mType = RenderNodeType::Shape;
		mNext = nullptr;
		mContext.mRasterState = eRS_MAX;
		mContext.mVertexFormat = eVERT_POSNORMTEX;
	}
	
	RenderShape::RenderShape(Mesh & _mesh) {
		this->Load(_mesh);
		mNext = nullptr;
		mType = RenderNodeType::Shape;
		mContext.mRasterState = eRS_MAX;
		mContext.mVertexFormat = eVERT_POSNORMTEX;
	}

	RenderShape::RenderShape(const char * _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs) {
		mType = RenderNodeType::Shape;
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
			Debug::SetBreakpoint();
			SystemLogger::GetError() << "[Error] Attempting to load an empty mesh in RenderShape!" << std::endl;
		}
#endif

		mIndexOffset = IndexBufferManager::Instance().AddToBuffer(_mesh.GetName(), _mesh.GetIndicies(), (unsigned int)_mesh.IndicieSize());
		mVertexOffset = VertexBufferManager::Instance().GetInternalBuffer<VertexPosNormTex>()->AddVerts(_mesh.GetName(), _mesh.GetVerts(), (unsigned int)_mesh.VertSize());


		ID3D11Buffer *tBuffer;
		mIndexBuffer = nullptr;
		mVertexBuffer = nullptr;
		auto device = *Renderer::Instance()->iGetDevice().get();
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
		CD3D11_BUFFER_DESC indexBufferDesc((UINT)_mesh.IndicieSize() * sizeof(unsigned int), D3D11_BIND_INDEX_BUFFER);
		result = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &tBuffer);
		mIndexBuffer = std::make_shared<ID3D11Buffer*>(tBuffer);
	}

	void RenderShape::Load(const char * _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs) {
		mName = std::string("Shape: ") + _path;
		mMesh.Load(_path);
		if (_invert) {
			mMesh.Invert();
		}
		Load(mMesh);
		SetShaders(_ps, _vs);
	}
	
	void RenderShape::SetShaders(PixelShaderFormat pf, VertexShaderFormat vf)
	{
		if (pf < ePS_BASIC || pf >= ePS_MAX) {
			SystemLogger::GetError() << "[Error] Invalid Pixel shader enumation at RenderShape::SetShaders: " << pf << ". Forcing to ePS_MAX!" << std::endl;
			pf = ePS_MAX;
		}
		if (vf < eVS_BASIC || vf >= eVS_MAX) {
			SystemLogger::GetError() << "[Error] Invalid VertexShader enumation at RenderShape::SetShaders: " << vf << ". Forcing to eVS_MAX!" << std::endl;
			vf = eVS_MAX;
		}
		mContext.mVertexShaderFormat = vf;
		mContext.mPixelShaderFormat = pf;
	}

	RenderShape& RenderShape::AddTexture(const char * _path, TextureType _position) {
		std::shared_ptr<ID3D11ShaderResourceView*> srv;
		TextureManager::TextureStatus stat = TextureManager::Instance()->iGetTexture2D(_path, &srv, nullptr);
		if (stat == TextureManager::TextureStatus::eSuccess) {
			mContext.mTextures[_position] = srv;
		} else {
			SystemLogger::GetError() << "[Error] Failed to get synchronous texture from TextureManager!" << std::endl;
		}
		return *this;
	}

	RenderShape& RenderShape::AddTexture(const wchar_t * _path, TextureType _position) {
		char *narrow = nullptr;
		Engine::MakeNarrow(_path, &narrow, (unsigned int)std::wcslen(_path) + 1);
		AddTexture(narrow, _position);
		delete[] narrow;
		return *this;
	}

	void RenderShape::Render() {
		UINT stride = sizeof(VertexPosNormTex), offset = 0;
		ID3D11Buffer * vBuffer = VertexBufferManager::GetBuffer(eVERT_POSNORMTEX);
		(*Renderer::Instance()->iGetContext())->IASetIndexBuffer(IndexBufferManager::GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
		(*Renderer::Instance()->iGetContext())->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
		(*Renderer::Instance()->iGetContext())->DrawIndexedInstanced(mIndexCount, 1, mIndexOffset, mVertexOffset, 0);
	}

}