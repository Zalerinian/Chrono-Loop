#pragma once
#pragma once
#include <d3d11.h>
#include <d2d1_1.h>
#include "../Actions/CodeComponent.hpp"
#include "../Rendering/Renderer.h"
#include "../Input/VRInputManager.h"


namespace Epoch
{

	struct TimeManipulation : public CodeComponent
	{
	private:
		ControllerType mControllerRole = eControllerType_Primary;
		int mCount = 0;
		bool mTexturedApplied = false;
		bool mPauseTime = false;
		static unsigned int mCloneCount;
		ID3D11Texture2D* mCountTxt;
		ID2D1Bitmap1* mCountMap;
		unsigned int HotfixButtonDown = 0;

		static unsigned short mCurrTexture;
		std::string mTextures[10] = { "../Resources/CloneTexture.png",
			"../Resources/CloneTexture_Green.png",
			"../Resources/CloneTexture_Pink.png",
			"../Resources/CloneTexture_Orange.png",
			"../Resources/CloneTexture_Purple.png",
			"../Resources/CloneTexture_Red.png",
			"../Resources/CloneTexture_White.png",
			"../Resources/CloneTexture_Yellow.png",
			"../Resources/CloneTexture_Brown.png",
			"../Resources/CloneTexture_Grey.png" };

	public:
		TimeManipulation();
		TimeManipulation(ControllerType _t) { mControllerRole = _t; };
		~TimeManipulation();
		std::string GetTexture(unsigned short _indx) { return mTextures[_indx]; }
		void RaycastCloneCheck();
		bool isTimePaused() { return mPauseTime; };
		void makeTimePaused(bool _isPaused) { mPauseTime = _isPaused; };
		void Start();
		void Update();
	};

} // Epoch Namespace