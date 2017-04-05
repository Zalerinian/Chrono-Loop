#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "RenderShape.h"
#include "RenderList.h"
#include <list>

namespace Epoch {

	class RenderSet {
		unsigned int mSize = 0;
		std::list<RenderList*> mRenderList;

	public:
		GhostList<matrix4>::GhostNode* AddShape(RenderShape& _shape);
		void RemoveShape(RenderShape& _shape);
		void ClearSet();
		RenderList* GetListForShape(RenderShape& _shape);
		//void SortNodes();
		std::list<RenderList*>::iterator Begin();
		std::list<RenderList*>::iterator End();
		
		~RenderSet();
	};
}