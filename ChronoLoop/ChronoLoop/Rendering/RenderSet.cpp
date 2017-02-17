//#include "stdafx.h"
#include "RenderSet.h"
#include "RenderNode.h"
#include "RenderContext.h"
#include "RenderShape.h"
#include "../Common/Breakpoint.h"
#include "../Common/Logger.h"

namespace RenderEngine {
	void RenderSet::AddNode(RenderNode *_node, RenderContext* _rc) {
		if (_node->mType == RenderNode::RenderNodeType::Shape) {
			if (((RenderShape*)_node)->mIndexCount == 0) {
//				OutputDebugString(L"Attempting to set render shape with 0 indices!\n");
				Debug::SetBreakpoint();
			}
		}
		if (mTail == nullptr) {
			mContexts.push_back(_rc);
			mHead = (RenderNode*)_rc;
			mTail = _node;
			_rc->mNext = _node;
			_node->mNext = nullptr;
			mContexts.push_back(_rc);
		} else {
			bool foundContext = false;
			for (auto it = mContexts.begin(); it != mContexts.end(); ++it) {
				if (*(*it) == *_rc) {
					foundContext = true;
					_node->mNext = (*it)->mNext;
					(*it)->mNext = _node;
					break;
				}
			}
			if (!foundContext) {
				// Insert a new context and mesh.
				_rc->mNext = _node;
				mTail->mNext = _rc;
				mTail = _node;
			}
		}
	}

	void RenderSet::RemoveShape(RenderShape *_node) {
		// The render set does NOT delete anything, it is not technically a container.
		// It simply
		RenderNode *parent = nullptr, *grandparent = nullptr, *cur = mHead;
		while (cur != nullptr) {
			if (cur == _node) {
				if (parent) {
					if (parent->mType == RenderNode::RenderNodeType::Context && cur->mNext && cur->mNext->mType == RenderNode::RenderNodeType::Context) {
						// This is the only shape with this context.
						if (grandparent) {
							grandparent->mNext = cur->mNext;
						} else {
							mHead = cur->mNext;
						}
					} else {
						// There are more shapes with the current context, only remove this shape.
						parent->mNext = cur->mNext;
					}
				} else {
					mHead = cur->mNext;
				}
			}
			if (parent) {
				grandparent = parent;
			}
			parent = cur;
			cur = cur->mNext;
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