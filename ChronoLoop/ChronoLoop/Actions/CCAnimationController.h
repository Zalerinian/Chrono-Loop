#pragma once
#include "CodeComponent.hpp"
#include "../Rendering/RendererDefines.h"
#include "../Objects/MeshComponent.h"
#include "../Objects/BaseObject.h"
#include "../Core/TimeManager.h"

namespace Epoch {

	struct CCAnimationController : CodeComponent {
		GSAnimatedQuad_Data mData;
		MeshComponent *mesh = nullptr;
		float mErrorTimer = 3.0f, mFrameRate = 1.0f / 60.0f, mTweenTime = 0;

		CCAnimationController(unsigned int _framesWide = 1, unsigned int _framesTall = 1, unsigned int _frameCount = 1, float _frameRate = 1.0f / 60.0f) {
			mData.currentFrame = 0;
			mData.framesWide = _framesWide;
			mData.framesTall = _framesTall;
			mData.frameCount = _frameCount;
			mFrameRate = _frameRate;
		}

		virtual void Start() {
			mesh = (MeshComponent*)mObject->GetComponentIndexed(eCOMPONENT_MESH, 0);
			if (mesh) {
				mesh->SetData(eCB_GEO, eBufferDataType_AnimatedQuad, eGB_REGISTER1, &mData);
			}
		}

		virtual void Update() {
			if (mesh != nullptr) {
				bool updated = false;
				mTweenTime += TimeManager::Instance()->GetDeltaTime();
				while (mTweenTime > mFrameRate) {
					mTweenTime -= mFrameRate;
					mData.currentFrame = (mData.currentFrame + 1) % mData.frameCount;
					updated = true;
				}
				if (updated) {
					mesh->UpdateData(eCB_GEO, eGB_REGISTER1, &mData);
				}
			} else {
				mErrorTimer += TimeManager::Instance()->GetDeltaTime();
				if (mErrorTimer >= 3.0f) {
					SystemLogger::Error() << "Could not animate an animated mesh, because it's pointer is null!" << std::endl;
					mErrorTimer -= 3.0f;
				}
			}
		}



	}; // CCAnimationController

}
