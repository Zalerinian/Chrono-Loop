#pragma once
#include "../Objects/BaseObject.h"

namespace Epoch
{

	class Pool
	{
		struct Node
		{
			Node *mNext;
			BaseObject* data;
		};
		static Pool *sInstance;

		unsigned int mSize = 0;
		Node* mHead = nullptr;
		Pool(int _capacity = 50);
		~Pool();
	public:
		static Pool* Instance();
		static void Initialize(unsigned int _size = 50);
		static void DestroyInstance();

		inline unsigned int iGetSize();
		void iResize(unsigned int _size);
		BaseObject* iGetObject();
		void iRemoveObject(unsigned short _id);
		void iAddObject(BaseObject* _obj);
		void iClear();
	};

}