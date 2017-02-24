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
#include "Common/Math.h"
#include "Objects/MeshComponent.h"
#include "Actions/HeadsetFollow.hpp"
#include "Actions/BoxSnapToControllerAction.hpp"
#include "Actions/TeleportAction.hpp"
#include "Actions/CCElasticReactionWithPlane.h"
#include "Actions/CCElasticAABBtoAABB.h"
#include "Actions/CCButtonPress.h"
#include "Core/Level.h"
#include "Common/Logger.h"
//#include "Rendering/TextureManager.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Actions/TimeManipulation.h"


#define CONSOLE_OVERRIDE 1

HWND hwnd;
LPCTSTR WndClassName = L"ChronoWindow";
HINSTANCE hInst;
bool VREnabled = false;

bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);
std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
static float timeFrame = 0.0f;
static float deltaTime;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Update();
void UpdateTime();

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(7251);
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
	vr::VR_Shutdown();
	RenderEngine::ShutdownSystems();
	Level::DestroyInstance();
	SystemLogger::CloseLog();
	SystemLogger::CloseError();
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
	BaseObject* PhysicsBox = Pool::Instance()->iGetObject()->Reset("aabb", transform);//new BaseObject("aabb", transform);
	CubeCollider *BoxCollider = new CubeCollider(PhysicsBox, true, vec4f(0.0f, -1.0f, 0.0f, 1.0f), 1.0f, 0.5f, 0.0f, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f));
	BoxCollider->AddForce(vec4f(2, 0, 0, 0));
	CodeComponent* PlaneCollision = new CCElasticReactionWithPlane;
	CodeComponent* BoxCollision = new CCElasticAABBtoAABB;
	PhysicsBox->AddComponent(BoxCollider);
	PhysicsBox->AddComponent(PlaneCollision);
	PhysicsBox->AddComponent(BoxCollision);
	TimeManager::Instance()->AddObjectToTimeline(PhysicsBox);

	Transform ButtonTransform;
	ButtonTransform.SetMatrix(MatrixIdentity());
	matrix4 ButtonMat = MatrixTranslation(-3, 1, 0);
	ButtonTransform.SetMatrix(ButtonMat);
	BaseObject* Button = new BaseObject("button", ButtonTransform);
	ButtonCollider* ButtonCol = new ButtonCollider(Button, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), 3, 1, vec4f(0,1,0,0));
	CodeComponent* ButtonCollision = new CCButtonPress;
	Button->AddComponent(ButtonCol);
	Button->AddComponent(ButtonCollision);
	TimeManager::Instance()->AddObjectToTimeline(Button);

	Transform PlaneTransform;
	PlaneTransform.SetMatrix(MatrixTranslation(0, -1, 0));
	BaseObject* Floor = Pool::Instance()->iGetObject()->Reset("plane", PlaneTransform);// new BaseObject("plane", PlaneTransform);
	PlaneCollider* plane = new PlaneCollider(false, vec4f(0.0f, 0.0f, 0.0f, 1.0f), 10.0f, 0.5f, 0.5f, -1.0f, vec4f(0.0f, 1.0f, 0.0f , 1.0f));
	MeshComponent *planeObj = new MeshComponent("../Resources/BigFloor.obj");
	planeObj->AddTexture("../Resources/floorg.png", RenderEngine::eTEX_DIFFUSE);
	Floor->AddComponent(plane);
	Floor->AddComponent(planeObj);
	
	Transform identity;
	identity.SetMatrix(Math::MatrixIdentity());

	BaseObject* walls = Pool::Instance()->iGetObject()->Reset("walls", PlaneTransform);// new BaseObject("walls", PlaneTransform);
	MeshComponent *wallMesh = new MeshComponent("../Resources/BigWall.obj");
	wallMesh->AddTexture("../Resources/Wallg.png", RenderEngine::eTEX_DIFFUSE);
	CubeCollider* ButtonRoomBackWall = new CubeCollider(walls, false, vec4f(0,0,0,0), 10, 0, .2f, vec4f(-7.034f, -1, -8, 1), vec4f(1.011f, 2, -7.026f, 1));
	CubeCollider* ExitLeftWall = new CubeCollider(walls, false, vec4f(0,0,0,0), 10, 0, .2f, vec4f(-0.985f, -1, -9.008f, 1), vec4f(1.011f, 2, -7.026f, 1));
	CubeCollider* ExitBackWall = new CubeCollider(walls, false, vec4f(0,0,0,0), 10, 0, .2f, vec4f(0.985f, -1, -10.008f, 1), vec4f(3.112f, 2, -9.008f, 1));
	CubeCollider* ExitRightWall = new CubeCollider(walls, false, vec4f(0,0,0,0), 10, 0, .2f, vec4f(3.112f, -1, -9.008f, 1), vec4f(4.112f, 2, -6.991f, 1));
	CubeCollider* MainBackWall = new CubeCollider(walls, false, vec4f(0,0,0,0), 10, 0, .2f, vec4f(4.112f, -1, -7.991f, 1), vec4f(7.036f, 2, -6.991f, 1));
	CubeCollider* RightWall = new CubeCollider(walls, false, vec4f(0,0,0,0), 10, 0, .2f, vec4f(7.036f, -1, -6.991f, 1), vec4f(8.036f, 2, 7.142f, 1));
	CubeCollider* MainFrontWall = new CubeCollider(walls, false, vec4f(0,0,0,0), 10, 0, .2f, vec4f(3.063f, -1, 7.142f, 1), vec4f(8.036f, 2, 8.142f, 1));
	CubeCollider* EnterRightWall = new CubeCollider(walls, false, vec4f(0,0,0,0), 10, 0, .2f, vec4f(3.063f, -1, 7.142f, 1), vec4f(4.063f, 2, 9.055f, 1));
	CubeCollider* EnterBackWall = new CubeCollider(walls, false, vec4f(0,0,0,0), 10, 0, .2f, vec4f(0.918f, -1, 9.055f, 1), vec4f(4.063f, 2, 10.055f, 1));
	CubeCollider* EnterLeftWall = new CubeCollider(walls, false, vec4f(0,0,0,0), 10, 0, .2f, vec4f(-0.918f, -1, 7.014f, 1), vec4f(0.918f, 2, 9.055f, 1));
	CubeCollider* ButtonRoomFrontWall = new CubeCollider(walls, false, vec4f(0,0,0,0), 10, 0, .2f, vec4f(-7.054f, -1, 7.014f, 1), vec4f(0.918f, -1, 8.014f, 1));
	CubeCollider* LeftWall = new CubeCollider(walls, false, vec4f(0,0,0,0), 10, 0, .2f, vec4f(-8.054f, -1, -7.014f, 1), vec4f(-7.054f, 2, 7.014f, 1));
	CubeCollider* DividerWall = new CubeCollider(walls, false, vec4f(0,0,0,0), 10, 0, .2f, vec4f(-1.273f, -1, -7.022f, 1), vec4f(-0.871f, 2, 3.125f, 1));
	walls->AddComponent(ButtonRoomBackWall);
	walls->AddComponent(ExitLeftWall);
	walls->AddComponent(ExitBackWall);
	walls->AddComponent(ExitRightWall);
	walls->AddComponent(MainBackWall);
	walls->AddComponent(RightWall);
	walls->AddComponent(MainFrontWall);
	walls->AddComponent(EnterRightWall);
	walls->AddComponent(EnterBackWall);
	walls->AddComponent(EnterLeftWall);
	walls->AddComponent(ButtonRoomFrontWall);
	walls->AddComponent(LeftWall);
	walls->AddComponent(DividerWall);
	walls->AddComponent(wallMesh);

	BaseObject* RightController = Pool::Instance()->iGetObject()->Reset("Controller", identity);// new BaseObject("Controller", identity);
	MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");
	mc->AddTexture("../Resources/vr_controller_lowpoly_texture.png", RenderEngine::eTEX_DIFFUSE);
	TeleportAction *ta = new TeleportAction(false);
	TimeManipulation* tm = new TimeManipulation(false);
	ControllerCollider* rightConCol = new ControllerCollider(RightController, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), false);
	RightController->AddComponent(mc);
	RightController->AddComponent(ta);
	RightController->AddComponent(rightConCol);
	RightController->AddComponent(tm);
	TimeManager::Instance()->AddObjectToTimeline(RightController);

	MeshComponent *visibleMesh = new MeshComponent("../Resources/Cube.obj");
	visibleMesh->AddTexture("../Resources/cube_texture.png", RenderEngine::eTEX_DIFFUSE);
	PhysicsBox->AddComponent(visibleMesh);
	//CodeComponent *codeComponent = new BoxSnapToControllerAction();
	//PhysicsBox->AddComponent(codeComponent);

	MeshComponent *ButtonMesh = new MeshComponent("../Resources/cube.obj");
	ButtonMesh->AddTexture("../Resources/cube_texture.png", RenderEngine::eTEX_DIFFUSE);
	
	Button->AddComponent(ButtonMesh);

	//pat added
	BaseObject* LeftController = Pool::Instance()->iGetObject()->Reset("Controller2", identity); //new BaseObject("Controller2", identity);
	MeshComponent *mc2 = new MeshComponent("../Resources/Controller.obj");
	mc2->AddTexture("../Resources/vr_controller_lowpoly_texture.png", RenderEngine::eTEX_DIFFUSE);
	TeleportAction *ta2 = new TeleportAction(true);
	TimeManipulation* tm2 = new TimeManipulation(true);
	ControllerCollider* leftConCol = new ControllerCollider(LeftController, vec4f(-0.15f, -0.15f, -0.15f, 1.0f), vec4f(0.15f, 0.15f, 0.15f, 1.0f), true);
	LeftController->AddComponent(leftConCol);
	LeftController->AddComponent(mc2);
	LeftController->AddComponent(ta2);
	LeftController->AddComponent(tm2);
	TimeManager::Instance()->AddObjectToTimeline(LeftController);

	BaseObject* headset = Pool::Instance()->iGetObject()->Reset("headset", transform); //new BaseObject("headset", transform);
	MeshComponent *visibleMesh2 = new MeshComponent("../Resources/Cube.obj");
	visibleMesh2->AddTexture("../Resources/cube_texture.png", RenderEngine::eTEX_DIFFUSE);
	visibleMesh2->SetVisible(false);
	HeadsetFollow* hfollow = new HeadsetFollow();
	headset->AddComponent(hfollow);
	headset->AddComponent(visibleMesh2);

	TimeManager::Instance()->AddObjectToTimeline(headset);


	Physics::Instance()->mObjects.push_back(PhysicsBox);
	Physics::Instance()->mObjects.push_back(Floor);
	Physics::Instance()->mObjects.push_back(walls);
	Physics::Instance()->mObjects.push_back(RightController);
	Physics::Instance()->mObjects.push_back(LeftController);
	Physics::Instance()->mObjects.push_back(Button);
	TimeManager::Instance()->HotFixAddClone(Button);
	Level::Initialize(headset, RightController, LeftController);
	Level* L1 = Level::Instance(); 
	L1->iAddObject(PhysicsBox);
	L1->iAddObject(Floor);
	L1->iAddObject(RightController);
	L1->iAddObject(walls);
	L1->iAddObject(headset);
	L1->iAddObject(LeftController);
	L1->iCallStart();

	L1->iAddObject(Button);
	//// Test for TextureManager::iAddTexture2D. Works nicely!
	//D3D11_TEXTURE2D_DESC AddedTextureDesc;
	//AddedTextureDesc.Width = 800;
	//AddedTextureDesc.Height = 600;
	//AddedTextureDesc.MipLevels = 1;
	//AddedTextureDesc.ArraySize = 1;
	//AddedTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//AddedTextureDesc.SampleDesc.Count = 1;
	//AddedTextureDesc.SampleDesc.Quality = 0;
	//AddedTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	//AddedTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//AddedTextureDesc.CPUAccessFlags = 0;
	//AddedTextureDesc.MiscFlags = 0;
	//ID3D11Texture2D *AddedTex;
	//(*Renderer::Instance()->iGetDevice())->CreateTexture2D(&AddedTextureDesc, nullptr, &AddedTex);
	//std::string AddedTextureName = "Bootleg";
	//TextureManager::Instance()->iAddTexture2D(AddedTextureName, AddedTex, nullptr);
	//planeObj->AddTexture(AddedTextureName.c_str(), eTEX_CUSTOM1);

	//*////////////////////////////////////////////////////////////////////
	if (VREnabled) {
		VRInputManager::Instance().iUpdate();
	}
	
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


			//SystemLogger::GetLog() << "[Debug] Regular Update " << std::endl;
			UpdateTime();
			Level::Instance()->iUpdate();
			
			TimeManager::Instance()->Update(deltaTime);
			RenderEngine::Renderer::Instance()->Render(deltaTime);
			Physics::Instance()->Update(deltaTime);
			if (VREnabled) {
				VRInputManager::Instance().iUpdate();
			}
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
