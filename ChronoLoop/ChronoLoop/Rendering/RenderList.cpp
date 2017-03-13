#include "RenderList.h"

namespace Epoch {


	GhostList<matrix4>::GhostNode* RenderList::Push(matrix4 & _m) {
		return mPositions.Push(_m);
	}

}
