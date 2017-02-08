#include "stdafx.h"
#include "RenderContext.h"

bool RenderEngine::RenderContext::operator==(RenderContext & other) {
	if(mRasterState != other.mRasterState ||
		 mVertexFormat != other.mVertexFormat) {
		return false;
	}

	return true;
}

