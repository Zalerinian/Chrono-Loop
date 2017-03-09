#pragma once
#include "Rendering\Mesh.h"
#include "Common\Math.h"
#include "NavMesh.h"
#include <unordered_set>
#include <unordered_map>
#include <queue>

namespace Epoch
{
	struct SearchTri
	{
		Triangle* mCur;
		Triangle* mTouching[3];
		float mTouchingCosts[3];

		SearchTri(){
			mCur = nullptr;
		}
		SearchTri(Triangle* _currentTri) {
			mCur = _currentTri;
		}
		static float GetTouchingCost(Triangle* _curr, Triangle* _edge) {
			return (_curr->Centeroid - _edge->Centeroid).Magnitude();
		}
	};
	struct PlannerTri
	{
		PlannerTri* mParent;
		SearchTri* mSearch;
		float mHeuristicCost;
		PlannerTri(){
			mParent = nullptr;
			mSearch = nullptr;
		}
		PlannerTri(SearchTri* _search) {
			mParent = nullptr;
			mSearch = _search;
		}
		PlannerTri(PlannerTri* _parent,SearchTri* _search) {
			mParent = _parent;
			mSearch = _search;
		}
	};
	class PathPlanner
	{
		PathPlanner();
		~PathPlanner();
		std::unordered_map<Triangle*, SearchTri*> mMeshTris;//All the Tris in the NavMesh
		std::unordered_map<SearchTri*, PlannerTri*> mVisited;//Tris that were visited
		std::queue<PlannerTri*> mOpen;//Current Open Tris
		std::vector<Triangle const*> mSolution;
		SearchTri* mStart, *mGoal;
		NavMesh* mNevMash;
		bool isDone = false;

		void Initialize(NavMesh* _navMesh); //Gets all the triangles from the navmesh and stores them and their values in the correct places
		bool GreedyBestFirst(SearchTri* _start, SearchTri* _end);
		void Shutdown();
		void exit();
		void enter(Triangle* _start, Triangle* _goal);

		bool isGreater(PlannerTri* const &lhs, PlannerTri* const & rhs) { return (lhs->mHeuristicCost > rhs->mHeuristicCost); }
		float GetEstimate(Triangle* _start, Triangle* _end) { return (_end->Centeroid - _start->Centeroid).Magnitude(); }

	};
}