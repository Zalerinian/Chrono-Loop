//#include "stdafx.h"
#include "Rendering/SystemInitializer.h"
#include "Rendering/renderer.h"
#include "Objects/BaseObject.h"
#include "Rendering/InputLayoutManager.h"
#include "Input/VRInputManager.h"
#include "Core/TimeManager.h"
#include "Common/Logger.h"
#include <openvr.h>
#include <ctime>
#include <chrono>
#include "Messager\Messager.h"
#include "Objects\BaseObject.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

HWND hwnd;
LPCTSTR WndClassName = L"ChronoWindow";
HINSTANCE hInst;
Messager msger = Messager::Instance();
bool VREnabled = false;

const wchar_t* _basePath = L"../ChronoLoop/Sound/Sound/Soundbanks/1";
const wchar_t* _initSB = L"Init.bnk";
const wchar_t* _aSB = L"Test_Soundbank.bnk";

#if defined(_WIN64)
typedef unsigned __int64 AudioEvent;				///< Integer (unsigned) type for pointers
#else
typedef __w64 unsigned int AudioEvent;			///< Integer (unsigned) type for pointers
#endif

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
	//_CrtSetBreakAlloc(354);
	if (!InitializeWindow(hInstance, nCmdShow, 800, 600, true)) {
		MessageBox(NULL, L"Kablamo.", L"The window broke.", MB_ICONERROR | MB_OK);
		return 2;
	}

	// Initialize Rendering systems and VR
	vr::HmdError pError;
	vr::IVRSystem *vrsys = nullptr;
	if (vr::VR_IsHmdPresent()) {
		vrsys = vr::VR_Init(&pError, vr::VRApplication_Scene);
		if (pError != vr::HmdError::VRInitError_None) {
			SystemLogger::GetLog() << "Could not initialize OpenVR for reasons!" << std::endl;
		}
	}
	else {
		SystemLogger::GetLog() << "There is no VR Headset present. VR will not be enabled." << std::endl;
	}

	//vrsys = nullptr;
	if (vr::VR_IsHmdPresent() && vrsys == nullptr) {
		SystemLogger::GetLog() << "VR seems to be ready, but the VR pointer is null. Was VR forcefully disabled?" << std::endl;
	}

	if (vrsys != nullptr) {
		VREnabled = true;
	}

	if (!RenderEngine::InitializeSystems(hwnd, 800, 600, false, 90, false, 1000, 0.1f, vrsys)) {
		return 1;
	}

	SystemLogger::GetLog() << "Hello World! " << "We hope you have at least " << 5 << " smiles today." << std::endl;

	// Update everything
	Update();

	// Cleanup
	RenderEngine::ShutdownSystems();
	SystemLogger::CloseLog();
	SystemLogger::CloseError();
	vr::VR_Shutdown();
	vrsys = nullptr;

#if _DEBUG || CONSOLE_OVERRIDE
	FreeConsole();
#endif

	return 0;
}

void UpdateTime()
{
	deltaTime = (float)(std::chrono::steady_clock::now().time_since_epoch().count() - lastTime.time_since_epoch().count()) / 1000.0f / 1000.0f / 1000.0f;
	lastTime = std::chrono::steady_clock::now();
}


void Update() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	/*///////////////////////Using this to test physics//////////////////
	Transform transform, transform1;
	transform.SetMatrix(Identity());
	BaseObject obj;
	obj.SetName("aabb");
	Collider aabb;
	aabb.mAcceleration.vector = { 0.0f, 0.0f, 0.0f, 0.0f };
	aabb.mCubeMax.vector = { -1.0f, 2.0f, -1.0f, 1.0f };
	aabb.mCubeMin.vector = { 1.0f, 1.0f, 1.0f, 1.0f };
	aabb.mIsCube = true;
	aabb.mIsSphere = false;
	aabb.mIsPlane = false;
	aabb.mShouldMove = true;
	aabb.mMass = 1.0f;
	aabb.mVelocity.vector = { 0.0f, 0.0f, 0.0f, 0.0f };
	obj.AddComponent(&aabb);
	obj.SetTransform(transform);
	aabb.object = &obj;

	matrix4 mat = Identity();
	mat.fourth.x = 0.0f;
	mat.fourth.y = -1.0f;
	mat.fourth.z = 0.0f;
	mat.fourth.w = 1.0f;

	transform1.SetMatrix(mat);
	BaseObject obj1;
	obj1.SetName("plane");
	Collider plane;
	plane.mAcceleration.vector = { 0.0f, 0.0f, 0.0f, 0.0f };
	plane.mIsCube = false;
	plane.mIsSphere = false;
	plane.mIsPlane = true;
	plane.mShouldMove = false;
	plane.mMass = 0.0f;
	plane.mPlaneNorm.vector = { 0.0f, 1.0f, 0.0f, 0.0f };
	plane.mPlaneOffset = 0.0f;
	plane.mVelocity.vector = { 0.0f, 0.0f, 0.0f, 0.0f };
	obj1.AddComponent(&plane);
	obj1.SetTransform(transform1);
	plane.object = &obj1;

	Physics::Instance()->mColliders.push_back(&aabb);
	Physics::Instance()->mColliders.push_back(&plane);
	*////////////////////////////////////////////////////////////////////


	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// Handle windows message.
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			if (GetAsyncKeyState(VK_ESCAPE)) {
				break;
			}
			Physics::Instance()->Update(deltaTime);
			UpdateTime();
			if (VREnabled) {
				VRInputManager::Instance().update();
			}
			// Logic.Update(float deltaTime);
			TManager->Instance()->Update(deltaTime);
			RenderEngine::Renderer::Instance()->Render();
		}
	}
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
//void Update() {
//	MSG msg;
//	ZeroMemory(&msg, sizeof(MSG));
//
//	msger.SendInMessage(new Message(msgTypes::mSound, soundMsg::INITIALIZE_Audio, 0, false));
//	msger.SendInMessage(new Message(msgTypes::mSound, soundMsg::SET_BasePath, 0, false, (void*)new m_Path(_basePath)));
//	msger.SendInMessage(new Message(msgTypes::mSound, soundMsg::ADD_Soundbank, 0, false, (void*)new m_Path(_initSB)));
//	msger.SendInMessage(new Message(msgTypes::mSound, soundMsg::ADD_Soundbank, 0, false, (void*)new m_Path(_aSB)));
//	BaseObject* test = new BaseObject();
//	Listener* lisnr = new Listener();
//	test->AddComponent(lisnr);
//	msger.SendInMessage(new Message(msgTypes::mSound, soundMsg::ADD_Listener, 0, false, (void*)new m_Listener(lisnr, nullptr)));
//	msger.SendInMessage(new Message(msgTypes::mSound, soundMsg::MAKEEVENT_Loc, 0, false, (void*)new m_LocEvent((AudioEvent)AK::EVENTS::PLAY_TEST2, new vec4f(0, 0, 0, 0))));
//
//	while (true) {
//
//		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
//			// Handle windows message.
//			if (msg.message == WM_QUIT) {
//				break;
//			}
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		} else {
//			if (GetAsyncKeyState(VK_ESCAPE)) {
//				msger.SendInMessage(new Message(msgTypes::mSound, soundMsg::SHUTDOWN_Audio, 0, false));
//				break;
//			}
//			msger.SendInMessage(new Message(msgTypes::mSound, soundMsg::UPDATE_Audio, 0, false));
//			UpdateTime();
//			VRInputManager::Instance().update();
//			// Logic.Update(float deltaTime);
//			TManager->Instance()->Update(deltaTime);
//			RenderEngine::Renderer::Instance()->Render();
//		}
//	}
//
//	msger.SendInMessage(new Message(msgTypes::mSound, soundMsg::SHUTDOWN_Audio, 0, false));
//}
