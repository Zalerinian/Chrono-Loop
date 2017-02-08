#include "stdafx.h"
#include "RenderNode.h"

RenderNode::RenderNode() {
	mNext = nullptr;
	mNodeFunction = nullptr;
}

RenderNode::RenderNode(NodeFunc _func) {
	mNext = nullptr;
	mNodeFunction = _func;
}