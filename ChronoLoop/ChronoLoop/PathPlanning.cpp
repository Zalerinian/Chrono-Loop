#include "PathPlanning.h"


void Epoch::PathPlanner::Initialize(NavMesh * _navMesh)
{
	int edgeCounter = 0;
	mNevMash = _navMesh;
	for (size_t x = 0; mNevMash->mTriangles.size(); ++x)
	{
		Triangle* tempTri = &mNevMash->mTriangles[x];
		SearchTri* triInfo = new SearchTri(tempTri);
		for (size_t y = edgeCounter; y < (edgeCounter + mNevMash->mEdgeAmount[x]); ++y)
		{
			triInfo->mTouching.push_back(&mNevMash->mTriangles[mNevMash->mEdgeIndicies[y]]);
		}
		for (size_t y = 0; y < triInfo->mTouching.size(); ++y)
		{
			triInfo->mTouchingCosts.push_back(GetEstimate(tempTri, triInfo->mTouching[y]));
		}

		edgeCounter += mNevMash->mEdgeAmount[x];
		mMeshTris[tempTri] = triInfo;
	}
}
void Epoch::PathPlanner::enter(vec4f _start, vec4f _goal)
{

}
bool Epoch::PathPlanner::GreedyBestFirst(SearchTri * _start, SearchTri * _end)
{
	return false;
}

void Epoch::PathPlanner::exit()
{
}
void Epoch::PathPlanner::Shutdown()
{
}
Epoch::PathPlanner::~PathPlanner()
{
}
