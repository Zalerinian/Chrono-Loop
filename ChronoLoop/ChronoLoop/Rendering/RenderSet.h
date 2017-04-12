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
		// ********************************************************************************
		/// <summary>
		/// Removes elements from the list that have a size of 0, meaning that no shape
		/// matches that entry anymore.
		/// </summary>
		/// <created>Drew,4/9/2017</created>
		/// <changed>Drew,4/9/2017</changed>
		// ********************************************************************************
		void Prune();
		RenderList* GetListForShape(RenderShape& _shape);
		//void SortNodes();
		std::list<RenderList*>::iterator Begin();
		std::list<RenderList*>::iterator End();
		
		~RenderSet();
	};
}