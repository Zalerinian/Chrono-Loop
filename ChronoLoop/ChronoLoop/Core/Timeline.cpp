//#include "stdafx.h"
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
	ClearTimeLine();
}

void Timeline::AddBaseObject(BaseObject* _object, unsigned short _id)
{
	mLiveObjects[_id] = _object;
}

void Timeline::AddSnapshot(float _snaptime,Snapshot* _snapshot)
{
	mSnaptimes.push_back(_snaptime);
	mSnapshots[_snaptime] = _snapshot;
}

void Timeline::ClearTimeLine()
{
	for (auto snapshot : mSnapshots)
	{
		for (auto snapInfo : snapshot.second->mSnapinfos)
		{
			if (snapInfo.second)
				delete snapInfo.second;

		}
		snapshot.second->mSnapinfos.clear();
		snapshot.second->mUpdatedtimes.clear();
		if (snapshot.second)
			delete snapshot.second;
	}
	mSnapshots.clear();
	mSnaptimes.clear();
	mLiveObjects.clear();
}

SnapInfo* Timeline::GenerateSnapInfo(BaseObject* _object)
{
	SnapInfo* info = new SnapInfo();
	info->id = _object->GetUniqueId();
	info->mTransform = _object->GetTransform();
	//info->components = _object->GetComponets();
	//Add alot more componet data when componets get made
	return info;
}

Snapshot* Timeline::GenerateSnapShot(float _time)
{
	Snapshot* snap = new Snapshot();
	snap->mTime = _time;
	//If first snapshot taken
	if (mSnapshots.size() == 0)
	{
		for (std::pair<unsigned short,BaseObject*> _b : mLiveObjects)
		{
			if (_b.second)
			{
				unsigned short id = _b.first;
				snap->mSnapinfos[id] = GenerateSnapInfo(_b.second);
				snap->mUpdatedtimes[id] = _time;
			}
		}
	}
	else
	{
		snap->mUpdatedtimes = mSnapshots[mSnaptimes[mSnaptimes.size() -1]]->mUpdatedtimes;
		for (std::pair<unsigned short, BaseObject*> _b : mLiveObjects)
		{
			if (_b.second)
			{
				//TODO PAT: Do somesort of duplicate infomation checking to decide to add it or not
				//If change add to mSnapinfos and Updatetime
				unsigned short id = _b.first;
				delete snap->mSnapinfos[id];
				snap->mSnapinfos[id] = GenerateSnapInfo(_b.second);
				snap->mUpdatedtimes[id] = _time;
			}
		}
	}
	return snap;
}


