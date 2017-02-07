#include "stdafx.h"
#include "Timeline.h"


std::vector<float> Timeline::mSnaptimes;
std::unordered_map<float, Snapshot> Timeline::mSnapshots;		//The key will be the time they were taken (mSnapTimes)
std::unordered_map<short, BaseObject> Timeline::mLiveObjects;

bool Snapshot::IsObjectStored(short _id)
{
	try
	{
		mSnapinfos.at(_id);
	}
	catch (std::exception& e)
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

void Timeline::AddSnapshot(float _snaptime,Snapshot _snapshot)
{
	mSnaptimes.push_back(_snaptime);
	
	//if >1 copy over updated
	if(mSnapshots.size() > 1)
	{
		//_snapshot.mUpdatedtimes = mSnapshots[mSnaptimes.size() - 2];
		mSnapshots[mSnaptimes.size() - 1] = _snapshot;
		
	}
	else
	{
		
	}
}

