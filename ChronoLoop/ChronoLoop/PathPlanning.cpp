#include "PathPlanning.h"


void Epoch::PathPlanner::Initialize(NavMesh * _navMesh)
{
	//int edgeCounter = 0;
	mNevMash = _navMesh;
	for (size_t x = 0; mNevMash->mTriangles.size(); ++x)
	{
		Triangle* tempTri = &mNevMash->mTriangles[x];
		SearchTri* triInfo = new SearchTri(tempTri);
		for (size_t y = 0; y < 3; ++y)
		{
			triInfo->mTouching[y] = &mNevMash->mTriangles[mNevMash->mEdgeIndicies[y]];
			triInfo->mTouchingCosts[y] = GetEstimate(tempTri, triInfo->mTouching[y]);
		}
		//edgeCounter += mNevMash->mEdgeAmount[x];
		mMeshTris[tempTri] = triInfo;
	}
}
void Epoch::PathPlanner::enter(Triangle* _start, Triangle* _goal)
{
	mStart = nullptr;
	mStart->mCur = _start;
	mGoal = nullptr;
	mGoal->mCur = _goal;

	isDone = false;
	mSolution.clear();
	PlannerTri* PTri = new PlannerTri(mStart);
	mOpen.push(PTri);
	mVisited[mStart] = mOpen.front();

	mOpen.front()->mHeuristicCost = GetEstimate(_start, _goal);

}
bool Epoch::PathPlanner::GreedyBestFirst(SearchTri * _start, SearchTri * _end)
{
	mOpen.push(new PlannerTri(_start));
	mVisited[_start] = mOpen.front();
	mVisited[_start]->mHeuristicCost = GetEstimate(_start->mCur, _end->mCur);

	while (!mOpen.empty())
	{
		PlannerTri* current = mOpen.front();
		mOpen.pop();
		if (current->mSearch == _end) {
			isDone = true;
			break;
		}
		for (unsigned int x = 0; x < 3; ++x) {
			if (current->mSearch->mTouching[x] != nullptr) {
				SearchTri* succ = mMeshTris[current->mSearch->mTouching[x]];
				if (mVisited[succ] == nullptr) {
					PlannerTri* succNode = new PlannerTri(succ);
					succNode->mParent = current;
					succNode->mHeuristicCost = GetEstimate(succ->mCur, _end->mCur);
					mVisited[succ] = succNode;
					mOpen.push(succNode);
				}
			}
		}
	}
	return false;
}

void Epoch::PathPlanner::exit()
{
	while (!mOpen.empty())
		mOpen.pop();
	mVisited.clear();
	mStart = nullptr;
	mGoal = nullptr;
}
void Epoch::PathPlanner::Shutdown()
{
	for (auto itr = mMeshTris.begin(); itr != mMeshTris.end(); itr++)
	{
		if (itr->second != NULL)
			delete itr->second;
	}
	mMeshTris.clear();
	while (!mOpen.empty())
		mOpen.pop();
	for (auto itr = mVisited.begin(); itr != mVisited.end(); itr++)
	{
		if (itr->second != NULL)
			delete itr->second;
	}
}
Epoch::PathPlanner::~PathPlanner()
{
}
