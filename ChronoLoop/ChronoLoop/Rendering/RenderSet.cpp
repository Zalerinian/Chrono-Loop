#include "stdafx.h"
#include "RenderSet.h"
#include "RenderNode.h"

namespace RenderEngine {


void RenderSet::AddNode(RenderNode *node) {
	if (mTail) {
		mTail->mNext = node;
		mTail = node;
		node->mNext = nullptr;
	} else {
		mHead = mTail = node;
		node->mNext = nullptr;
	}
}

void RenderSet::ClearSet() {
	mHead = mTail = nullptr;
}

const RenderNode * RenderSet::GetHead() {
	return mHead;
}

}