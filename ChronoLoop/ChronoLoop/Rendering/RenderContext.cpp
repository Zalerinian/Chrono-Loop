//#include "stdafx.h"
#include "RenderContext.h"

RenderEngine::RenderContext::RenderContext()
{
//	mTextures.insert(std::pair<int, ID3D11ShaderResourceView*>(1, nullptr));
}

RenderEngine::RenderContext::~RenderContext()
{
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

	if(mRasterState != other.mRasterState ||
		 mVertexFormat != other.mVertexFormat ||
		 /*mEye != other.mEye ||*/
		 false) {
		return false;
	}

	return true;
}

