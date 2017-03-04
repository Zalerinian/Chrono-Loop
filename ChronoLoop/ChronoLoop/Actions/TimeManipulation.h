#pragma once
#pragma once
#include <d3d11.h>
#include <d2d1_1.h>
#include "../Actions/CodeComponent.hpp"
#include "../Rendering/Renderer.h"

namespace Epoch
{

	struct TimeManipulation : public CodeComponent
	{
	private:
		ControllerType mControllerRole = eControllerType_Primary;
		int mCount = 0;
		bool mTexturedApplied = false;
		static unsigned int mCloneCount;
		ID3D11Texture2D* mCountTxt;
		ID2D1Bitmap1* mCountMap;

	public:
		TimeManipulation();
		TimeManipulation(ControllerType _t) { mControllerRole = _t; };
		~TimeManipulation();
		void Start();
		void Update();
	};

} // Epoch Namespace