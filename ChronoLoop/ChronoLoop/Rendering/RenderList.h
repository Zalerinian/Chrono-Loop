#pragma once
#include <vector>
#include "RenderShape.h"
#include "../Common/GhostList.h"

namespace Epoch {

	struct matrix4;

	struct RenderList {
		RenderShape mShape;
		GhostList<matrix4> mPositions;

		GhostList<matrix4>::GhostNode* Push(matrix4& _m);
	};

} // Epoch Namespace