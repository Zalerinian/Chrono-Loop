#pragma once
#include "Math.h"
#include "Logger.h"

namespace Epoch {

	template <class T>
	class LimitedList {
		template <class T>
		struct Node {
			T data;
			Node *mNext = nullptr, *mPrev = nullptr;
			Node<T>(T _d) : data(_d) {}
		};

		Node<T> *mHead = nullptr, *mTail = nullptr;
		unsigned int mSize, mLimit = 9;

	public:
		template <class T>
		class Iterator {
			Node<T>* mNode;
			Iterator<T>& operator++() { mNode = mNode->mNext; return *this; }; // Prefix operator.
			Iterator<T>& operator--(); // Prefix operator.
			Iterator<T>& operator++(int); // Postfix operator.
			Iterator<T>& operator--(int); // Postfix operator.
			T& operator*();
			T* operator->();
		};

		bool IsFull();
		unsigned int GetSize();
		unsigned int GetLimit();
		void SetLimit(unsigned int _size);
		void AddHead(T _node);
		T& GetHead();
		T& GetTail();
		T& operator[](unsigned int _index);
		void Clear();
		~LimitedList();
	};

	template <class T>
	unsigned int LimitedList<T>::GetSize() {
		return mSize;
	}

	template <class T>
	unsigned int LimitedList<T>::GetLimit() {
		return mLimit;
	}

	template <class T>
	bool LimitedList<T>::IsFull() {
		return mSize == mLimit;
	}

	template <class T>
	void LimitedList<T>::SetLimit(unsigned int _size) {
		if (_size < mLimit) {
			SystemLogger::GetError() << "[Warning] The requested size (" << _size << ") is smaller than the current size (" << mLimit << "). A memory leak is probable." << std::endl;
		}
		if (_size > 100) {
			SystemLogger::GetError() << "[Warning] The requested size (" << _size << ") is greater than 100. Are you sure LimitedList is the best solution for your problem?" << std::endl;
		}
		mLimit = _size;
	}

	template <class T>
	void LimitedList<T>::AddHead(T _value) {
		if (mHead == nullptr) {
			mHead = new Node<T>(_value);
			mTail = mHead;
			mHead->mNext = mHead->mPrev = nullptr;
			mSize = 1;
		} else {
			if (mSize >= mLimit) {
				Node<T> *tail = mTail->mPrev;
				delete mTail;
				mTail = tail;
				mTail->mNext = nullptr;
				--mSize;
			}
			Node<T>* n = new Node<T>(_value);
			n->mPrev = nullptr;
			n->mNext = mHead;
			mHead->mPrev = n;
			mHead = n;
			mSize++;
		}
	}

	template<class T>
	inline T & LimitedList<T>::GetHead() {
		return mHead->data;
	}

	template<class T>
	inline T & LimitedList<T>::GetTail() {
		return mTail->data;
	}


	template <class T>
	T& LimitedList<T>::operator[](unsigned int _index) {
		Node<T>* n = mHead;
		for (unsigned int i = 0; i < _index; ++i) {
			n = mHead->mNext;
		}
		return n->data;
	}

	template <class T>
	void LimitedList<T>::Clear() {
		Node<T>* n = mHead;
		while (n) {
			Node<T>* self = n;
			n = n->mNext;
			delete self;
		}
	}

	template <class T>
	LimitedList<T>::~LimitedList() {
		Clear();
	}

	//template<class T>
	//template<class T>
	//LimitedList<T>::Iterator<T>& LimitedList<T>::Iterator<T>::operator--() {
	//	// TODO: insert return statement here
	//	return *this;
	//}
}

