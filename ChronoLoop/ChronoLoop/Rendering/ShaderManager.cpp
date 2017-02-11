//#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager* ShaderManager::sInstance = nullptr;

ShaderManager * ShaderManager::Instance() {
	if (nullptr == sInstance) {
		sInstance = new ShaderManager;
	}
	return sInstance;
}

void ShaderManager::DestroyInstance() {
	delete sInstance;
	sInstance = nullptr;
}
