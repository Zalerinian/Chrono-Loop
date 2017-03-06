#pragma once
#include <vector>

namespace Epoch {
	struct RenderNode;
	struct RenderContext;
	struct RenderShape;

	class RenderSet {
		RenderNode *mHead = nullptr, *mTail = nullptr;
		unsigned int mSize = 0;
		std::vector<RenderContext*> mContexts;

	public:
		//RenderSet(); // Maybe have a constructor that reserves a smallish number of renderer contexts so it doesn't have to expand on demand?
		void AddNode(RenderNode* _node, RenderContext *_rc);
		void RemoveNode(RenderNode* _node);
		void RemoveContext(std::vector<RenderContext*>::iterator _it);
		void RemoveShape(RenderShape* _node);
		void ClearSet();
		const RenderNode *GetHead();

		~RenderSet();
	};
}