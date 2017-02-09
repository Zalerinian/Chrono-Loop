#pragma once

#include "RenderNode.h"
#include "RendererDefines.h"


namespace RenderEngine {

	struct RenderContext : RenderNode {
		
		RasterState mRasterState;
		VertFormat mVertexFormat;

		bool operator==(RenderContext& other);
	};

}