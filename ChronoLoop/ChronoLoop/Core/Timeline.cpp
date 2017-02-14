//#include "stdafx.h"
#include "Timeline.h"
#include "../Objects/Component.h"


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
			for (auto snapComps : snapInfo.second->mComponets)
				delete snapComps;

			snapInfo.second->mComponets.clear();

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
	info->mId = _object->GetUniqueID();
	info->mTransform = _object->GetTransform();
	

	//Componet information
	for (unsigned int i = 0; i < _object->GetNumofComponets(); i++) {
		 Component* temp = _object->GetComponet(i);

		//If there is a componet we want to store make a snap component
		switch (temp->GetType())
		{
		case ComponentType::eCOMPONENT_COLLIDER:
			{
			SnapComponent* newComp = new SnapComponent();
			newComp->CompType = temp->GetType();
			((SnapComponent_Physics*)newComp)->acceleration = ((Collider*)temp)->GetAcceleration();
			((SnapComponent_Physics*)newComp)->velocity = ((Collider*)temp)->GetVelocity();
			info->mComponets.push_back(newComp);
			break;
			}
		default:
			//This component is not having its information stored in snapshot
			break;
		}
	}

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


