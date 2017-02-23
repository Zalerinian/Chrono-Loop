#include "Pool.h"
#include "Logger.h"

Pool::Pool(int _capacity) {
	Resize(_capacity);
}

Pool::~Pool() {
	Clear();
}

inline unsigned int Pool::GetSize() {
	return mSize;
}

void Pool::Resize(unsigned int _size) {
	if (_size > 5000) {
		SystemLogger::GetError() << "[Warning] The size of the pool has been set to " << _size << ", which is pretty big. Was this intentional?" << std::endl;
	}
	if (_size < mSize) {
		// Delete objects to reach the size.
		while (_size < mSize) {
			delete GetObject();
		}
	} else if(_size > mSize) {
		// Add more objects to meet the size
		AddObject(new BaseObject);
	}
}

BaseObject * Pool::GetObject() {
	if (mHead != nullptr) {
		Node* n = mHead;
		mHead = mHead->mNext;
		BaseObject* obj = n->data;
		delete n;
		--mSize;
		return obj;
	}
	return nullptr;
}

void Pool::AddObject(BaseObject * _obj) {
	Node* n = new Node;
	n->data = _obj;
	n->mNext = mHead;
	mHead = n;
	++mSize;
}

void Pool::Clear() {
	Node *n = mHead, *t = nullptr;
	while (n != nullptr) {
		t = n;
		n = n->mNext;
		delete t->data;
		delete t;
	}
}
