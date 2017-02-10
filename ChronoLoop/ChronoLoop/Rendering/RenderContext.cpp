#include "stdafx.h"
#include "RenderContext.h"

bool RenderEngine::RenderContext::operator==(RenderContext & other) {
	if(mRasterState != other.mRasterState ||
		 mVertexFormat != other.mVertexFormat ||
		 mEye != other.mEye) {
		return false;
	}

	return true;
}

