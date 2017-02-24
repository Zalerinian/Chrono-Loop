//#include "stdafx.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "RasterizerStateManager.h"
#include "ShaderManager.h"
#include "InputLayoutManager.h"

RenderEngine::RenderContext::RenderContext() {
	mType = RenderNodeType::Context;
	mName = "A rendering context.";
	//	mTextures.insert(std::pair<int, ID3D11ShaderResourceView*>(1, nullptr));
}

RenderEngine::RenderContext::~RenderContext() {}

void RenderEngine::RenderContext::Apply() {
	if (mRasterState != eRS_MAX) {
		RasterizerStateManager::Instance()->ApplyState(mRasterState);
	}
	if (mVertexFormat != eVERT_MAX) {
		InputLayoutManager::Instance().ApplyLayout(mVertexFormat);
	}
	if (mPixelShaderFormat != ePS_MAX) {
		ShaderManager::Instance()->ApplyPShader(mPixelShaderFormat);
	}
	if (mVertexShaderFormat != eVS_MAX) {
		ShaderManager::Instance()->ApplyVShader(mVertexShaderFormat);
	}
	for (auto it = mTextures.begin(); it != mTextures.end(); ++it) {
		if (it->second.get() != nullptr) {
			(*Renderer::Instance()->iGetContext())->PSSetShaderResources((UINT)it->first, 1, it->second.get());
			//(*Renderer::Instance()->iGetContext())->PSSetSamplers((UINT)it->first, 1, nullptr); //TODO: Consider adding samplers to contexts. Curently a global sampler is applied in the renderer.
		}
	}
}

void RenderEngine::RenderContext::Apply(RenderContext & from) {
	if (mRasterState != eRS_MAX && mRasterState != from.mRasterState) {
		RasterizerStateManager::Instance()->ApplyState(mRasterState);
	}
	if (mVertexFormat != eVERT_MAX && mVertexFormat != from.mVertexFormat) {
		InputLayoutManager::Instance().ApplyLayout(mVertexFormat);
	}
	if (mPixelShaderFormat != ePS_MAX && mPixelShaderFormat != from.mPixelShaderFormat) {
		ShaderManager::Instance()->ApplyPShader(mPixelShaderFormat);
	}
	if (mVertexShaderFormat != eVS_MAX && mVertexShaderFormat != from.mVertexShaderFormat) {
		ShaderManager::Instance()->ApplyVShader(mVertexShaderFormat);
	}
	for (auto it = mTextures.begin(); it != mTextures.end(); ++it) {
		if (it->second.get() != nullptr && from.mTextures[it->first].get() != it->second.get()) {
			(*Renderer::Instance()->iGetContext())->PSSetShaderResources((UINT)it->first, 1, it->second.get());
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
			mPixelShaderFormat != other.mPixelShaderFormat ||
			mVertexShaderFormat != other.mVertexShaderFormat) {
		return false;
	}

	return true;
}

