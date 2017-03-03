#include "Pool.h"
#include "../Common/Logger.h"

namespace Epoch
{

	Pool* Pool::sInstance = nullptr;

	Pool::Pool(int _capacity)
	{
		iResize(_capacity);
	}

	Pool::~Pool()
	{
		iClear();
	}

	Pool * Pool::Instance()
	{
		return sInstance;
	}

	void Pool::Initialize(unsigned int _size)
	{
		if (nullptr == sInstance)
		{
			sInstance = new Pool(_size);
		}
	}

	void Pool::DestroyInstance()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
			sInstance = nullptr;
		}
	}

	inline unsigned int Pool::iGetSize()
	{
		return mSize;
	}

	void Pool::iResize(unsigned int _size)
	{
		if (_size > 5000)
		{
			SystemLogger::GetError() << "[Warning] The size of the pool has been set to " << _size << ", which is pretty big. Was this intentional?" << std::endl;
		}
		if (_size < mSize)
		{
			// Delete objects to reach the size.
			while (_size < mSize)
			{
				delete iGetObject();
			}
		}
		else if (_size > mSize)
		{
			// Add more objects to meet the size
			while (_size > mSize)
			{
				iAddObject(new BaseObject);
			}
		}
	}

	BaseObject * Pool::iGetObject()
	{
		if (mSize == 0)
		{
			// TODO: Actually implement capacity.
			iResize(50);
		}
		if (mHead != nullptr)
		{
			Node* n = mHead;
			mHead = mHead->mNext;
			BaseObject* obj = n->data;
			delete n;
			--mSize;
			return obj;
		}
		return nullptr;
	}

	void Pool::iAddObject(BaseObject * _obj)
	{
		Node* n = new Node;
		n->data = _obj;
		n->mNext = mHead;
		mHead = n;
		++mSize;
	}

	void Pool::iClear()
	{
		Node *n = mHead, *t = nullptr;
		while (n != nullptr)
		{
			t = n;
			n = n->mNext;
			delete t->data;
			delete t;
		}
	}

}