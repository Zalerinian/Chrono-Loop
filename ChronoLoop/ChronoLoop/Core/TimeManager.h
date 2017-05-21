#pragma once
#include <chrono>
#include <vector>
#include "../Common/Interpolator.h"
#include "../Input/Controller.h"
#include <unordered_map>
#include <bitset>
#define RecordingRate .1f // 1/10th of a second in milliseconds 

namespace Epoch {
	class BaseObject;

	class Timeline;

	//made this to find other baseobjects of the same clone
	struct Clonepair
	{
		unsigned short mCur;
		unsigned short mOther1;
		unsigned short mOther2;
	};
	//This class handles all game time as well as managing the Timeline
	class TimeManager {
		static TimeManager* instanceTimemanager;
		static Timeline* mTimeline;

		//Time variables
		float mTimestamp = 0;
		float mSnapTweenTime = 0;
		float mDeltaTime = 0;
		float mTotalGameTime = 0;
		unsigned int mLevelTime = 0, noMovementCnt = 0;
		unsigned int mRewindGettingFaster = 1, mRewindShouldGetFaster = 0;
		bool mRewindMakeClone = false;
		bool mShouldUpdateInterpolators = false;
		bool mShouldPulse = false;
		unsigned int mtempCurSnapFrame = 0;
		std::vector<BaseObject*>mClones;
		std::unordered_map<unsigned short, Interpolator<matrix4>*>mCloneInterpolators;
		//This is for the hitboxes for the controller
		std::unordered_map<unsigned short, Interpolator<matrix4>*>mCloneColliderInterpolators;
		std::unordered_map<unsigned short, Interpolator<matrix4>*>mObjectRewindInterpolators; 
		std::unordered_map<unsigned short, Clonepair*>mClonePairs;
		//Timeline* GetTimeLine();

		
		std::bitset<10>mCloneTextureBitset;
		//Pass in baseobject id to get bitset location
		std::unordered_map<unsigned short, unsigned int>mCloneTextures;

		
		Timeline* GetTimeLine() { return mTimeline; };

		TimeManager();
		~TimeManager();
	public:
		//Add only headset and controllers to this
		void UpdatePlayerObjectInTimeline(BaseObject* _obj);
		//Add an BaseObject to be recorded by the timeline
		void AddObjectToTimeline(BaseObject* _obj);
		void AddInterpolatorForClone(BaseObject* _obj);
		void AddInterpolatorToObject(BaseObject* _obj);
		//Turn all bits off for clone colors 
		void ActivateAllTexturesToBitset();
		void AssignTextureToClone(unsigned short _id);
		//Clears the list of BaseObject* the Timemanager has refrence to.
		void ClearClones();
		//Checks and see if you can rewind to passed in frame
		bool CheckRewindAvaliable(unsigned int _RewindNumOfframes);
		void DeleteClone(unsigned short _id1, bool _useParticleEffect);
		static void Destroy();
		bool DoesCloneExist(unsigned short _id,unsigned int _frame);
		void EraseClonePair(unsigned short _id) { mClonePairs.erase(_id); };
		//void FindOtherClones(Clonepair& _pair);
		//Returns the current snapshot indx
		unsigned int GetCurrentSnapFrame();
		//Retrieves delta time
		float GetDeltaTime() { return mDeltaTime; }
		bool GetInterpolatorsUpdating() { return mShouldUpdateInterpolators; }
		Interpolator<matrix4>* GetCloneInterpolator(unsigned short _id);
		Interpolator<matrix4>* GetCloneColliderInterpolator(unsigned short _id);
		Interpolator<matrix4>* GetObjectInterpolator(unsigned short _id);
		Clonepair* GetClonePair(unsigned short _id);
		std::vector<BaseObject*>& GetClonesVec() { return mClones; };
		std::string GetNextTexture();
		//the temporary frame that time paused is on
		unsigned int GetTempCurSnap() { return mtempCurSnapFrame; };
		bool GetShouldPulse() { return mShouldPulse; };
		float GetSnapTweenTime() { return mSnapTweenTime; }
		float GetTimeLineObjectInterpTime();
		static TimeManager* Instance();
		//The time that has passed in overallgame
		float GetTotalGameTime() { return mTotalGameTime; };
		unsigned int GetTotalSnapsmade();
		//Go back into time. Send in dest frame and send in player headset and conrollers id
		void RewindTimeline(unsigned int _frame, unsigned short _id1, unsigned short _id2, unsigned short _id3);
		//Go back into time and make clone. Send in dest frame and send in player headset and conrollers baseObjects
		void RewindMakeClone(unsigned int _frame, BaseObject*& _ob1, BaseObject*& _ob2, BaseObject*& _ob3);
		//Save Settings to be set back on restart level 
		void SaveSettingIntToTimeline(std::string _str, int _val);
		//Save Settings to be set back on restart level 
		void SaveSettingBoolToTimeline(std::string _str, bool _val);
		//Set the frame the clone has been made
		void SetCreationTimeofClone(unsigned short _id1, unsigned short _id2, unsigned short _id3);
		//Add a clone pair to unordered map to find other two base objects
		void SetClonePair(unsigned short _id, Clonepair* _pair) { mClonePairs[_id] = _pair; };
		void SetClonesFinalFrame(unsigned short _id, unsigned int _frame);
		void SetTempCurSnap() { mtempCurSnapFrame = GetCurrentSnapFrame(); };
		void SetTimelineObjectInterpTime(float _time);
		void SetupClonePairs(unsigned short _id1, unsigned short _id2, unsigned short _id3);
		void ShowTimelineColliders(bool _show);
		void Update(float _delta);

		//Function Pointer / Command Console
		static void ToggleCloneCountDisplay(void* _command, std::wstring _ifOn);
		static void ToggleSnapshotCountDisplay(void* _command, std::wstring _ifOn);
		void DisplayCloneCount();
		void DisplaySnapshotCount();
		void UpdateCloneMadeTime(unsigned short _id1, unsigned short _id2, unsigned short _id3);
		void BrowseTimeline(int _gesture, int _frameRewind);
		void MoveAllObjectExceptPlayer(unsigned int _snaptime, unsigned short _headset, unsigned short _rightC, unsigned short _leftC);

		void ResetTimeLineandLevel();
	};
}
