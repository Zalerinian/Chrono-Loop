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
#include "MeshCache.h"
#include <memory>


namespace Epoch {

	RenderShape::RenderShape() {
		mType = RenderNodeType::Shape;
		mNext = nullptr;
		mContext.mRasterState = eRS_MAX;
		mContext.mVertexFormat = eVERT_POSNORMTEX;
	}
	
	RenderShape::RenderShape(Mesh & _mesh) {
		this->Load(&_mesh);
		mNext = nullptr;
		mType = RenderNodeType::Shape;
		mContext.mRasterState = eRS_MAX;
		mContext.mVertexFormat = eVERT_POSNORMTEX;
	}

	RenderShape::RenderShape(const RenderShape & _other) {
		this->mContext = _other.mContext;
		this->mIndexCount = _other.mIndexCount;
		this->mIndexOffset = _other.mIndexOffset;
		this->mMesh = _other.mMesh;
		this->mName = _other.mName;
		this->mNext = _other.mNext;
		this->mPosition = _other.mPosition;
		this->mPrevious = _other.mPrevious;
		this->mType = _other.mType;
		this->mVertexOffset = _other.mVertexOffset;
	}

	RenderShape::RenderShape(const char * _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs) {
		mType = RenderNodeType::Shape;
		Load(_path, _invert, _ps, _vs);
	}

	RenderShape::RenderShape(const char * _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs, GeometryShaderFormat _gs) {
		mType = RenderNodeType::Shape;
		Load(_path, _invert, _ps, _vs, _gs);
	}

	RenderShape::~RenderShape() {
	}
	
	void RenderShape::Load(Mesh *_mesh) {
#if _DEBUG
		if (_mesh->VertSize() == 0) {
			Debug::SetBreakpoint();
			SystemLogger::GetError() << "[Error] Attempting to load an empty mesh in RenderShape!" << std::endl;
		}
#endif

		mIndexOffset = IndexBufferManager::GetInstance().AddToBuffer(_mesh->GetName(), _mesh->GetIndicies(), (unsigned int)_mesh->IndicieSize());
		mVertexOffset = VertexBufferManager::Instance().GetVertexBuffer<VertexPosNormTex>()->AddVerts(_mesh->GetName(), _mesh->GetVerts(), (unsigned int)_mesh->VertSize());
		mIndexCount = (unsigned int)_mesh->VertSize();
	}

	void RenderShape::Load(const char * _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs) {
		mName = std::string("Shape: ") + _path;
		mMesh = MeshCache::GetInstance().GetMesh(_path);
		if (_invert) {
			mMesh->Invert();
		}
		Load(mMesh);
		SetShaders(_ps, _vs);
	}

	void RenderShape::Load(const char * _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs, GeometryShaderFormat _gs) {
		mName = std::string("Shape: ") + _path;
		mMesh = MeshCache::GetInstance().GetMesh(_path);
		if (_invert) {
			mMesh->Invert();
		}
		Load(mMesh);
		SetShaders(_ps, _vs, _gs);
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

	void RenderShape::SetShaders(PixelShaderFormat pf, VertexShaderFormat vf, GeometryShaderFormat _gf) {
		if (pf < ePS_BASIC || pf >= ePS_MAX) {
			SystemLogger::GetError() << "[Error] Invalid Pixel shader enumation at RenderShape::SetShaders: " << pf << ". Forcing to ePS_MAX!" << std::endl;
			pf = ePS_MAX;
		}
		if (vf < eVS_BASIC || vf >= eVS_MAX) {
			SystemLogger::GetError() << "[Error] Invalid Vertex Shader enumation at RenderShape::SetShaders: " << vf << ". Forcing to eVS_MAX!" << std::endl;
			vf = eVS_MAX;
		}
		if (_gf < eGS_PosNormTex || _gf >= eGS_MAX) {
			SystemLogger::Error() << "Invalid Geometry Shader enumation at RenderShape::SetShaders: " << _gf << ". Forcing to eVS_MAX!" << std::endl;
			_gf = eGS_MAX;
		}
		mContext.mVertexShaderFormat = vf;
		mContext.mPixelShaderFormat = pf;
		mContext.mGeoShaderFormat = _gf;
	}

	RenderShape& RenderShape::AddTexture(const char * _path, TextureType _position) {
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
		TextureManager::TextureStatus stat = TextureManager::Instance()->iGetTexture2D(_path, &srv, nullptr);
		if (stat == TextureManager::TextureStatus::eSuccess) {
			mContext.mTextures[_position] = srv.Get();
		} else {
			SystemLogger::GetError() << "[Error] Failed to get synchronous texture from TextureManager!" << std::endl;
		}
		return *this;
	}

	RenderShape& RenderShape::AddTexture(const wchar_t * _path, TextureType _position) {
		char *narrow = nullptr;
		MakeNarrow(_path, &narrow, (unsigned int)std::wcslen(_path) + 1);
		AddTexture(narrow, _position);
		delete[] narrow;
		return *this;
	}

	void RenderShape::Render(UINT _instanceCount) const {
		UINT stride = sizeof(VertexPosNormTex), offset = 0;
		Microsoft::WRL::ComPtr<ID3D11Buffer> vBuffer = VertexBufferManager::GetBuffer(eVERT_POSNORMTEX);
		Renderer::Instance()->GetContext()->IASetIndexBuffer(IndexBufferManager::GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
		Renderer::Instance()->GetContext()->IASetVertexBuffers(0, 1, vBuffer.GetAddressOf(), &stride, &offset);
		Renderer::Instance()->GetContext()->DrawIndexedInstanced(mIndexCount, _instanceCount, mIndexOffset, mVertexOffset, 0);
	}

	bool RenderShape::operator==(const RenderShape & _other) const {
		return mVertexOffset == _other.mVertexOffset &&
			mIndexOffset == _other.mIndexOffset &&
			mContext == _other.mContext;
	}

}