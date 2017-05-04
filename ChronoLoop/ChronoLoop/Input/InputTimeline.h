#include <openvr.h>
#include "../Common/Math/vec3f.h"

namespace Epoch {

	class InputTimeline {
	public:
		//store Controller input information.
		struct InputData {
			//BaseObject id of the controller at the time
			unsigned short mControllerId;
			float mTime;
			unsigned int mLastFrame;
			vr::EVRButtonId mButton;
			//-1:Down 0:Press 1:Up
			short mButtonState;
			vec3f mVelocity;
			bool mPrimary;
		};
		struct InputNode {
			InputNode* mNext = nullptr;
			InputNode* mPrev = nullptr;
			InputData mData;
		};
	private:
		InputNode* mHead = nullptr;
		InputNode* mCurrent = nullptr;
		//This is a variable that will be set to as a start point before input is grabbed from steam vr
		//This is helpful so we don't have to start from the head everytime and don't lose percision if we started from mCurrent
		InputNode* mInsertStart = nullptr;
	public:
		InputTimeline();
		~InputTimeline();

		void Push_back(InputNode* _new);
		void Insert(InputNode* _data);
		InputNode* GetHead() { return mHead; };
		InputNode* GetCurr() { return mCurrent; };
		InputNode* GetInsertStart() { return mInsertStart; }
		void DisplayTimeline();
		//This func is for an external source that will keep the input timeline in check as it moves
		void SetHead(InputNode* _set) { mHead = _set; };
		void SetCurr(InputNode* _set) { mCurrent = _set; };
		void SetInsertStart(InputNode* _set) { mInsertStart = _set; };
		void Clear();

	};
}
