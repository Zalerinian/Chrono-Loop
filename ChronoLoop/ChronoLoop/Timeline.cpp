#include "stdafx.h"
#include "Timeline.h"


bool Snapshot::IsObjectStored(short _id)
{
	try
	{
		mSnapinfos.at(_id);
	}
	catch (std::exception e)
	{
		return false;
	}

	return true;
}


Timeline::Timeline()
{
}


Timeline::~Timeline()
{
}

void Timeline::AddBaseObject(BaseObject& _object, short _id)
{
	
}

void Timeline::AddSnapshot(float _snaptime,Snapshot _snapshot)
{
	mSnaptimes.push_back(_snaptime);
	mSnapshots[_snaptime] = _snapshot;
	
}

SnapInfo Timeline::GenerateSnapInfo(BaseObject& _object)
{
	SnapInfo info;
	info.mTransform = _object.GetTransform();

	//Add alot more componet data when componets get made
	return info;
}

Snapshot Timeline::GenerateSnapShot()
{
	Snapshot snap;
	//TODO PAT: Make a game Time varible 
	float TakenTime = -1;
	//If first snapshot taken
	if (mSnapshots.size() == 0)
	{
		for (int i = 0; i < mLiveObjects.size(); i++)
		{
			float id = mLiveObjects[i].GetUniqueId();
			snap.mSnapinfos[id] = GenerateSnapInfo(mLiveObjects[i]);
			snap.mUpdatedtimes[id] = TakenTime;
		}
	}
	else
	{
		snap.mUpdatedtimes = mSnapshots[mSnaptimes.size() - 1].mUpdatedtimes;
		for (int i = 0; i < mLiveObjects.size(); i++)
		{
			//TODO PAT: Do somesort of duplicate infomation checking to decide to add it or not
			//If change add to mSnapinfos and Updatetime
			float id = mLiveObjects[i].GetUniqueId();
			snap.mSnapinfos[id] = GenerateSnapInfo(mLiveObjects[i]);
			snap.mUpdatedtimes[id] = TakenTime;
		}
	}
	return snap;
}

