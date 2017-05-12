#pragma once
#include "CodeComponent.hpp"
#include "../Common/Settings.h"
#include "CCMazeHelper.h"

namespace Epoch
{
	struct CCLevel3BoxSmash : public CodeComponent
	{
		CCMazeHelper* maze; //use this to figure out if box is interpolating
		Collider* mCol;
		Level* mLevel;
		unsigned int mBoxNum = 0;
		float mTimer = 0;
		bool mBoxDone = false, mRestartLevel = false;
		Renderer::TimeManipulationEffectData mEffectData;
		Interpolator<vec2f> mDesaturationInterpolator;
		virtual void Start()
		{
			mEffectData.saturationColor.Set(0.9f, 0.4f, 0.11f, 0);
			mEffectData.tintColor.Set(1, 0.85f, 1, 1);
			mEffectData.ratios.Set(0, 0);
			mEffectData.fullRatios.Set(0.7f, 0.3f);

			mLevel = LevelManager::GetInstance().GetCurrentLevel();
			Settings::GetInstance().SetBool("PlayerHit", false);
			mCol = (Collider*)mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);
			std::vector<Component*>&comps = LevelManager::GetInstance().GetCurrentLevel()->FindObjectWithName("L3Button")->GetComponents(eCOMPONENT_CODE);
			for (unsigned int i = 0; i < comps.size(); i++) {
				if(dynamic_cast<CCMazeHelper*>(comps[i]))
				{
					maze = (CCMazeHelper*)comps[i];
				}
			}
			if(mObject->GetName().find('1'))
			{
				mBoxNum = 1;
			}
			else if(mObject->GetName().find('2'))
			{
				mBoxNum = 2;
			}
			else
			{
				mBoxNum = 3;
			}
		}
		virtual void OnTriggerEnter(Collider& _col1, Collider& _col2)
		{
			if (!Settings::GetInstance().GetBool("IsTimePaused")) {
				if (_col2.GetBaseObject()->GetUniqueID() == mLevel->GetLeftController()->GetUniqueID() ||
					_col2.GetBaseObject()->GetUniqueID() == mLevel->GetRightController()->GetUniqueID() ||
					_col2.GetBaseObject()->GetUniqueID() == mLevel->GetHeadset()->GetUniqueID())
					if (!Settings::GetInstance().GetBool("PlayerHit") && !mBoxDone) {
						Settings::GetInstance().SetBool("PlayerHit", true);
						//cause player screen to change color
						vec2f finalRatios(0.9f, 0.8f);
						mDesaturationInterpolator.Prepare(0.2f, mEffectData.ratios, finalRatios, mEffectData.ratios);
						mDesaturationInterpolator.SetActive(true);
						//Disable Player
						Settings::GetInstance().SetBool("CantPauseTime", true);
						Settings::GetInstance().SetBool("CantTeleport", true);
						//Invoke Reset level
						mRestartLevel = true;
						//Play sounds
						((SFXEmitter*)mLevel->GetHeadset()->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 2))->CallEvent(Emitter::ePlay);
					}
			}
		}
		virtual void Update() {
			switch (mBoxNum) {
			case 1:
				mBoxDone = maze->mBox1Done;
				break;
			case 2:
				mBoxDone = maze->mBox2Done;
				break;
			case 3:
				mBoxDone = maze->mBox3Done;
				break;
			}
			if (!mBoxDone) {
				mCol->SetPos(mObject->GetTransform().GetMatrix().Position);
			}

			if (mRestartLevel) {
				mTimer += TimeManager::Instance()->GetDeltaTime();
				if (mTimer > 4.0f) {
					mRestartLevel = false;
					TimeManager::Instance()->ResetTimeLineandLevel();
					vec2f finalRatios(0, 0);
					Settings::GetInstance().SetBool("CantPauseTime", false);
					Settings::GetInstance().SetBool("CantTeleport", false);
					mDesaturationInterpolator.Prepare(0.2f, mEffectData.ratios, finalRatios, mEffectData.ratios);
					mDesaturationInterpolator.SetActive(true);
					mTimer = 0;
				}
			}
			if (mDesaturationInterpolator.GetActive()) {
				RenderShape* quad = Renderer::Instance()->GetSceneQuad();
				Renderer::Instance()->GetContext()->UpdateSubresource(quad->GetContext().mPixelCBuffers[ePB_REGISTER1].Get(), 0, NULL, &mEffectData, 0, 0);
			}
			if (mDesaturationInterpolator.Update(TimeManager::Instance()->GetDeltaTime())) {
				mDesaturationInterpolator.SetActive(false);
				RenderShape* quad = Renderer::Instance()->GetSceneQuad();
				Renderer::Instance()->GetContext()->UpdateSubresource(quad->GetContext().mPixelCBuffers[ePB_REGISTER1].Get(), 0, NULL, &mEffectData, 0, 0);
			}

			//if (GetAsyncKeyState(VK_F2) & 1) {
			//	 {
			//		Settings::GetInstance().SetBool("PlayerHit", true);
			//		//cause player screen to change color
			//		vec2f finalRatios(0.9f, 0.8f);
			//		mDesaturationInterpolator.Prepare(0.2f, mEffectData.ratios, finalRatios, mEffectData.ratios);
			//		mDesaturationInterpolator.SetActive(true);
			//		//Disable Player
			//		Settings::GetInstance().SetBool("PauseMenuUp", true);
			//		//Invoke Reset level
			//		mRestartLevel = true;
			//	}
			//}
		}
	};
}
