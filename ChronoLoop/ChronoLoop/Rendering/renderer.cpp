//#include "stdafx.h"
#include "Renderer.h"
#include <d3d11.h>
#include<d2d1_1.h>
#include <dxgi1_2.h>
#include <openvr.h>
#include "../Common/Logger.h"
#include "Mesh.h"
#include "InputLayoutManager.h"
#include "RenderShape.h"
#include "../Input/VRInputManager.h"
#include "../Input/Controller.h"
#include "../Common/Common.h"
#include "../Common/Breakpoint.h"
#include "../Input/CommandConsole.h"
#include "../Rendering/Draw2D.h"

#define ENABLE_TEXT 1


using namespace std;
using namespace D2D1;

namespace Epoch {

	Renderer* Renderer::sInstance = nullptr;

#pragma region Singleton Functions

	Renderer* Renderer::Instance() {
		if (nullptr == sInstance) {
			sInstance = new Renderer;
		}
		return sInstance;
	}

	void Renderer::DestroyInstance() {
		if (sInstance != nullptr) {
			delete sInstance;
		}
		sInstance = nullptr;
	}

#pragma endregion Singleton Functions

#pragma region Instance Functions


#pragma region Private Functions


	void Renderer::ThrowIfFailed(HRESULT hr) {
		if (FAILED(hr)) {
			throw "Something has gone catastrophically wrong!";
		}
	}

	matrix4 Renderer::GetEye(vr::EVREye e) {
		vr::HmdMatrix34_t matEyeRight = mVrSystem->GetEyeToHeadTransform(e);
		matrix4 matrixObj(
			matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
			matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
			matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
			matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
		);

		return matrixObj.Invert();
	}

	matrix4 Renderer::GetProjection(vr::EVREye e) {

		vr::HmdMatrix44_t mat = mVrSystem->GetProjectionMatrix(e, 0.1f, 1000);

		return matrix4(
			mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
			mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
			mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
			mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
		);
	}

	void Renderer::GetMVP(vr::EVREye e, ViewProjectionBuffer &data) {
		matrix4 hmd = VRInputManager::GetInstance().GetTrackedPositions()[0].mDeviceToAbsoluteTracking;

		matrix4 hmdPos = (hmd * VRInputManager::GetInstance().GetPlayerPosition()).Invert();
		if (e == vr::EVREye::Eye_Left) {
			data.view = (hmdPos * mEyePosLeft).Transpose();
			data.projection = mEyeProjLeft.Transpose();
		} else {
			data.view = (hmdPos * mEyePosRight).Transpose();
			data.projection = mEyeProjRight.Transpose();
		}
	}

	Renderer::Renderer() {}

	Renderer::Renderer::~Renderer() {
		(*mContext)->Release();
		(*mDSView)->Release();
		(*mDepthBuffer)->Release();
		(*mMainView)->Release();
		(*mFactory)->Release();
		(*mChain)->Release();
		// The device's release has been moved to ChronoLoop.cpp
		(*mVPBuffer)->Release();
		(*mPositionBuffer)->Release();
		(*mMainViewTexture)->Release();
		mContext.reset();
		mDSView.reset();
		mDepthBuffer.reset();
		mMainView.reset();
		mFactory.reset();
		mChain.reset();
		mDevice.reset();

#if ENABLE_TEXT

#endif
	}

	void Renderer::InitializeD3DDevice() {
		UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if _DEBUG
		flags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif

		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		UINT numLevels = ARRAYSIZE(levels);
		D3D_FEATURE_LEVEL levelUsed;

		ID3D11Device *dev;
		ID3D11DeviceContext *ctx;

		ThrowIfFailed(D3D11CreateDevice(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			flags,
			levels,
			numLevels,
			D3D11_SDK_VERSION,
			&dev,
			&levelUsed,
			&ctx
		));

		mDevice = make_shared<ID3D11Device*>(dev);
		mContext = make_shared<ID3D11DeviceContext*>(ctx);
	}

	void Renderer::InitializeDXGIFactory() {
		IDXGIFactory1 *factory;
		ThrowIfFailed(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory));
		mFactory = make_shared<IDXGIFactory1*>(factory);

	}


	void Renderer::InitializeDXGISwapChain(HWND &_win, bool _fullscreen, int _fps, int _width, int _height) {
		DXGI_SWAP_CHAIN_DESC scDesc;
		memset(&scDesc, 0, sizeof(scDesc));
		scDesc.BufferCount = 1;
		scDesc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		scDesc.BufferDesc.Width = _width;
		scDesc.BufferDesc.Height = _height;
		scDesc.BufferDesc.RefreshRate.Numerator = 1;
		scDesc.BufferDesc.RefreshRate.Denominator = _fps;
		scDesc.SampleDesc.Count = 1;
		scDesc.Windowed = !_fullscreen;
		scDesc.OutputWindow = _win;
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain *chain;

		ThrowIfFailed((*sInstance->mFactory)->CreateSwapChain((*sInstance->mDevice),
																													&scDesc,
																													&chain));
		sInstance->mChain = make_shared<IDXGISwapChain*>(chain);
	}

	void Renderer::InitializeViews(int _width, int _height) {
		ID3D11Resource *bbuffer;
		ID3D11RenderTargetView *rtv;
		ThrowIfFailed((*mChain)->GetBuffer(0, __uuidof(bbuffer), (void**)(&bbuffer)));
		ThrowIfFailed((*mDevice)->CreateRenderTargetView(bbuffer, NULL, &rtv));
		mMainView = make_shared<ID3D11RenderTargetView*>(rtv);
		mMainViewTexture = make_shared<ID3D11Texture2D*>((ID3D11Texture2D*)bbuffer);

		ID3D11Texture2D *depthTexture;
		ID3D11DepthStencilView *depthView;

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = _width;
		depthStencilDesc.Height = _height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;
		ThrowIfFailed((*mDevice)->CreateTexture2D(&depthStencilDesc, NULL, &depthTexture));
		ThrowIfFailed((*mDevice)->CreateDepthStencilView(depthTexture, NULL, &depthView));
		(*mContext)->OMSetRenderTargets(1, &rtv, depthView);

		mDepthBuffer = make_shared<ID3D11Texture2D*>(depthTexture);
		mDSView = make_shared<ID3D11DepthStencilView*>(depthView);

		// Viewport
		DXGI_SWAP_CHAIN_DESC scd;
		(*mChain)->GetDesc(&scd);
		mViewport.Width = (FLOAT)scd.BufferDesc.Width;
		mViewport.Height = (FLOAT)scd.BufferDesc.Height;
		mViewport.MinDepth = 0;
		mViewport.MaxDepth = 1;
		mViewport.TopLeftX = 0;
		mViewport.TopLeftY = 0;
		(*mContext)->RSSetViewports(1, &mViewport);
	}

	void Renderer::InitializeBuffers() {
		ID3D11Buffer* pBuff;
		CD3D11_BUFFER_DESC desc(sizeof(ViewProjectionBuffer), D3D11_BIND_CONSTANT_BUFFER);
		(*mDevice)->CreateBuffer(&desc, nullptr, &pBuff);
		mVPBuffer = std::make_shared<ID3D11Buffer*>(pBuff);

		desc.ByteWidth = sizeof(matrix4);
		(*mDevice)->CreateBuffer(&desc, nullptr, &pBuff);
		mPositionBuffer = std::make_shared<ID3D11Buffer*>(pBuff);
	}

	void Renderer::InitializeSamplerState() {
		D3D11_SAMPLER_DESC sDesc;
		memset(&sDesc, 0, sizeof(sDesc));
		sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sDesc.MaxAnisotropy = 16;
		sDesc.MinLOD = 0;
		sDesc.MaxLOD = D3D11_FLOAT32_MAX;

		ID3D11SamplerState *ss;
		ThrowIfFailed((*mDevice)->CreateSamplerState(&sDesc, &ss));
		mSamplerState = make_shared<ID3D11SamplerState*>(ss);
		(*mContext)->PSSetSamplers(0, 1, &ss);
	}

	void Renderer::InitializeObjectNames() {
#if _DEBUG
		SetD3DName((*mDevice), "Rendering Device");
		SetD3DName((*mContext), "Device Context");
		SetD3DName((*mChain), "Swapchain");
		SetD3DName((*mFactory), "DXGI Factory");
		SetD3DName((*mMainView), "Window Render Target");
		SetD3DName((*mMainViewTexture), "Window Rener Texture");
		SetD3DName((*mDSView), "Main Depth-Stencil View");
		SetD3DName((*mDepthBuffer), "Main Depth Buffer");
		SetD3DName((*mVPBuffer), "View-Projection Constant Buffer");
		SetD3DName((*mPositionBuffer), "Model Constant Buffer");
#endif
	}

	void Renderer::SetStaticBuffers() {
		(*mContext)->VSSetConstantBuffers(0, 1, mVPBuffer.get());
		(*mContext)->VSSetConstantBuffers(1, 1, mPositionBuffer.get());
		//(*mContext)->VSSetConstantBuffers(2, 1, nullptr); // This will crash. - Instance Buffer
		//(*mContext)->VSSetConstantBuffers(3, 1, nullptr); // This will crash. - Animation Data Buffer

		//(*mContext)->PSSetConstantBuffers(0, 1, nullptr); // This will crash. - Light Buffer
	}
	void Renderer::UpdateCamera(float const _moveSpd, float const _rotSpd, float _delta) {
#if _DEBUG || 1
		if (GetActiveWindow() != *mWindow) {
			return;
		}
		if (!CommandConsole::Instance().willTakeInput()) {
			//w
			if (GetAsyncKeyState('W')) {
				mDebugCameraPos = matrix4::CreateTranslation(0, 0, -_moveSpd * _delta) * mDebugCameraPos;
			}
			//s
			if (GetAsyncKeyState('S')) {
				mDebugCameraPos = matrix4::CreateTranslation(0, 0, _moveSpd * _delta) * mDebugCameraPos;
			}
			//a
			if (GetAsyncKeyState('A')) {
				mDebugCameraPos = matrix4::CreateTranslation(-_moveSpd * _delta, 0, 0) * mDebugCameraPos;
			}
			//d
			if (GetAsyncKeyState('D')) {
				mDebugCameraPos = matrix4::CreateTranslation(_moveSpd * _delta, 0, 0) * mDebugCameraPos;
			}
			// Q
			if (GetAsyncKeyState('Q')) {
				mDebugCameraPos = matrix4::CreateZRotation(_rotSpd * _delta) * mDebugCameraPos;
			}
			// E
			if (GetAsyncKeyState('E')) {
				mDebugCameraPos = matrix4::CreateZRotation(-_rotSpd * _delta) * mDebugCameraPos;
			}
			//x
			if (GetAsyncKeyState(VK_CONTROL)) {
				mDebugCameraPos = matrix4::CreateTranslation(0, -_moveSpd * _delta, 0) * mDebugCameraPos;
			}

			if (GetAsyncKeyState(VK_SPACE)) {
				mDebugCameraPos = matrix4::CreateTranslation(0, _moveSpd * _delta, 0) * mDebugCameraPos;
			}
			if (GetAsyncKeyState(VK_LBUTTON) & 1 && !mIsMouseDown) {
				GetCursorPos(&mMouseOrigin);
				mIsMouseDown = true;
			}
			if (!GetAsyncKeyState(VK_LBUTTON)) {
				mIsMouseDown = false;
			}
		}
		if (mIsMouseDown) {
			POINT now;
			GetCursorPos(&now);
			if (now.x != mMouseOrigin.x || now.y != mMouseOrigin.y) {
				float dx = -(now.x - mMouseOrigin.x) * _rotSpd * _delta;
				float dy = -(now.y - mMouseOrigin.y) * _rotSpd * _delta;

				mDebugCameraPos = matrix4::CreateXRotation(dy) * matrix4::CreateYRotation(dx) * mDebugCameraPos;

				// Reset cursor to center of the window.
				WINDOWINFO winfo;
				winfo.cbSize = sizeof(WINDOWINFO);
				GetWindowInfo(*mWindow, &winfo);
				SetCursorPos((winfo.rcClient.left + winfo.rcClient.right) / 2, (winfo.rcClient.top + winfo.rcClient.bottom) / 2);
				GetCursorPos(&mMouseOrigin);
			}
		}

		mVPData.view = mDebugCameraPos.Transpose().Invert();
		(*mContext)->UpdateSubresource(*mVPBuffer, 0, nullptr, &mVPData, 0, 0);
#endif
	}

	void Renderer::RenderVR(float _delta) {
		(*mContext)->ClearDepthStencilView((*mDSView), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
		vr::VRCompositor()->CompositorBringToFront();
		float color[4] = { 0.251f, 0.709f, 0.541f, 1 };
		for (int i = 0; i < 2; ++i) {
			vr::EVREye currentEye;
			if (i == 0) {
				currentEye = vr::EVREye::Eye_Left;
			} else {
				currentEye = vr::EVREye::Eye_Right;
				(*mContext)->ClearRenderTargetView((*mMainView), color);
				(*mContext)->ClearDepthStencilView((*mDSView), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
			}
			ViewProjectionBuffer data;
			GetMVP(currentEye, data);
			(*mContext)->UpdateSubresource(*mVPBuffer, 0, nullptr, (void*)&data, 0, 0);

			ProcessRenderSet();

			vr::Texture_t submitTexture = { (void*)(*mMainViewTexture), vr::TextureType_DirectX, vr::ColorSpace_Auto };
			vr::VRCompositor()->Submit(currentEye, &submitTexture);
#if ENABLE_TEXT
			CommandConsole::Instance().SetVRBool(true);
			CommandConsole::Instance().Update();
#endif
		}
	}
	void Renderer::RenderNoVR(float _delta) {
		UpdateCamera(2, 2, _delta);

		ProcessRenderSet();
#if ENABLE_TEXT
		CommandConsole::Instance().SetVRBool(false);
		CommandConsole::Instance().Update();
#endif

	}

	void Renderer::ProcessRenderSet() {
		const RenderNode* head = mRenderSet.GetHead();
		while (head != nullptr) {
			if (head->mType == RenderNode::RenderNodeType::Context) {
				((RenderContext*)head)->Apply();
			} else if (head->mType == RenderNode::RenderNodeType::Shape) {
				(*mContext)->UpdateSubresource(*mPositionBuffer, 0, nullptr, &((RenderShape*)head)->mPosition, 0, 0);
				((RenderShape*)head)->Render();
			}
			head = head->GetNext();
		}
	}


#pragma endregion Private Functions

#pragma region Public Functions

	void Renderer::AddNode(RenderShape *_node) {
		mRenderSet.AddNode(_node, &_node->GetContext());
	}
	void Renderer::RemoveNode(RenderShape *_node) {
		mRenderSet.RemoveShape(_node);
	}

	bool Renderer::iInitialize(HWND _Window, unsigned int _width, unsigned int _height, bool _vsync, int _fps, bool _fullscreen, float _farPlane, float _nearPlane, vr::IVRSystem * _vrsys) {
		mWindow = make_shared<HWND>(_Window);
		mVrSystem = _vrsys;

		int rtvWidth = _width;
		int rtvHeight = _height;

		if (mVrSystem) {
			uint32_t texWidth, texHeight;
			mVrSystem->GetRecommendedRenderTargetSize(&texWidth, &texHeight);
			SystemLogger::GetLog() << "According to VR, the view of our headset is " << texWidth << "x" << texHeight << std::endl;
			SystemLogger::GetLog() << "The screen will probably look bad. We're just using one render target view currently, and it gets set to the VR headset's recommended resolution when it's plugged in.\n" <<
				"We should account for that later." << std::endl;
			rtvWidth = (int)texWidth;
			rtvHeight = (int)texHeight;


			mEyePosLeft = GetEye(vr::EVREye::Eye_Left);
			mEyePosRight = GetEye(vr::EVREye::Eye_Right);
			mEyeProjLeft = GetProjection(vr::EVREye::Eye_Left);
			mEyeProjRight = GetProjection(vr::EVREye::Eye_Right);
		}


		InitializeD3DDevice();
		InitializeDXGIFactory();
		InitializeDXGISwapChain(_Window, _fullscreen, _fps, rtvWidth, rtvHeight);
		InitializeViews(rtvWidth, rtvHeight);
		InitializeBuffers();
#if ENABLE_TEXT

#endif
#if _DEBUG
		InitializeObjectNames();
#endif
		InitializeSamplerState();
		SetStaticBuffers();
		// TODO Eventually: Give each shape a topology enum, perhaps?
		(*mContext)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// TODO Eventually: Actually assign input layouts for each render shape.
		InputLayoutManager::Instance().ApplyLayout(eVERT_POSNORMTEX);

		mUseVsync = _vsync;


		if (!mVrSystem) {
			mDebugCameraPos = matrix4::CreateTranslation(1.9f, 0.5f, 8);
			mVPData.projection.matrix = DirectX::XMMatrixPerspectiveFovRH(70, (float)_height / (float)_width, 0.1f, 1000);
			mVPData.view = mDebugCameraPos.Transpose().Invert();
			mVPData.projection = mVPData.projection.Transpose();
			(*mContext)->UpdateSubresource(*mVPBuffer, 0, NULL, &mVPData, 0, 0);
			(*mContext)->VSSetConstantBuffers(0, 1, mVPBuffer.get());
		}

		//// Print out the render model names available from openVR.
		//char buffer[2048];
		//for (int i = 0; i < vr::VRRenderModels()->GetRenderModelCount(); ++i) {
		//	vr::VRRenderModels()->GetRenderModelName((uint32_t)i, buffer, 2048);
		//	SystemLogger::GetLog() << buffer << std::endl;
		//}
		return true;
	}

	void Renderer::Render(float _deltaTime) {

		float color[4] = { 0.251f, 0.709f, 0.541f, 1 };
		(*mContext)->ClearRenderTargetView((*mMainView), color);
		(*mContext)->ClearDepthStencilView((*mDSView), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
		if (nullptr == mVrSystem) {
			RenderNoVR(_deltaTime);
		} else {
			RenderVR(_deltaTime);
		}
		(*mChain)->Present(mUseVsync ? 1 : 0, 0);
	}

#pragma endregion Public Functions

#pragma endregion Instance Functions

}