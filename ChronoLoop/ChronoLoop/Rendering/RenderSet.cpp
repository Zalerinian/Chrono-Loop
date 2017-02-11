//#include "stdafx.h"
#include "RenderSet.h"
#include "RenderNode.h"
#include "RenderContext.h"

namespace RenderEngine {
	void RenderSet::AddNode(RenderNode *node, RenderContext* rc) {
		if (mTail = nullptr) {
			mContexts.push_back(rc);
			mHead = mTail = (RenderNode*)rc;
			rc->mNext = node;
			node->mNext = nullptr;
		}
	}
	
	void RenderSet::ClearSet() {
		mHead = mTail = nullptr;
		mContexts.clear();
	}
	
	const RenderNode * RenderSet::GetHead() {
		return mHead;
	}
}