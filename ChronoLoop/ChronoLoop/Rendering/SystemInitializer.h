#pragma once
#include "../Rendering/Renderer.h"
#include "../Rendering/RasterizerStateManager.h"
#include "../Rendering/ShaderManager.h"
#include "../Rendering/InputLayoutManager.h"
#include "../Rendering/TextureManager.h"
#include "../Input/VRInputManager.h"
#include "../Core/TimeManager.h"
#include "../Physics/Physics.h"
#include "../Core/Pool.h"
#include "../Rendering/IndexBufferManager.h"
#include "../Rendering/VertexBufferManager.h"
#include "../Core/Level.h"
#include "../Rendering/MeshCache.h"
#include "../Input/CommandConsole.h"
#include "../Input/KeyboardInput.h"

namespace Epoch {
	bool InitializeSystems(HWND _Window, unsigned int _width, unsigned int _height,
												 bool _vsync, int _fps, bool _fullscreen, float _farPlane, float _nearPlane,
												 vr::IVRSystem* _vrsys) {
		Renderer::Instance()->iInitialize(_Window, _width, _height, _vsync, _fps, _fullscreen, _farPlane, _nearPlane, _vrsys);
		RasterizerStateManager::Instance();
		TimeManager::Instance();
		InputLayoutManager::Instance();
		TextureManager::Instance();
		VRInputManager::Initialize(_vrsys);
		ShaderManager::Instance();
		Pool::Initialize();
		Physics::Instance();
		IndexBufferManager::GetInstance();
		VertexBufferManager::Instance();
		// Level is initialized in ChronoLoop
		MeshCache::GetInstance();
		CommandConsole::Instance();
		KeyboardInput::Instance();
		return true;
	}

	bool ShutdownSystems() {
		KeyboardInput::DestroyInstance();
		CommandConsole::DestroyInstance();
		MeshCache::DestroyInstance();
		Level::DestroyInstance();
		VertexBufferManager::Shutdown();
		IndexBufferManager::DestroyInstance();
		Physics::Destroy();
		Pool::DestroyInstance();
		ShaderManager::DestroyInstance();
		VRInputManager::DestroyInstance();
		TextureManager::DestroyInstance();
		InputLayoutManager::DestroyInstance();
		TimeManager::Destroy();
		RasterizerStateManager::DestroyInstance();
		Renderer::DestroyInstance();
		return true;
	}
}