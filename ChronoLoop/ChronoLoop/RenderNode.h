#pragma once

namespace RenderEngine {

	struct RenderNode {
		typedef void(*NodeFunc)(RenderNode& n);
		NodeFunc mNodeFunction;
	
		RenderNode();
		RenderNode(NodeFunc _function);
	
		inline RenderNode* GetNext() { return mNext; }
		inline RenderNode& SetNext(RenderNode* n) { mNext = n; return *this; }
	protected:
		RenderNode* mNext;
	};

}