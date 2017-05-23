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
#include "../Actions/CCMazeHelper.h"
#include "../Common/Breakpoint.h"
#include "LevelManager.h"
#include "../Common/Settings.h"
#include "../Particles/ParticleSystem.h"
#include "../Sound/SoundEngine.h"
#include "../Actions/CCCloneIndicator.h"

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
		if (VRInputManager::GetInstance().GetInputTimeline())
			VRInputManager::GetInstance().GetInputTimeline()->Clear();
		ClearClones();
		mTimeline = nullptr;
		instanceTimemanager = nullptr;
		for (auto it = mObjectRewindInterpolators.begin(); it != mObjectRewindInterpolators.end(); ++it) {
			delete it->second;
		}

		mObjectRewindInterpolators.clear();
	}
	//made this function so the timeline could stay as a black box
	void TimeManager::SetCreationTimeofClone(unsigned short _id1, unsigned short _id2, unsigned short _id3) {
		mTimeline->SetCloneObjectCreationTime(_id1, _id2, _id3);
	}

	void TimeManager::Update(float _delta) {

		mDeltaTime = _delta;
		mTotalGameTime += _delta;
		if (LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator() != nullptr) {

			if (!LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused()) {
				mTimestamp += _delta;
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
				mSnapTweenTime = mTimestamp / RecordingRate;
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
								if (mClones[i]->GetUniqueID() == temp->mNext->mData.mControllerId) {
									if (DoesCloneExist(mClones[i]->GetUniqueID(), mLevelTime)) {
										SystemLogger::GetLog() << "Clone:" << "id " << temp->mData.mControllerId << " " << temp->mNext->mData.mButton << ':' << temp->mNext->mData.mButtonState << std::endl;
									} else {
										SystemLogger::GetLog() << "Found false" << std::endl;
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
	float TimeManager::GetTimeLineObjectInterpTime() {
		return mTimeline->GetObjectInterpolationTime();
	}
	void TimeManager::SetClonesFinalFrame(unsigned short _id, unsigned int _frame)
	{
		mTimeline->SetCloneFrameCompletion(_id, _frame);
	}
	void TimeManager::SetTimelineObjectInterpTime(float _time) {
		mTimeline->SetObjectInterpolationTime(_time);
	}

	void TimeManager::SetupClonePairs(unsigned short _id1, unsigned short _id2, unsigned short _id3) {
		//annoying but helps for finding clones
		Clonepair* p1 = new Clonepair();
		Clonepair* p2 = new Clonepair();
		Clonepair* p3 = new Clonepair();
		p1->mCur = _id1;
		p1->mOther1 = _id2;
		p1->mOther2 = _id3;
		SetClonePair(_id1, p1);

		p2->mCur = _id2;
		p2->mOther1 = _id1;
		p2->mOther2 = _id3;
		SetClonePair(_id2, p2);

		p3->mCur = _id3;
		p3->mOther1 = _id1;
		p3->mOther2 = _id2;
		SetClonePair(_id3, p3);
	}

	void TimeManager::ShowTimelineColliders(bool _show)
	{
		mTimeline->ShowLiveObjectsColliders(GetCurrentSnapFrame(),_show);
	}


	TimeManager * TimeManager::Instance() {
		if (!instanceTimemanager) {
			instanceTimemanager = new TimeManager();


			instanceTimemanager->ActivateAllTexturesToBitset();
		}
		return instanceTimemanager;
	}

	void TimeManager::AddObjectToTimeline(BaseObject * _obj) {
		if (_obj != nullptr) {
			if (!mTimeline)
				mTimeline = new Timeline();
			mTimeline->AddBaseObject(_obj, _obj->GetUniqueID());
			//Level* templvl = LevelManager::GetInstance().GetCurrentLevel();
			instanceTimemanager->AddInterpolatorToObject(_obj);
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
	void TimeManager::ActivateAllTexturesToBitset() {
		for (unsigned int i = 0; i < mCloneTextureBitset.size(); i++) {
			mCloneTextureBitset[i] = false;
		}
	}

	void TimeManager::AssignTextureToClone(unsigned short _id) {
		for (unsigned int i = 0; i < mCloneTextureBitset.size(); i++) {
			if (mCloneTextureBitset[i] == false) {
				mCloneTextureBitset[i] = true;
				mCloneTextures[_id] = i;
				break;
			}
			if (mCloneTextureBitset[i] == false && mCloneTextureBitset.size() - 1) {
				ActivateAllTexturesToBitset();
			}
		}
	}

	void TimeManager::UpdatePlayerObjectInTimeline(BaseObject *  _obj) {
		if (_obj != nullptr)
			mTimeline->UpdatePlayerBaseObject(_obj, _obj->GetUniqueID());
	}

	void TimeManager::ClearClones() {
		mClones.clear();
		ActivateAllTexturesToBitset();
		//Clean up the interpolators
		for (auto Interp : mCloneInterpolators) {
			if (Interp.second)
				delete Interp.second;
		}
		for (auto pair : mClonePairs) {
			if (pair.second)
				delete pair.second;
		}
		mClonePairs.clear();
		mCloneInterpolators.clear();
	}


	bool TimeManager::CheckRewindAvaliable(unsigned int _frame) {
		//wrapped
		if (mTimeline->GetCurrentGameTimeIndx() - (int)_frame > mTimeline->GetCurrentGameTimeIndx())
			return false;
		else
			return true;
	}

	void TimeManager::DeleteClone(unsigned short _id1, bool _useParticleEffect) {
		//USe a copy instead of a pointer so you will still have it after the pair gets deleted
		Clonepair pair = *GetClonePair(_id1);
		bool del = false;
		Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();
		for (int i = 0; i < mClones.size(); ) {
			del = false;
			if (mClones[i]->GetUniqueID() == _id1 || mClones[i]->GetUniqueID() == pair.mOther1 || mClones[i]->GetUniqueID() == pair.mOther2) {

				if (mClones[i]->GetUniqueID() == pair.mCur && _useParticleEffect) {
					Particle * p = &Particle::Init();
					p->SetColors(vec4f(1, 1, 1, 1), vec4f());
					p->SetLife(300);
					p->SetSize(.25f, .15f);
					vec3f EPos = vec3f(mClones[i]->GetTransform().GetPosition()->x, mClones[i]->GetTransform().GetPosition()->y, mClones[i]->GetTransform().GetPosition()->z);
					ParticleEmitter *emit = new ParticleEmitter(200, 200, 20, EPos);
					emit->SetParticle(p);
					emit->SetTexture("../Resources/BasicCircleP.png");
					ParticleSystem::Instance()->AddEmitter(emit);

					vec4f temp = EPos;
					AudioWrapper::GetInstance().MakeEventAtLocation(AK::EVENTS::SFX_BLOP, &temp);
					emit->FIRE();
				}

				mClones[i]->RemoveAllComponents();

				for (int k = 0; k < Physics::Instance()->mObjects.size(); ++k) {
					if (Physics::Instance()->mObjects[k]->GetUniqueID() == mClones[i]->GetUniqueID()) {
						//I know I could have just iterated through it with an iterator but im lazy and tired
						Physics::Instance()->mObjects.erase(Physics::Instance()->mObjects.begin() + k);
						break;
					}
				}
				//This doesnt delete the input left of the clone. We may not want to do that to minimize delete calls
				std::unordered_map<unsigned short, unsigned int>::iterator textureIterator = mCloneTextures.find(mClones[i]->GetUniqueID());
				if (textureIterator != mCloneTextures.end()) {
					mCloneTextureBitset[textureIterator->second] = false;
					mCloneTextures.erase(mClones[i]->GetUniqueID());
				}

				//Find the clone interpolator and delete it
				for (auto j = mCloneInterpolators.begin(); j != mCloneInterpolators.end(); ++j) {
					if (j->first == mClones[i]->GetUniqueID()) {
						delete mCloneInterpolators[j->first];
						mCloneInterpolators.erase(mClones[i]->GetUniqueID());
						break;
					}
				}
				bool mbreak = false;
				std::list<BaseObject*>&children = cLevel->GetTimeManipulator()->GetBaseObject()->GetChildren();
				for(auto c = children.begin(); c != children.end(); ++c)
				{
					std::vector<Component*>& comps = (*c)->GetComponents(eCOMPONENT_CODE);
					for (unsigned int p = 0; p < comps.size(); p++)
					{
						if(dynamic_cast<CCCloneIndicator*>(comps[p]))
						{
							unsigned int cId = ((CCCloneIndicator*)comps[p])->mId;
							if(cId == mClones[i]->GetUniqueID())
							{
								BaseObject* del = *c;
								children.remove(*c);
								Pool::Instance()->iAddObject(del);
								mbreak = true;
								break;
							}
						}
					}
					if(mbreak)
					{
						cLevel->GetTimeManipulator()->GetBaseObject()->SetChildren(children);
						break;
					}
				}

				//Find the clone's pair and delete it
				for (auto j = mClonePairs.begin(); j != mClonePairs.end(); ++j) {
					if (j->first == mClones[i]->GetUniqueID()) {
						delete mClonePairs[j->first];
						mClonePairs.erase(mClones[i]->GetUniqueID());
						break;
					}
				}
				//TODO Pat: COmment this back in sometime
				/*for (auto j = mCloneColliderInterpolators.begin(); j != mCloneColliderInterpolators.end(); ++j) {
					if (mClones[i]->GetComponentCount(eCOMPONENT_COLLIDER) > 0 && j->first == mClones[i]->GetComponentIndexed(eCOMPONENT_COLLIDER,0)->GetColliderId())
					{
						delete mCloneColliderInterpolators[j->first];
						mCloneColliderInterpolators.erase(mClones[i]->GetUniqueID());
						break;
					}
				}*/

				SystemLogger::GetLog() << "Clone id:" << mClones[i]->GetUniqueID() << " has been deleted" << std::endl;
				//Remove it from being tracked by timeline
				mTimeline->RemoveFromTimeline(mClones[i]->GetUniqueID());
				LevelManager::GetInstance().GetCurrentLevel()->RemoveObject(mClones[i]);
				Pool::Instance()->iAddObject(mClones[i]);
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
	Clonepair * TimeManager::GetClonePair(unsigned short _id) {
		if (mClonePairs.find(_id) != mClonePairs.end())
			return mClonePairs[_id];
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
				TimeManipulation* timemanip = LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator();
				if (timemanip) {
					//SystemLogger::GetLog() << "Left Controller returned " << left->GetTexture(i) << std::endl;
					return timemanip->GetTexture(i);
				}
			}
			if (mCloneTextureBitset[i] == 1 && i == mCloneTextureBitset.size() - 1) {
				ActivateAllTexturesToBitset();
				return GetNextTexture();
			}
		}
		//Default to plain if something goes wrong
		return "../Resources/CloneTexture.png";
	}

	unsigned int TimeManager::GetTotalSnapsmade() {
		return mTimeline->GetTotalSnaps();
	}

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

	void TimeManager::UpdateCloneMadeTime(unsigned short _id1, unsigned short _id2, unsigned short _id3) {
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
	void TimeManager::SaveSettingIntToTimeline(std::string _str, int _val) {
		mTimeline->mSettings.mLevelInts[_str] = _val;
	}
	void TimeManager::SaveSettingBoolToTimeline(std::string _str, bool _val) {
		mTimeline->mSettings.mLevelBools[_str] = _val;
	}

	//I Take no credit in this func for it is Ryan Bronk's from Nth Dimensional
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
	//I Take no credit in this func for it is Ryan Bronk's from Nth Dimensional
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
	//I Take no credit in this func for it is Ryan Bronk's from Nth Dimensional
	void TimeManager::DisplayCloneCount() {
		if (Settings::GetInstance().GetBool("CloneCounter")) {
			std::wstring CloneCount = L"Clone(s): " + std::to_wstring(mClones.size());

			Font tempFont;
			if (!CommandConsole::Instance().isVRon()) {
				tempFont = Font(L"Times New Roman", 25, (D2D1::ColorF(D2D1::ColorF::Purple, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
			} else {
				tempFont = Font(L"Calibri", 40, (D2D1::ColorF(D2D1::ColorF::Purple, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
			}
			Draw::Instance().DrawTextToBitmap(
				(*Draw::Instance().GetContext2D())->GetSize().width*(25.0f / 32.0f),
				(*Draw::Instance().GetContext2D())->GetSize().height*(29.0f / 32.0f),
				(*Draw::Instance().GetContext2D())->GetSize().width,
				(*Draw::Instance().GetContext2D())->GetSize().height*(30.5f / 32.0f), tempFont,
				CloneCount, *(Draw::Instance().GetScreenBitmap()).get());
		}
	}
	//I Take no credit in this func for it is Ryan Bronk's from Nth Dimensional
	void TimeManager::DisplaySnapshotCount() {
		if (Settings::GetInstance().GetBool("SnapCounter")) {
			std::wstring CloneCount = L"Snapshots: " + std::to_wstring(mTimeline->GetCurrentGameTimeIndx());

			Font tempFont;
			if (!CommandConsole::Instance().isVRon()) {
				tempFont = Font(L"Times New Roman", 25, (D2D1::ColorF(D2D1::ColorF::Blue, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
			} else {
				tempFont = Font(L"Calibri", 40, (D2D1::ColorF(D2D1::ColorF::Blue, 1.0f)), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
			}
			Draw::Instance().DrawTextToBitmap(
				(*Draw::Instance().GetContext2D())->GetSize().width*(25.0f / 32.0f),
				(*Draw::Instance().GetContext2D())->GetSize().height*(26.0f / 32.0f),
				(*Draw::Instance().GetContext2D())->GetSize().width,
				(*Draw::Instance().GetContext2D())->GetSize().height*(29.0f / 32.0f), tempFont,
				CloneCount, *(Draw::Instance().GetScreenBitmap()).get());


		}

		//I Take no credit in this func for it is Ryan Bronk's from Nth Dimensional
	}
	void TimeManager::BrowseTimeline(int _gesture, int _frameRewind) {
		bool UpdateComponents = false;

		if (mShouldUpdateInterpolators) {
			for (auto it : mObjectRewindInterpolators) {
				bool complete = it.second->Update(GetDeltaTime());
				if (complete) {
					mShouldUpdateInterpolators = false;
					UpdateComponents = true;
				}
			}
			if (UpdateComponents == true) {
				TimeManipulation* temp = LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator();
				Level* tempLevel = LevelManager::GetInstance().GetCurrentLevel();
				if (temp->mCurCloneHeadset && temp->mCurCloneController1 && temp->mCurCloneController2) {
					mTimeline->MoveAllComponentsToSnapExceptPlayer(mtempCurSnapFrame, temp->mCurCloneHeadset->GetUniqueID(), temp->mCurCloneController1->GetUniqueID(), temp->mCurCloneController2->GetUniqueID());
				} else if (tempLevel->GetHeadset() && tempLevel->GetLeftController() && tempLevel->GetRightController()) {
					mTimeline->MoveAllComponentsToSnapExceptPlayer(mtempCurSnapFrame, tempLevel->GetHeadset()->GetUniqueID(), tempLevel->GetRightController()->GetUniqueID(), tempLevel->GetLeftController()->GetUniqueID());
				}
			}
			return;
		}
		//SystemLogger::GetLog() << "Gesture ID: " << _gesture << std::endl;
		if (_gesture == 0) {
			if(noMovementCnt < 15)
				noMovementCnt++;
			if(noMovementCnt == 15)
			{
				mRewindShouldGetFaster = 0;
				mRewindGettingFaster = 1;
			}
			return;
		}
		if (_gesture == 1)
			_frameRewind *= -1;
		else if (_gesture == 2) {
			/*LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->RaycastCloneCheck();
			LevelManager::GetInstance().GetCurrentLevel()->GetLeftTimeManipulator()->RaycastCloneCheck();*/
			return;
		}

		unsigned int temp = instanceTimemanager->GetCurrentSnapFrame();
			if ((mtempCurSnapFrame != 0 && _gesture == -1) || (mtempCurSnapFrame != temp && _gesture == 1)) {
				int placeHolder = mtempCurSnapFrame;
				if (mtempCurSnapFrame - (_frameRewind * mRewindGettingFaster) > 0 && (mtempCurSnapFrame - (_frameRewind * mRewindGettingFaster) < temp))
					mtempCurSnapFrame -= (_frameRewind) * mRewindGettingFaster;
				mTimeline->PrepareAllObjectInterpolators(placeHolder, mtempCurSnapFrame);
				mShouldUpdateInterpolators = true;
				mShouldPulse = true;
				noMovementCnt = 0;
				VRInputManager::GetInstance().GetController(eControllerType_Primary).TriggerHapticPulse(600, vr::k_EButton_SteamVR_Touchpad);
 				mRewindShouldGetFaster++;
				if (mRewindShouldGetFaster % 20 == 0)
					mRewindGettingFaster++;
				//SystemLogger::GetLog() << "mRewindGettingFaster: " << mRewindGettingFaster << std::endl;
				//SystemLogger::GetLog() << "mRewindShouldGetFaster: " << mRewindShouldGetFaster << std::endl;
				if (_gesture == 1)
					Settings::GetInstance().SetUInt("TutorialRewind - CurProgress", Settings::GetInstance().GetUInt("TutorialRewind - CurProgress") - mRewindGettingFaster);
				if (_gesture == -1)
					Settings::GetInstance().SetUInt("TutorialRewind - CurProgress", Settings::GetInstance().GetUInt("TutorialRewind - CurProgress") + mRewindGettingFaster);
				if(Settings::GetInstance().GetUInt("TutorialRewind - CurProgress") >= Settings::GetInstance().GetUInt("TutorialRewind - FinalProgress"))//Rewind
				{
					if (Settings::GetInstance().GetInt("tutStep") == 3)
					{
						if (Settings::GetInstance().GetBool("Level1Tutorial"))
							Settings::GetInstance().SetInt("tutStep", 4);//Accept time (tut 1)
						else
							Settings::GetInstance().SetInt("tutStep", 5);//Create Clone (tut 2)
					}
				}
			} else {
				mShouldPulse = false;
			}

			mTimeline->ShowLiveObjectsColliders(mtempCurSnapFrame,true);
	}
	void TimeManager::MoveAllObjectExceptPlayer(unsigned int _snaptime, unsigned short _headset, unsigned short _rightC, unsigned short _leftC) {
		mTimeline->MoveAllObjectsToSnapExceptPlayer(_snaptime, _headset, _leftC, _rightC);
	}

	void TimeManager::ResetTimeLineandLevel() {
		Level* cLevel = LevelManager::GetInstance().GetCurrentLevel();
		RewindTimeline(0, LevelManager::GetInstance().GetCurrentLevel()->GetLeftController()->GetUniqueID(), LevelManager::GetInstance().GetCurrentLevel()->GetRightController()->GetUniqueID(), LevelManager::GetInstance().GetCurrentLevel()->GetHeadset()->GetUniqueID());
		mTimeline->SetObjectBirthTime(cLevel->GetLeftController()->GetUniqueID());
		mTimeline->SetObjectBirthTime(cLevel->GetRightController()->GetUniqueID());
		mTimeline->SetObjectBirthTime(cLevel->GetHeadset()->GetUniqueID());
		mTimeline->ResetTimelineAndLevel();
		for (int i = 0; i < mClones.size(); ++i) {
			mClones[i]->RemoveAllComponents();

			if (mObjectRewindInterpolators.find(mClones[i]->GetUniqueID()) != mObjectRewindInterpolators.end()) {
				Interpolator<matrix4>* clone = mObjectRewindInterpolators[mClones[i]->GetUniqueID()];
				mObjectRewindInterpolators.erase(mClones[i]->GetUniqueID());
				delete clone;
			}

			for (int k = 0; k < Physics::Instance()->mObjects.size(); ++k) {
				if (Physics::Instance()->mObjects[k]->GetUniqueID() == mClones[i]->GetUniqueID()) {
					Physics::Instance()->mObjects.erase(Physics::Instance()->mObjects.begin() + k);
				}
			}
			bool mbreak = false;
			std::list<BaseObject*>&children = cLevel->GetTimeManipulator()->GetBaseObject()->GetChildren();
			for (auto c = children.begin(); c != children.end(); ++c)
			{
				std::vector<Component*>& comps = (*c)->GetComponents(eCOMPONENT_CODE);
				for (unsigned int p = 0; p < comps.size(); p++)
				{
					if (dynamic_cast<CCCloneIndicator*>(comps[p]))
					{
						unsigned int cId = ((CCCloneIndicator*)comps[p])->mId;
						if (cId == mClones[i]->GetUniqueID())
						{
							BaseObject* del = *c;
							children.remove(*c);
							Pool::Instance()->iAddObject(del);
							mbreak = true;
							break;
						}
					}
				}
				if (mbreak)
				{
					cLevel->GetTimeManipulator()->GetBaseObject()->SetChildren(children);
					break;
				}
			}

			//Remove it from being tracked by timeline
			mTimeline->RemoveFromTimeline(mClones[i]->GetUniqueID());

			Pool::Instance()->iRemoveObject(mClones[i]->GetUniqueID());
			mCloneTextures.erase(mClones[i]->GetUniqueID());
		}
		ClearClones();
		ActivateAllTexturesToBitset();
		mTimeline->SetSavedSettings();
		if (VRInputManager::GetInstance().IsVREnabled()) {
			VRInputManager::GetInstance().GetInputTimeline()->Clear();
			vec4f start = cLevel->GetStartPos();
			VRInputManager::GetInstance().GetPlayerPosition()[3].Set(start.x, start.y, start.z, start.w);
		}
		AudioEmitter* ambient = (AudioEmitter*)cLevel->GetHeadset()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0);
		AudioEmitter* resetLevelSFX = (AudioEmitter*)cLevel->GetHeadset()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 4);
		if (ambient)
		{
			ambient->CallEvent(Emitter::EventType::eStop);
			ambient->CallEvent(Emitter::EventType::ePlay);
		}
		if(resetLevelSFX)
		{
			resetLevelSFX->CallEvent(Emitter::EventType::ePlay);
		}
		if(Settings::GetInstance().GetInt("CurrentLevel") == 3)
		{
			Settings::GetInstance().SetBool("ResetElevator", true);
			std::vector<Component*>& comps = LevelManager::GetInstance().GetCurrentLevel()->GetHeadset()->GetComponents(eCOMPONENT_CODE);
			for (unsigned int i = 0; i < comps.size(); i++) {
				if (dynamic_cast<CCMazeHelper*>(comps[i])) {
					((CCMazeHelper*)comps[i])->ResetBoxes();
					break;
				}
			}
		}
			
	}
}
