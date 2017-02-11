#pragma once
#include "RendererDefines.h"

class ShaderManager {
	static ShaderManager* sInstance;
public:
	static ShaderManager* Instance();
	static void DestroyInstance();
	

};