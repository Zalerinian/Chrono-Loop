//#include "stdafx.h"
#include "Timeline.h"
#include "../Objects/Component.h"
#include "../Input/VrInputManager.h"
#include "../Rendering/Renderer.h"



bool Snapshot::IsObjectStored(unsigned short _id) {
	try {
		mSnapinfos.at(_id);
	} catch (std::exception e) {
		return false;
	}

	return true;
}


Timeline::Timeline() {

}


Timeline::~Timeline() {
	ClearTimeLine();
}

void Timeline::AddBaseObject(BaseObject* _object, unsigned short _id) {
	mLiveObjects[_id] = _object;
}


void Timeline::AddSnapshot(unsigned int _snaptime, Snapshot* _snapshot) {
	mSnapshots[_snaptime] = _snapshot;
}

bool Timeline::RewindNoClone(unsigned int _snaptime, unsigned short _id1, unsigned short _id2, unsigned short _id3) {
	if (_snaptime < 0 || _snaptime > mSnaptimes.size() - 1)
		return false;

	mCurrentGameTimeIndx = _snaptime;
	MoveAllObjectsToSnapExceptPlayer(_snaptime, _id1, _id2, _id3);
	return true;
}

bool Timeline::RewindMakeClone(unsigned int _snaptime) {
	if (_snaptime < 0 || _snaptime > mSnaptimes.size() - 1)
		return false;
	mCurrentGameTimeIndx = _snaptime;
	MoveAllObjectsToSnap(_snaptime);
	return true;
}

void Timeline::ChangeBitsetToSnap(SnapInfo * _destinfo, Component* _curComp)
{
	if(_destinfo->mBitset[_curComp->GetComponentNum()])
		_curComp->Enable();

	else
		_curComp->Disable();
	
}

void Timeline::SetComponent(SnapComponent* _destComp, BaseObject * _obj, SnapInfo* _destInfo) {
	switch (_destComp->mCompType) {
		//For each of the collider in the vec
	case ComponentType::eCOMPONENT_COLLIDER:
	{
		//Loop to find the same collider component
		for (unsigned int j = 0; j < _obj->GetComponentCount(eCOMPONENT_COLLIDER); j++) {
			Component* currComp = _obj->GetComponentIndexed(eCOMPONENT_COLLIDER, j);
			if (currComp->GetColliderId() == _destComp->mId) {
				((Collider*)currComp)->mRewind = true;
				((Collider*)currComp)->mShouldMove = false;
				((Collider*)currComp)->mAcceleration = ((SnapComponent_Physics*)_destComp)->mAcc;
				((Collider*)currComp)->mVelocity = ((SnapComponent_Physics*)_destComp)->mVel;
				((Collider*)currComp)->AddForce(((SnapComponent_Physics*)_destComp)->mForces);
				((Collider*)currComp)->SetPos(*_destInfo->mTransform.GetPosition());

				//Set the bitset
				ChangeBitsetToSnap(_destInfo, currComp);
			}
		}
	}
	default:
	{
		for (unsigned int j = 0; j < _obj->GetComponentCount(_destComp->mCompType); j++) {
			Component* currComp = _obj->GetComponentIndexed(_destComp->mCompType, j);
			if (currComp->GetColliderId() == _destComp->mId) {
				//Set the bitset
				ChangeBitsetToSnap(_destInfo, currComp);
			}
		}
	}
	}
}

void Timeline::MoveObjectToSnap(unsigned int _snaptime, unsigned short _id) {
	//TODO PAT: THIS DOESNT TAKE IN ACCOUNT IF SOMETHING WAS MADE IN THE FUTURE TO DELETE IT
	Snapshot* destination = mSnapshots[_snaptime];
	SnapInfo* destInfo;
	//If the object doesnt have a info, then check against the list for the last snap it was updated
	bool stored = destination->IsObjectStored(_id);
	if (stored) {
		destInfo = destination->mSnapinfos[_id];
	} else if (!stored) {
		unsigned int lastUpdated = destination->mUpdatedtimes[_id];
		destInfo = mSnapshots[lastUpdated]->mSnapinfos[_id];
	}
	//Set Object data
	BaseObject* baseobject = mLiveObjects[_id];
	baseobject->SetTransform(destInfo->mTransform);

	//Set all componets to time recorded
	for (unsigned int i = 0; i < destInfo->mComponets.size(); i++) {
		SnapComponent* destComp = destInfo->mComponets[i];
		SetComponent(destComp, baseobject, destInfo);
	}

}

void Timeline::MoveAllObjectsToSnap(unsigned int _snaptime) {
	//TODO PAT: THIS DOESNT TAKE IN ACCOUNT IF SOMETHING WAS MADE IN THE FUTURE TO DELETE IT
	Snapshot* destination = mSnapshots[_snaptime];
	for (auto object : mLiveObjects) {
		unsigned short id = object.second->GetUniqueID();
		SnapInfo* destInfo;
		//If the object doesnt have a info, then check against the list for the last snap it was updated
		bool stored = destination->IsObjectStored(id);
		if (stored) {
			destInfo = destination->mSnapinfos[id];
		} else if (!stored) {
			unsigned int lastUpdated = destination->mUpdatedtimes[id];
			if (lastUpdated == 0) //assume its broken
				continue;
			destInfo = mSnapshots[lastUpdated]->mSnapinfos[id];
		}
		//Set Object data
		BaseObject* baseobject = object.second;
		if (baseobject)
			baseobject->SetTransform(destInfo->mTransform);

		//Set all componets back to time recorded
		for (unsigned int i = 0; i < destInfo->mComponets.size(); i++) {
			SnapComponent* destComp = destInfo->mComponets[i];
			SetComponent(destComp, baseobject, destInfo);
		}
	}
}

void Timeline::MoveAllObjectsToSnapExceptPlayer(unsigned int _snaptime, unsigned short _id1, unsigned short _id2, unsigned short _id3) {
	//TODO PAT: THIS DOESNT TAKE IN ACCOUNT IF SOMETHING WAS MADE IN THE FUTURE or past
	Snapshot* destination = mSnapshots[_snaptime];
	for (auto object : mLiveObjects) {
		unsigned short id = object.second->GetUniqueID();
		if (id == _id1 || id == _id2 || id == _id3)
			return;
		SnapInfo* destInfo;
		//If the object doesnt have a info, then check against the list for the last snap it was updated
		bool stored = destination->IsObjectStored(id);
		if (stored) {
			destInfo = destination->mSnapinfos[id];
		} else if (!stored) {
			unsigned int lastUpdated = destination->mUpdatedtimes[id];
			if (lastUpdated == 0) //assume its broken
				continue;
			destInfo = mSnapshots[lastUpdated]->mSnapinfos[id];
		}
		//Set Object data
		BaseObject* baseobject = object.second;
		baseobject->SetTransform(destInfo->mTransform);

		//Set all componets back to time recorded
		//TODO PAT: WRITE A SetComponets Func to take in SnapComonets.
		for (unsigned int i = 0; i < destInfo->mComponets.size(); i++) {
			SnapComponent* destComp = destInfo->mComponets[i];
			SetComponent(destComp, baseobject, destInfo);
		}
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


SnapInfo* Timeline::GenerateSnapInfo(BaseObject* _object, SnapInfo* _info) {
	if (_info == nullptr)
		_info = new SnapInfo();
	_info->mId = _object->GetUniqueID();
	_info->mTransform = _object->GetTransform();
	//making a new snap info. If this obj
	//_info->mBitset[0] = true;

	//TODO PAT: ADD MORE COMPONETS WHEN WE NEED THEM.
	std::vector<Component*>temp;
	for (int i = 1; i < ComponentType::eCOMPONENT_MAX; i++) {
		{
			if (i == ComponentType::eCOMPONENT_COLLIDER) {
				//Physics componets
				temp = _object->GetComponents(ComponentType::eCOMPONENT_COLLIDER);
				for (unsigned int i = 0; i < temp.size(); i++) {
					SnapComponent_Physics* newComp = new SnapComponent_Physics();
					newComp->mCompType = eCOMPONENT_COLLIDER;
					newComp->mForces = ((Collider*)temp[i])->mForces;
					newComp->mAcc = ((Collider*)temp[i])->mAcceleration;
					newComp->mVel = ((Collider*)temp[i])->mVelocity;
					newComp->mId = temp[i]->GetColliderId();
					newComp->mBitNum = temp[i]->GetComponentNum();
					_info->mComponets.push_back(newComp);
				}
			} else {
				temp = _object->GetComponents((ComponentType)i);
				for (unsigned int i = 0; i < temp.size(); i++) {
					SnapComponent* newComp = new SnapComponent;
					newComp->mCompType = eCOMPONENT_UNKNOWN;
					newComp->mId = temp[i]->GetColliderId();
					newComp->mBitNum = temp[i]->GetComponentNum();
					_info->mComponets.push_back(newComp);
				}
			}
		}
	}

	return _info;
}

//SnapInfoPlayer * Timeline::GenerateSnapInfoPlayer() {
//	SnapInfoPlayer* snapP = new SnapInfoPlayer(
//		*RenderEngine::Renderer::Instance()->GetPlayerWorldPos(),//Player World Matrix
//		Math::FromMatrix(VRInputManager::Instance().GetController(true).GetPose().mDeviceToAbsoluteTracking),//Left Controller World Matrix
//		Math::FromMatrix(VRInputManager::Instance().GetController(false).GetPose().mDeviceToAbsoluteTracking));//Right Controller World Matrix
//
//	return snapP;
//}


Snapshot* Timeline::GenerateSnapShot(unsigned int _time, std::vector<BaseObject*> & _clones) {
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
				snap->mSnapinfos[id] = GenerateSnapInfo(_b.second, nullptr);
				snap->mUpdatedtimes[id] = _time;
			}
		}
	} else {
		//Copy the exciting updated times to this one
		snap->mUpdatedtimes = mSnapshots[mSnaptimes[mCurrentGameTimeIndx]]->mUpdatedtimes;
		for (std::pair<unsigned short, BaseObject*> _b : mLiveObjects) {
			if (_b.second) {
				unsigned short id = _b.first;
				if (_clones.size() > 0) {
					for (unsigned int i = 0; i < _clones.size(); i++) {
						//Move clone to next frame if frame is avalible
						if (snap->mSnapinfos[id] != nullptr && id == _clones[i]->GetUniqueId()) {
							MoveObjectToSnap(_time, id);
						}
						//If we are a clone but dont have a next movement then record one at position
						else if (snap->mSnapinfos[id] == nullptr && id == _clones[i]->GetUniqueId()) {
							//If change add to mSnapinfos and Updatetime
							if (!CheckForDuplicateData(id, _b.second)) {
								snap->mSnapinfos[id] = GenerateSnapInfo(_b.second, nullptr);
								snap->mUpdatedtimes[id] = _time;
							}
						}
						//If we made it through the list do the normal
						else if (id != _clones[i]->GetUniqueId() && i == _clones.size() - 1) {
							//If change add to mSnapinfos and Updatetime
							if (!CheckForDuplicateData(id, _b.second)) {
								snap->mSnapinfos[id] = GenerateSnapInfo(_b.second, snap->mSnapinfos[id]);
								snap->mUpdatedtimes[id] = _time;
							}
						}
					}
				} else {
					//if (snap->mSnapinfos[id] != nullptr)
					//	delete snap->mSnapinfos[id];
					//If change add to mSnapinfos and Updatetime
					if (!CheckForDuplicateData(id, _b.second)) {
						snap->mSnapinfos[id] = GenerateSnapInfo(_b.second, snap->mSnapinfos[id]);
						snap->mUpdatedtimes[id] = _time;
					}
				}
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
	if (mCurrentGameTimeIndx == 0)
		return false;
	SnapInfo* info = mSnapshots[mSnaptimes[mCurrentGameTimeIndx-1]]->mSnapinfos[_id];
	if (!info)
		info = mSnapshots[mSnapshots[mSnaptimes[mCurrentGameTimeIndx - 1]]->mUpdatedtimes[_id]]->mSnapinfos[_id];

	if (info->mTransform != _object->GetTransform())
		return false;

	for (unsigned int i = 0; i < info->mComponets.size(); i++) {
		SnapComponent* comp = info->mComponets[i];

		switch (comp->mCompType) {
			//For each of the collider in the vec
		case ComponentType::eCOMPONENT_COLLIDER:
		{
			//Loop to find the same collider component
			for (unsigned int j = 0; j < _object->GetComponentCount(eCOMPONENT_COLLIDER); j++) {
				Component* currComp = _object->GetComponentIndexed(eCOMPONENT_COLLIDER, j);
				if (currComp->GetColliderId() == comp->mId) {
					if (((Collider*)currComp)->mAcceleration != ((SnapComponent_Physics*)comp)->mAcc ||
						((Collider*)currComp)->mVelocity != ((SnapComponent_Physics*)comp)->mVel ||
						((Collider*)currComp)->mForces != ((SnapComponent_Physics*)comp)->mForces)
						return false;
				}
			}
		}
		}
	}

	return true;
}