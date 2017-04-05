#include "BVH.h"
#include "Objects\BaseObject.h"

namespace Epoch
{
	void Internal::Traverse(TraversalData* _data)
	{

	}

	void Leaf::Traverse(TraversalData* _data)
	{

	}

	Tree::~Tree()
	{

	}

	void Tree::Traverse(TraversalData* _data)
	{

	}

	void Tree::Insert(BaseObject* _obj)
	{

	}
	void Tree::Remove(BaseObject* _obj)
	{

	}

	void Tree::UpdateObjects(BaseObject* _obj)
	{

	}
	void Tree::Update()
	{

	}

	void Tree::Get(std::vector<BaseObject*>& _return, BaseObject* _obj)
	{

	}

	unsigned int Tree::ClassifyAABBtoPlane(AABB _ab, unsigned int _axis, float _offset)
	{

		return 0;
	}
	unsigned int Tree::ClassifyObjectPerSplit(Internal* _inode, TraversalData* _data)
	{
		return 0;
	}
	bool Tree::InsertObjectPerLeaf(Leaf* _lnode, TraversalData* _data)
	{
		return true;
	}
	void Tree::Retrieve(BaseObject* _obj, TraversalData* _data)
	{

	}

	Tree* Tree::CreateTree(AABB _ab)
	{
		return nullptr;
	}

	void BuildAABB(AABB _ab)
	{

	}

	void BuildTree(Tree* _tree, Leaf* _lnode)
	{

	}
}