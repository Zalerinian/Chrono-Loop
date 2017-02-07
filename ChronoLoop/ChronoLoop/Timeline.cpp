#include "stdafx.h"
#include "Timeline.h"


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
		_snapshot.mUpdatedtimes = mSnapshots[mSnaptimes.size()-2]
		mSnapshots[mSnaptimes.size() - 1] = _snapshot;
		
	}
	else
	{
		
	}
}

