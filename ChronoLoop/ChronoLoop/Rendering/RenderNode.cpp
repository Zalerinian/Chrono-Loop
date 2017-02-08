#include "stdafx.h"
#include "RenderNode.h"

namespace RenderEngine {

	RenderNode::RenderNode() {
		mNext = nullptr;
		mNodeFunction = nullptr;
	}
	
	RenderNode::RenderNode(NodeFunc _func) {
		mNext = nullptr;
		mNodeFunction = _func;
	}
}