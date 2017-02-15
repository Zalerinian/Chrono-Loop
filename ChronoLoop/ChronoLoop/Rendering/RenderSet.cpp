//#include "stdafx.h"
#include "RenderSet.h"
#include "RenderNode.h"
#include "RenderContext.h"
#include "RenderShape.h"
#include "../Common/Breakpoint.h"
#include "../Common/Logger.h"

namespace RenderEngine {
	void RenderSet::AddNode(RenderNode *node, RenderContext* rc) {
		if (node->mType == RenderNode::RenderNodeType::Shape) {
			if (((RenderShape*)node)->mIndexCount == 0) {
				OutputDebugString(L"Attempting to set render shape with 0 indices!\n");
				Debug::SetBreakpoint();
			}
		}
		if (mTail == nullptr) {
			mContexts.push_back(rc);
			mHead = (RenderNode*)rc;
			mTail = node;
			rc->mNext = node;
			node->mNext = nullptr;
			mContexts.push_back(rc);
		} else {
			bool foundContext = false;
			for (auto it = mContexts.begin(); it != mContexts.end(); ++it) {
				if (*(*it) == *rc) {
					foundContext = true;
					node->mNext = (*it)->mNext;
					(*it)->mNext = node;
					break;
				}
			}
			if (!foundContext) {
				// Insert a new context and mesh.
				rc->mNext = node;
				mTail->mNext = rc;
				mTail = node;
			}
		}
	}
	
	void RenderSet::ClearSet() {
		mHead = mTail = nullptr;
		mContexts.clear();
	}
	
	const RenderNode * RenderSet::GetHead() {
		return mHead;
	}

	RenderSet::~RenderSet() {
		mContexts.clear();
		while (mHead) {
			RenderNode *n = mHead;
			mHead = mHead->mNext;
			//delete n;
		}
	}

}