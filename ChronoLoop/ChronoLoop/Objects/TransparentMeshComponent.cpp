#include "TransparentMeshComponent.h"
#include "../Rendering/Renderer.h"
#include "../Common/Common.h"

namespace Epoch {

	void TransparentMeshComponent::CreateNode()
	{
		if (IsOpaque()) {
			mNode = Renderer::Instance()->AddOpaqueNode(*mShape);
		} else {
			mNode = Renderer::Instance()->AddTransparentNode(*mShape);
		}
	}

	void TransparentMeshComponent::RemoveShape() {
		DESTROY_NODE(mNode);
		if (IsOpaque()) {
			RemoveOpaqueShape();
		} else {
			RemoveTransparentShape();
		}
	}

	void TransparentMeshComponent::RemoveTransparentShape() {
		Renderer::Instance()->RemoveTransparentNode(*mShape);
	}

	void TransparentMeshComponent::RemoveOpaqueShape() {
		Renderer::Instance()->RemoveOpaqueNode(*mShape);
	}

	void TransparentMeshComponent::CreateAlphaBuffer()
	{
		Renderer::Instance()->GetRendererLock().lock();
		ID3D11Buffer* AlphaBuff;
		CD3D11_BUFFER_DESC AlphaDesc(sizeof(AlphaData), D3D11_BIND_CONSTANT_BUFFER);
		D3D11_SUBRESOURCE_DATA BufferData;
		BufferData.pSysMem = &mAlpha;
		HRESULT hr = Renderer::Instance()->GetDevice()->CreateBuffer(&AlphaDesc, &BufferData, &AlphaBuff);
		SetD3DName(AlphaBuff, (mShape->GetName() + " alpha buffer").c_str());
		mShape->GetContext().mPixelCBuffers[ePB_REGISTER1].Attach(AlphaBuff);
		Renderer::Instance()->GetRendererLock().unlock();
	}

	TransparentMeshComponent::TransparentMeshComponent(const char * _file, float _alpha) : MeshComponent(_file)
	{
		mAlpha.alpha.x = _alpha;
		mShape->SetPixelShader(ePS_TRANSPARENT);
		RemoveShape();
		if (CanCreateNode()) {
			CreateNode();
			mVisible = true;
		}
		else {
			mVisible = false;
			mNode = nullptr; // For good measure.
		}
		CreateAlphaBuffer();
	}

	float TransparentMeshComponent::GetAlpha()
	{
		return mAlpha.alpha.x;
	}

	void TransparentMeshComponent::SetAlpha(float _a)
	{
		//return;
		SystemLogger::Debug() << "Setting alpha of " << std::hex << (int)this << std::dec << " to: " << _a << std::endl;
		float PrevAlpha = mAlpha.alpha.x;
		mAlpha.alpha.x = _a;
		auto buffer = mShape->GetContext().mPixelCBuffers[ePB_REGISTER1];
		Renderer::Instance()->GetContext()->UpdateSubresource(buffer.Get(), 0, NULL, &mAlpha, 0, 0);
		if (PrevAlpha >= 1.0f) {
			Renderer::Instance()->UpdateOpaqueNodeBuffer(*mShape, eCB_PIXEL, ePB_REGISTER1);
		} else {
			Renderer::Instance()->UpdateTransparentNodeBuffer(*mShape, eCB_PIXEL, ePB_REGISTER1);
		}
	}

	bool TransparentMeshComponent::IsOpaque() {
		//mWasOpaque = mOpaque;
		//mOpaque = mAlpha >= 1.0f;
		//return mOpaque;
		return mAlpha.alpha.x >= 1.0f;
	}

} // Epoch namespace