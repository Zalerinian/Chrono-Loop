#pragma once
//#include "stdafx.h"
#include "RendererDefines.h"

struct ID3D11InputLayout;

namespace Epoch {

	class InputLayoutManager
	{
	private:
		static InputLayoutManager* sInstance;
	
		InputLayoutManager();
		~InputLayoutManager();
		void Initialize();
		ID3D11InputLayout* mInputLayouts[VertFormat::eVERT_MAX];
	public:
		static InputLayoutManager &Instance();
		static void DestroyInstance();

		void ApplyLayout(VertFormat v);
		ID3D11InputLayout *GetInputLayout(VertFormat _index) { return mInputLayouts[_index]; };
	};

}
