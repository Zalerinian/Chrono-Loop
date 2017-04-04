#pragma once

#include "Common\Math.h"
#include <vector>
#include <unordered_map>
#include <list>

namespace Epoch
{
	class Tree;
	class Node;
	class Leaf;
	class Internal;
	struct TraversalData;
	class BaseObject;
	struct AABB;

	//TODO: typedef functions for traversal data?
	typedef unsigned int(*PerSplit)(Internal*, TraversalData*);
	typedef bool(*PerLeaf)(Leaf*, TraversalData*);
	typedef bool(*PerObject)(BaseObject*, TraversalData*);

	struct AABB
	{
		vec3f mMin, mMax;
	};

	struct TraversalData
	{
		void* mData;

		PerSplit mSplit;
		PerLeaf mLeaf;
		PerObject mObject;

		TraversalData()
		{
			mData = nullptr;
			mSplit = 0;
			mLeaf = 0;
			mObject = 0;
		}
	};

	class Node
	{
		friend class Tree;
	private:
		Internal* mParent;
	protected:
		Internal* GetParent();
	public:
		enum NodeType { eInternalNode = 0, eLeafNode };

		virtual void Traverse(TraversalData* _data) = 0;

		Node(void) : mParent(nullptr) {}
		virtual ~Node() {}
		virtual NodeType GetNodeType(void) = 0;

		friend void BuildTree(Tree*, Node*);
	};

	class Leaf : public Node
	{
		friend class Tree;
	private:
		AABB mBounds;
		std::list<BaseObject*> mContains;

	public:
		Leaf(void) {}
		~Leaf() {}

		NodeType GetNodeType(void) { return eLeafNode; }

		const AABB& GetBounds(void) { return mBounds; }

		std::list<BaseObject*>& GetContainedObjects(void) { return mContains; }

		void Traverse(TraversalData* _data);

		friend void BuildTree(Tree*, Node*);

	};

	class Internal : public Node
	{
		friend class Tree;
	private:
		unsigned int mAxis;
		float mOffset;

		Node* mLeft, *mRight;

	public:
		Internal(void) : mAxis(0), mOffset(0.0f), mLeft(nullptr), mRight(nullptr) {}
		NodeType GetNodeType(void) { return eInternalNode; }

		unsigned int GetAxis() { return mAxis; }
		float GetOffset() { return mOffset; }

		void Traversal(TraversalData* _data);

		~Internal();

		friend void BuildTree(Tree*, Node*);
	};

	class Tree
	{
	private:
		Node* mRoot;
		AABB mSceneBounds;
		std::unordered_map<BaseObject*, std::vector<Leaf*>> MMasterList;
		std::list<Leaf*> mLeafs;

		Tree(void) : mRoot(nullptr) {}
		Tree(const Tree&) {}
		Tree& operator=(const Tree&) { return *this; }

		void UpdateObjects(BaseObject*);

	public:
		~Tree();

		void Insert(BaseObject*);
		void Remove(BaseObject*);

		Tree* CreateTree(AABB);

		int ClassifyAABBtoPlane(AABB, unsigned int, float);

	};
}