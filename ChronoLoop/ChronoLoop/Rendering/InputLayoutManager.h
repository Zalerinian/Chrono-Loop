#pragma once
#include "stdafx.h"

enum VertFormat
{
	POS = 0,
	POSCOLOR,
	POSTEX,
	POSNORMTEX,
	POSNORMTANTEX,
	POSBONEWEIGHT,
	POSBONEWEIGHTNORMTEX,
	POSBONEWEIGHTNORMTANTEX,
	MAX
};

class ID3D11InputLayout;

class InputLayoutManager
{
private:
	static InputLayoutManager* sInstance;

	InputLayoutManager();
	void Initialize();
	ID3D11InputLayout* InputLayouts[VertFormat::MAX];
public:
	~InputLayoutManager();
	static InputLayoutManager &Instance();
	static void DestroyInstance();
	ID3D11InputLayout *GetInputLayout(VertFormat _index) { return InputLayouts[_index]; };
};

