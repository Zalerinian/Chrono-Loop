#pragma once
#include <vector>
#include <unordered_map>
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
		std::unordered_map<RenderShape, GhostList<matrix4>> mRenderShapes;
		std::unordered_set<RenderShape> mShadowSet;
		std::unordered_set<RenderShape> mDiffuseSet;
		std::unordered_set<RenderShape> mPostSet;

		RenderNode *mHead = nullptr, *mTail = nullptr;
		unsigned int mSize = 0;
		std::vector<RenderContext*> mContexts;
		std::vector<RenderShape> mKeys;

	public:
		class Iterator {
			unsigned int mIndex = 0;
			RenderSet* mSet = nullptr;
			friend RenderSet;

			Iterator(RenderSet* _set) : mSet(_set) {}
		public:
			Iterator& operator++(); // ++Prefix
			Iterator& operator--(); // --Prefix
			Iterator  operator++(int); // Postfix++
			Iterator  operator--(int); // Postfix--
			RenderShape& operator*(); // Get the key at this index in the vector
			GhostList<matrix4>& operator()(RenderShape& _shape); // Get the ghost list for the given shape.
			bool operator==(const Iterator& _it);
			bool operator!=(const Iterator& _it);
		};

		GhostList<matrix4>::GhostNode* AddNode(RenderShape& _shape, unsigned int _rs = RenderStage_Shadows | RenderStage_Diffuse);
		void AddNode(RenderNode* _node, RenderContext *_rc);
		void RemoveNode(RenderNode* _node);
		void RemoveContext(std::vector<RenderContext*>::iterator _it);
		void RemoveShape(RenderShape* _node);
		void ClearSet();
		const RenderNode *GetHead();
		void SortNodes();
		Iterator Begin();
		Iterator End();
		
		~RenderSet();
	};
}