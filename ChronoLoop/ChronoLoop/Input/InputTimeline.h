#include <openvr.h>

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
		};
		struct InputNode {
			InputNode* mNext;
			InputNode* mPrev;
			InputData mData;
		};
	private:
		InputNode* mHead = nullptr;
		InputNode* mCurrent = nullptr;
	public:
		InputTimeline();
		~InputTimeline();
		
		void Push_back(InputNode* _new);
		void Insert(InputNode* _data);
		InputNode* GetHead() { return mHead; };
		InputNode* GetCurr() { return mCurrent; };
		void SetCurr(InputNode* _set) { mCurrent = _set; };
		void Clear();

	};
}
