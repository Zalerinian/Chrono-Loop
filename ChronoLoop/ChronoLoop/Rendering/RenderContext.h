#pragma once

#include "RenderNode.h"
#include "RendererDefines.h"


namespace RenderEngine {

	struct RenderContext : RenderNode {
		
		RasterState mRasterState = eRS_MAX;
		VertFormat mVertexFormat = eVERT_MAX;
		RenderEye mEye = eEYE_MAX;

		bool operator==(RenderContext& other);
	};

}