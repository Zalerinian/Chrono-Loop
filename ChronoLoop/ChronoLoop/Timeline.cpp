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
	mLiveObjects[_id] = _object;
}

void Timeline::AddSnapshot(float _snaptime,Snapshot _snapshot)
{
	mSnaptimes.push_back(_snaptime);
	mSnapshots[_snaptime] = _snapshot;
}

SnapInfo Timeline::GenerateSnapInfo(BaseObject& _object)
{
	SnapInfo info;
	info.id = _object.GetUniqueId();

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
			short id = mLiveObjects[i].GetUniqueId();
			snap.mSnapinfos[id] = GenerateSnapInfo(mLiveObjects[i]);
			snap.mUpdatedtimes[id] = TakenTime;
		}
	}
	else
	{
		snap.mUpdatedtimes = mSnapshots[(float)(mSnaptimes.size() - 1)].mUpdatedtimes;
		for (int i = 0; i < mLiveObjects.size(); i++)
		{
			//TODO PAT: Do somesort of duplicate infomation checking to decide to add it or not
			//If change add to mSnapinfos and Updatetime
			short id = mLiveObjects[i].GetUniqueId();
			snap.mSnapinfos[id] = GenerateSnapInfo(mLiveObjects[i]);
			snap.mUpdatedtimes[id] = TakenTime;
		}
	}
	return snap;
}


