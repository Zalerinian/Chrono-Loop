#pragma once
#include <vector>
#include <unordered_map>
#include "../Objects/BaseObject.h"
#include <bitset>

namespace Epoch {
	//Componet Structs for storing;
#pragma region ComponetStructs
	struct SnapComponent {
		ComponentType mCompType;
		unsigned short mBitNum;
		unsigned int mId;
	};
	struct SnapComponent_Physics : SnapComponent {
		vec3f mForces;
		vec3f mVel;
		vec3f mAcc;
		vec3f mTotforce;
	};
	struct SnapComponent_Mesh : SnapComponent
	{
		bool misVisible = true;
	};
	struct SnapComponent_Code_MazeHelp : SnapComponent {
		int mMazeGrid[4][4];
	};
	//Add more componets when we need it
#pragma endregion ComponetStructs


	//Extracted values from BaseObjects and its componets
	struct SnapInfo {
		unsigned short mId;	//unique id of the object
		Transform mTransform;	//positional data of the object
		std::vector<SnapComponent*>mComponents;
		std::bitset<32>mBitset;	//Base object and componets enabled or disabled //0 for the baseobject and 1-31 for the components
	};


	//If you rewind time and there is no snapshot for your object that means nothing changed from the last 
	struct Snapshot {
		unsigned int mTime = 0;									//Snapshot number this was taken at. Currently the game takes a snap every 1/10th of a second;
		std::unordered_map<unsigned short, SnapInfo*> mSnapinfos;
		std::unordered_map<unsigned short, unsigned int> mUpdatedtimes;		//Map of int times of last updated times. Use the float retreaved to access a different snapshot 

		bool IsObjectStored(unsigned short _id);
	};

	struct SnapLevelSettings
	{
		//tutorial step
		std::unordered_map<std::string, int> mLevelInts;
		std::unordered_map<std::string, bool> mLevelBools;
	};

	struct ObjectLifeTime {
		unsigned int mBirth = 0;
		unsigned int mDeath = INT32_MAX;
	};
	//Clones objects can be made at a different time than when the clone time he is made in the game
	struct CloneLifeTime : ObjectLifeTime
	{
		unsigned int mMade = INT32_MAX;
	};

	class Timeline {
		//Where we are at in the timeline
		unsigned int mCurrentGameTimeIndx = 0;
		float mObjectInterpolationTime = 0.04f;
		std::vector<unsigned int> mSnaptimes;
		std::unordered_map<unsigned int, Snapshot*> mSnapshots;		//The key will be the time they were taken (mSnapTimes)
		std::unordered_map<unsigned short, BaseObject*> mLiveObjects;
		std::unordered_map<unsigned short, ObjectLifeTime*> mObjectLifeTimes;
		std::unordered_map<unsigned short, unsigned int> mCloneDone; //Key:clone id, value: frame clone finished its timeline. 

	public:
		SnapLevelSettings mSettings;

		Timeline();
		~Timeline();
		//this will turn the bitset[0] to 1 so he acts as a normal game object in the timeline
		void ActivateCloneBitset(unsigned short _ids[3]);
		//add to the list of recorded objects
		void AddBaseObject(BaseObject* _object, unsigned short _id); 
		//Update the Player baseobject pointer in the recorded objects
		void UpdatePlayerBaseObject(BaseObject* _object, unsigned short _id);
		//Add a snapshot to unordered map of all snapshots
		void AddSnapshot(unsigned int _snaptime, Snapshot* _snapshot);
		//Remove all snapshots, snapinfos, objectLifeTimes, recorded objects from list
		void ClearCloneCompletionTime() { if(mCloneDone.size() > 0) mCloneDone.clear(); };
		void ClearTimeLine();
		void ChangeBitsetToSnap(SnapInfo* _destinfo, Component* _curComp);
		void ChangePlayerBitsetToSnap(SnapInfo * _destinfo, Component* _curComp);
		bool CheckForDuplicateData(unsigned short _id, BaseObject* _object, unsigned int _frame);
		//This function removes non-clone objects that were created in the future
		void CheckforLostObjects(std::vector<BaseObject*>&mClones);
		void CopySnapInfo(SnapInfo* _src, SnapInfo* _dst);
		ObjectLifeTime* GetObjectLifetime(unsigned short _id);
		SnapInfo* GenerateSnapInfo(BaseObject* _object, SnapInfo* _info);
		Snapshot* GenerateSnapShot(unsigned int _time, std::vector<BaseObject*> & _clones);
		//retreive what frame/snap the game is recording on
		const unsigned int& GetCurrentGameTimeIndx() { return mCurrentGameTimeIndx; }
		//total number of snaps recorded
		unsigned int GetTotalSnaps() { return (unsigned int)mSnapshots.size(); };
		float GetObjectInterpolationTime() { return mObjectInterpolationTime; };
		void SetObjectInterpolationTime(float _set) { mObjectInterpolationTime = _set; };
		void ResetTimelineAndLevel();
		void RemoveFromTimeline(unsigned short _id);
		//Rewind time to snap and make a clone created at that point 
		bool RewindMakeClone(unsigned int _snaptime);
		//Rewind time to snap execpt player baseobjects
		bool RewindNoClone(unsigned int _snaptime, unsigned short _id1, unsigned short _id2, unsigned short _id3);
		//rewind only component data to snap of baseobjects
		void MoveAllComponentsToSnapExceptPlayer(unsigned int _snaptime, unsigned short _id1, unsigned short _id2, unsigned short _id3);
		//rewind a object to a particular snap 
		void MoveObjectToSnap(unsigned int _snaptime, unsigned short _id, bool isClone);
		void MoveAllObjectsToSnap(unsigned int _snaptime);
		void MoveAllObjectsToSnapExceptPlayer(unsigned int _snaptime, unsigned short _id1, unsigned short _id2, unsigned short _id3);
		void SetObjectBirthTime(unsigned short _id);
		void SetBaseObjectDeathTime(unsigned short _id);
		void SetCloneFrameCompletion(unsigned short _id, unsigned int _frame) { mCloneDone[_id] = _frame; };
		void SetCloneObjectCreationTime(unsigned short _id1, unsigned short _id2, unsigned short _id3);
		void SetCloneMadeTime(unsigned short _id1, unsigned short _id2, unsigned short _id3);
		void SetCloneDeathTime(unsigned short _id1, unsigned short _id2, unsigned short _id3);
		void SetComponent(SnapComponent* _destComp, BaseObject* _obj, SnapInfo* _destInfo);
		void SetCurrentGameTimeIndx(int _time) { mCurrentGameTimeIndx = _time; };
		void SetSavedSettings();
		void ShowLiveObjectsColliders(unsigned int _frame,bool _show);
		void UpdateCloneInterpolators(unsigned short _cloneid, SnapInfo* _currSnap, unsigned int _currTime);
		void PrepareAllObjectInterpolators(unsigned int _fromSnapTime, unsigned int _toSnapTime);
	};

}