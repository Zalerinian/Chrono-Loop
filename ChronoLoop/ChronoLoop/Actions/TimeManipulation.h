#pragma once
#pragma once
#include <d3d11.h>
#include <d2d1_1.h>
#include "../Actions/CodeComponent.hpp"
#include "../Rendering/Renderer.h"


struct TimeManipulation : public CodeComponent {
private:
	bool mLeft;
	static unsigned int mCloneCount;
	ID3D11Texture2D* mCountTxt;
	ID2D1Bitmap1* mCountMap;

public:
	TimeManipulation();
	TimeManipulation(bool _left) { mLeft = _left; };
	~TimeManipulation();
	void Start();
	void Update();
};



