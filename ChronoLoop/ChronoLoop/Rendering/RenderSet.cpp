//#include "stdafx.h"
#include "RenderSet.h"
#include "RenderNode.h"
#include "RenderContext.h"
#include "RenderShape.h"
#include "../Common/Breakpoint.h"
#include "../Common/Logger.h"
#include <algorithm>

namespace Epoch {
	GhostList<matrix4>::GhostNode* RenderSet::AddShape(RenderShape & _shape) {
		for (auto it = mRenderList.begin(); it != mRenderList.end(); ++it) {
			if ((*it)->mShape == _shape) {
				// Same mesh and context
				return (*it)->Push(_shape.mPosition);
			}
		}
		// No comparable rneder list was found. Make a new entry.
		RenderList *r = new RenderList;
		r->mShape = _shape;
		GhostList<matrix4>::GhostNode* n = r->Push(_shape.mPosition);
		mRenderList.push_back(r);
		return n;
	}

	void RenderSet::ClearSet()
	{
		for (auto it = mRenderList.begin(); it != mRenderList.end(); ++it) {
			delete *it;
		}
	}


	std::list<RenderList*>::iterator RenderSet::Begin() {
		return mRenderList.begin();
	}

	std::list<RenderList*>::iterator RenderSet::End() {
		return mRenderList.end();
	}

	RenderSet::~RenderSet() {
		for (auto it = mRenderList.begin(); it != mRenderList.end(); ++it) {
			if ((*it)->mPositions.GetSize() > 0) {
				SystemLogger::Warn() << "Looks like there are still nodes in the render list?" << std::endl;
			}
			delete *it;
		}
	}

} // Epoch Namespace