//#include "stdafx.h"
#include "RenderSet.h"
#include "RenderNode.h"
#include "RenderContext.h"
#include "RenderShape.h"
#include "../Common/Breakpoint.h"
#include "../Common/Logger.h"
#include <algorithm>

namespace Epoch {

	GhostList<matrix4>::GhostNode* RenderSet::AddNode(RenderShape& _shape, unsigned int _rs) {
		GhostList<matrix4>::GhostNode* node = mRenderShapes[_shape].Push(_shape.mPosition);
		static int ImLazy = 0;
		SystemLogger::Debug() << "Added object for a total of " << ++ImLazy << " additions. The list is currently " << mSize << " nodes in size." << std::endl;
		// This shape casts shadows.
		if (_rs & RenderStage_Shadows) {
			mShadowSet.insert(_shape);
		}

		// This shape draws onto the screen normally.
		if (_rs & RenderStage_Diffuse) {
			mDiffuseSet.insert(_shape);
		}

		// This shape draws only at the post processing stage.
		// I don't even know what to put here :|
		if (_rs & RenderStage_PostProcess) {
			mPostSet.insert(_shape);
		}
		return node;
	}

	void RenderSet::AddNode(RenderNode *_node, RenderContext* _rc) {
		if (_node->mType == RenderNode::RenderNodeType::Shape) {
			if (((RenderShape*)_node)->mIndexCount == 0) {
				SystemLogger::GetError() << "[Error] Attempting to set render shape with 0 indices!" << std::endl;
				Debug::SetBreakpoint();
			}
		}
		if (mTail == nullptr) {
			mContexts.push_back(_rc);
			mHead = (RenderNode*)_rc;
			mTail = _node;
			_rc->mNext = _node;
			_rc->mPrevious = nullptr;
			_node->mNext = nullptr;
			_node->mPrevious = _rc;
		} else {
			bool foundContext = false;
			for (auto it = mContexts.begin(); it != mContexts.end(); ++it) {
				if (*(*it) == *_rc) {
					foundContext = true;
					_node->mNext = (*it)->mNext;
					_node->mPrevious = (*it);
					_node->mNext->mPrevious = _node;
					(*it)->mNext = _node;
					break;
				}
			}
			if (!foundContext) {
				// Insert a new context and mesh.
				_node->mNext = nullptr;
				_node->mPrevious = _rc;
				_rc->mNext = _node;
				_rc->mPrevious = mTail;
				mTail->mNext = _rc;
				mTail = _node;
				mContexts.push_back(_rc);
			}
		}
		mSize++;
	}

	void RenderSet::RemoveNode(RenderNode * _node) {
		// The render set does NOT delete anything, it is not technically a container.
		// It simply
		RenderNode *cur = mHead;
		while (cur != nullptr) {
			if (cur == _node) {
				if (cur->mPrevious) {
					cur->mPrevious->mNext = cur->mNext;
					if (cur->mNext) {
						cur->mNext->mPrevious = cur->mPrevious;
					}
				} else {
					mHead = cur->mNext;
					if (mHead) {
						mHead->mPrevious = nullptr;
					} else {
						mTail = nullptr;
					}
				}
				break;
			}
			cur = cur->mNext;
		}
	}

	void RenderSet::RemoveContext(std::vector<RenderContext*>::iterator _it) {
		RenderContext* rc = (*_it);
		mContexts.erase(_it);
		RemoveNode(rc);
	}

	void RenderSet::RemoveShape(RenderShape *_node) {
		auto it = std::find(mContexts.begin(), mContexts.end(), &_node->mContext);
		if (it != mContexts.end()) {
			// This shape's context is in the list
			RemoveContext(it);
		}
		RemoveNode(_node);
		mSize--;
	}
	
	void RenderSet::ClearSet() {
		mHead = mTail = nullptr;
		mContexts.clear();
	}
	
	const RenderNode * RenderSet::GetHead() {
		return mHead;
	}

	std::map<RenderShape, GhostList<matrix4>>::iterator RenderSet::Begin() {
		return mRenderShapes.begin();
	}

	std::map<RenderShape, GhostList<matrix4>>::iterator RenderSet::End() {
		return mRenderShapes.end();
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