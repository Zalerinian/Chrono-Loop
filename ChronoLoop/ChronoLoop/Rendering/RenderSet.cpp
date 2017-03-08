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
		auto location = std::find(mKeys.begin(), mKeys.end(), _shape);
		if (location == mKeys.end()) {
			mKeys.push_back(_shape);
		}
		static int ImLazy = 0;
		SystemLogger::Debug() << "Added object for a total of " << ++ImLazy << " additions. The list currently has " << (mSize + 1) << " nodes." << std::endl;
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
		return mRenderShapes[_shape].Push(_shape.mPosition);
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

	void RenderSet::SortNodes() {
		// TODO make a better sorting algorithm, becuase this probably sucks.
		//std::sort(mKeys.begin(), mKeys.end());
	}

	RenderSet::Iterator RenderSet::Begin() {
		Iterator it(this);
		it.mIndex = 0;
		return it;
	}

	RenderSet::Iterator RenderSet::End() {
		Iterator it(this);
		it.mIndex = (unsigned int)mKeys.size();
		return it;
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


	// Iterator code!
	// --Prefix
	RenderSet::Iterator& RenderSet::Iterator::operator--() {
		--mIndex;
		return *this;
	}

	// ++Prefix
	RenderSet::Iterator& RenderSet::Iterator::operator++() {
		++mIndex;
		return *this;
	}

	// Postfix--
	RenderSet::Iterator RenderSet::Iterator::operator--(int) {
		RenderSet::Iterator i(mSet);
		i.mIndex = mIndex--;
		return i;
	}

	// Postfix++
	RenderSet::Iterator RenderSet::Iterator::operator++(int) {
		RenderSet::Iterator i(mSet);
		i.mIndex = mIndex++;
		return i;
	}

	// Get the RenderShape key.
	RenderShape& RenderSet::Iterator::operator*() {
		return mSet->mKeys[mIndex];
	}

	// Get the Ghostlist Value.
	GhostList<matrix4>& RenderSet::Iterator::operator()(RenderShape& _shape) {
		return mSet->mRenderShapes[_shape];
	}
	bool RenderSet::Iterator::operator==(const Iterator & _it) {
		return this->mIndex == _it.mIndex && this->mSet == _it.mSet;
	}

	bool RenderSet::Iterator::operator!=(const Iterator & _it) {
		return this->mIndex != _it.mIndex || this->mSet != _it.mSet;
	}
}