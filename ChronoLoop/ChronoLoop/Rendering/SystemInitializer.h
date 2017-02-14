#pragma once
#include "Renderer.h"
#include "RasterizerStateManager.h"
#include "ShaderManager.h"
#include "InputLayoutManager.h"
#include "TextureManager.h"
#include "../Input/VRInputManager.h"
#include "../Core/TimeManager.h"

namespace RenderEngine {
	bool InitializeSystems(HWND _Window, unsigned int _width, unsigned int _height,
												 bool _vsync, int _fps, bool _fullscreen, float _farPlane, float _nearPlane,
												 vr::IVRSystem* _vrsys) {
		Renderer::Instance()->Initialize(_Window, _width, _height, _vsync, _fps, _fullscreen, _farPlane, _nearPlane, _vrsys);
		RasterizerStateManager::Instance();
		TimeManager::Instance();
		InputLayoutManager::Instance();
		TextureManager::Instance();
		if (_vrsys) {
			VRInputManager::Instance();
			VRInputManager::Initialize(_vrsys);
		}
		ShaderManager::Instance();
		return true;
	}

	bool ShutdownSystems() {
		ShaderManager::DestroyInstance();
		VRInputManager::Shutdown();
		TextureManager::DestroyInstance();
		InputLayoutManager::DestroyInstance();
		TimeManager::Destroy();
		RasterizerStateManager::DestroyInstance();
		Renderer::DestroyInstance();
		return true;
	}
}