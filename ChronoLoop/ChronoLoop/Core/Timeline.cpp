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
	mSnapshots[_snaptime] = _snapshot;
}

void Timeline::RewindNoClone(float _snaptime)
{
	//Look for the snap inx we want to rewind back to
	for (int i = 0; i<mSnaptimes.size(); i++)
	{
		if (_snaptime == mSnaptimes[i]) {
			mCurrentGameTimeIndx = i;
			break;
		}
	}

	MoveAllObjectsToSnap(_snaptime);

}

void Timeline::MoveAllObjectsToSnap(float _snaptime)
{
	//TODO PAT: THIS DOESNT TAKE IN ACCOUNT IF SOMETHING WAS MADE IN THE FUTURE TO DELETE IT
	Snapshot* destination = mSnapshots[_snaptime];
	for(auto object : mLiveObjects)
	{
		short id = object.second->GetUniqueID();
		SnapInfo* destInfo = destination->mSnapinfos[id];
		//If the object doesnt have a info, then check against the list for the last snap it was updated
		if( destInfo == nullptr)
		{
			float lastUpdated = destination->mUpdatedtimes[id];
			destInfo = mSnapshots[lastUpdated]->mSnapinfos[id];
		}
		//Set Object data
		BaseObject* baseobject = object.second;
		baseobject->SetTransform(destInfo->mTransform);
		//TODO PAT: WRITE A SetComponets Func to take in SnapComonets
		//baseobject->SetComponents(destInfo->mComponets)
		
	}
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
	Snapshot* snap;
	bool OldSnap = false;

	//We are on an old Snapshot
	if(mCurrentGameTimeIndx < mSnaptimes.size()-1)
	{
		_time = mSnaptimes[mCurrentGameTimeIndx];
		snap = mSnapshots[_time];
		OldSnap = true;
	}
	//We are making a new snap in the timeline
	else
	{
		snap = new Snapshot();
		snap->mTime = _time;
	}
	
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
		//Copy the exciting updated times to this one
		snap->mUpdatedtimes = mSnapshots[mSnaptimes[mCurrentGameTimeIndx]]->mUpdatedtimes;
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
	if (!OldSnap) {
		mSnaptimes.push_back(_time);
	}
	mCurrentGameTimeIndx++;
	return snap;
}


