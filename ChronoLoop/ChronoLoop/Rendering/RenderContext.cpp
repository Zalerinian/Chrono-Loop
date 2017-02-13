//#include "stdafx.h"
#include "RenderContext.h"
#include "renderer.h"
#include "RasterizerStateManager.h"
#include "ShaderManager.h"
#include "InputLayoutManager.h"

RenderEngine::RenderContext::RenderContext() {
	mType = RenderNodeType::Context;
	//	mTextures.insert(std::pair<int, ID3D11ShaderResourceView*>(1, nullptr));
}

RenderEngine::RenderContext::~RenderContext() {}

void RenderEngine::RenderContext::Apply() {
	RasterizerStateManager::Instance()->ApplyState(mRasterState);
	InputLayoutManager::Instance().ApplyLayout(mVertexFormat);
	ShaderManager::Instance()->ApplyPShader(mPixelShaderFormat);
	ShaderManager::Instance()->ApplyVShader(mVertexShaderFormat);
	for (auto it = mTextures.begin(); it != mTextures.end(); ++it) {
		if (it->second.get() != nullptr) {
			(*Renderer::Instance()->GetContext())->PSSetShaderResources((UINT)it->first, 1, it->second.get());
			//(*Renderer::Instance()->GetContext())->PSSetSamplers((UINT)it->first, 1, nullptr); //TODO: Create a Sampler state and set it here.
		}
	}
}

void RenderEngine::RenderContext::Apply(RenderContext & from) {
	if (mRasterState != from.mRasterState) {
		RasterizerStateManager::Instance()->ApplyState(mRasterState);
	}
	if (mVertexFormat != from.mVertexFormat) {
		InputLayoutManager::Instance().ApplyLayout(mVertexFormat);
	}
	if (mPixelShaderFormat != from.mPixelShaderFormat) {
		ShaderManager::Instance()->ApplyPShader(mPixelShaderFormat);
	}
	if (mVertexShaderFormat != from.mVertexShaderFormat) {
		ShaderManager::Instance()->ApplyVShader(mVertexShaderFormat);
	}
	for (auto it = mTextures.begin(); it != mTextures.end(); ++it) {
		if (it->second.get() != nullptr && from.mTextures[it->first].get() != it->second.get()) {
			(*Renderer::Instance()->GetContext())->PSSetShaderResources((UINT)it->first, 1, it->second.get());
		}
	}
}

bool RenderEngine::RenderContext::operator==(RenderContext & other) {
	for (int i = eTEX_DIFFUSE; i < eTEX_MAX; ++i) {
		std::shared_ptr<ID3D11ShaderResourceView*> srv = other.mTextures[i];
		if (srv.get() == nullptr || srv.get() == this->mTextures[i].get()) {
			// If the textures aren't different, or aren't used, we can continue on.
			continue;
		}
		return false;
	}

	if (mRasterState != other.mRasterState ||
		mVertexFormat != other.mVertexFormat ||
		/*mEye != other.mEye ||*/
		false) {
		return false;
	}

	return true;
}

