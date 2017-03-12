#pragma once

#include "RenderNode.h"
#include "RendererDefines.h"
#include <memory>
#include <unordered_map>
#include <wrl/client.h>
#include <d3d11.h>

namespace Epoch {

	struct RenderContext : RenderNode {
		RasterState mRasterState = eRS_MAX;
		VertFormat mVertexFormat = eVERT_MAX;
		PixelShaderFormat mPixelShaderFormat = ePS_MAX;
		VertexShaderFormat mVertexShaderFormat = eVS_MAX;
		GeometryShaderFormat mGeoShaderFormat = eGS_MAX;
		//RenderEye mEye = eEYE_MAX;
		std::unordered_map<int, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> mTextures;

		RenderContext();
		RenderContext(const RenderContext& _copy);
		~RenderContext();
		void Apply();
		void Apply(RenderContext& from);
		virtual bool operator==(RenderContext& other);
		virtual RenderContext& operator=(const RenderContext& _other);
		virtual bool operator==(const RenderContext& _other) const;
		virtual bool operator!=(RenderContext& _other);
	};

}