#pragma once
#include <vector>
#include <unordered_map>
#include "../Objects/BaseObject.h"

class Component;
//Componet Structs for storing;
#pragma region ComponetStructs
struct SnapComponent {
	ComponentType CompType;
};
struct SnapComponent_Physics : SnapComponent {
	vec4f acceleration;
	vec4f velocity;
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

//If you rewind time and there is no snapshot for your object that means nothing changed from the last 
struct Snapshot
{
	unsigned int mTime = -1;									//This is the time in the world the snapshot was taken. If was not assigned mTime= -1;
	std::unordered_map<unsigned short, SnapInfo*> mSnapinfos;
	std::unordered_map<unsigned short, unsigned int> mUpdatedtimes;		//Map of float times of last updated times. Use the float retreaved to access a different snapshot 

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
	void AddBaseObject(BaseObject* _object, unsigned short _id);						//add to the list of recorded objects. This func should be called constructer
	//TODO PAT: add a remove base object func that says the object has been removed in the next snap recording
	void AddSnapshot(unsigned int _snaptime, Snapshot* _snapshot);
	bool RewindNoClone(unsigned int _snaptime);
	void MoveAllObjectsToSnap(unsigned int _snaptime);
	void ClearTimeLine();
	SnapInfo* GenerateSnapInfo(BaseObject* _object);							//Error check agianst the BaseObject* if it is null or not
	Snapshot* GenerateSnapShot(unsigned int _time);
};

