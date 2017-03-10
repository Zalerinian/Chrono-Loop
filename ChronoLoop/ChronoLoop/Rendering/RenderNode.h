#pragma once
#include <string>

namespace Epoch {

	class RenderSet;

	struct RenderNode {
	/*	typedef void(*NodeFunc)(RenderNode& n);
		NodeFunc mNodeFunction;
	*/
		RenderNode();
		//RenderNode(NodeFunc _function);
		enum class RenderNodeType {
			Node,
			Context,
			Shape
		} mType = RenderNodeType::Node;
	
		inline RenderNode* GetNext() const { return mNext; }
		inline RenderNode* GetPrevious() const { return mPrevious; }
		inline RenderNode& SetNext(RenderNode* n) { mNext = n; return *this; }
		inline RenderNode& SetPrevious(RenderNode *n) { mPrevious = n; return *this; }
	protected:
		std::string mName;
		RenderNode* mNext, *mPrevious;
		friend RenderSet;
	};

}