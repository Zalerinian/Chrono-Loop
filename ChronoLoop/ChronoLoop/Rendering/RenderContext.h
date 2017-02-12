#pragma once

#include "RenderNode.h"
#include "RendererDefines.h"
#include <memory>
#include <unordered_map>

struct ID3D11ShaderResourceView;

namespace RenderEngine {

	struct RenderContext : RenderNode {
		RasterState mRasterState = eRS_MAX;
		VertFormat mVertexFormat = eVERT_MAX;
		PixelShaderFormat mPixelShaderFormat = ePS_MAX;
		VertexShaderFormat mVertexShaderFormat = eVS_MAX;
		//RenderEye mEye = eEYE_MAX;
		std::unordered_map<int, std::shared_ptr<ID3D11ShaderResourceView*>> mTextures;

		RenderContext();
		~RenderContext();
		bool operator==(RenderContext& other);
	};

}