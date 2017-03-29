#include "TransparentMeshComponent.h"
#include "../Rendering/Renderer.h"
#include "../Common/Common.h"

namespace Epoch {

	void TransparentMeshComponent::CreateNode()
	{
		DESTROY_NODE(mNode);
		mNode = Renderer::Instance()->AddTransparentNode(mShape);
	}

	void TransparentMeshComponent::CreateAlphaBuffer()
	{
		Renderer::Instance()->GetRendererLock().lock();
		ID3D11Buffer* AlphaBuff;
		CD3D11_BUFFER_DESC AlphaDesc(sizeof(float) * 4, D3D11_BIND_CONSTANT_BUFFER);
		D3D11_SUBRESOURCE_DATA AlphaData;
		float alpha = 1.0f;
		AlphaData.pSysMem = &alpha;
		auto device = Renderer::Instance()->GetDevice();
		HRESULT hr = device->CreateBuffer(&AlphaDesc, &AlphaData, &AlphaBuff);
		SetD3DName(AlphaBuff, (mShape->GetName() + " alpha buffer").c_str());
		mShape->GetContext().mPixelCBuffers[ePB_TP_Alpha].Attach(AlphaBuff);
		Renderer::Instance()->GetRendererLock().unlock();
	}

	TransparentMeshComponent::TransparentMeshComponent(const char * _file, float _alpha) : MeshComponent(_file)
	{
		mAlpha = _alpha;
		mShape->SetPixelShader(ePS_TRANSPARENT);
		DESTROY_NODE(mNode);
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
		return mAlpha;
	}

	void TransparentMeshComponent::SetAlpha(float _a)
	{
		SystemLogger::Debug() << "Setting alpha of " << std::hex << (int)this << std::dec << " to: " << _a << std::endl;
		mAlpha = _a;
		auto buffer = mShape->GetContext().mPixelCBuffers[ePB_TP_Alpha];
		Renderer::Instance()->GetContext()->UpdateSubresource(buffer.Get(), 0, NULL, &mAlpha, 0, 0);
	}

} // Epoch namespace