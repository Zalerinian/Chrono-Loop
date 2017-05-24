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

#include "Sound\SoundEngine.h"
#include "Objects\BaseObject.h"
#include "Actions/TeleportAction.hpp"
#include "Actions/CCElasticReactionWithPlane.h"
#include "Actions/BoxSnapToControllerAction.hpp"
#include "Actions/CCElasticSphereToSphere.h"
#include "Actions/CCElasticAABBtoAABB.h"
#include "Actions/CCElasticAABBToSphere.h"
#include "Actions/TimeManipulation.h"
#include "Actions/CCAnimationController.h"
#include "Actions/HeadsetFollow.hpp"
#include "Actions\CodeComponent.hpp"
#include "Actions/CCButtonPress.h"
#include "Actions/MainMenuBT.h"
#include "Core/Level.h"
#include "Common/Logger.h"
#include "Particles\ParticleSystem.h"
//#include "Rendering/TextureManager.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Actions/TimelineIndicator.hpp"
//#include "ParticleEffectPrototypes.h"

using namespace Epoch;
#define LEVEL_1 0
#define MAINMENU 0
#define CONSOLE_OVERRIDE 1
#define FIXED_UPDATE_INTERVAL (1 / 180.0f)
#define RENDER_INTERVAL (1 / 90.0f)

HWND hwnd;
LPCTSTR WndClassName = L"ChronoWindow";
HINSTANCE hInst;
bool VREnabled = false;

const wchar_t* _basePath = L"../Resources/Soundbanks/";
const wchar_t* _initSB = L"Init.bnk";
const wchar_t* _aSB = L"Test_Soundbank.bnk";
const wchar_t* _mainS = L"Chrono_Sound.bnk";
const wchar_t* _sbpkg1 = L"SB_SoundPackage1.bnk";

#if defined(_WIN64)
typedef unsigned __int64 AudioEvent;				///< Integer (unsigned) type for pointers
#else
typedef __w64 unsigned int AudioEvent;			///< Integer (unsigned) type for pointers
#endif
#define CONSOLE_OVERRIDE 1

bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);
std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
static float renderDelta = 0.0f;
static float deltaTime, fixedTime;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Update();
void UpdateTime();

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(48272); // 
	if (!InitializeWindow(hInstance, nCmdShow, 1366, 720, true)) {
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

	if (!InitializeSystems(hwnd, 1366, 720, false, 90, false, 1000, 0.1f, vrsys)) {
		return 1;
	}

	Microsoft::WRL::ComPtr<ID3D11Device> renderingDevice = Renderer::Instance()->GetDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> renderingContext = Renderer::Instance()->GetContext();

	// Update everything
	deltaTime = (float)(std::chrono::steady_clock::now().time_since_epoch().count());
	srand((unsigned int)time(NULL));
	Update();

	// Close the window so we can clean up.
	DestroyWindow(Renderer::Instance()->GetWindow());


	// Cleanup
	ShutdownSystems();
	vr::VR_Shutdown();
	vrsys = nullptr;
	renderingContext->Flush();
	renderingContext = nullptr;

	for (int i = 0; i < 40; ++i) {
		SystemLogger::Warn() << "THE CONSOLE HAS BEEN DETATCHED, AND IS NOW OWNED BY THE STEAMVR SERVER. DO NOT CLOSE IT." << std::endl;
	}
	SystemLogger::DestroyInstance();
#if _DEBUG || CONSOLE_OVERRIDE
	FreeConsole();
#endif



#if _DEBUG
	// In debug mode, dump any remaining live DirectX objects. This list should be hopefully small at this point.
	ID3D11Debug *debug;
	renderingDevice.Get()->QueryInterface(IID_ID3D11Debug, (void**)&debug);
	if (debug) {
		OutputDebugStringA("\n\n\n");
		debug->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_DETAIL);
		debug->Release();
		OutputDebugStringA("\n\n\n");
	}
#endif
	renderingDevice.Get()->Release();

	return 0;
}

void UpdateTime() {
	deltaTime = (float)(std::chrono::steady_clock::now().time_since_epoch().count() - lastTime.time_since_epoch().count()) / 1000.0f / 1000.0f / 1000.0f;
	fixedTime += deltaTime;
	lastTime = std::chrono::steady_clock::now();
}


void Update() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// TODO: Replace all this with a level to run.
	///*///////////////////////Using this to test physics//////////////////
	//_CrtSetBreakAlloc(4390);
	
	////Sound Initializing---------------------------------------------------
	AudioWrapper::GetInstance().Initialize();
	//Soundbanks
	AudioWrapper::GetInstance().SetBasePath(_basePath);
	AudioWrapper::GetInstance().LoadSoundBank(_initSB);
	AudioWrapper::GetInstance().LoadSoundBank(_aSB);
	AudioWrapper::GetInstance().LoadSoundBank(_mainS);
	AudioWrapper::GetInstance().LoadSoundBank(_sbpkg1);


	Transform identity, transform;
	BaseObject* RightController = Pool::Instance()->iGetObject()->Reset("Controller1 - 0", identity);
	MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");
	MeshComponent *rightRaycaster = new MeshComponent("../Resources/RaycastCylinder.obj");
	rightRaycaster->AddTexture("../Resources/Teal.png", eTEX_DIFFUSE);
	rightRaycaster->SetVisible(false);
	mc->AddTexture("../Resources/Controller_Diffuse.png", eTEX_DIFFUSE);
	mc->AddTexture("../Resources/Controller_Normal", eTEX_NORMAL);
	mc->AddTexture("../Resources/Controller_Specular", eTEX_SPECULAR);
	TeleportAction* rightTele = new TeleportAction(eControllerType_Primary);
	ControllerCollider* rightConCol = new ControllerCollider(RightController, vec3f(-0.10f, -0.10f, -0.10f), vec3f(0.10f, 0.10f, 0.10f), false);
	RightController->AddComponent(mc);
	RightController->AddComponent(rightTele);
	RightController->AddComponent(rightRaycaster);
	RightController->AddComponent(rightConCol);
	TimeManager::Instance()->AddObjectToTimeline(RightController);

	BaseObject* LeftController = Pool::Instance()->iGetObject()->Reset("Controller2 - 0", identity); //new BaseObject("Controller2", identity);
	MeshComponent *mc2 = new MeshComponent("../Resources/Player_hand.obj");
	MeshComponent *leftRaycaster = new MeshComponent("../Resources/RaycastCylinder.obj");
	leftRaycaster->AddTexture("../Resources/Teal.png", eTEX_DIFFUSE);
	leftRaycaster->SetVisible(false);
	mc2->AddTexture("../Resources/Player_hand_Diffuse.png", eTEX_DIFFUSE);
	mc2->AddTexture("../Resources/Player_hand_Emissive.png", eTEX_EMISSIVE);
	mc2->AddTexture("../Resources/Player_hand_Normal.png", eTEX_NORMAL);
	mc2->AddTexture("../Resources/Player_hand_Specular", eTEX_SPECULAR);
	TeleportAction* leftTele = new TeleportAction(eControllerType_Secondary);
	ControllerCollider* leftConCol = new ControllerCollider(LeftController, vec3f(-0.10f, -0.10f, -0.10f), vec3f(0.10f, 0.10f, 0.10f), true);
	LeftController->AddComponent(leftConCol);
	//LeftController->AddComponent(leftRaycaster);
	LeftController->AddComponent(leftTele);
	LeftController->AddComponent(mc2);
	TimeManager::Instance()->AddObjectToTimeline(LeftController);

	BaseObject* headset = Pool::Instance()->iGetObject()->Reset("headset", transform); //new BaseObject("headset", transform);
	HeadsetFollow* hfollow = new HeadsetFollow();
	MainMenuBT *bt = new MainMenuBT();
	headset->AddComponent(bt);
	headset->AddComponent(hfollow);
	TimeManager::Instance()->AddObjectToTimeline(headset);


	//Level 1 door////////////////////////////////////////////////////////////////////////
	Particle* p1 = &Particle::Init();
	p1->SetPos(vec3f(0, 0, 0));
	p1->SetColors(vec3f(1, 0, 0), vec3f(.5f, 0, .5f));
	p1->SetLife(550);
	p1->SetSize(.35f, .15f);
	ParticleEmitter* emit11 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, 2.820054f, 1));
	emit11->SetParticle(p1);
	emit11->SetTexture("../Resources/BasicRectP.png");
	((TeleportEffect*)emit11)->y1 = 8;
	((TeleportEffect*)emit11)->y2 = 12;
	((TeleportEffect*)emit11)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
	((TeleportEffect*)emit11)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
	ParticleSystem::Instance()->AddEmitter(emit11);
	emit11->FIRE();

	p1 = &Particle::Init();
	p1->SetPos(vec3f(0, 0, 0));
	p1->SetColors(vec3f(.5f, 0, .5f), vec3f(1, 0, 0));
	p1->SetLife(550);
	p1->SetSize(.15f, .05f);
	ParticleEmitter* emit12 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, 2.820054f, 1));
	emit12->SetTexture("../Resources/BasicCircleP.png");
	emit12->SetParticle(p1);
	((TeleportEffect*)emit12)->y1 = 1;
	((TeleportEffect*)emit12)->y2 = 5;
	((TeleportEffect*)emit12)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
	((TeleportEffect*)emit12)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
	ParticleSystem::Instance()->AddEmitter(emit12);
	emit12->FIRE();
	//////////////////////////////////////////////////////////////////////////////////////

	//Level 2 door////////////////////////////////////////////////////////////////////////
	//Particle* p2 = &Particle::Init();
	//p2->SetPos(vec3f(0, 0, 0));
	//p2->SetColors(vec3f(1, 0, 0), vec3f(.5f, 0, .5f));
	//p2->SetLife(500);
	//p2->SetSize(.35f, .15f);
	//ParticleEmitter* emit21 = new TeleportEffect(-1, 150, 2, vec4f(-2.82f, -10, 0, 1));
	//emit21->SetParticle(p2);
	//emit21->SetTexture("../Resources/BasicRectP.png");
	//((TeleportEffect*)emit21)->y1 = 8;
	//((TeleportEffect*)emit21)->y2 = 12;
	//((TeleportEffect*)emit21)->SetPosBounds(vec3f(0, 0, -.5f), vec3f(0, 1, .5f));
	//((TeleportEffect*)emit21)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
	//ParticleSystem::Instance()->AddEmitter(emit21);
	//emit21->FIRE();
	//
	//p2 = &Particle::Init();
	//p2->SetPos(vec3f(0, 0, 0));
	//p2->SetColors(vec3f(.5f, 0, .5f), vec3f(1, 0, 0));
	//p2->SetLife(500);
	//p2->SetSize(.15f, .05f);
	//ParticleEmitter* emit22 = new TeleportEffect(-1, 150, 2, vec4f(-2.82f, -10, 0, 1));
	//emit22->SetTexture("../Resources/BasicCircleP.png");
	//emit22->SetParticle(p2);
	//((TeleportEffect*)emit22)->y1 = 1;
	//((TeleportEffect*)emit22)->y2 = 5;
	//((TeleportEffect*)emit22)->SetPosBounds(vec3f(0, 0, -.5f), vec3f(0, 1, .5f));
	//((TeleportEffect*)emit22)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
	//ParticleSystem::Instance()->AddEmitter(emit22);
	//emit22->FIRE();
	//////////////////////////////////////////////////////////////////////////////////////

	//Exit Side Chamber////////////////////////////////////////////////////////////////////////
	Particle* exit = &Particle::Init();
	exit->SetPos(vec3f(0, 0, 0));
	exit->SetColors(vec3f(0, 0, 1), vec3f(0, .5f, .5f));
	exit->SetLife(500);
	exit->SetSize(.15f, .05f);
	ParticleEmitter* exitEmit = new ParticleEmitter(-1, 150, 2, vec4f(-3.918808f, 1.5f, 0, 1));
	exitEmit->SetParticle(exit);
	exitEmit->SetTexture("../Resources/BasicRectP.png");
	((ParticleEmitter*)exitEmit)->SetPosBounds(vec3f(-.5f, -.5f, -.5f), vec3f(.5f, .5f, .5f));
	((ParticleEmitter*)exitEmit)->SetVelBounds(vec3f(-.5f, -.5f, -.5f), vec3f(.5f, .5f, .5f));
	ParticleSystem::Instance()->AddEmitter(exitEmit);
	exitEmit->FIRE();
	
	exit = &Particle::Init();
	exit->SetPos(vec3f(0, 0, 0));
	exit->SetColors(vec3f(.5f, 0, .5f), vec3f(1, 0, 0));
	exit->SetLife(500);
	exit->SetSize(.1f, .05f);
	ParticleEmitter* exitEmit2 = new ParticleEmitter(-1, 150, 2, vec4f(-3.918808f, 1.5f, 0, 1));
	exitEmit2->SetTexture("../Resources/BasicCircleP.png");
	exitEmit2->SetParticle(exit);
	((ParticleEmitter*)exitEmit2)->SetPosBounds(vec3f(-.5f, -.5f, -.5f), vec3f(.5f, .5f, .5f));
	((ParticleEmitter*)exitEmit2)->SetVelBounds(vec3f(-.5f, -.5f, -.5f), vec3f(.5f, .5f, .5f));
	ParticleSystem::Instance()->AddEmitter(exitEmit2);
	exitEmit2->FIRE();
	//////////////////////////////////////////////////////////////////////////////////////

	//Start Side Chamber////////////////////////////////////////////////////////////////////////
	Particle* start = &Particle::Init();
	start->SetPos(vec3f(0, 0, 0));
	start->SetColors(vec3f(.2f, .2f, 1), vec3f(0, 1, .2f));
	start->SetLife(500);
	start->SetSize(.15f, .05f);
	ParticleEmitter* startEmit = new TeleportEffect(-1, 150, 2, vec4f(0, .04f, 3.918808f, 1));
	startEmit->SetParticle(start);
	startEmit->SetTexture("../Resources/BasicRectP.png");
	((TeleportEffect*)startEmit)->y1 = 8;
	((TeleportEffect*)startEmit)->y2 = 12;
	((TeleportEffect*)startEmit)->SetPosBounds(vec3f(-.75f, 0, -.5f), vec3f(.75f, 1, .5f));
	((TeleportEffect*)startEmit)->SetVelBounds(vec3f(.5f, 1, .5f), vec3f(.5f, 5, .5f));
	ParticleSystem::Instance()->AddEmitter(startEmit);
	startEmit->FIRE();
	
	start = &Particle::Init();
	start->SetPos(vec3f(0, 0, 0));
	start->SetColors(vec3f(.5f, 0, .25f), vec3f(.2f, .8f, .5f));
	start->SetLife(500);
	start->SetSize(.15f, .05f);
	ParticleEmitter* startEmit2 = new TeleportEffect(-1, 150, 2, vec4f(0, .04f, 3.918808f, 1));
	startEmit2->SetTexture("../Resources/BasicCircleP.png");
	startEmit2->SetParticle(start);
	((TeleportEffect*)startEmit2)->y1 = 1;
	((TeleportEffect*)startEmit2)->y2 = 5;
	((TeleportEffect*)startEmit2)->SetPosBounds(vec3f(-.75f, 0, -.5f), vec3f(.75f, 1, .5f));
	((TeleportEffect*)startEmit2)->SetVelBounds(vec3f(.5f, 1, .5f), vec3f(.5f, 5, .5f));
	ParticleSystem::Instance()->AddEmitter(startEmit2);
	startEmit2->FIRE();
	//////////////////////////////////////////////////////////////////////////////////////

	//Level 3 door////////////////////////////////////////////////////////////////////////
	//Particle* lvl3 = &Particle::Init();
	//lvl3->SetPos(vec3f(0, 0, 0));
	//lvl3->SetColors(vec3f(1, 0, 0), vec3f(.5f, 0, .5f));
	//lvl3->SetLife(500);
	//lvl3->SetSize(.35f, .15f);
	//ParticleEmitter* emitlvl3 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, -2.82f, 1));
	//emitlvl3->SetParticle(lvl3);
	//emitlvl3->SetTexture("../Resources/BasicRectP.png");
	//((TeleportEffect*)emitlvl3)->y1 = 8;
	//((TeleportEffect*)emitlvl3)->y2 = 12;
	//((TeleportEffect*)emitlvl3)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
	//((TeleportEffect*)emitlvl3)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
	//ParticleSystem::Instance()->AddEmitter(emitlvl3);
	//emitlvl3->FIRE();
	//
	//lvl3 = &Particle::Init();
	//lvl3->SetPos(vec3f(0, 0, 0));
	//lvl3->SetColors(vec3f(.5f, 0, .5f), vec3f(1, 0, 0));
	//lvl3->SetLife(500);
	//lvl3->SetSize(.15f, .05f);
	//ParticleEmitter* emit2lvl3 = new TeleportEffect(-1, 150, 2, vec4f(0, -10, -2.82f, 1));
	//emit2lvl3->SetTexture("../Resources/BasicCircleP.png");
	//emit2lvl3->SetParticle(lvl3);
	//((TeleportEffect*)emit2lvl3)->y1 = 1;
	//((TeleportEffect*)emit2lvl3)->y2 = 5;
	//((TeleportEffect*)emit2lvl3)->SetPosBounds(vec3f(-.5f, 0, 0), vec3f(.5f, 1, 0));
	//((TeleportEffect*)emit2lvl3)->SetVelBounds(vec3f(0, .5f, 0), vec3f(0, 5, 0));
	//ParticleSystem::Instance()->AddEmitter(emit2lvl3);
	//emit2lvl3->FIRE();
	//////////////////////////////////////////////////////////////////////////////////////

	//Test Animate Quad///////////////////////////////////////////////////////////////////
	//Transform OneBack;
	//OneBack.SetMatrix(matrix4::CreateNewTranslation(0, 1, -3));
	//BaseObject *Forcefield = Pool::Instance()->iGetObject()->Reset("ForceField Quad", OneBack);
	//MeshComponent *forcefieldMesh = new MeshComponent("../Resources/AnimationPlane1x1.obj", 0.25f);
	//forcefieldMesh->AddTexture("../Resources/ForcefieldHex gray.png", eTEX_DIFFUSE);
	//forcefieldMesh->SetGeometryShader(eGS_PosNormTex_AnimQuad);
	//forcefieldMesh->SetTopmost(true);
	//Forcefield->AddComponent(forcefieldMesh);
	//Forcefield->AddComponent(new CCAnimationController(8, 6, 48, 1.0f / 24.0f));
	//////////////////////////////////////////////////////////////////////////////////////

	// Test Gamma Correction /////////////////////////////////////////////////////////////
	/*Transform gammaform;
	gammaform.SetMatrix(matrix4::CreateNewTranslation(0, 1, 0));
	BaseObject *GammaObject = Pool::Instance()->iGetObject()->Reset("ForceField Quad", gammaform);
	MeshComponent *GammaMesh = new MeshComponent("../Resources/AnimationPlane1x1.obj", 0.25f);
	GammaMesh->AddTexture("../Resources/grey.png", eTEX_DIFFUSE);
	GammaMesh->SetPixelShader(ePS_PURETEXTURE);
	GammaObject->AddComponent(GammaMesh);*/
	//////////////////////////////////////////////////////////////////////////////////////


	/// Raycast debug cube
	//Transform cubeScale;
	//cubeScale.SetMatrix(matrix4::CreateNewScale(0.01f, 0.01f, 0.01f));
	//BaseObject* mmCube = Pool::Instance()->iGetObject()->Reset("mmCube", cubeScale);// new BaseObject("walls", PlaneTransform);
	//MeshComponent *mmCubeMesh = new MeshComponent("../Resources/Cube.obj");
	//mmCubeMesh->AddTexture("../Resources/cube_texture.png", eTEX_DIFFUSE);
	//mmCube->AddComponent(mmCubeMesh);
	Physics::Instance()->mObjects.push_back(RightController);
	Physics::Instance()->mObjects.push_back(LeftController);

	Level* mainMenu;

	while (LevelManager::GetInstance().LoadLevelAsync("../Resources/MainMenu.elf", &mainMenu) != Epoch::LM::LevelStatus::Success) {}

	mainMenu->AssignPlayerControls(headset, LeftController, RightController);
	mainMenu->AddObject(RightController);
	mainMenu->AddObject(headset);
	mainMenu->AddObject(LeftController);
	//mainMenu->AddObject(GammaObject);
	auto& levelObjects = mainMenu->GetLevelObjects();
	for (auto it = levelObjects.begin(); it != levelObjects.end(); ++it)
	{
		if ((*it)->mComponents[eCOMPONENT_COLLIDER].size() > 0)
		{
			Physics::Instance()->mObjects.push_back((*it));
		}
	}

	LevelManager::GetInstance().RequestLevelChange(mainMenu);
	mainMenu->CallStart();
	((MeshComponent*)mainMenu->FindObjectWithName("Test2")->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
	((MeshComponent*)mainMenu->FindObjectWithName("Test3")->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
	((MeshComponent*)mainMenu->FindObjectWithName("Test4")->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);



	if (VREnabled) {
		VRInputManager::GetInstance().Update();
		CommandConsole::Instance().SetVRBool(true);
	}


	
	UpdateTime();
	fixedTime = 0;
	renderDelta = RENDER_INTERVAL;
	while (LevelManager::GetInstance().GetCurrentLevel()->ChronoLoop) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// Handle windows message.
			bool quit = false;
			switch (msg.message) {
				case WM_QUIT:
					quit = true;
					break;
				case WM_KEYDOWN:
					KeyboardInput::Instance().OnKeyDown(msg.wParam, msg.lParam);
					break;
				case WM_KEYUP:
					break;
				default:
					break;
			}
			if (quit) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			if (GetAsyncKeyState(VK_ESCAPE) && GetActiveWindow() == Renderer::Instance()->GetWindow()) {
				break;
			}

			CommandConsole::Instance().Update();
			AudioWrapper::GetInstance().Update();
			UpdateTime();
			LevelManager::GetInstance().Update();
			ParticleSystem::Instance()->Update();
			TimeManager::Instance()->Update(deltaTime);
			if (VREnabled || renderDelta >= RENDER_INTERVAL) {
				Renderer::Instance()->Render(renderDelta); 
				renderDelta = 0;
			} 
			else {
				renderDelta += deltaTime;
			}
			while (fixedTime >= FIXED_UPDATE_INTERVAL) {
				Physics::Instance()->Update(FIXED_UPDATE_INTERVAL);
				fixedTime -= FIXED_UPDATE_INTERVAL;
				if(fixedTime >=  0.25f)
				{
					UpdateTime();
					fixedTime = 0;
				}
			}

			if (VREnabled) {
				VRInputManager::GetInstance().Update();
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
		200, 200,                            //Top left corner of window
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