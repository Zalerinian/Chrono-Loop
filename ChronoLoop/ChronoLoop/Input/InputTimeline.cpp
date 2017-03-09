#include "InputTimeline.h"
#include "../Core/TimeManager.h"

namespace Epoch {

	InputTimeline::InputTimeline(){}

	InputTimeline::~InputTimeline()
	{
		
	}

	void InputTimeline::Push_back(InputNode * _new)
	{
		if(!mHead)
		{
			mHead = _new;
			mCurrent = _new;
			_new->mNext = nullptr;
			_new->mPrev = nullptr;
		}
		else if(mCurrent == mHead)
		{
			mHead->mNext = _new;
			_new->mPrev = mHead;
			_new->mNext = nullptr;
			mCurrent = _new;
		}
		else
		{
			_new->mPrev = mCurrent;
			mCurrent->mNext = _new;
			mCurrent = _new;
		}

	}

	void InputTimeline::Insert(InputNode * _data)
	{
		/*SystemLogger::GetLog().flush();
		InputNode* temp2 = mHead;
		while(temp2)
		{
			SystemLogger::GetLog() << temp2->mData.mLastFrame << ":" << temp2->mData.mTime << std::endl;
			temp2 = temp2->mNext;
		}*/

		if (!mCurrent) {
			Push_back(_data);
			return;
		}
		//mCurrent is the last in the list
		if(mCurrent == mHead || !mCurrent->mNext)
		{
			Push_back(_data);
			return;
		}
		InputNode* temp = mCurrent;
		while(temp)
		{
			//Add to the head 
			if(temp == mHead && _data->mData.mLastFrame == mHead->mData.mLastFrame && _data->mData.mTime < mHead->mData.mTime)
			{
				_data->mNext = mHead;
				_data->mPrev = nullptr;
				mCurrent = _data;
				mHead = _data;
			}
			//If greater than current but there is no next
			//Move Current pointer if we don't have a next so we can continue to record.
			else if (_data->mData.mLastFrame >= temp->mData.mLastFrame && _data->mData.mTime > temp->mData.mTime && !temp->mNext)
			{
				Push_back(_data);
				return;
			}
			//if greatr than temp but less then next
			else if(_data->mData.mLastFrame >= temp->mData.mLastFrame && _data->mData.mTime > temp->mData.mTime && temp->mNext && _data->mData.mLastFrame <= temp->mNext->mData.mLastFrame && _data->mData.mTime < temp->mNext->mData.mTime)
			{
				temp->mNext->mPrev = _data;
				_data->mNext = temp->mNext;
				_data->mPrev = temp;
				temp->mNext = _data;

			}

			temp = temp->mNext;
		}
		
	}


	void InputTimeline::Clear()
	{
		InputNode* temp = mHead;
		while(temp)
		{
			InputNode* del = temp;
			temp = temp->mNext;
			delete del;
		}
	}
}
