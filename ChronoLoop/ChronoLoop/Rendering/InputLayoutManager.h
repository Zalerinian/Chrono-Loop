#pragma once
//#include "stdafx.h"
#include "RendererDefines.h"

struct ID3D11InputLayout;

namespace RenderEngine {

	class InputLayoutManager
	{
	private:
		static InputLayoutManager* sInstance;
	
		InputLayoutManager();
		void Initialize();
		ID3D11InputLayout* mInputLayouts[VertFormat::eVERT_MAX];
	public:
		~InputLayoutManager();
		static InputLayoutManager &Instance();
		static void DestroyInstance();
		static void LoadShader(char* _fileName, char**_output, int& _outputSize);
		ID3D11InputLayout *GetInputLayout(VertFormat _index) { return mInputLayouts[_index]; };
	};

}
