#pragma once


namespace RenderEngine {
	struct RenderNode;

	class RenderSet {
		RenderNode *mHead = nullptr, *mTail = nullptr;

	public:
		void AddNode(RenderNode* node);
		void ClearSet();
		const RenderNode *GetHead();
	};
}