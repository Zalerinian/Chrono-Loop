//#include "stdafx.h"*
#include "Timeline.h"
#include "../Objects/Component.h"
#include "../Input/VrInputManager.h"
#include "../Rendering/Renderer.h"
#include "../Objects/MeshComponent.h"
#include "../Common/Logger.h"
#include "Pool.h"
#include "LevelManager.h"
#include "../Common/Settings.h"
#include "../Actions/CCMazeHelper.h"

namespace Epoch {
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

	void Timeline::ActivateCloneBitset(unsigned short _ids[3]) {
		Snapshot* snap = mSnapshots[mSnaptimes[mCurrentGameTimeIndx]];
		//This will will always exist in mLiveobjects so no checking needed
		BaseObject* obj1 = mLiveObjects[_ids[0]];
		BaseObject* obj2 = mLiveObjects[_ids[1]];
		BaseObject* obj3 = mLiveObjects[_ids[2]];

		SnapInfo* info;
		for (unsigned int i = 0; i < 3; i++) {
			//If the clone doesnt have a snap at this time, 1)find the most recent and snap. 2)Copy it. 3)make it a snap in the current time
			if (snap->mSnapinfos.find(_ids[i]) == snap->mSnapinfos.end()) {
				if (snap->mUpdatedtimes.find(_ids[i]) != snap->mUpdatedtimes.end()) {
					Snapshot* destSnap = mSnapshots[snap->mUpdatedtimes[_ids[i]]];
					if (destSnap->mSnapinfos.find(_ids[i]) == destSnap->mSnapinfos.end())
						continue;

					info = mSnapshots[snap->mUpdatedtimes[_ids[i]]]->mSnapinfos[_ids[i]];
					SnapInfo* cpyInfo = new SnapInfo();
					CopySnapInfo(info, cpyInfo);
					snap->mSnapinfos[_ids[i]] = cpyInfo;
					snap->mUpdatedtimes[_ids[i]] = mCurrentGameTimeIndx;
					info = snap->mSnapinfos[_ids[i]];
					info->mBitset[0] = true;
				}
				//The object hasnt been made yet. Make the make time the birth time
				else {
					if (mObjectLifeTimes.find(_ids[i]) != mObjectLifeTimes.end()) {
						unsigned int birth = mObjectLifeTimes[_ids[i]]->mBirth;
						((CloneLifeTime*)mObjectLifeTimes[_ids[i]])->mMade = birth;

					} else {
						SystemLogger::GetError() << "Object id: " << _ids[i] << " didnt have a birth time";
					}
				}

			} else {
				info = snap->mSnapinfos[_ids[i]];
				info->mBitset[0] = true;
			}

		}
		SetCloneMadeTime(_ids[0], _ids[1], _ids[2]);
	}

	void Timeline::AddBaseObject(BaseObject* _object, unsigned short _id) {
		mLiveObjects[_id] = _object;
		ObjectLifeTime* newObject = new ObjectLifeTime();
		if (mSnaptimes.size() != 0)
			newObject->mBirth = mSnaptimes[mCurrentGameTimeIndx];
		else
			newObject->mBirth = 0;

		mObjectLifeTimes[_id] = newObject;

	}
	void Timeline::UpdatePlayerBaseObject(BaseObject* _object, unsigned short _id) {
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

		//Set interpolators back and turn off;
		std::vector<BaseObject*>clones = TimeManager::Instance()->GetClonesVec();
		for (unsigned int i = 0; i < clones.size(); i++) {
			Interpolator<matrix4>* temp = TimeManager::Instance()->GetCloneInterpolator(clones[i]->GetUniqueID());
			Interpolator<matrix4>* tempCol = nullptr;
			//	if(clones[i]->GetComponentCount(eCOMPONENT_COLLIDER) > 0)
				//tempCol= TimeManager::Instance()->GetCloneColliderInterpolator(clones[i]->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)->GetColliderId());
			if (temp)
				temp->SetActive(false);
			//if (tempCol)
				//tempCol->SetActive(false);
		}

		//CheckforLostObjects(TimeManager::Instance()->GetClonesVec());
		return true;
	}

	void Timeline::ResetTimelineAndLevel() {
		std::vector<BaseObject*>clones = TimeManager::Instance()->GetClonesVec();
		for (auto lifespan : mObjectLifeTimes) {
			//Delete the creation of the clones
			//Clones should only have been born after 0
			if (lifespan.second && lifespan.second->mBirth > 0)
				delete lifespan.second;
		}

		for (auto objects = mLiveObjects.begin(); objects != mLiveObjects.end(); ++objects) {
			for (unsigned int i = 0; i < clones.size(); ++i) {
				if ((*objects).second && (*objects).second->GetUniqueID() == clones[i]->GetUniqueID()) {
					auto tempIter = (*objects);
					++objects;
					mLiveObjects.erase(tempIter.first);
				}
			}
		}
		if(mCloneDone.size() > 0)
		mCloneDone.clear();
	}

	void Timeline::RemoveFromTimeline(unsigned short _id) {
		mLiveObjects.erase(_id);
		if (mObjectLifeTimes.find(_id) != mObjectLifeTimes.end())
			mObjectLifeTimes.erase(_id);
	}

	bool Timeline::RewindMakeClone(unsigned int _snaptime) {
		if (_snaptime < 0 || _snaptime > mSnaptimes.size() - 1)
			return false;
		mCurrentGameTimeIndx = _snaptime;
		MoveAllObjectsToSnap(_snaptime);

		//Set interpolators back and turn off
		std::vector<BaseObject*>clones = TimeManager::Instance()->GetClonesVec();
		for (unsigned int i = 0; i < clones.size(); i++) {
			Interpolator<matrix4>* temp = TimeManager::Instance()->GetCloneInterpolator(clones[i]->GetUniqueID());
			Interpolator<matrix4>* tempCol = nullptr;
			//if (clones[i]->GetComponentCount(eCOMPONENT_COLLIDER) > 0)
				//tempCol = TimeManager::Instance()->GetCloneColliderInterpolator(clones[i]->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)->GetColliderId());
			if (temp)
				temp->SetActive(false);
			//if (tempCol)
				//tempCol->SetActive(false);
		}

		//CheckforLostObjects(TimeManager::Instance()->GetClonesVec());
		return true;
	}

	void Timeline::ChangeBitsetToSnap(SnapInfo * _destinfo, Component* _curComp) {
		unsigned short bitnum = _curComp->GetComponentNum();

		//Object is active
		if (_destinfo->mBitset[0]) {
			//If the object is active
			if (_curComp->GetType() != eCOMPONENT_MESH) {
				if (_destinfo->mBitset[bitnum])
					_curComp->Enable();

				else
					_curComp->Disable();
			}
			//The comp is a mesh
			else {
				if (_destinfo->mBitset[bitnum])
					((MeshComponent*)_curComp)->SetVisible(true);

				else
					((MeshComponent*)_curComp)->SetVisible(false);
			}
		}

		//Object is inactive
		else {
			if (_curComp->GetType() != eCOMPONENT_MESH) {
				_curComp->Disable();
			}
			//The comp is a mesh
			else {
				((MeshComponent*)_curComp)->SetVisible(false);
			}
		}
	}
	//This one doesnt care if its active or not
	void Timeline::ChangePlayerBitsetToSnap(SnapInfo * _destinfo, Component* _curComp) {
		unsigned short bitnum = _curComp->GetComponentNum();


		if (_curComp->GetType() != eCOMPONENT_MESH) {
			if (_destinfo->mBitset[bitnum])
				_curComp->Enable();

			else
				_curComp->Disable();
		}
		//The comp is a mesh
		else {
			if (_destinfo->mBitset[bitnum])
				((MeshComponent*)_curComp)->SetVisible(true);

			else
				((MeshComponent*)_curComp)->SetVisible(false);
		}

	}


	void Timeline::SetComponent(SnapComponent* _destComp, BaseObject * _obj, SnapInfo* _destInfo) {
		switch (_destComp->mCompType) {
			//For each of the collider in the vec
		case ComponentType::eCOMPONENT_COLLIDER:
		{
			//Loop to find the same collider component
			for (unsigned int j = 0; j < _obj->GetComponentCount(eCOMPONENT_COLLIDER); j++) {
				Component* currComp = _obj->GetComponentIndexed(eCOMPONENT_COLLIDER, j);
				//if (currComp->GetColliderId() == _destComp->mId) {
					//((Collider*)currComp)->mRewind = true;
					//((Collider*)currComp)->mShouldMove = false;
				((Collider*)currComp)->mAcceleration = ((SnapComponent_Physics*)_destComp)->mAcc;
				((Collider*)currComp)->mVelocity = ((SnapComponent_Physics*)_destComp)->mVel;
				((Collider*)currComp)->mTotalForce = ((SnapComponent_Physics*)_destComp)->mTotforce;
				((Collider*)currComp)->AddForce(((SnapComponent_Physics*)_destComp)->mForces);
				((Collider*)currComp)->SetPos(*_destInfo->mTransform.GetPosition());

				//Set the bitset
				Level* currlevel = LevelManager::GetInstance().GetCurrentLevel();
				if (_obj->GetUniqueID() == currlevel->GetLeftController()->GetUniqueID() ||
					_obj->GetUniqueID() == currlevel->GetRightController()->GetUniqueID() ||
					_obj->GetUniqueID() == currlevel->GetHeadset()->GetUniqueID())
					ChangePlayerBitsetToSnap(_destInfo, currComp);
				else
					ChangeBitsetToSnap(_destInfo, currComp);
				//}
			}
			break;
		}
		case ComponentType::eCOMPONENT_CODE:
		{
			for (unsigned int j = 0; j < _obj->GetComponentCount(eCOMPONENT_CODE); ++j) {
				Component* currComp = _obj->GetComponentIndexed(eCOMPONENT_CODE, j);
				if (dynamic_cast<CCMazeHelper*>(currComp) && currComp->GetColliderId() == _destComp->mId) {
					for (int x = 0; x < 4; ++x) {
						for (int y = 0; y < 4; ++y) {
							((CCMazeHelper*)currComp)->mGrid[x][y] = ((SnapComponent_Code_MazeHelp*)_destComp)->mMazeGrid[x][y];
						}
					}
					((CCMazeHelper*)currComp)->SetBoxesAsGrid();
					((CCMazeHelper*)currComp)->SetBoxesPosition(&((CCMazeHelper*)currComp)->mazeBoxes[0].mBox->GetTransform().GetMatrix(),
						&((CCMazeHelper*)currComp)->mazeBoxes[1].mBox->GetTransform().GetMatrix(),
						&((CCMazeHelper*)currComp)->mazeBoxes[2].mBox->GetTransform().GetMatrix());
					//Set the bitset
					Level* currlevel = LevelManager::GetInstance().GetCurrentLevel();
					if (_obj->GetUniqueID() == currlevel->GetLeftController()->GetUniqueID() ||
						_obj->GetUniqueID() == currlevel->GetRightController()->GetUniqueID() ||
						_obj->GetUniqueID() == currlevel->GetHeadset()->GetUniqueID())
						ChangePlayerBitsetToSnap(_destInfo, currComp);
					else
						ChangeBitsetToSnap(_destInfo, currComp);
				} else if (currComp->GetColliderId() == _destComp->mId) {
					//Set the bitset
					Level* currlevel = LevelManager::GetInstance().GetCurrentLevel();
					if (_obj->GetUniqueID() == currlevel->GetLeftController()->GetUniqueID() ||
						_obj->GetUniqueID() == currlevel->GetRightController()->GetUniqueID() ||
						_obj->GetUniqueID() == currlevel->GetHeadset()->GetUniqueID())
						ChangePlayerBitsetToSnap(_destInfo, currComp);
					else
						ChangeBitsetToSnap(_destInfo, currComp);
				}
			}
			break;
		}
		default:
		{
			for (unsigned int j = 0; j < _obj->GetComponentCount(_destComp->mCompType); j++) {
				Component* currComp = _obj->GetComponentIndexed(_destComp->mCompType, j);
				if (currComp->GetColliderId() == _destComp->mId) {
					//Set the bitset
					Level* currlevel = LevelManager::GetInstance().GetCurrentLevel();
					if (_obj->GetUniqueID() == currlevel->GetLeftController()->GetUniqueID() ||
						_obj->GetUniqueID() == currlevel->GetRightController()->GetUniqueID() ||
						_obj->GetUniqueID() == currlevel->GetHeadset()->GetUniqueID())
						ChangePlayerBitsetToSnap(_destInfo, currComp);
					else
						ChangeBitsetToSnap(_destInfo, currComp);
				}
			}
			break;
		}
		}
	}

	void Timeline::UpdateCloneInterpolators(unsigned short _cloneid, SnapInfo* _currSnap, unsigned int _currTime) {
		Snapshot* nextsnap;
		SnapInfo* nextInfo;
		Interpolator<matrix4>* cloneInterp = TimeManager::Instance()->GetCloneInterpolator(_cloneid);
		//Interpolator<matrix4>* cloneColliderInterp = nullptr;
		//if (mLiveObjects.find(_cloneid) != mLiveObjects.end() && mLiveObjects[_cloneid]->GetComponentCount(eCOMPONENT_COLLIDER) > 0)
		//{
		//	cloneColliderInterp = TimeManager::Instance()->GetCloneColliderInterpolator(mLiveObjects[_cloneid]->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)->GetColliderId());
		//}

		if (mObjectLifeTimes.find(_cloneid) != mObjectLifeTimes.end() && (mObjectLifeTimes[_cloneid]->mBirth > mCurrentGameTimeIndx || mObjectLifeTimes[_cloneid]->mDeath < mCurrentGameTimeIndx)) {
			cloneInterp->SetActive(false);
			//if(cloneColliderInterp)
			//cloneColliderInterp->SetActive(false);
		}

		if (_currTime + 1 <= mSnaptimes.size() - 1) {
			nextsnap = mSnapshots[mSnaptimes[(unsigned int)_currTime + 1]];
			//found a clone snapinfo in the next snapshot
			if (nextsnap->mSnapinfos.find(_cloneid) != nextsnap->mSnapinfos.end()) {
				nextInfo = nextsnap->mSnapinfos[_cloneid];
				cloneInterp->SetActive(true);
				//if (cloneColliderInterp)
				//cloneColliderInterp->SetActive(true);
				//Loop to find the same clone's baseObject
				std::vector<BaseObject*> clones = TimeManager::Instance()->GetClonesVec();
				for (int i = 0; i < clones.size(); ++i) {
					if (_cloneid == clones[i]->GetUniqueID()) {
						cloneInterp->Prepare(0.1f, _currSnap->mTransform.GetMatrix(), nextInfo->mTransform.GetMatrix(), clones[i]->GetTransform().GetMatrix());
						//if (cloneColliderInterp)
						//cloneColliderInterp->Prepare(0.1f, _currSnap->mTransform.GetMatrix(), nextInfo->mTransform.GetMatrix(), clones[i]->GetComponentIndexed(eCOMPONENT_COLLIDER,0)->GetTransform().GetMatrix());
						break;
					}
				}
			} else {
				cloneInterp->SetActive(false);
				//if (cloneColliderInterp)
				//cloneColliderInterp->SetActive(false);
			}
		} else {
			cloneInterp->SetActive(false);
			//if (cloneColliderInterp)
			//cloneColliderInterp->SetActive(false);
		}
	}
	void Timeline::PrepareAllObjectInterpolators(unsigned int _fromSnapTime, unsigned int _toSnapTime) {
		Interpolator<matrix4> * objInterp;
		if (_toSnapTime <= mSnaptimes.size() - 1 && _toSnapTime >= 0) {
			Snapshot* _fromShot = mSnapshots[mSnaptimes[_fromSnapTime]];
			Snapshot* _toShot = mSnapshots[mSnaptimes[_toSnapTime]];
			unsigned int temp2 = LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->GetNumClones();

			for (std::pair<unsigned short, Epoch::BaseObject*> it : mLiveObjects) {
				if (it.second->GetName().find("Controller1 - " + std::to_string(temp2)) == std::string::npos &&
					it.second->GetName().find("Controller2 - " + std::to_string(temp2)) == std::string::npos) {
					objInterp = TimeManager::Instance()->GetObjectInterpolator(it.first);
					if (_fromShot->mSnapinfos.find(it.first) != _fromShot->mSnapinfos.end() && _toShot->mSnapinfos.find(it.first) != _toShot->mSnapinfos.end()) {
						SnapInfo* _fromSnap = _fromShot->mSnapinfos[it.first];
						SnapInfo* _toSnap = _toShot->mSnapinfos[it.first];

						objInterp->SetActive(true);
						objInterp->Prepare(mObjectInterpolationTime, _fromSnap->mTransform.GetMatrix(), _toSnap->mTransform.GetMatrix(), it.second->GetTransform().GetMatrix());
					} else {
						objInterp->SetActive(false);
					}
				}
			}
		}
	}

	void Timeline::SetCloneObjectCreationTime(unsigned short _id1, unsigned short _id2, unsigned short _id3) {
		ObjectLifeTime* newObject = new ObjectLifeTime();
		newObject->mBirth = mSnaptimes[mCurrentGameTimeIndx];
		mObjectLifeTimes[_id1] = newObject;

		ObjectLifeTime* newObject1 = new ObjectLifeTime();
		newObject1->mBirth = mSnaptimes[mCurrentGameTimeIndx];
		mObjectLifeTimes[_id2] = newObject1;

		ObjectLifeTime* newObject2 = new ObjectLifeTime();
		newObject2->mBirth = mSnaptimes[mCurrentGameTimeIndx];
		mObjectLifeTimes[_id3] = newObject2;

		SystemLogger::GetLog() << "Clone Birth: " << mCurrentGameTimeIndx << " snapshot" << std::endl;

	}

	void Timeline::SetCloneDeathTime(unsigned short _id1, unsigned short _id2, unsigned short _id3) {
		if (mObjectLifeTimes.find(_id1) != mObjectLifeTimes.end()) {
			ObjectLifeTime* newObject = mObjectLifeTimes[_id1];
			newObject->mDeath = mSnaptimes[mCurrentGameTimeIndx];
		}

		if (mObjectLifeTimes.find(_id2) != mObjectLifeTimes.end()) {
			ObjectLifeTime* newObject1 = mObjectLifeTimes[_id2];
			newObject1->mDeath = mSnaptimes[mCurrentGameTimeIndx];
		}

		if (mObjectLifeTimes.find(_id3) != mObjectLifeTimes.end()) {
			ObjectLifeTime* newObject2 = mObjectLifeTimes[_id3];
			newObject2->mDeath = mSnaptimes[mCurrentGameTimeIndx];
		}
		SystemLogger::GetLog() << "Clone Death: " << mCurrentGameTimeIndx << " snapshot" << std::endl;
	}

	void Timeline::SetCloneMadeTime(unsigned short _id1, unsigned short _id2, unsigned short _id3) {
		CloneLifeTime* newObject = new CloneLifeTime();
		newObject->mMade = mSnaptimes[mCurrentGameTimeIndx];
		mObjectLifeTimes[_id1] = newObject;

		CloneLifeTime* newObject1 = new CloneLifeTime();
		newObject1->mMade = mSnaptimes[mCurrentGameTimeIndx];
		mObjectLifeTimes[_id2] = newObject1;

		CloneLifeTime* newObject2 = new CloneLifeTime();
		newObject2->mMade = mSnaptimes[mCurrentGameTimeIndx];
		mObjectLifeTimes[_id3] = newObject2;

		SystemLogger::GetLog() << "Clone MadeTime: " << mCurrentGameTimeIndx << " snapshot" << std::endl;
	}

	void Timeline::SetBaseObjectDeathTime(unsigned short _id) {
		if (mObjectLifeTimes.find(_id) != mObjectLifeTimes.end()) {
			ObjectLifeTime* newObject = mObjectLifeTimes[_id];
			newObject->mDeath = mSnaptimes[mCurrentGameTimeIndx];
		}
	}


	//This hasn't been tested yet
	void Timeline::CheckforLostObjects(std::vector<BaseObject*>& mClones) {
		Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();
		for (auto obj : mObjectLifeTimes) {
			if (obj.second->mBirth > mCurrentGameTimeIndx) {
				for (unsigned int i = 0; i < mClones.size(); i++) {
					if (mClones[i]->GetUniqueID() == obj.first || cLevel->GetLeftController()->GetUniqueID() == obj.first || cLevel->GetHeadset()->GetUniqueID() == obj.first || cLevel->GetRightController()->GetUniqueID() == obj.first)
						break;

					if (mClones[i]->GetUniqueID() != obj.first && i == mClones.size() - 1) {
						//delete the birth and death struct
						delete obj.second;

						//get rid of object if its in physics
						for (int k = 0; k < Physics::Instance()->mObjects.size(); ++k) {
							if (Physics::Instance()->mObjects[k]->GetUniqueID() == obj.first) {
								Physics::Instance()->mObjects.erase(Physics::Instance()->mObjects.begin() + k);
							}
						}

						mLiveObjects[obj.first]->RemoveAllComponents();

						//add it back to the pool
						Pool::Instance()->iRemoveObject(obj.first);
						mObjectLifeTimes.erase(obj.first);
						mLiveObjects.erase(obj.first);
					}
				}
			}
		}
	}


	void Timeline::CopySnapInfo(SnapInfo * _src, SnapInfo * _dst) {
		_dst->mTransform = _src->mTransform;
		_dst->mId = _src->mId;
		for (unsigned int i = 0; i < _src->mComponents.size(); i++) {
			if (_src->mComponents[i]->mCompType == ComponentType::eCOMPONENT_COLLIDER) {
				SnapComponent_Physics* temp = new SnapComponent_Physics();
				memcpy(temp, (SnapComponent_Physics*)_src->mComponents[i], sizeof(SnapComponent_Physics));
			} else {
				SnapComponent* temp = new SnapComponent();
				memcpy(temp, _src->mComponents[i], sizeof(SnapComponent));
			}
		}
	}

	ObjectLifeTime * Timeline::GetObjectLifetime(unsigned short _id) {
		if (mObjectLifeTimes.find(_id) != mObjectLifeTimes.end())
			return mObjectLifeTimes[_id];

		return nullptr;
	}

	void Timeline::MoveObjectToSnap(unsigned int _snaptime, unsigned short _id, bool isClone) {

		Snapshot* destination = mSnapshots[_snaptime];
		SnapInfo* destInfo;
		//If the object doesnt have a info, then check against the list for the last snap it was updated
		bool stored = destination->IsObjectStored(_id);
		if (stored) {
			destInfo = destination->mSnapinfos[_id];
		} else if (!stored) {
			if (destination->mUpdatedtimes.find(_id) == destination->mUpdatedtimes.end())
				return;
			destInfo = mSnapshots[destination->mUpdatedtimes[_id]]->mSnapinfos[_id];
		}

		//Set Object data
		BaseObject* baseobject = mLiveObjects[_id];
		baseobject->SetTransform(destInfo->mTransform);


		//If the object is a clone and this func is used to move clones forward in time. Assume the clone is made forward in time
		if (isClone &&
			mObjectLifeTimes.find(_id) != mObjectLifeTimes.end() &&
			((CloneLifeTime*)mObjectLifeTimes[_id])->mMade <= mCurrentGameTimeIndx) {
			destInfo->mBitset[0] = true;
		}

		//Set all componets to time recorded
		for (unsigned int i = 0; i < destInfo->mComponents.size(); i++) {
			SnapComponent* destComp = destInfo->mComponents[i];
			SetComponent(destComp, baseobject, destInfo);
		}

	}

	void Timeline::MoveAllObjectsToSnap(unsigned int _snaptime) {
		Snapshot* destination = mSnapshots[_snaptime];
		for (auto object : mLiveObjects) {
			unsigned short id = object.second->GetUniqueID();
			SnapInfo* destInfo;
			//If the object doesnt have a info, then check against the list for the last snap it was updated
			bool stored = destination->IsObjectStored(id);
			if (stored) {
				destInfo = destination->mSnapinfos[id];
			} else if (!stored) {
				if (destination->mUpdatedtimes.find(id) == destination->mUpdatedtimes.end())
					continue;
				destInfo = mSnapshots[destination->mUpdatedtimes[id]]->mSnapinfos[id];
			}

			//Set Object data
			BaseObject* baseobject = object.second;
			if (baseobject)
				baseobject->SetTransform(destInfo->mTransform);


			//Set all componets back to time recorded
			for (unsigned int i = 0; i < destInfo->mComponents.size(); i++) {
				SnapComponent* destComp = destInfo->mComponents[i];
				SetComponent(destComp, baseobject, destInfo);
			}
		}
	}
	void Timeline::MoveAllObjectsToSnapExceptPlayer(unsigned int _snaptime, unsigned short _id1, unsigned short _id2, unsigned short _id3) {
		Snapshot* destination = mSnapshots[_snaptime];
		for (auto object : mLiveObjects) {
			unsigned short id = object.second->GetUniqueID();
			if (id == _id1 || id == _id2 || id == _id3)
				continue;
			SnapInfo* destInfo;
			//If the object doesnt have a info, then check against the list for the last snap it was updated
			bool stored = destination->IsObjectStored(id);
			if (stored) {
				destInfo = destination->mSnapinfos[id];
			} else if (!stored) {
				if (destination->mUpdatedtimes.find(id) == destination->mUpdatedtimes.end())
					continue;
				destInfo = mSnapshots[destination->mUpdatedtimes[id]]->mSnapinfos[id];
			}
			//Set Object data
			BaseObject* baseobject = object.second;
			baseobject->SetTransform(destInfo->mTransform);

			//Set all componets back to time recorded
			for (unsigned int i = 0; i < destInfo->mComponents.size(); i++) {
				SnapComponent* destComp = destInfo->mComponents[i];
				SetComponent(destComp, baseobject, destInfo);
			}
		}
	}
	void Timeline::MoveAllComponentsToSnapExceptPlayer(unsigned int _snaptime, unsigned short _id1, unsigned short _id2, unsigned short _id3) {
		Snapshot* destination = mSnapshots[_snaptime];
		for (auto object : mLiveObjects) {
			unsigned short id = object.second->GetUniqueID();
			if (id == _id1 || id == _id2 || id == _id3)
				continue;
			SnapInfo* destInfo;
			//If the object doesnt have a info, then check against the list for the last snap it was updated
			bool stored = destination->IsObjectStored(id);
			if (stored) {
				destInfo = destination->mSnapinfos[id];
			} else if (!stored) {
				if (destination->mUpdatedtimes.find(id) == destination->mUpdatedtimes.end())
					continue;
				destInfo = mSnapshots[destination->mUpdatedtimes[id]]->mSnapinfos[id];
			}

			//Set all componets back to time recorded
			for (unsigned int i = 0; i < destInfo->mComponents.size(); i++) {
				SnapComponent* destComp = destInfo->mComponents[i];
				SetComponent(destComp, object.second, destInfo);
			}
		}
	}

	void Timeline::SetSavedSettings() {
		for (auto key : mSettings.mLevelInts) {
			Settings::GetInstance().SetInt(key.first, key.second);
		}
		for (auto key : mSettings.mLevelBools) {
			Settings::GetInstance().SetBool(key.first, key.second);
		}

	}

	void Timeline::ShowLiveObjectsColliders(unsigned int _frame,bool _show)
	{
		unsigned int level = Settings::GetInstance().GetInt("CurrentLevel");
		Level* clevel = LevelManager::GetInstance().GetCurrentLevel();
		for (auto obj : mLiveObjects)
		{
			MeshComponent* mesh = (MeshComponent*)obj.second->GetComponentIndexed(eCOMPONENT_MESH, 0);
			if(!_show)
			{
				if(mesh)
				{
				mesh->SetInMotion(false);
				continue;
				}
			}

			//bol for determining if we care about this object or not
			bool CheckObjectHighlight = false;

			Component* comp = obj.second->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);
			unsigned int id = obj.second->GetUniqueID();
			unsigned int HeadsetId = obj.second->GetUniqueID();
			unsigned int Controlelr1Id = clevel->GetLeftController()->GetUniqueID();
			unsigned int Controller2id = clevel->GetRightController()->GetUniqueID();
			if (comp && (id != HeadsetId || id !=Controlelr1Id || id!=Controller2id))
			{
				CheckObjectHighlight = true;
			}

			//Specific items we want highlighted in the level
			switch (level)
			{
			case 1:
				if (obj.second->GetName().find("Chamber") != std::string::npos)
				{
					CheckObjectHighlight = false;
				}
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				if (obj.second->GetName().find("Energy") != std::string::npos)
				{
					CheckObjectHighlight = false;
				}
				break;
			}
			
			if(CheckObjectHighlight)
			{
				//Find if object should be shown
				unsigned int i = _frame;
				if (!(i - 5 < 0))
					 i-= 5;
				bool highlight = false;
				for (i; i < _frame + 10; i++)
				{
					if(mSnapshots.find(i) != mSnapshots.end())
					{
						//If the data is not the same in the last 5 and next 5 frames we want to highlight it
						if(!CheckForDuplicateData(obj.second->GetUniqueID(),obj.second,i))
						{
							highlight = true; 
							break;
						}
					}
				}
				if(highlight)
				{
					if(mesh)
					mesh->SetInMotion(true);
				}
				else
				{
					if(mesh)
					mesh->SetInMotion(false);
				}
				
			}

		}
	}
	
	void Timeline::SetObjectBirthTime(unsigned short _id) {
		if (mObjectLifeTimes.find(_id) != mObjectLifeTimes.end()) {
			mObjectLifeTimes[_id]->mBirth = mCurrentGameTimeIndx;
		}
	}
	void Timeline::
		ClearTimeLine() {
		for (auto snapshot : mSnapshots) {
			for (auto snapInfo : snapshot.second->mSnapinfos) {
				if (snapInfo.second) {
					for (auto comp : snapInfo.second->mComponents)
						delete comp;
					/*auto comp = snapInfo.second->mComponents.begin();
					while (comp != snapInfo.second->mComponents.end())
					{
						delete *comp;
						comp = snapInfo.second->mComponents.erase(comp);
					}*/
				}

				if (snapInfo.second)
					snapInfo.second->mComponents.clear();

				if (snapInfo.second)
					delete snapInfo.second;

			}
			snapshot.second->mSnapinfos.clear();
			snapshot.second->mUpdatedtimes.clear();
			if (snapshot.second)
				delete snapshot.second;
		}

		for (auto Objectlife : mObjectLifeTimes) {
			if (Objectlife.second)
				delete Objectlife.second;
		}
		if(mCloneDone.size() > 0)
		mCloneDone.clear();
		mObjectLifeTimes.clear();
		mSnapshots.clear();
		mSnaptimes.clear();
		mLiveObjects.clear();
	}


	SnapInfo* Timeline::GenerateSnapInfo(BaseObject* _object, SnapInfo* _info) {
		if (_info == nullptr)
			_info = new SnapInfo();
		_info->mId = _object->GetUniqueID();
		_info->mTransform = _object->GetTransform();

		//Find if object doesn't have a life time
		if (mObjectLifeTimes.find(_info->mId) == mObjectLifeTimes.end()) {
			_info->mBitset[0] = false;
		}
		//If object life time was made
		else {
			//If it was alive during this snap
			if (mObjectLifeTimes[_info->mId]->mBirth > mCurrentGameTimeIndx || mObjectLifeTimes[_info->mId]->mDeath < mCurrentGameTimeIndx) {
				_info->mBitset[0] = false;
			}
			Level* currlevel = LevelManager::GetInstance().GetCurrentLevel();
			//If the object is the current headset and controller, then set as not alive so the ids are recorded as not alive until the player makes the it.
			//Otherwise the clones would be recorded as always alive with their components alive.
			if (_object->GetUniqueID() == currlevel->GetLeftController()->GetUniqueID() ||
				_object->GetUniqueID() == currlevel->GetRightController()->GetUniqueID() ||
				_object->GetUniqueID() == currlevel->GetHeadset()->GetUniqueID()) {
				_info->mBitset[0] = false;
			} else
				_info->mBitset[0] = true;
		}


		//TODO PAT: Do this better
		std::vector<Component*>temp;
		for (int i = 1; i < ComponentType::eCOMPONENT_MAX; i++) {
			{
				SnapComponent* newComp;
				temp = _object->GetComponents((ComponentType)i);
				for (unsigned int j = 0; j < temp.size(); j++) {

					if (i == ComponentType::eCOMPONENT_COLLIDER) {
						newComp = new SnapComponent_Physics();
						((SnapComponent_Physics*)newComp)->mForces = ((Collider*)temp[j])->mForces;
						((SnapComponent_Physics*)newComp)->mAcc = ((Collider*)temp[j])->mAcceleration;
						((SnapComponent_Physics*)newComp)->mVel = ((Collider*)temp[j])->mVelocity;
						((SnapComponent_Physics*)newComp)->mTotforce = ((Collider*)temp[j])->mTotalForce;
					} else if (i == ComponentType::eCOMPONENT_MESH) {
						newComp = new SnapComponent_Mesh;
						((SnapComponent_Mesh*)newComp)->misVisible = ((MeshComponent*)temp[j])->IsVisible();
					} else if (i == ComponentType::eCOMPONENT_CODE && dynamic_cast<CCMazeHelper*>(temp[j])) {
						newComp = new SnapComponent_Code_MazeHelp;
						for (int x = 0; x < 4; ++x) {
							for (int y = 0; y < 4; ++y) {
								((SnapComponent_Code_MazeHelp*)newComp)->mMazeGrid[x][y] = ((CCMazeHelper*)temp[j])->mGrid[x][y];
							}
						}
					} else {
						newComp = new SnapComponent;
					}
					newComp->mCompType = temp[j]->GetType();
					newComp->mBitNum = temp[j]->GetComponentNum();
					_info->mBitset[newComp->mBitNum] = temp[j]->IsEnabled();
					newComp->mId = temp[j]->GetColliderId();
					_info->mComponents.push_back(newComp);
				}
			}
		}

		return _info;
	}


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

		//TODO PAT: break up the big logic loop

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
							bool Complete = false;
							if(mCloneDone.find(id) != mCloneDone.end())
							{
								Complete = (mCurrentGameTimeIndx >= mCloneDone[id]);
							}
							if (snap->mSnapinfos.find(id) != snap->mSnapinfos.end() && id == _clones[i]->GetUniqueID() && !Complete) {
								MoveObjectToSnap(_time, id, true);
								//Update the clone interpolators to move if there is a next next snap available.
								UpdateCloneInterpolators(_clones[i]->GetUniqueID(), snap->mSnapinfos[id], _time);
								break;
							}
							//If we are a clone but dont have a next movement then record one at position
							else if (snap->mSnapinfos.find(id) == snap->mSnapinfos.end() && id == _clones[i]->GetUniqueID()) {
								//If change add to mSnapinfos and Updatetime
								if (!CheckForDuplicateData(id, _b.second,mCurrentGameTimeIndx)) {
									snap->mSnapinfos[id] = GenerateSnapInfo(_b.second, nullptr);
									snap->mUpdatedtimes[id] = _time;
									break;
								}
							}
							//If we made it through the list do the normal
							else if (id != _clones[i]->GetUniqueID() && i == _clones.size() - 1) {
								//If change add to mSnapinfos and Updatetime
								if (!CheckForDuplicateData(id, _b.second,mCurrentGameTimeIndx)) {
									snap->mSnapinfos[id] = GenerateSnapInfo(_b.second, snap->mSnapinfos[id]);
									snap->mUpdatedtimes[id] = _time;
								}
							}
						}
					} else {
						//If change add to mSnapinfos and Updatetime
						if (!CheckForDuplicateData(id, _b.second,mCurrentGameTimeIndx)) {
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
	bool Timeline::CheckForDuplicateData(unsigned short _id, BaseObject* _object, unsigned int _frame) {
		if (_frame == 0)
			return false;
		//If the object has not been made yet or is already dead return so we dont make one yet
		if (mObjectLifeTimes.find(_id) != mObjectLifeTimes.end() && (mObjectLifeTimes[_id]->mBirth > _frame || mObjectLifeTimes[_id]->mDeath < _frame))
			return true;

		SnapInfo* info;
		Snapshot* snap = mSnapshots[mSnaptimes[_frame]];
		//find if the object exist
		if (snap->mSnapinfos.find(_id) != snap->mSnapinfos.end())
			info = mSnapshots[mSnaptimes[_frame]]->mSnapinfos[_id];
		else if (snap->mUpdatedtimes.find(_id) != snap->mUpdatedtimes.end()) {
			info = mSnapshots[snap->mUpdatedtimes[_id]]->mSnapinfos[_id];
		} else {
			//if the object is not yet made(new headset or controller data) then we need to make it. This is the only exception I think
			//SystemLogger::GetLog() << _object->GetName() << std::endl;
			return false;
		}

		if (info->mTransform != _object->GetTransform())
			return false;

		for (unsigned int i = 0; i < info->mComponents.size(); i++) {
			SnapComponent* comp = info->mComponents[i];

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
				break;
			}
			case ComponentType::eCOMPONENT_MESH:
				for (unsigned int j = 0; j < _object->GetComponentCount(comp->mCompType); j++) {
					Component* currComp = _object->GetComponentIndexed(comp->mCompType, j);
					if (currComp->GetColliderId() == comp->mId) {
						if (((MeshComponent*)currComp)->IsVisible() != ((SnapComponent_Mesh*)comp)->misVisible)
							return false;
					}
				}
				break;
			case ComponentType::eCOMPONENT_CODE:
			{
				for (unsigned int j = 0; j < _object->GetComponentCount(comp->mCompType); j++) {
					Component* currComp = _object->GetComponentIndexed(comp->mCompType, j);
					if (dynamic_cast<CCMazeHelper*>(currComp) && currComp->GetColliderId() == comp->mId) {
						for (int x = 0; x < 4; ++x) {
							for (int y = 0; y < 4; ++y) {
								if (((SnapComponent_Code_MazeHelp*)comp)->mMazeGrid[x][y] != ((CCMazeHelper*)currComp)->mGrid[x][y]) {
									return false;
								}
							}
						}
					} else if (currComp->GetColliderId() == comp->mId) {
						if (info->mBitset[comp->mBitNum] != currComp->IsEnabled())
							return false;
					}
				}
				break;
			}
			default:
			{
				for (unsigned int j = 0; j < _object->GetComponentCount(comp->mCompType); j++) {
					Component* currComp = _object->GetComponentIndexed(comp->mCompType, j);
					if (currComp->GetColliderId() == comp->mId) {
						if (info->mBitset[comp->mBitNum] != currComp->IsEnabled())
							return false;
					}
				}
				break;
			}
			}
		}

		return true;
	}
}
