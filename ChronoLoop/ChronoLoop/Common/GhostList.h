#pragma once
#include "Logger.h"
#include <vector>
#include "../Common/Math.h"

namespace Epoch {

	template <class T>
	class GhostList {
	public:
		class GhostNode;

	private:
		unsigned int mSize = 0;
		typename GhostNode *mHead = nullptr, *mTail = nullptr;

	public:
		class GhostNode {
			typename GhostNode *mPrev = nullptr, *mNext = nullptr;
			GhostList<T>* mList = nullptr;
			friend GhostList<T>;
			GhostNode(T _data) : data(_data) {}

			// This destructor is only provided as a way to idiot-proof destroying nodes in the Ghostly-linked list.
			// The destructor will remove the node from the list, ensuring it doesn't corrupt the whole thing.
		public:
			~GhostNode();
			T data;
		};
		~GhostList();
		typename GhostNode* Push(T& _data);
		const typename GhostNode* GetHead();
		unsigned int GetSize();
		void GetData(std::vector<T>& _data);

	};

	template<class T>
	inline GhostList<T>::~GhostList() {}

	template<class T>
	typename GhostList<T>::GhostNode* GhostList<T>::Push(T& _data) {
		GhostNode *node = new GhostNode(_data);
		node->mList = this;
		++mSize;

		node->mPrev = mTail;
		node->mNext = nullptr;
		if (mHead == nullptr) {
			mHead = node;
		}
		if (mTail == nullptr) {
			mTail = node;
		} else {
			mTail->mNext = node;
			mTail = node;
		}
		return node;
	}

	template<class T>
	const typename GhostList<T>::GhostNode* GhostList<T>::GetHead() {
		return mHead;
	}

	template<class T>
	unsigned int GhostList<T>::GetSize() {
		return mSize;
	}

	template<class T>
	void GhostList<T>::GetData(std::vector<T>& _data) {
		//_data.clear();
		_data.reserve(mSize);
		GhostNode *n = mHead;
		while (n != nullptr) {
			_data.push_back(n->data);
			n = n->mNext;
		}
		if (_data.size() != mSize) {
			SystemLogger::Error() << "Something has gone wrong creating the data list: The resulting size is not what was expected. Is something corrupt?" << std::endl;
		}
	}

	template<>
	inline void GhostList<matrix4>::GetData(std::vector<matrix4>& _data) {
		_data.clear();
		_data.reserve(mSize); 
		GhostNode *n = mHead;
		while (n != nullptr) {
			_data.push_back(n->data.Transpose());
			n = n->mNext;
		}
		if (_data.size() != mSize) {
			SystemLogger::Error() << "Something has gone wrong creating the data list: The resulting size is not what was expected. Is something corrupt?" << std::endl;
		}
	}

	template<class T>
	GhostList<T>::GhostNode::~GhostNode() {
		if (mPrev) {
			mPrev->mNext = mNext;
		}
		if (mNext) {
			mNext->mPrev = mPrev;
		}
		if (mList->mHead == this) {
			mList->mHead = mNext;
		}
		if (mList->mTail == this) {
			mList->mTail = mPrev;
		}
		--mList->mSize;
	}

} // Epoch namespace

