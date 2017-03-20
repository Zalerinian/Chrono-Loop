//#include "stdafx.h"
#include "TimeManager.h"
#include "Timeline.h"
#include "../Objects/BaseObject.h"
#include "../Common/Logger.h"
#include "../Rendering/Draw2D.h"
#include "../Input/CommandConsole.h"
#include "../Core/Pool.h"
#include "../Core/Level.h"
#include "../Input/VRInputManager.h"
#include "../Common/Breakpoint.h"
#include "LevelManager.h"
#include "../Common/Settings.h"

namespace Epoch {

	TimeManager* TimeManager::instanceTimemanager = nullptr;
	Timeline* TimeManager::mTimeline = nullptr;

	TimeManager::TimeManager() {
		mTimeline = new Timeline();
		mCloneTextureBitset.set(false);
		CommandConsole::Instance().AddCommand(L"/CLONECOUNT", ToggleCloneCountDisplay);
		CommandConsole::Instance().AddCommand(L"/SNAPCOUNT", ToggleSnapshotCountDisplay);

	}


	TimeManager::~TimeManager() {
		//Level manager will clear delete clones
		delete mTimeline;
		ClearClones();
	}

	void TimeManager::Update(float _delta) {
		if (LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator() != nullptr || LevelManager::GetInstance().GetCurrentLevel()->GetLeftTimeManipulator() != nullptr) {

			if (!LevelManager::GetInstance().GetCurrentLevel()->GetLeftTimeManipulator()->isTimePaused() && !LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator()->isTimePaused()) {

				mTimestamp += _delta;
				mDeltaTime = _delta;

				//If its time for a snapshot
				if (mTimestamp >= RecordingRate) {
					mTimestamp -= RecordingRate;
				#if _DEBUG
					mTimestamp = 0;
				#endif
					//Generate 
					Snapshot* s = mTimeline->GenerateSnapShot(mLevelTime, mClones);
					mTimeline->AddSnapshot(mLevelTime, s);
					mLevelTime = mTimeline->GetCurrentGameTimeIndx() + 1;
				}

				//SystemLogger::GetLog() << mTimestamp / mRecordingTime << std::endl; 
				for (auto Interp : mCloneInterpolators) {
					if (Interp.second)
						Interp.second->Update(_delta);
				}
				/*for (auto Interp : mCloneColliderInterpolators) {
					if (Interp.second)
					{
						Interp.second->Update(_delta);
						for (unsigned int i = 0; i < mClones.size(); i++)
						{
							if(mClones[i]->GetComponentCount(eCOMPONENT_COLLIDER) > 0 && mClones[i]->GetComponentIndexed(eCOMPONENT_COLLIDER,0)->GetColliderId() == Interp.first)
							{
								vec3f temp(Interp.second->GetEdit().Position[0], Interp.second->GetEdit().Position[1], Interp.second->GetEdit().Position[2]);
								((Collider*)mClones[i]->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->SetPos(temp);
							}
						}
					}
				}*/

					//Update inputTimeLine
					//This updates curr pointer of the input timeline along with the current time in the Timeline 
					if (VRInputManager::GetInstance().IsVREnabled()) {
						InputTimeline::InputNode* temp = VRInputManager::GetInstance().GetInputTimeline()->GetCurr();
						while (temp && temp->mNext && temp->mNext->mData.mLastFrame < mLevelTime) {
							if (temp->mData.mLastFrame < temp->mNext->mData.mLastFrame || (temp->mData.mLastFrame == temp->mNext->mData.mLastFrame && (temp->mNext->mData.mTime < (mTimestamp / RecordingRate)))) {
								for (unsigned int i = 0; i < mClones.size(); i++) {
									if (mClones[i]->GetUniqueId() == temp->mNext->mData.mControllerId) {
										if (DoesCloneExist(mClones[i]->GetUniqueId(), mLevelTime)) {
											//	SystemLogger::GetLog() << "Clone:" << "id " << temp->mData.mControllerId << " " << temp->mNext->mData.mButton << ':' << temp->mNext->mData.mButtonState << std::endl;
										} else {
											//	SystemLogger::GetLog() << "Found false" << std::endl;
										}
									}
								}
								VRInputManager::GetInstance().GetInputTimeline()->SetCurr(temp->mNext);
								temp = temp->mNext;
							} else {
								break;
							}

						}
					}
				}
		}
	}
		TimeManager * TimeManager::Instance() {
			if (!instanceTimemanager) {
				instanceTimemanager = new TimeManager();

				instanceTimemanager->AddAllTexturesToQueue();
			}
			return instanceTimemanager;
		}

		void TimeManager::AddObjectToTimeline(BaseObject * _obj) {
			if (_obj != nullptr)
			{
				mTimeline->AddBaseObject(_obj, _obj->GetUniqueID());
				//Level* templvl = LevelManager::GetInstance().GetCurrentLevel();
				instanceTimemanager->AddInterpolatorToObject(_obj);
				//if (LevelManager::GetInstance().GetCurrentLevel() == nullptr)
				//{
				//	if (_obj->GetName().find("Controller1 - 0") == std::string::npos &&
				//		_obj->GetName().find("Controller2 - 0") == std::string::npos) { //TODO RYAN: TEMPORARY FIX FOR INTERPOLATION
				//		instanceTimemanager->AddInterpolatorToObject(_obj);
				//	}
				//}7
				//else {
				//	if (_obj->GetName().find("Controller1 - " + std::to_string(LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator()->GetNumClones())) == std::string::npos &&
				//		_obj->GetName().find("Controller2 - " + std::to_string(LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator()->GetNumClones())) == std::string::npos) { //TODO RYAN: TEMPORARY FIX FOR INTERPOLATION
				//		instanceTimemanager->AddInterpolatorToObject(_obj);
				//	}
				//}
			}
		}

		void TimeManager::AddInterpolatorForClone(BaseObject * _obj) {
			Interpolator<matrix4>* temp = new Interpolator<matrix4>();
			mCloneInterpolators[_obj->GetUniqueID()] = temp;
			//Interpolator<matrix4>* temp2 = new Interpolator<matrix4>();
			//if(_obj->GetComponentCount(eCOMPONENT_COLLIDER) > 0)
			//mCloneColliderInterpolators[_obj->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)->GetColliderId()] = temp2;
		}
		void TimeManager::AddInterpolatorToObject(BaseObject* _obj) {
			Interpolator<matrix4>* temp = new Interpolator<matrix4>();
			mObjectRewindInterpolators[_obj->GetUniqueID()] = temp;
		}
		void TimeManager::AddAllTexturesToQueue()
	{
			for (unsigned int i = 0; i < mCloneTextureBitset.size(); i++) {
				mCloneTextureBitset[i] = false;
			}
	}

		void TimeManager::AssignTextureToClone(unsigned short _id)
	{
			for (unsigned int i = 0; i < mCloneTextureBitset.size(); i++) {
				if(mCloneTextureBitset[i] == false)
				{
					mCloneTextureBitset[i] = true;
					mCloneTextures[_id] = i;
					break;
				}
				if (mCloneTextureBitset[i] == false && mCloneTextureBitset.size() - 1)
				{
					AddAllTexturesToQueue();	
				}
			}

			SystemLogger::GetLog() << "Bitset:";
			for (unsigned int i = 0; i < mCloneTextureBitset.size(); i++)
			{
				if(mCloneTextureBitset[i] == true)
					SystemLogger::GetLog() << "1";
				else
					SystemLogger::GetLog() << "0";
			}
			SystemLogger::GetLog() << std::endl;
	}

		void TimeManager::UpdatePlayerObjectInTimeline(BaseObject *  _obj) {
			if (_obj != nullptr)
				mTimeline->UpdatePlayerBaseObject(_obj, _obj->GetUniqueID());
		}

		void TimeManager::ClearClones() {
			mClones.clear();
			AddAllTexturesToQueue();
			//Clean up the interpolators
			for (auto Interp : mCloneInterpolators) {
				if (Interp.second)
					delete Interp.second;
			}
			/*for (auto InterpCollider : mCloneColliderInterpolators) {
				if (InterpCollider.second)
					delete InterpCollider.second;
			}*/
			mCloneInterpolators.clear();
		}


		bool TimeManager::CheckRewindAvaliable(unsigned int _frame) {
			//wrapped
			if (mTimeline->GetCurrentGameTimeIndx() - (int)_frame > mTimeline->GetCurrentGameTimeIndx())
				return false;
			else
				return true;
		}

		void TimeManager::DeleteClone(unsigned short _id1)
	{
			Clonepair pair;
			pair.mCur = _id1;
			FindOtherClones(pair);
			bool del = false;
			for (int i = 0; i < mClones.size(); ) {
				del = false;
				if (mClones[i]->GetUniqueId() == _id1 || mClones[i]->GetUniqueId() == pair.mOther1 || mClones[i]->GetUniqueId() == pair.mOther2) {
					mClones[i]->RemoveAllComponents();

					for (int k = 0; k < Physics::Instance()->mObjects.size(); ++k) {
						if (Physics::Instance()->mObjects[k]->GetUniqueID() == mClones[i]->GetUniqueID()) {
							//I know I could have just iterated through it with an iterator but im lazy and tired
							Physics::Instance()->mObjects.erase(Physics::Instance()->mObjects.begin() + k);
							break;
						}
					}
					//This doesnt delete the input left of the clone. We may not want to do that to minimize delete calls
					std::unordered_map<unsigned short, unsigned int>::iterator textureIterator = mCloneTextures.find(mClones[i]->GetUniqueId());
					if(textureIterator != mCloneTextures.end())
					{
						mCloneTextureBitset[textureIterator->second] = false;
						mCloneTextures.erase(mClones[i]->GetUniqueId());
					}

					//Find the clone interpolator and delete it
					for (auto j = mCloneInterpolators.begin(); j != mCloneInterpolators.end(); ++j) {
						if(j->first == mClones[i]->GetUniqueID())
						{
							delete mCloneInterpolators[j->first];
							mCloneInterpolators.erase(mClones[i]->GetUniqueID());
							break;
						}
					}
					/*for (auto j = mCloneColliderInterpolators.begin(); j != mCloneColliderInterpolators.end(); ++j) {
						if (mClones[i]->GetComponentCount(eCOMPONENT_COLLIDER) > 0 && j->first == mClones[i]->GetComponentIndexed(eCOMPONENT_COLLIDER,0)->GetColliderId())
						{
							delete mCloneInterpolators[j->first];
							mCloneInterpolators.erase(mClones[i]->GetUniqueID());
							break;
						}
					}*/

					//Remove it from being tracked by timeline
					mTimeline->RemoveFromTimeline(mClones[i]->GetUniqueId());
					Pool::Instance()->iRemoveObject(mClones[i]->GetUniqueID());
					mClones.erase(mClones.begin() + i);
					del = true;
				}
				if (!del)
					i++;
			}
	}

		unsigned int TimeManager::GetCurrentSnapFrame() {
			return mTimeline->GetCurrentGameTimeIndx();
		}

		Interpolator<matrix4>* TimeManager::GetCloneInterpolator(unsigned short _id) {

			if (mCloneInterpolators.find(_id) != mCloneInterpolators.end())
				return mCloneInterpolators[_id];

			return nullptr;
		}
		Interpolator<matrix4>* TimeManager::GetObjectInterpolator(unsigned short _id) {
			if (mObjectRewindInterpolators.find(_id) != mObjectRewindInterpolators.end())
				return mObjectRewindInterpolators[_id];
			return nullptr;
		}
		//Interpolator<matrix4>* TimeManager::GetCloneColliderInterpolator(unsigned short _id) {

			//if (mCloneColliderInterpolators.find(_id) != mCloneColliderInterpolators.end())
				//return mCloneColliderInterpolators[_id];

			//return nullptr;
		//}

		std::string TimeManager::GetNextTexture() {
			for (unsigned int i = 0; i < mCloneTextureBitset.size(); i++) {
				if (mCloneTextureBitset[i] == false) {
					TimeManipulation* left = LevelManager::GetInstance().GetCurrentLevel()->GetLeftTimeManipulator();
					TimeManipulation* right = LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator();
					if (left) {
						SystemLogger::GetLog() << "Left Controller returned " << left->GetTexture(i) << std::endl;
						return left->GetTexture(i);
					} else if (right) {
						SystemLogger::GetLog() << "Right Controller returned " << right->GetTexture(i) << std::endl;
						return right->GetTexture(i);
					}
				}
				if (mCloneTextureBitset[i] == 1 && i == mCloneTextureBitset.size() - 1) {
					AddAllTexturesToQueue();
					 return GetNextTexture();
				}
			}
			//Default to plain if something goes wrong
			return "../Resources/CloneTexture.png";
		}

		unsigned int TimeManager::GetTotalSnapsmade() {
			return mTimeline->GetTotalSnaps();
		}

		/*Timeline * TimeManager::GetTimeLine() {
			if (!mTimeline) {
				mTimeline = new Timeline();
			};
			return mTimeline;
		}*/

		void TimeManager::RewindTimeline(unsigned int _frame, unsigned short _id1, unsigned short _id2, unsigned short _id3) {
			mTimeline->RewindNoClone(_frame, _id1, _id2, _id3);
			//Tell the time manager what frame the timeline its on
			mLevelTime = mTimeline->GetCurrentGameTimeIndx() + 1;
		}

		void TimeManager::RewindMakeClone(unsigned int _frame, BaseObject*& _ob1, BaseObject*& _ob2, BaseObject*& _ob3) {
			if (_ob1 == nullptr || _ob2 == nullptr || _ob3 == nullptr)
				SystemLogger::GetLog() << "When you tried to rewind time, you gave the timemanager bad BaseObject pointer(s)";
			mTimeline->RewindMakeClone(_frame);
			mClones.push_back(_ob1);
			mClones.push_back(_ob2);
			mClones.push_back(_ob3);
			mTimeline->SetCloneObjectCreationTime(_ob1->GetUniqueID(), _ob2->GetUniqueID(), _ob3->GetUniqueID());
			//Tell the time manager what frame the timeline its on
			mLevelTime = mTimeline->GetCurrentGameTimeIndx() + 1;
		}

		void TimeManager::UpdateCloneCreationTime(unsigned short _id1, unsigned short _id2, unsigned short _id3)
		{
			unsigned short ids[3] = { _id1,_id2,_id3 };
			mTimeline->ActivateCloneBitset(ids);
		}

		void TimeManager::Destroy() {
			delete instanceTimemanager;
		}

		bool TimeManager::DoesCloneExist(unsigned short _id, unsigned int _frame) {
			ObjectLifeTime* lifetemp = mTimeline->GetObjectLifetime(_id);
			if (lifetemp && (unsigned int)lifetemp->mBirth < _frame && (unsigned int)lifetemp->mDeath > _frame) {
				return true;
			}
			return false;

		}

		void TimeManager::FindOtherClones(Clonepair & _pair)
		{
			CloneLifeTime* curr = (CloneLifeTime*)mTimeline->GetObjectLifetime(_pair.mCur);
			if (!curr)
				return;
			bool pair1 = false;

			for (unsigned int i = 0; i < mClones.size(); i++) {
				if (mClones[i]->GetUniqueId() == _pair.mCur)
					continue;
				CloneLifeTime* temp = (CloneLifeTime*)mTimeline->GetObjectLifetime(mClones[i]->GetUniqueId());
				if(temp &&temp->mMade == curr->mMade && (mClones[i]->GetName().find("Headset") != std::string::npos || mClones[i]->GetName().find("Controller") != std::string::npos))
				{
				
					if(!pair1)
					{
						_pair.mOther1 = mClones[i]->GetUniqueId();
						pair1 = true;
					}
					else
					{
						//found the last pair
						_pair.mOther2 = mClones[i]->GetUniqueId();
						return;
					}

				}
			}
		}

		void TimeManager::ToggleCloneCountDisplay(void * _command, std::wstring _ifOn) {
			CommandConsole* cc = (CommandConsole*)_command;
			if (_ifOn == L"ON") {
				Settings::GetInstance().SetBool("CloneCounter", true);
				CommandConsole::Instance().DisplaySet(L"");
			} else if (_ifOn == L"OFF") {
				Settings::GetInstance().SetBool("CloneCounter", false);
				CommandConsole::Instance().DisplaySet(L"");

			} else {
				CommandConsole::Instance().DisplaySet(L"INVALID INPUT: " + _ifOn + L"\nCORRECT INPUT: /CLONECOUNT (ON/OFF)");
			}
		}
		void TimeManager::ToggleSnapshotCountDisplay(void * _command, std::wstring _ifOn) {
			CommandConsole* cc = (CommandConsole*)_command;
			if (_ifOn == L"ON") {
				Settings::GetInstance().SetBool("SnapCounter", true);
				CommandConsole::Instance().DisplaySet(L"");
			} else if (_ifOn == L"OFF") {
				Settings::GetInstance().SetBool("SnapCounter", false);
				CommandConsole::Instance().DisplaySet(L"");

			} else {
				CommandConsole::Instance().DisplaySet(L"INVALID INPUT: " + _ifOn + L"\nCORRECT INPUT: /SNAPCOUNT (ON/OFF)");
			}
		}
		void TimeManager::DisplayCloneCount() {
			if (Settings::GetInstance().GetBool("CloneCounter")) {
				std::wstring CloneCount = L"Clone(s): " + std::to_wstring(mClones.size());

				Font* tempFont;
				if (!CommandConsole::Instance().isVRon()) {
					tempFont = new Font(L"Times New Roman", 25, (D2D1::ColorF(D2D1::ColorF::Purple, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
				} else {
					tempFont = new Font(L"Calibri", 40, (D2D1::ColorF(D2D1::ColorF::Purple, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
				}
				Draw::Instance().DrawTextToBitmap(
					(*Draw::Instance().GetContext2D())->GetSize().width*(25.0f / 32.0f),
					(*Draw::Instance().GetContext2D())->GetSize().height*(29.0f / 32.0f),
					(*Draw::Instance().GetContext2D())->GetSize().width,
					(*Draw::Instance().GetContext2D())->GetSize().height*(30.5f / 32.0f), *tempFont,
					CloneCount, *(Draw::Instance().GetScreenBitmap()).get());
			}
		}
		void TimeManager::DisplaySnapshotCount() {
			if (Settings::GetInstance().GetBool("SnapCounter")) {
				std::wstring CloneCount = L"Snapshots: " + std::to_wstring(mTimeline->GetCurrentGameTimeIndx());

				Font* tempFont;
				if (!CommandConsole::Instance().isVRon()) {
					tempFont = new Font(L"Times New Roman", 25, (D2D1::ColorF(D2D1::ColorF::Blue, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
				} else {
					tempFont = new Font(L"Calibri", 40, (D2D1::ColorF(D2D1::ColorF::Blue, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
				}
				Draw::Instance().DrawTextToBitmap(
					(*Draw::Instance().GetContext2D())->GetSize().width*(25.0f / 32.0f),
					(*Draw::Instance().GetContext2D())->GetSize().height*(26.0f / 32.0f),
					(*Draw::Instance().GetContext2D())->GetSize().width,
					(*Draw::Instance().GetContext2D())->GetSize().height*(29.0f / 32.0f), *tempFont,
					CloneCount, *(Draw::Instance().GetScreenBitmap()).get());


			}
		}
		void TimeManager::BrowseTimeline(int _gesture, int _frameRewind) {
			
			if (mShouldUpdateInterpolators) {
				for (auto it : mObjectRewindInterpolators)
				{
					bool complete = it.second->Update(GetDeltaTime());
					if (complete)
						mShouldUpdateInterpolators = false;
				}
				return;
			}

			unsigned int temp = instanceTimemanager->GetCurrentSnapFrame();
			if (_gesture == 0)
				return;
			else if (_gesture == 1)
				_frameRewind *= -1;
			else if (_gesture == 2) {
				LevelManager::GetInstance().GetCurrentLevel()->GetRightTimeManipulator()->RaycastCloneCheck();
				LevelManager::GetInstance().GetCurrentLevel()->GetLeftTimeManipulator()->RaycastCloneCheck();
				return;
			}

			if ((mtempCurSnapFrame != 0 && _gesture == -1) || (mtempCurSnapFrame != temp && _gesture == 1)) {
				int placeHolder = mtempCurSnapFrame;
				mtempCurSnapFrame -= _frameRewind;
				mTimeline->PrepareAllObjectInterpolators(placeHolder, mtempCurSnapFrame);
				mShouldUpdateInterpolators = true;
				mShouldPulse = true;
			}
			else {
				mShouldPulse = false;

			}

		

		}
		void TimeManager::MoveAllObjectExceptPlayer(unsigned int _snaptime, unsigned short _headset, unsigned short _rightC, unsigned short _leftC) {
			mTimeline->MoveAllObjectsToSnapExceptPlayer(_snaptime, _headset, _leftC, _rightC);
		}

		void TimeManager::HotfixResetTimeline() {
			RewindTimeline(0, LevelManager::GetInstance().GetCurrentLevel()->GetLeftController()->GetUniqueID(), LevelManager::GetInstance().GetCurrentLevel()->GetRightController()->GetUniqueID(), LevelManager::GetInstance().GetCurrentLevel()->GetHeadset()->GetUniqueID());
			mTimeline->SetObjectBirthTime(LevelManager::GetInstance().GetCurrentLevel()->GetLeftController()->GetUniqueID());
			mTimeline->SetObjectBirthTime(LevelManager::GetInstance().GetCurrentLevel()->GetRightController()->GetUniqueID());
			mTimeline->SetObjectBirthTime(LevelManager::GetInstance().GetCurrentLevel()->GetHeadset()->GetUniqueID());
			mTimeline->HotFixResetLevel();
			for (int i = 0; i < mClones.size(); ++i) {
				mClones[i]->RemoveAllComponents();

				for (int k = 0; k < Physics::Instance()->mObjects.size(); ++k) {
					if (Physics::Instance()->mObjects[k]->GetUniqueID() == mClones[i]->GetUniqueID()) {
						Physics::Instance()->mObjects.erase(Physics::Instance()->mObjects.begin() + k);
					}
				}

				//Remove it from being tracked by timeline
				mTimeline->RemoveFromTimeline(mClones[i]->GetUniqueId());

				Pool::Instance()->iRemoveObject(mClones[i]->GetUniqueID());
				mCloneTextures.erase(mClones[i]->GetUniqueId());
			}
			ClearClones();
			AddAllTexturesToQueue();
			VRInputManager::GetInstance().GetInputTimeline()->Clear();
			vec4f start = LevelManager::GetInstance().GetCurrentLevel()->GetStartPos();
			VRInputManager::GetInstance().GetPlayerPosition()[3].Set(start.x, start.y, start.z, start.w);
		}
	}
