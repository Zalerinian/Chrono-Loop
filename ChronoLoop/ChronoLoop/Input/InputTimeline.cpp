#include "InputTimeline.h"
#include "../Core/TimeManager.h"

namespace Epoch {

	InputTimeline::InputTimeline() {}

	InputTimeline::~InputTimeline() {

	}

	void InputTimeline::Push_back(InputNode * _new) {
		if (!mHead) {
			mHead = _new;
			mCurrent = _new;
			_new->mNext = nullptr;
			_new->mPrev = nullptr;
		} else if (mCurrent == mHead) {
			mHead->mNext = _new;
			_new->mPrev = mHead;
			_new->mNext = nullptr;
			mCurrent = _new;
		} else {
			_new->mPrev = mCurrent;
			mCurrent->mNext = _new;
			mCurrent = _new;
		}

	}

	void InputTimeline::Insert(InputNode * _data) {
		
		if (!mCurrent) {
			Push_back(_data);
			//DisplayTimeline();
			return;
		}

		InputNode* temp;
		if (mInsertStart)
			temp = mInsertStart;
		else
			temp = mHead;
		

		while (temp) {

			//Add as head if its less than it.
			if (temp == mHead && (_data->mData.mLastFrame <= mHead->mData.mLastFrame || (_data->mData.mLastFrame == mHead->mData.mLastFrame && _data->mData.mTime < mHead->mData.mTime))) {
				_data->mNext = mHead;
				_data->mPrev = nullptr;
				mCurrent = _data;
				mHead = _data;
				//DisplayTimeline();
				return;
			}

			//If greater than temp but there is no next
			//Move Current pointer if we don't have a next so we can continue to record.
			if ((_data->mData.mLastFrame > temp->mData.mLastFrame || (_data->mData.mLastFrame == temp->mData.mLastFrame && _data->mData.mTime > temp->mData.mTime)) && !temp->mNext) {
				Push_back(_data);
				//DisplayTimeline();
				return;
			}
			
			//if greatr than temp but less then next
			if ((_data->mData.mLastFrame > temp->mData.mLastFrame || (_data->mData.mLastFrame == temp->mData.mLastFrame && _data->mData.mTime > temp->mData.mTime)) && temp->mNext &&
				(_data->mData.mLastFrame <= temp->mNext->mData.mLastFrame || (_data->mData.mLastFrame == temp->mNext->mData.mLastFrame && _data->mData.mTime < temp->mNext->mData.mTime))) {
				temp->mNext->mPrev = _data;
				_data->mNext = temp->mNext;
				_data->mPrev = temp;
				temp->mNext = _data;
				//DisplayTimeline();
				return;
			}
			//Increment further in the tree because 
			temp = temp->mNext;
		}

		SystemLogger::GetLog() << "ERROR: Adding input failed to find a spot";
	}


	void InputTimeline::DisplayTimeline() {
		int num = 0;
		while (num < 10) {
			SystemLogger::GetLog() << std::endl;
			num++;
		}
		InputNode* temp2 = mHead;
		while (temp2) {
			if (temp2 == mCurrent) {
				SystemLogger::GetLog() << "id:" << temp2->mData.mControllerId << " LastFrane:" << temp2->mData.mLastFrame << " Time:" << temp2->mData.mTime << " :Button" << temp2->mData.mButton << ' ' <<  temp2->mData.mButtonState << "<-Curr" << std::endl;
			} else {
				SystemLogger::GetLog() << "id:" << temp2->mData.mControllerId << " LastFrane:" << temp2->mData.mLastFrame << " Time:" << temp2->mData.mTime << " :Button" << temp2->mData.mButton << ' ' << temp2->mData.mButtonState << std::endl;
			}
			temp2 = temp2->mNext;
		}
	}


	void InputTimeline::Clear() {
		InputNode* temp = mHead;
		while (temp) {
			InputNode* del = temp;
			temp = temp->mNext;
			delete del;
		}
		mCurrent = nullptr;
		mHead = nullptr;
	}
}
