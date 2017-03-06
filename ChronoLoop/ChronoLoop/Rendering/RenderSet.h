#pragma once
#include <vector>
#include <map>
#include <unordered_set>
#include "RenderShape.h"
#include "../Common/GhostList.h"

namespace Epoch {
	struct RenderNode;
	struct RenderContext;

	const unsigned int RenderStage_Shadows = 1,
		RenderStage_Diffuse = 2,
		RenderStage_PostProcess = 4;

	class RenderSet {

		std::map<RenderShape, GhostList<matrix4>> mRenderShapes;
		std::unordered_set<RenderShape> mShadowSet;
		std::unordered_set<RenderShape> mDiffuseSet;
		std::unordered_set<RenderShape> mPostSet;

		RenderNode *mHead = nullptr, *mTail = nullptr;
		unsigned int mSize = 0;
		std::vector<RenderContext*> mContexts;

	public:
		GhostList<matrix4>::GhostNode* AddNode(RenderShape& _shape, unsigned int _rs = RenderStage_Shadows | RenderStage_Diffuse);
		void AddNode(RenderNode* _node, RenderContext *_rc);
		void RemoveNode(RenderNode* _node);
		void RemoveContext(std::vector<RenderContext*>::iterator _it);
		void RemoveShape(RenderShape* _node);
		void ClearSet();
		const RenderNode *GetHead();
		std::map<RenderShape, GhostList<matrix4>>::iterator Begin();
		std::map<RenderShape, GhostList<matrix4>>::iterator End();

		~RenderSet();
	};
}