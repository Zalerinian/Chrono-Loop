#pragma once
#include <vector>
#include <unordered_map>
#include "Objects/BaseObject.h"

//Extracted values from BaseObjects and its componets
struct SnapInfo
{
	
	/*matrix4 mTransform;
	vec4f mUp, mForward, mRight;*/

	//Transform mTransform;
	//Componet info needed

	
};

//If you rewind time and there is no snapshot for your object that means nothing changed from the last 
struct Snapshot
{
	float mTime = -1;									//This is the time in the world the snapshot was taken. If was not assigned mTime= -1;
	std::unordered_map<short, SnapInfo> mSnapinfos;
	std::unordered_map<short, float> mUpdatedtimes;		//Map of float times of last updated times. Use the float retreaved to access a different snapshot 

	bool IsObjectStored(short _id);
};

class Timeline
{
	static std::vector<float> mSnaptimes;
	static std::unordered_map<float, Snapshot> mSnapshots;		//The key will be the time they were taken (mSnapTimes)
	static std::unordered_map<short, BaseObject> mLiveObjects;  

public:
	Timeline();
	~Timeline();

	static void AddSnapshot(float _snaptime, Snapshot _snapshot);
	static SnapInfo GenerateSnapInfo();							//Error check agianst the BaseObject* if it is null or not
	static Snapshot GenerateSnapShot();
};

