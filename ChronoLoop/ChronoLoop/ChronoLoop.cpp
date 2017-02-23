//#include "stdafx.h"
#include "Rendering/SystemInitializer.h"
#include "Rendering/renderer.h"
#include "Objects/BaseObject.h"
#include "Rendering/InputLayoutManager.h"
#include ".\Rendering\RenderShape.h"
#include "Input/VRInputManager.h"
#include "Core/TimeManager.h"
#include "Core/Timeline.h"
#include "Common/Logger.h"
#include <openvr.h>
#include <ctime>
#include <chrono>
#include <d3d11.h>
#include "Actions\CodeComponent.hpp"
#include "Objects/MeshComponent.h"
#include "Actions/BoxSnapToControllerAction.hpp"
#include "Messager\Messager.h"
#include "Objects\BaseObject.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

HWND hwnd;
LPCTSTR WndClassName = L"ChronoWindow";
HINSTANCE hInst;
bool VREnabled = false;

const wchar_t* _basePath = L"../ChronoLoop/Sound/Sound/Soundbanks/";
const wchar_t* _initSB = L"Init.bnk";
const wchar_t* _aSB = L"Test_Soundbank.bnk";

#if defined(_WIN64)
typedef unsigned __int64 AudioEvent;				///< Integer (unsigned) type for pointers
#else
typedef __w64 unsigned int AudioEvent;			///< Integer (unsigned) type for pointers
#endif
#define CONSOLE_OVERRIDE 1

bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);
std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
static float timeFrame = 0.0f;
static float deltaTime;
TimeManager* TManager;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Update();
void UpdateTime();

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(1253);
	if (!InitializeWindow(hInstance, nCmdShow, 800, 600, true)) {
		MessageBox(NULL, L"Kablamo.", L"The window broke.", MB_ICONERROR | MB_OK);
		return 2;
	}

	// Initialize Rendering systems and VR
	vr::HmdError pError;
	vr::IVRSystem *vrsys = nullptr;
	vrsys = vr::VR_Init(&pError, vr::VRApplication_Scene);
	if (pError != vr::HmdError::VRInitError_None) {
		SystemLogger::GetLog() << "Could not initialize OpenVR for reasons!" << std::endl;
	}

	if (vrsys != nullptr) {
		VREnabled = true;
	}

	if (!RenderEngine::InitializeSystems(hwnd, 800, 600, false, 90, false, 1000, 0.1f, vrsys)) {
		return 1;
	}

	std::shared_ptr<ID3D11Device*> renderingDevice = RenderEngine::Renderer::Instance()->iGetDevice();

	// Update everything
	deltaTime = (float)(std::chrono::steady_clock::now().time_since_epoch().count());
	Update();

	// Cleanup
	RenderEngine::ShutdownSystems();
	SystemLogger::CloseLog();
	SystemLogger::CloseError();
	vr::VR_Shutdown();
	vrsys = nullptr;

#if _DEBUG
	// In debug mode, dump any remaining live DirectX objects. This list should be hopefully small at this point.
	ID3D11Debug *debug;
	(*renderingDevice)->QueryInterface(IID_ID3D11Debug, (void**)&debug);
	if (debug) {
		OutputDebugStringA("\n\n\n");
		debug->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_DETAIL);
		debug->Release();
		OutputDebugStringA("\n\n\n");
	}
#endif

	// Release the Device
	(*renderingDevice)->Release();

#if _DEBUG || CONSOLE_OVERRIDE
	FreeConsole();
#endif

	return 0;
}

void UpdateTime() {
	deltaTime = (float)(std::chrono::steady_clock::now().time_since_epoch().count() - lastTime.time_since_epoch().count()) / 1000.0f / 1000.0f / 1000.0f;
	lastTime = std::chrono::steady_clock::now();
}


void Update() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));


	// TODO: Replace all this with a level to run.
	///*///////////////////////Using this to test physics//////////////////
	Transform transform;
	transform.SetMatrix(MatrixIdentity());
	matrix4 mat1 = MatrixTranslation(0, 5, 0);
	transform.SetMatrix(mat1);
	BaseObject obj("aabb", transform);
	CubeCollider *aabb = new CubeCollider(true, vec4f(0.0f, -9.80f, 0.0f, 1.0f), 10.0f, 0.5f, 0.7f, vec4f(0.15f, -0.15f, .15f, 1.0f), vec4f(-0.15f, 0.15f, -0.15f, 1.0f));
	aabb->AddForce(vec4f(2, 0, 0, 0));
	obj.AddComponent(aabb);
	Emitter* aabbSound = new Emitter();
	obj.AddComponent(aabbSound);
	aabbSound->AddSoundEvent(Emitter::sfxTypes::ePlayLoop, AK::EVENTS::PLAY_TEST1);
	aabbSound->AddSoundEvent(Emitter::sfxTypes::ePauseLoop, AK::EVENTS::PAUSE_TEST1);
	aabbSound->AddSoundEvent(Emitter::sfxTypes::eResumeLoop, AK::EVENTS::RESUME_TEST1);
	aabbSound->AddSoundEvent(Emitter::sfxTypes::eStopLoop, AK::EVENTS::STOP_TEST1);

	matrix4 mat = MatrixTranslation(0, -1, 0);

	Transform transform1;
	transform1.SetMatrix(mat);
	BaseObject obj1("plane", transform1);
	PlaneCollider* plane = new PlaneCollider(false, vec4f(0.0f, -9.8f, 0.0f, 1.0f), 10.0f, 0.5f, 0.5f, -1.0f, vec4f(0.0f, 1.0f, 0.0f , 1.0f));
	MeshComponent *planeObj = new MeshComponent("../Resources/Liftoff.obj");
	planeObj->AddTexture("../Resources/cube_texture.png", RenderEngine::eTEX_DIFFUSE);
	obj1.AddComponent(plane);
	obj1.AddComponent(planeObj);
	

	TimeManager::Instance()->GetTimeLine()->AddBaseObject(&obj,obj.GetUniqueId());
	MeshComponent *visibleMesh = new MeshComponent("../Resources/Cube.obj");
	visibleMesh->AddTexture("../Resources/cube_texture.png", RenderEngine::eTEX_DIFFUSE);
	obj.AddComponent(visibleMesh);

	BoxSnapToControllerAction *boxSnap = new BoxSnapToControllerAction();
	obj.AddComponent(boxSnap);

	Physics::Instance()->mObjects.push_back(&obj);
	Physics::Instance()->mObjects.push_back(&obj1);
	//*////////////////////////////////////////////////////////////////////
	if (VREnabled) {
		VRInputManager::Instance().iUpdate();
	}

	//Sound Initializing---------------------------------------------------
	Messager::Instance().SendInMessage(new Message(msgTypes::mSound, soundMsg::INITIALIZE_Audio, 0, false));
	//Soundbanks
	Messager::Instance().SendInMessage(new Message(msgTypes::mSound, soundMsg::SET_BasePath, 0, false, (void*)new m_Path(_basePath)));
	Messager::Instance().SendInMessage(new Message(msgTypes::mSound, soundMsg::ADD_Soundbank, 0, false, (void*)new m_Path(_initSB)));
	Messager::Instance().SendInMessage(new Message(msgTypes::mSound, soundMsg::ADD_Soundbank, 0, false, (void*)new m_Path(_aSB)));
	
	//Temp Camera OBJ
	Transform camTrans;
	camTrans.SetMatrix(MatrixIdentity());
	BaseObject camObj("TempCam", camTrans);
	Listener* ears = new Listener();
	camObj.AddComponent(ears);
	Messager::Instance().SendInMessage(new Message(msgTypes::mSound, soundMsg::ADD_Listener, 0, false, (void*)new m_Listener(ears, "Listener")));
	Messager::Instance().SendInMessage(new Message(msgTypes::mSound, soundMsg::ADD_Emitter, 0, false, (void*)new m_Emitter(aabbSound, "aabbS")));
	aabbSound->Play();

	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// Handle windows message.
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			if (GetAsyncKeyState(VK_ESCAPE)) {
				break;
			}
			Messager::Instance().SendInMessage(new Message(msgTypes::mSound, soundMsg::UPDATE_Audio, 0, false, (void*)nullptr));
			UpdateTime();
			auto& objects = Physics::Instance()->mObjects;
			for (auto it = objects.begin(); it != objects.end(); ++it) {
				(*it)->Update();
			}
			TManager->Instance()->Update(deltaTime);
			RenderEngine::Renderer::Instance()->Render(deltaTime);
			Physics::Instance()->Update(deltaTime);
			if (VREnabled) {
				VRInputManager::Instance().iUpdate();
			}
		}
	}
	Messager::Destroy();

}

bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed) {
#if _DEBUG || CONSOLE_OVERRIDE
	if (AllocConsole()) {
		FILE* pCout;
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		SetConsoleTitle(L"Chrono::Loop - Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
#endif

	WNDCLASSEX wc;

	HINSTANCE hDll;
	hDll = LoadLibrary(L"SHELL32.dll");

	wc.cbSize = sizeof(WNDCLASSEX);    //Size of our windows class
	wc.style = (CS_HREDRAW | CS_VREDRAW);    //class styles
	wc.lpfnWndProc = WndProc;    //Default windows procedure function
	wc.cbClsExtra = NULL;    //Extra bytes after our wc structure
	wc.cbWndExtra = NULL;    //Extra bytes after our windows instance
	wc.hInstance = hInstance;    //Instance to current application
	wc.hIcon = LoadIcon(hDll, MAKEINTRESOURCE(512));    //Title bar Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);    //Default mouse Icon
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);    //Window bg color
	wc.lpszMenuName = NULL;    //Name of the menu attached to our window
	wc.lpszClassName = WndClassName;    //Name of our windows class
	wc.hIconSm = LoadIcon(hDll, MAKEINTRESOURCE(512)); //Icon in your taskbar

	if (!RegisterClassEx(&wc))    //Register our windows class
	{
		//if registration failed, display error
		DWORD code = GetLastError();
		MessageBox(NULL, L"Error registering class", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	hwnd = CreateWindowEx(                                     //Create our Extended Window
		NULL,                                //Extended style
		WndClassName,                        //Name of our windows class
		L"Chrono::Loop",                     //Name in the title bar of our window
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, //style of our window
		600, 150,                            //Top left corner of window
		width,                               //Width of our window
		height,                              //Height of our window
		NULL,                                //Handle to parent window
		NULL,                                //Handle to a Menu
		hInstance,                           //Specifies instance of current program
		NULL                                 //used for an MDI client window
	);
	if (!hwnd) {
		MessageBox(NULL, L"Error creating window", L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(hwnd, ShowWnd);
	UpdateWindow(hwnd);

	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
