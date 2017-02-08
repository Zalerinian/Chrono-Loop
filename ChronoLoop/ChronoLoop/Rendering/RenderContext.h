#pragma once

#include "RenderNode.h"
#include "RendererDefines.h"


namespace RenderEngine {

	struct RenderContext : RenderNode {
		
		RasterState mRasterState;

		bool operator==(RenderContext& other);
	};

}