#include "LimitedList.h"
#include "Logger.h"

bool LimitedList::IsFull() {
	return mSize == mLimit;
}

unsigned int LimitedList::GetSize() {
	return mSize;
}

unsigned int LimitedList::GetLimit() {
	return mLimit;
}

void LimitedList::SetLimit(unsigned int _size) {
	if (_size < mLimit) {
		SystemLogger::GetError() << "[Warning] The requested size (" << _size << ") is smaller than the current size (" << mLimit << "). A memory leak is probable." << std::endl;
	}
	if (_size > 100) {
		SystemLogger::GetError() << "[Warning] The requested size (" << _size << ") is greater than 100. Are you sure LimitedList is the best solution for your problem?" << std::endl;
	}
	mLimit = _size;
}

void LimitedList::AddHead(vec2f _node) {
	if (mHead == nullptr) {
		mHead = new Node(_node);
		mTail = mHead;
		mHead->mNext = mHead->mPrev = nullptr;
		mSize = 1;
	} else {
		if (mSize >= mLimit) {
			Node *tail = mTail->mPrev;
			delete mTail;
			mTail = tail;
			mTail->mNext = nullptr;
			--mSize;
		}
		Node* n = new Node(_node);
		n->mPrev = nullptr;
		n->mNext = mHead;
		mHead->mPrev = n;
		mHead = n;
		mSize++;
	}
}

vec2f & LimitedList::operator[](unsigned int _index) {
	Node* n = mHead;
	if (!n) {
		return vec2f();
	}
	for (unsigned int i = 0; i < _index; ++i) {
		n = mHead->mNext;
	}
	return n->data;
}

void LimitedList::Clear() {
	Node* n = mHead;
	while (n) {
		Node* self = n;
		n = n->mNext;
		delete self;
	}
}

LimitedList::~LimitedList() {
	Clear();
}
