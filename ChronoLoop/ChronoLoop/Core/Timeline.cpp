//#include "stdafx.h"
#include "Timeline.h"
#include "../Objects/Component.h"


bool Snapshot::IsObjectStored(short _id) {
	try {
		mSnapinfos.at(_id);
	} catch (std::exception e) {
		return false;
	}

	return true;
}


Timeline::Timeline() {}


Timeline::~Timeline() {
	ClearTimeLine();
}

void Timeline::AddBaseObject(BaseObject* _object, unsigned short _id) {
	mLiveObjects[_id] = _object;
}


void Timeline::AddSnapshot(unsigned int _snaptime, Snapshot* _snapshot) {
	mSnapshots[_snaptime] = _snapshot;
}

bool Timeline::RewindNoClone(unsigned int _snaptime) {
	if (_snaptime < 0 || _snaptime > mSnaptimes.size() - 1)
		return false;

	mCurrentGameTimeIndx = _snaptime;
	MoveAllObjectsToSnap(_snaptime);
	return true;
}

void Timeline::MoveAllObjectsToSnap(unsigned int _snaptime) {
	//TODO PAT: THIS DOESNT TAKE IN ACCOUNT IF SOMETHING WAS MADE IN THE FUTURE TO DELETE IT
	Snapshot* destination = mSnapshots[_snaptime];
	for (auto object : mLiveObjects) {
		short id = object.second->GetUniqueID();
		SnapInfo* destInfo = destination->mSnapinfos[id];
		//If the object doesnt have a info, then check against the list for the last snap it was updated
		if (destInfo == nullptr) {
			unsigned int lastUpdated = destination->mUpdatedtimes[id];
			destInfo = mSnapshots[lastUpdated]->mSnapinfos[id];
		}
		//Set Object data
		BaseObject* baseobject = object.second;
		baseobject->SetTransform(destInfo->mTransform);

		//Set all componets back to time recorded
		//TODO PAT: MAKE THIS MORE EFFICIENT
		for (unsigned int i = 0; i < destInfo->mComponets.size(); i++) {
			SnapComponent* destComp = destInfo->mComponets[i];
			switch (destComp->mCompType) {
				//For each of the collider in the vec
			case ComponentType::eCOMPONENT_COLLIDER:
			{
				//Loop to find the same collider component
				for (unsigned int j = 0; j < baseobject->GetComponentCount(eCOMPONENT_COLLIDER); j++) {
					Component* currComp = baseobject->GetComponentIndexed(eCOMPONENT_COLLIDER, j);
					if (currComp->GetColliderId() == destComp->mId) {
						((Collider*)currComp)->mRewind = true;
						((Collider*)currComp)->mShouldMove = false;
						((Collider*)currComp)->mAcceleration = ((SnapComponent_Physics*)destComp)->mAcc;
						((Collider*)currComp)->mVelocity = ((SnapComponent_Physics*)destComp)->mVel;
						((Collider*)currComp)->AddForce(((SnapComponent_Physics*)destComp)->mForces);
					}
				}
			}
			}
		}
		//TODO PAT: WRITE A SetComponets Func to take in SnapComonets
		//baseobject->SetComponents(destInfo->mComponets)

	}
}


void Timeline::ClearTimeLine() {
	for (auto snapshot : mSnapshots) {
		for (auto snapInfo : snapshot.second->mSnapinfos) {
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

SnapInfo* Timeline::GenerateSnapInfo(BaseObject* _object) {
	SnapInfo* info = new SnapInfo();
	info->mId = _object->GetUniqueID();
	info->mTransform = _object->GetTransform();
	//TODO PAT: ADD MORE COMPONETS WHEN WE NEED THEM

	//Physics componets
	std::vector<Component*>temp = _object->GetComponents(ComponentType::eCOMPONENT_COLLIDER);
	for (unsigned int i = 0; i < temp.size(); i++) {
		SnapComponent_Physics* newComp = new SnapComponent_Physics();
		newComp->mCompType = eCOMPONENT_COLLIDER;
		newComp->mForces = ((Collider*)temp[i])->mForces;
		newComp->mAcc = ((Collider*)temp[i])->mAcceleration;
		newComp->mVel = ((Collider*)temp[i])->mVelocity;
		newComp->mId = temp[i]->GetColliderId();
		info->mComponets.push_back(newComp);
	}

	return info;
}

Snapshot* Timeline::GenerateSnapShot(unsigned int _time) {
	Snapshot* snap;
	bool OldSnap = false;

	//We are making a new snap in the timeline
	//If the CurrentFrame is the last one on the list, make a new one
	if (mCurrentGameTimeIndx == mSnaptimes.size() - 1 || mSnaptimes.size() == 0) {
		snap = new Snapshot();
		snap->mTime = _time;
	}
	//We are on an old Snapshot
	else {
		snap = mSnapshots[_time];
		OldSnap = true;
	}

	//If first snapshot taken
	if (mSnapshots.size() == 0) {
		for (std::pair<unsigned short, BaseObject*> _b : mLiveObjects) {
			if (_b.second) {
				unsigned short id = _b.first;
				snap->mSnapinfos[id] = GenerateSnapInfo(_b.second);
				snap->mUpdatedtimes[id] = _time;
			}
		}
	} else {
		//Copy the exciting updated times to this one
		snap->mUpdatedtimes = mSnapshots[mSnaptimes[mCurrentGameTimeIndx]]->mUpdatedtimes;
		for (std::pair<unsigned short, BaseObject*> _b : mLiveObjects) {
			if (_b.second) {
				unsigned short id = _b.first;
				//delete an old snapshot
				if (snap->mSnapinfos[id] != nullptr)
					delete snap->mSnapinfos[id];
				//If change add to mSnapinfos and Updatetime
			    //if (!CheckForDuplicateData(id,_b.second)) {
				snap->mSnapinfos[id] = GenerateSnapInfo(_b.second);
				snap->mUpdatedtimes[id] = _time;

			}
		}
	}
	if (!OldSnap) {
		mSnaptimes.push_back(_time);
	}

	if (mSnapshots.size() != 0)
		mCurrentGameTimeIndx++;

	return snap;
}

//Returns True if the data is the same from last snap
bool Timeline::CheckForDuplicateData(unsigned short _id, BaseObject* _object) {
	SnapInfo* info = mSnapshots[mSnaptimes[mCurrentGameTimeIndx]]->mSnapinfos[_id];
	//TODO PAT: Once all the structs and componets are final fill this out
	return true;
}