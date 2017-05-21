#include "MeshComponent.h"
#include "BaseObject.h"
#include "Component.h"
#include "../Rendering/RenderShape.h"
#include "../Rendering/Renderer.h"
#include "../Common/Settings.h"
#include "../Common/Common.h"

namespace Epoch {
	void MeshComponent::CreateOpaqueNode() {
		mNode = Renderer::Instance()->AddOpaqueNode(*mShape);
	}

	void MeshComponent::CreateTransparentNode() {
		mNode = Renderer::Instance()->AddTransparentNode(*mShape);
	}

	void MeshComponent::CreateTopmostNode() {
		mNode = Renderer::Instance()->AddTopmostNode(*mShape);
	}

	void MeshComponent::CreateMotionNode() {
		mNode = Renderer::Instance()->AddMotionNode(*mShape);
	}

	void MeshComponent::CreateNode() {
		if (GetInMotion()) {
			CreateMotionNode();
		} else if (GetTopmost()) {
			CreateTopmostNode();
		} else if (mBlended) {
			CreateTransparentNode();
		} else {
			CreateOpaqueNode();
		}
	}

	void MeshComponent::RemoveOpaqueNode() {
		DESTROY_NODE(mNode);
		Renderer::Instance()->RemoveOpaqueNode(*mShape);
	}

	void MeshComponent::RemoveTransparentNode() {
		DESTROY_NODE(mNode);
		Renderer::Instance()->RemoveTransparentNode(*mShape);
	}

	void MeshComponent::RemoveTopmostNode() {
		DESTROY_NODE(mNode);
		Renderer::Instance()->RemoveTopmostNode(*mShape);
	}

	void MeshComponent::RemoveMotionNode() {
		DESTROY_NODE(mNode);
		Renderer::Instance()->RemoveMotionNode(*mShape);
	}

	void MeshComponent::RemoveNode()
	{
		if (mNode)
		{
			if (GetInMotion())
			{
				RemoveMotionNode();
			}
			else if (GetTopmost())
			{
				RemoveTopmostNode();
			}
			else if (mBlended)
			{
				RemoveTransparentNode();
			}
			else
			{
				RemoveOpaqueNode();
			}
		}
	}

	void MeshComponent::UpdateBuffer(ConstantBufferType _t, unsigned char _index) {
		if(GetTopmost()) {
			Renderer::Instance()->UpdateTopmostNodeBuffer(*mShape, _t, _index);
		} else if (mBlended) {
			Renderer::Instance()->UpdateTransparentNodeBuffer(*mShape, _t, _index);
		} else {
			Renderer::Instance()->UpdateOpaqueNodeBuffer(*mShape, _t, _index);
		}
	}

	void MeshComponent::CreateAlphaBuffer(float alpha) {
		Renderer::Instance()->GetRendererLock().lock();
		mPixelBufferTypes[ePB_REGISTER1] = eBufferDataType_Alpha;
		ID3D11Buffer* AlphaBuff;
		CD3D11_BUFFER_DESC AlphaDesc(sizeof(PSTransparent_Data), D3D11_BIND_CONSTANT_BUFFER);
		D3D11_SUBRESOURCE_DATA BufferData;
		PSTransparent_Data a;
		a.alpha.x = alpha;
		BufferData.pSysMem = &a;
		HRESULT hr = Renderer::Instance()->GetDevice()->CreateBuffer(&AlphaDesc, &BufferData, &AlphaBuff);
		SetD3DName(AlphaBuff, (mShape->GetName() + " alpha buffer").c_str());
		mShape->GetContext().mPixelCBuffers[ePB_REGISTER1].Attach(AlphaBuff);
		Renderer::Instance()->GetRendererLock().unlock();
	}

	MeshComponent::MeshComponent(const char * _path, float _alpha) {
		mType = eCOMPONENT_MESH;
		mShape = new RenderShape(
			_path,
			true,
			(_alpha < 1.0f ? ePS_TRANSPARENT : ePS_TEXTURED),
			eVS_TEXTURED,
			eGS_PosNormTex
		);
		mShape->GetContext().mRasterState = eRS_FILLED;
		mBlended = _alpha < 1.0f;
		if (CanCreateNode()) {
			CreateNode();
			mVisible = true;
		} else {
			mVisible = true;
			mNode = nullptr;
		}

		for (int i = 0; i < eVB_MAX; ++i) {
			mVertexBufferTypes[i] = eBufferDataType_Nullptr;
		}

		for (int i = 0; i < ePB_MAX; ++i) {
			mPixelBufferTypes[i] = eBufferDataType_Nullptr;
		}

		for (int i = 0; i < eGB_MAX; ++i) {
			mGeoBufferTypes[i] = eBufferDataType_Nullptr;
		}

		CreateAlphaBuffer(_alpha);
	}

	MeshComponent::MeshComponent(const char * _path, float _alpha, PixelShaderFormat _psf, VertexShaderFormat _vsf, GeometryShaderFormat _gsf) {
		mType = eCOMPONENT_MESH;
		if (_psf == ePS_TEXTURED && _alpha < 1.0f) {
			_psf = ePS_TRANSPARENT;
		}
		mShape = new RenderShape(
			_path,
			true,
			_psf,
			_vsf,
			_gsf
		);
		mShape->GetContext().mRasterState = eRS_FILLED;
		mBlended = _alpha < 1.0f;
		if (CanCreateNode()) {
			CreateNode();
			mVisible = true;
		} else {
			mVisible = true;
			mNode = nullptr;
		}

		for (int i = 0; i < eVB_MAX; ++i) {
			mVertexBufferTypes[i] = eBufferDataType_Nullptr;
		}

		for (int i = 0; i < ePB_MAX; ++i) {
			mPixelBufferTypes[i] = eBufferDataType_Nullptr;
		}

		for (int i = 0; i < eGB_MAX; ++i) {
			mGeoBufferTypes[i] = eBufferDataType_Nullptr;
		}

		CreateAlphaBuffer(_alpha);
	}

	void MeshComponent::Update() {
		if (mNode && mVisible) {
			mNode->data = mObject->GetWorld();
		}
	}

	void MeshComponent::Destroy() {
		SystemLogger::Debug() << "Destroying shape " << mShape->GetName() << std::endl;
		RemoveNode();
		delete mShape;
	}

	MeshComponent* MeshComponent::SetVisible(bool _vis) {
		if (_vis) {
			if (!mVisible) {
				mVisible = true;
				if (mNode == nullptr && CanCreateNode()) {
					CreateNode();
				}
				Update();
			}
		} else {
			if (mVisible) {
				if (mNode) {
					memset(&mNode->data, 0, sizeof(mNode->data));
				}
				mVisible = false;
			}
		}
		return this;
	}

	MeshComponent * MeshComponent::SetAlpha(float _a) {
		if (mPixelBufferTypes[ePB_REGISTER1] != eBufferDataType_Alpha) {
			SystemLogger::Error() << "Attempted to set the alpha of a mesh whos Register 1 pixel buffer is not an alpha buffer!" << std::endl;
			return this;
		}
		if (_a >= 1.0f && mShape->GetContext().mPixelShaderFormat == ePS_TRANSPARENT) {
			SetPixelShader(ePS_TEXTURED);
			SetBlended(false);
			return this;
		} else if (_a < 1.0f && mShape->GetPixelShader() == ePS_TEXTURED) {
			SetPixelShader(ePS_TRANSPARENT);
			SetBlended(true);
			// Don't return, because we need to update the alpha of the object.
		}

		if (mShape->GetContext().mPixelShaderFormat != ePS_TRANSPARENT) {
			SystemLogger::Error() << "Attempted to set the alpha of a mesh whos pixel shader is not ePS_TRANSPARENT!" << std::endl;
			return this;
		}
		PSTransparent_Data alpha;
		alpha.alpha.x = _a;
		Renderer::Instance()->GetContext()->UpdateSubresource(mShape->GetContext().mPixelCBuffers[ePB_REGISTER1].Get(), 0, 0, &alpha, 0, 0);
		if (mBuffersCanUpdate) {
			UpdateBuffer(eCB_PIXEL, ePB_REGISTER1);
		}
		return this;
	}

	MeshComponent* MeshComponent::AddTexture(const char * _path, TextureType _type) {
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
		if (TextureManager::Instance()->iGetTexture2D(_path, &srv, nullptr) == TextureManager::TextureStatus::eSuccess) {

			// If the texture at the given path is not what is already set, remove the shape from the render set, add the
			// texture, and, if visible, reinsert the shape.
			if (srv != mShape->GetContext().mTextures[_type]) {
				RemoveNode();
				mShape->AddTexture(_path, _type);
				if (mVisible && CanCreateNode())
				{
					CreateNode();
				}
			}
		}
		return this;
	}

	void MeshComponent::SetRasterState(RasterState _t) {
		if (_t != mShape->GetContext().mRasterState) {
			RemoveNode();
			mShape->GetContext().mRasterState = _t;
			if (mVisible && CanCreateNode())
			{
				CreateNode();
			}
		}
	}

	void MeshComponent::SetVertexShader(VertexShaderFormat _vf) {
		if (_vf != mShape->GetContext().mGeoShaderFormat) {
			RemoveNode();
			mShape->GetContext().mVertexShaderFormat = _vf;
			if (mVisible && CanCreateNode())
			{
				CreateNode();
			}
		}
	}

	void MeshComponent::SetPixelShader(PixelShaderFormat _pf) {
		if (_pf != mShape->GetContext().mGeoShaderFormat) {
			RemoveNode();
			mShape->GetContext().mPixelShaderFormat = _pf;
			if (mVisible && CanCreateNode())
			{
				CreateNode();
			}
		}
	}

	void MeshComponent::SetGeometryShader(GeometryShaderFormat _gf) {
		if (_gf != mShape->GetContext().mGeoShaderFormat) {
			RemoveNode();
			mShape->GetContext().mGeoShaderFormat = _gf;
			if (mVisible && CanCreateNode())
			{
				CreateNode();
			}
		}
	}

	void MeshComponent::SetShaders(PixelShaderFormat _psf, VertexShaderFormat _vsf, GeometryShaderFormat _gsf) {
		if (_psf != mShape->GetContext().mPixelShaderFormat ||
			_vsf != mShape->GetContext().mVertexShaderFormat ||
			_gsf != mShape->GetContext().mGeoShaderFormat) {
			RemoveNode();
			mShape->GetContext().mPixelShaderFormat = _psf;
			mShape->GetContext().mVertexShaderFormat = _vsf;
			mShape->GetContext().mGeoShaderFormat = _gsf;
			if (mVisible && CanCreateNode())
			{
				CreateNode();
			}
		}
	}

	void MeshComponent::SetTopmost(bool _topmost) {
		if (GetTopmost() != _topmost) {
			RemoveNode();
			mTopmost = _topmost;
			if (IsVisible() && CanCreateNode()) {
				CreateNode();
			}
		}
	}

	void MeshComponent::SetInMotion(bool _inMotion) {
		if (mActiveRewind != _inMotion) {
			RemoveNode();
			mActiveRewind = _inMotion;
			if (mVisible && CanCreateNode())
			{
				CreateNode();
			}
		}
	}

	void MeshComponent::SetBlended(bool _ButWillItBlend) {
		if (_ButWillItBlend != mBlended)
		{
			RemoveNode();
			mBlended = _ButWillItBlend;
			CreateNode();
		}

	}

	RenderShape * MeshComponent::GetShape() {
		return mShape;
	}

	bool MeshComponent::GetBlended() {
		return mBlended;
	}

	void MeshComponent::ForceReinsertion() {
		RemoveNode();
		CreateNode();
	}

	void MeshComponent::EnableBufferUpdates(bool _updateNow) {
		mBuffersCanUpdate = true;
		if (_updateNow) {
			// Todo: only update buffers that actually changed in the future.
			for (int i = 0; i < eVB_MAX; ++i) {
				Renderer::Instance()->UpdateOpaqueNodeBuffer(*mShape, eCB_VERTEX, i);
			}

			for (int i = 0; i < eVB_MAX; ++i) {
				Renderer::Instance()->UpdateOpaqueNodeBuffer(*mShape, eCB_PIXEL, i);
			}

			for (int i = 0; i < eVB_MAX; ++i) {
				Renderer::Instance()->UpdateOpaqueNodeBuffer(*mShape, eCB_GEO, i);
			}
		}
	}

	void MeshComponent::DisableBufferUpdates() {
		mBuffersCanUpdate = false;
	}

	bool MeshComponent::GetBufferUpdates() {
		return mBuffersCanUpdate;
	}

	bool MeshComponent::GetTopmost() {
		return mTopmost;
	}

	bool MeshComponent::GetInMotion() {
		return mActiveRewind;
	}

	void MeshComponent::SetData(ConstantBufferType _t, BufferDataType _bt, unsigned char _index, void * _data) {
		Microsoft::WRL::ComPtr<ID3D11Buffer>* buff = nullptr;
		switch (_t) {
			case eCB_VERTEX:
				buff = &mShape->GetContext().mVertexCBuffers[_index];
				mVertexBufferTypes[_index] = _bt;
				break;
			case eCB_PIXEL:
				buff = &mShape->GetContext().mPixelCBuffers[_index];
				mPixelBufferTypes[_index] = _bt;
				break;
			case eCB_GEO:
				buff = &mShape->GetContext().mGeometryCBuffers[_index];
				mGeoBufferTypes[_index] = _bt;
				break;
			default:
				SystemLogger::Error() << "Could not set buffer data: Invalid ConstantBufferType given: " << _t << std::endl;
				return;
		}
		if (buff->Get() != nullptr) {
			Renderer::Instance()->GetContext()->UpdateSubresource(buff->Get(), 0, 0, _data, 0, 0);
		} else {
			Renderer::Instance()->GetRendererLock().lock();

			CD3D11_BUFFER_DESC desc(sizeof(GSAnimatedQuad_Data), D3D11_BIND_CONSTANT_BUFFER);
			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = _data;
			Renderer::Instance()->GetDevice()->CreateBuffer(&desc, &data, buff->GetAddressOf());
			switch (_t) {
				case eCB_VERTEX:
					mShape->GetContext().mVertexCBuffers[_index] = *buff;
					break;
				case eCB_PIXEL:
					mShape->GetContext().mPixelCBuffers[_index] = *buff;
					break;
				case eCB_GEO:
					mShape->GetContext().mGeometryCBuffers[_index] = *buff;
					break;
				default:
					return;
			}
			Renderer::Instance()->GetRendererLock().unlock();
		}
		if (GetBufferUpdates()) {
			UpdateBuffer(_t, _index);
		}
	}

	void MeshComponent::UpdateData(ConstantBufferType _t, unsigned char _index, void * _data) {
		ID3D11Buffer* buff = nullptr;
		switch (_t) {
			case eCB_VERTEX:
				buff = mShape->GetContext().mVertexCBuffers[_index].Get();
				break;
			case eCB_PIXEL:
				buff = mShape->GetContext().mPixelCBuffers[_index].Get();
				break;
			case eCB_GEO:
				buff = mShape->GetContext().mGeometryCBuffers[_index].Get();
				break;
			default:
				return;
		}
		Renderer::Instance()->GetContext()->UpdateSubresource(buff, 0, 0, _data, 0, 0);
		if (GetBufferUpdates()) {
			UpdateBuffer(_t, _index);
		}
	}

	bool MeshComponent::CanCreateNode() {
		return !Settings::GetInstance().GetBool("LevelIsLoading");
	}

}