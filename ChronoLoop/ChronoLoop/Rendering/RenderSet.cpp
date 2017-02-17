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

	void RenderSet::RemoveNode(RenderNode * _node) {
		// The render set does NOT delete anything, it is not technically a container.
		// It simply
		RenderNode *parent = nullptr, *cur = mHead;
		while (cur != nullptr) {
			if (cur == _node) {
				if (_node->mType == RenderNode::RenderNodeType::Context) {
					if (_node->mNext && _node->mNext->mType == RenderNode::RenderNodeType::Shape) {
						// If this is a context, and the next node is a shape, replace this node with their context (via parent pointer or mHead).
						if (parent) {
							((RenderShape*)_node->mNext)->mContext.mNext = _node->mNext;
							parent->mNext = &((RenderShape*)_node->mNext)->mContext;
						} else {
							((RenderShape*)_node->mNext)->mContext.mNext = _node->mNext;
							mHead = _node->mNext;
						}
					} else if (_node->mNext && _node->mNext->mType == RenderNode::RenderNodeType::Context) {
						// This is a context and the next one's a context, this can just be trivially removed.
						if (parent) {
							parent->mNext = _node->mNext;
						} else {
							mHead = _node->mNext;
						}
					}
				} else {
					// Nodes that aren't contexts can be easily removed.
					if (parent) {
						parent->mNext = cur->mNext;
					} else {
						mHead = cur->mNext;
					}
				}
				break;
			}
			parent = cur;
			cur = cur->mNext;
		}
	}

	void RenderSet::RemoveShape(RenderShape *_node) {
		RemoveNode(_node);
		RemoveNode(&_node->mContext);
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
			SystemLogger::GetError() << "[Warning] Nodes are being removed from the RenderSet in its destructor. This means nodes are no longer being rendered, but are still in memory, likely implying a leak somewhere." << std::endl;
			RenderNode *n = mHead;
			mHead = mHead->mNext;
			//delete n;
		}
	}

}