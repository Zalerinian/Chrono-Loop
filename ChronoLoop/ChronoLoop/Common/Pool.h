#pragma once
#include "../Objects/BaseObject.h"

class Pool {
	struct Node {
		Node *mNext;
		BaseObject* data;
	};

	unsigned int mSize = 0;
	Node* mHead = nullptr;
public:
	Pool(int _capacity = 50);
	~Pool();
	inline unsigned int GetSize();
	void Resize(unsigned int _size);
	BaseObject* GetObject();
	void AddObject(BaseObject* _obj);
	void Clear();
};