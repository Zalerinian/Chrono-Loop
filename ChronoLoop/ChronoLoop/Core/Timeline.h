#pragma once
#include <vector>
#include <unordered_map>
#include "../Objects/BaseObject.h"


class Component;
//Componet Structs for storing;
#pragma region ComponetStructs
struct SnapComponent {
	ComponentType mCompType;
	unsigned short mId;
};
struct SnapComponent_Physics : SnapComponent {
	vec4f mForces;
	vec4f mVel;
	vec4f mAcc;

};
//Add more componets when we need it
#pragma endregion ComponetStructs


//Extracted values from BaseObjects and its componets
struct SnapInfo
{
	unsigned short mId;	//unique id of the object
	Transform mTransform;	//positional data of the object
	std::vector<SnapComponent*>mComponets;
	//TODO PAT: ADD HEADSET POSITION DATA
	//Componet info needed
};
struct SnapInfoPlayer : SnapInfo {
	//TODO PAT: ADD HEADSET POSITION DATA

	matrix4 mPlayerWorldPos;
	//Left Controller World Pos
	matrix4 mLCWorldPos;
	//Right Controller World Pos
	matrix4 mRCWorldPos;

	SnapInfoPlayer() {};
	SnapInfoPlayer(matrix4 _playerWorldPos, matrix4 _leftController, matrix4 _rightController) {
		mPlayerWorldPos = _playerWorldPos;
		mLCWorldPos = _leftController;
		mRCWorldPos = _rightController;
	}

};

//If you rewind time and there is no snapshot for your object that means nothing changed from the last 
struct Snapshot
{
	unsigned int mTime = -1;									//This is the time in the world the snapshot was taken. If was not assigned mTime= -1;
	std::unordered_map<unsigned short, SnapInfo*> mSnapinfos;
	std::unordered_map<unsigned short, unsigned int> mUpdatedtimes;		//Map of int times of last updated times. Use the float retreaved to access a different snapshot 

	bool IsObjectStored(short _id);
};

class Timeline
{
	std::vector<unsigned int> mSnaptimes;
	std::unordered_map<unsigned int, Snapshot*> mSnapshots;		//The key will be the time they were taken (mSnapTimes)
	std::unordered_map<unsigned short, BaseObject*> mLiveObjects;  

public:
	Timeline();
	~Timeline();
	//Where we are at in the timeline
	unsigned int mCurrentGameTimeIndx = 0;
	unsigned int GetCurrentGameTimeIndx() { return mCurrentGameTimeIndx; }
	void AddBaseObject(BaseObject* _object, unsigned short _id);						//add to the list of recorded objects.
	//TODO PAT: add a remove base object func that says the object has been removed in the next snap recording
	void AddSnapshot(unsigned int _snaptime, Snapshot* _snapshot);
	bool RewindNoClone(unsigned int _snaptime);
	BaseObject* RewindMakeClone(unsigned int _snaptime);
	void MoveAllObjectsToSnap(unsigned int _snaptime);
	void ClearTimeLine();
	SnapInfo* GenerateSnapInfo(BaseObject* _object);							//Error check agianst the BaseObject* if it is null or not
	Snapshot* GenerateSnapShot(unsigned int _time);
	SnapInfoPlayer * GenerateSnapInfoPlayer();
	bool CheckForDuplicateData(unsigned short _id,BaseObject* _object);
};

