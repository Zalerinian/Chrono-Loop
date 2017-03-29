
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
#include "../Particles/ParticleSystem.h"
#include "RasterizerStateManager.h"
#include "RenderShaderDefines.hlsli"

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

	void Renderer::UpdateViewProjection() {
		matrix4 hmd = VRInputManager::GetInstance().GetTrackedPositions()[0].mDeviceToAbsoluteTracking;
		matrix4 hmdPos = (hmd * VRInputManager::GetInstance().GetPlayerPosition()).Invert();

		mVPLeftData.view = (hmdPos * mEyePosLeft).Transpose();
		mVPLeftData.projection = mEyeProjLeft.Transpose();
		mVPRightData.view = (hmdPos * mEyePosRight).Transpose();
		mVPRightData.projection = mEyeProjRight.Transpose();
	}

	void Renderer::UpdateGSBuffers() {
		ViewProjectionBuffer buffers[] = { mVPLeftData, mVPRightData };
		mContext->UpdateSubresource(mVPBuffer.Get(), 0, nullptr, buffers, 0, 0);
	}

	void Renderer::UpdateLBuffers()
	{
		Light buffs[] = { (*mLData[0]), (*mLData[1]), (*mLData[2]) };
		mContext->UpdateSubresource(mLBuffer.Get(), 0, nullptr, buffs, 0, 0);

		//mContext->UpdateSubresource(mDLBufferS.Get(), 0, nullptr, &mDLVPB, 0, 0);
		//mContext->UpdateSubresource(mSLBufferS.Get(), 0, nullptr, &mSLVPB, 0, 0);
		mPLVPB.view = matrix4();
		mPLVPB.view.fourth = mLData[0]->Position;
		mPLVPB.view = mPLVPB.view.Invert();
		mPLVPB.projection = DirectX::XMMatrixPerspectiveFovRH(360, (float)1366.0f / (float)720.0f, 0.1f, 1000);
		//6 dir, cube map or parabolic for performance
		mContext->UpdateSubresource(mPLBufferS.Get(), 0, nullptr, &mPLVPB, 0, 0);
	}
	Renderer::Renderer() {}

	Renderer::Renderer::~Renderer() {
		if (mScenePPQuad) {
			delete mScenePPQuad;
		}
		if (mSceneScreenQuad) {
			delete mSceneScreenQuad;
		}
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

		mDevice.Attach(dev);
		mContext.Attach(ctx);
	}

	void Renderer::InitializeDXGIFactory() {
		IDXGIFactory1 *factory;
		ThrowIfFailed(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory));
		mFactory.Attach(factory);

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

		ThrowIfFailed(mFactory->CreateSwapChain(mDevice.Get(),
												&scDesc,
												&chain));
		mChain.Attach(chain);
	}

	void Renderer::InitializeViews(int _width, int _height) {
		ID3D11Resource *bbuffer;
		ID3D11RenderTargetView *rtv;
		ThrowIfFailed(mChain->GetBuffer(0, __uuidof(bbuffer), (void**)(&bbuffer)));
		ThrowIfFailed(mDevice->CreateRenderTargetView(bbuffer, NULL, &rtv));
		mMainView.Attach(rtv);
		mMainViewTexture.Attach((ID3D11Texture2D*)bbuffer);
		D3D11_TEXTURE2D_DESC texdesc;
		mMainViewTexture->GetDesc(&texdesc);
		D3D11_RENDER_TARGET_VIEW_DESC rtvdesc;
		mMainView->GetDesc(&rtvdesc);

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
		ThrowIfFailed(mDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthTexture));
		ThrowIfFailed(mDevice->CreateDepthStencilView(depthTexture, NULL, &depthView));

		mDepthBuffer.Attach(depthTexture);
		mDSView.Attach(depthView);

		ID3D11Texture2D *postTex;
		CD3D11_TEXTURE2D_DESC t2d(DXGI_FORMAT_R16G16B16A16_FLOAT, _width, _height, 1, 0, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
		mDevice->CreateTexture2D(&t2d, nullptr, &postTex);
		mSceneTexture.Attach(postTex);

		// Render target view in order to draw to the texture.
		ID3D11RenderTargetView *sceneRTV;
		HRESULT hr = mDevice->CreateRenderTargetView((ID3D11Resource*)postTex, NULL, &sceneRTV);
		ThrowIfFailed(hr);
		mSceneView.Attach(sceneRTV);
		mContext->OMSetRenderTargets(1, &sceneRTV, depthView);

		// Shader resource view for using the texture to draw the post quad.
		ID3D11ShaderResourceView *sceneSRV;
		ThrowIfFailed(mDevice->CreateShaderResourceView((ID3D11Resource*)postTex, NULL, &sceneSRV));
		mSceneSRV.Attach(sceneSRV);

		// Viewport
		DXGI_SWAP_CHAIN_DESC scd;
		mChain->GetDesc(&scd);
		mLeftViewport.Width = (FLOAT)scd.BufferDesc.Width / 2;
		mLeftViewport.Height = (FLOAT)scd.BufferDesc.Height;
		mLeftViewport.MinDepth = 0;
		mLeftViewport.MaxDepth = 1;
		mLeftViewport.TopLeftX = 0;
		mLeftViewport.TopLeftY = 0;

		mRightViewport = mLeftViewport;
		mRightViewport.TopLeftX = (FLOAT)scd.BufferDesc.Width / 2;

		mFullViewport = mLeftViewport;
		mFullViewport.Width = (FLOAT)scd.BufferDesc.Width;

		D3D11_VIEWPORT viewports[] = { mLeftViewport, mRightViewport, mFullViewport };
		mContext->RSSetViewports(ARRAYSIZE(viewports), viewports);

		//Shadows
		depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		D3D11_DEPTH_STENCIL_VIEW_DESC dvsDesc;
		dvsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dvsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dvsDesc.Texture2D.MipSlice = 0;
		dvsDesc.Flags = 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = depthStencilDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		//Point light
		for (int i = 0; i < 2; i++)
		{
			mDevice->CreateTexture2D(&depthStencilDesc, NULL, mShadowTextures[i].GetAddressOf());
			mDevice->CreateDepthStencilView(mShadowTextures[i].Get(), &dvsDesc, mSDSView[i].GetAddressOf());
			mDevice->CreateShaderResourceView(mShadowTextures[i].Get(), &srvDesc, mShadowSRV[i].GetAddressOf());
		}

	}

	void Renderer::InitializeBuffers() {
		ID3D11Buffer* pBuff;

		// View-Projction buffer
		CD3D11_BUFFER_DESC desc(sizeof(ViewProjectionBuffer) * 2, D3D11_BIND_CONSTANT_BUFFER);
		mDevice->CreateBuffer(&desc, nullptr, &pBuff);
		mVPBuffer.Attach(pBuff);


		// Model position buffer
		desc.ByteWidth = sizeof(matrix4) * 256;
		mDevice->CreateBuffer(&desc, nullptr, &pBuff);
		mPositionBuffer.Attach(pBuff);

		//Light buffers
		desc.ByteWidth = sizeof(Light) * 3;
		mDevice->CreateBuffer(&desc, nullptr, &pBuff);
		mLBuffer.Attach(pBuff);

		desc.ByteWidth = sizeof(ViewProjectionBuffer);
		mDevice->CreateBuffer(&desc, nullptr, &pBuff);
		mPLBufferS.Attach(pBuff);

		desc.ByteWidth = sizeof(float) * 4;
		mDevice->CreateBuffer(&desc, nullptr, &pBuff);
		mPLBSDir.Attach(pBuff);

		for (int i = 0; i < 3; i++)
			mLData[i] = new Light();

		//TODO: GET RID OF THIS
		mLData[1]->Type = 1;
		mLData[1]->Color = vec3f(.5, .5, .5);
		mLData[1]->Direction = vec3f(0, -1, 0);

		mLData[0]->Type = 2;
		mLData[0]->Position = vec3f(5, 1, 2);
		mLData[0]->Color = vec3f(1, 1, 1);

		mLData[2]->Type = 4;
		mLData[2]->Color = vec3f(0, .25, .25);
		mLData[2]->ConeDirection = vec3f(0, -1, 0);
		mLData[2]->Position = vec3f(3, 4, 0);
		mLData[2]->ConeRatio = .5;

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
		ThrowIfFailed(mDevice->CreateSamplerState(&sDesc, &ss));
		mSamplerState.Attach(ss);
		mContext->PSSetSamplers(0, 1, &ss);

		sDesc.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
		sDesc.ComparisonFunc = D3D11_COMPARISON_LESS;

		mDevice->CreateSamplerState(&sDesc, mSSamplerState.GetAddressOf());

		char * buffer;
		int bytes;
		FileIO::LoadBytes("ShadowVS.cso", &buffer, bytes);
		mDevice->CreateVertexShader(buffer, bytes, NULL, mShadowVS.GetAddressOf());
		delete buffer;
	}

	void Renderer::InitializeObjectNames() {
#if _DEBUG
		SetD3DName(mDevice.Get(), "Rendering Device");
		SetD3DName(mContext.Get(), "Device Context");
		SetD3DName(mChain.Get(), "Swapchain");
		SetD3DName(mFactory.Get(), "DXGI Factory");
		SetD3DName(mMainView.Get(), "Window Render Target");
		SetD3DName(mMainViewTexture.Get(), "Window Render Texture");
		SetD3DName(mDSView.Get(), "Main Depth-Stencil View");
		SetD3DName(mDepthBuffer.Get(), "Main Depth Buffer");
		SetD3DName(mVPBuffer.Get(), "View-Projection Constant Buffer");
		SetD3DName(mPositionBuffer.Get(), "Model Constant Buffer");

		SetD3DName(mSceneTexture.Get(), "Post Processing Texture");
		SetD3DName(mSceneSRV.Get(), "Scene Texture SRV");

#endif
	}

	void Renderer::InitializeSceneQuad() {
		mScenePPQuad = new RenderShape("../Resources/VerticalPlane.obj", true, ePS_POSTPROCESS, eVS_NDC, eGS_PosNormTex_NDC);
		mScenePPQuad->GetContext().mTextures[eTEX_DIFFUSE] = mSceneSRV;
		mScenePPQuad->GetContext().mRasterState = eRS_FILLED;

		ID3D11Buffer *ColorRatioBuffer;
		TimeManipulationEffectData initial;
		initial.saturationColor.Set(0.30f, 0.59f, 0.11f, 0);
		initial.saturationColor.Set(0.30f, 0, 0.30f, 0);
		initial.ratios.Set(0, 0);
		initial.fullRatios.Set(0.7f, 0.3f);
		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = &initial;
		CD3D11_BUFFER_DESC bufferDesc(sizeof(TimeManipulationEffectData), D3D11_BIND_CONSTANT_BUFFER);
		mDevice->CreateBuffer(&bufferDesc, &initialData, &ColorRatioBuffer);
		mScenePPQuad->GetContext().mPixelCBuffers[ePB_PP_Ratios].Attach(ColorRatioBuffer);

		mSceneScreenQuad = new RenderShape("../Resources/VerticalPlaneHalfU.obj", true, ePS_PURETEXTURE, eVS_NDC, eGS_PosNormTex_NDC);
		mSceneScreenQuad->GetContext().mTextures[eTEX_DIFFUSE] = mSceneSRV;
	}

	void Renderer::SetStaticBuffers() {
		mContext->GSSetConstantBuffers(0, 1, mVPBuffer.GetAddressOf());
		mContext->VSSetConstantBuffers(0, 1, mPositionBuffer.GetAddressOf());
		mContext->PSSetConstantBuffers(0, 1, mLBuffer.GetAddressOf());
		//(*mContext)->VSSetConstantBuffers(2, 1, nullptr); // This will crash. - Instance Buffer
		//(*mContext)->VSSetConstantBuffers(3, 1, nullptr); // This will crash. - Animation Data Buffer

		//(*mContext)->PSSetConstantBuffers(0, 1, nullptr); // This will crash. - Light Buffer
	}

	void Renderer::InitializeStates()
	{
		ID3D11DepthStencilState *opaqueState, *transparentState;
		D3D11_DEPTH_STENCIL_DESC opaqueDepth, transparentDepth;
		memset(&opaqueDepth, 0, sizeof(opaqueDepth));
		opaqueDepth.DepthEnable = true;
		opaqueDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		opaqueDepth.DepthFunc = D3D11_COMPARISON_LESS;
		transparentDepth = opaqueDepth;
		transparentDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		ThrowIfFailed(mDevice->CreateDepthStencilState(&opaqueDepth, &opaqueState));
		ThrowIfFailed(mDevice->CreateDepthStencilState(&transparentDepth, &transparentState));
		mOpaqueState.Attach(opaqueState);
		mTransparentState.Attach(transparentState);


		ID3D11BlendState *opaqueBS, *transparentBS;
		D3D11_BLEND_DESC opaqueBlend, transparentBlend;
		transparentBlend.IndependentBlendEnable = FALSE;
		transparentBlend.AlphaToCoverageEnable = FALSE;
		transparentBlend.RenderTarget[0].BlendEnable = TRUE;
		transparentBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		transparentBlend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		transparentBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		transparentBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		transparentBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		transparentBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		transparentBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		
		opaqueBlend = transparentBlend;
		opaqueBlend.RenderTarget[0].BlendEnable = FALSE;
		ThrowIfFailed(mDevice->CreateBlendState(&opaqueBlend, &opaqueBS));
		ThrowIfFailed(mDevice->CreateBlendState(&transparentBlend, &transparentBS));
		mOpaqueBlendState.Attach(opaqueBS);
		mTransparentBlendState.Attach(transparentBS);
	}
	
	void Renderer::UpdateCamera(float const _moveSpd, float const _rotSpd, float _delta) {
#if _DEBUG || 1
		if (GetActiveWindow() != mWindow) {
			return;
		}
		if (!CommandConsole::Instance().willTakeInput()) {
			//w
			if (GetAsyncKeyState('W')) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateTranslation(0, 0, -_moveSpd * _delta) * VRInputManager::GetInstance().GetPlayerPosition();
			}
			//s
			if (GetAsyncKeyState('S')) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateTranslation(0, 0, _moveSpd * _delta) * VRInputManager::GetInstance().GetPlayerPosition();
			}
			//a
			if (GetAsyncKeyState('A')) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateTranslation(-_moveSpd * _delta, 0, 0) * VRInputManager::GetInstance().GetPlayerPosition();
			}
			//d
			if (GetAsyncKeyState('D')) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateTranslation(_moveSpd * _delta, 0, 0) * VRInputManager::GetInstance().GetPlayerPosition();
			}
			// Q
			if (GetAsyncKeyState('Q')) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateZRotation(_rotSpd * _delta) * VRInputManager::GetInstance().GetPlayerPosition();
			}
			// E
			if (GetAsyncKeyState('E')) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateZRotation(-_rotSpd * _delta) * VRInputManager::GetInstance().GetPlayerPosition();
			}
			//x
			if (GetAsyncKeyState(VK_CONTROL)) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateTranslation(0, -_moveSpd * _delta, 0) * VRInputManager::GetInstance().GetPlayerPosition();
			}

			if (GetAsyncKeyState(VK_SPACE)) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateTranslation(0, _moveSpd * _delta, 0) * VRInputManager::GetInstance().GetPlayerPosition();
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

				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateXRotation(dy) * matrix4::CreateYRotation(dx) * VRInputManager::GetInstance().GetPlayerPosition();

				// Reset cursor to center of the window.
				WINDOWINFO winfo;
				winfo.cbSize = sizeof(WINDOWINFO);
				GetWindowInfo(mWindow, &winfo);
				SetCursorPos((winfo.rcClient.left + winfo.rcClient.right) / 2, (winfo.rcClient.top + winfo.rcClient.bottom) / 2);
				GetCursorPos(&mMouseOrigin);
			}
		}

		mVPLeftData.view = VRInputManager::GetInstance().GetPlayerPosition().Transpose().Invert();
		mVPRightData = mVPLeftData;
		UpdateGSBuffers();
		//UpdateLBuffers();
#endif
	}

	void Renderer::RenderShadowMaps(float _delta)
	{
		//TODO: Rasterstate set back face culling to NO
		RasterizerStateManager::Instance()->ApplyState(RasterState::eRS_NO_CULL);
		//Directional TODO: IGNORE
		//TODO: TO-Drew fix buffer thing
		mContext->OMSetRenderTargets(0, 0, mSDSView[0].Get());
		mContext->ClearDepthStencilView(mSDSView[0].Get(), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
		//Set VS & PS & GS and light buffer
		//TODO: Need stages, this shit gets over-written
		mContext->GSSetShader(NULL, NULL, NULL);
		mContext->VSSetShader(mShadowVS.Get(), 0, 0);
		mContext->PSSetShader(mPSST.Get(), 0, 0);

		mContext->VSSetConstantBuffers(1, 1, mPLBufferS.GetAddressOf());
		mContext->VSSetConstantBuffers(2, 1, mPLBSDir.GetAddressOf());

		ProcessRenderSet();

		mContext->VSSetShader(mShadowVS2.Get(), 0, 0);

		mContext->OMSetRenderTargets(0, 0, mSDSView[1].Get());
		mContext->ClearDepthStencilView(mSDSView[1].Get(), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);

		ProcessRenderSet();

		RasterizerStateManager::Instance()->ApplyState(RasterState::eRS_FILLED);
		SetStaticBuffers();
		mContext->OMSetRenderTargets(1, mMainView.GetAddressOf(), mDSView.Get());
	}

	void Renderer::RenderVR(float _delta) {
		vr::VRCompositor()->CompositorBringToFront();
		UpdateViewProjection();
		UpdateGSBuffers();
		UpdateLBuffers();
		ProcessRenderSet();
		ParticleSystem::Instance()->Render();

		RenderScreenQuad();


		vr::Texture_t submitTexture = { (void*)mMainViewTexture.Get(), vr::TextureType_DirectX, vr::ColorSpace_Auto };
		vr::VRTextureBounds_t boundary;
		boundary.uMin = 0.0f;
		boundary.uMax = 0.5f;
		boundary.vMin = 0.0f;
		boundary.vMax = 1.0f;

		vr::VRCompositor()->Submit(vr::EVREye::Eye_Left, &submitTexture, &boundary);

		boundary.uMin = 0.5f;
		boundary.uMax = 1.0;
		vr::VRCompositor()->Submit(vr::EVREye::Eye_Right, &submitTexture, &boundary);

#if ENABLE_TEXT
		CommandConsole::Instance().SetVRBool(true);
		CommandConsole::Instance().Update();
#endif
	}


	void Renderer::RenderNoVR(float _delta) {
		UpdateCamera(2, 2, _delta);
		UpdateLBuffers();
		ProcessRenderSet();
		ParticleSystem::Instance()->Render();
		RenderScreenQuad();

		CommandConsole::Instance().SetVRBool(false);
		CommandConsole::Instance().Update();


	}

	void Renderer::ProcessRenderSet() {
		//const RenderNode* head = mRenderSet.GetHead();
		//while (head != nullptr) {
		//	if (head->mType == RenderNode::RenderNodeType::Context) {
		//		((RenderContext*)head)->Apply();
		//	} else if (head->mType == RenderNode::RenderNodeType::Shape) {
		//		mContext->UpdateSubresource(mPositionBuffer.Get(), 0, nullptr, &((RenderShape*)head)->mPosition, 0, 0);
		//		((RenderShape*)head)->Render(1);
		//	}
		//	head = head->GetNext();
		//}

		// TODO: Make a ShaderLimits.h file that has macros for the number of instances and other such arrays a shader supports.

		// Big TODO: When rendering more than 256 instances of a given mesh, there is a pretty good chance the game
		// will crash due to copying garbage memory into the graphics driver for the buffer's contents. Fix that.
		std::vector<matrix4> positions;
		positions.reserve(256);

		// Go through opaque objects first

		mContext->OMSetDepthStencilState(mOpaqueState.Get(), 1);
		mContext->OMSetBlendState(mOpaqueBlendState.Get(), NULL, 0xFFFFFFFF);
		for (auto it = mOpaqueSet.Begin(); it != mOpaqueSet.End(); ++it) {
			(*it)->mPositions.GetData(positions);
			if (positions.size() > 0) {
				unsigned int offset = 0;
#if ENABLE_INSTANCING
				while (positions.size() - offset <= positions.size()) {
					(*it)->mShape.GetContext().Apply();
					mContext->UpdateSubresource(mPositionBuffer.Get(), 0, nullptr, positions.data() + offset, 0, 0);
					(*it)->mShape.Render((UINT)positions.size() - offset);
					offset += 256;
				}
#else
				(*it)->mShape.GetContext().Apply();
				for (unsigned int i = 0; i < positions.size(); ++i) {
					mContext->UpdateSubresource(mPositionBuffer.Get(), 0, nullptr, &positions[i] + offset, 0, 0);
					(*it)->mShape.Render(1); // Without instancing, the instance count doesn't matter, but we're only drawing one :)
				}
#endif
			}
		}


		mContext->OMSetDepthStencilState(mTransparentState.Get(), 1);
		mContext->OMSetBlendState(mTransparentBlendState.Get(), NULL, 0xFFFFFFFF);
		for (auto it = mTransparentSet.Begin(); it != mTransparentSet.End(); ++it) {
			(*it)->mPositions.GetData(positions);
			if (positions.size() > 0) {
				unsigned int offset = 0;
#if ENABLE_INSTANCING
				while (positions.size() - offset <= positions.size()) {
					(*it)->mShape.GetContext().Apply();
					mContext->UpdateSubresource(mPositionBuffer.Get(), 0, nullptr, positions.data() + offset, 0, 0);
					(*it)->mShape.Render((UINT)positions.size() - offset);
					offset += 256;
				}
#else
				(*it)->mShape.GetContext().Apply();
				for (unsigned int i = 0; i < positions.size(); ++i) {
					mContext->UpdateSubresource(mPositionBuffer.Get(), 0, nullptr, &positions[i] + offset, 0, 0);
					(*it)->mShape.Render(1); // Without instancing, the instance count doesn't matter, but we're only drawing one :)
				}
#endif
			}
		}
	}

	void Renderer::RenderScreenQuad()
	{
		mContext->OMSetBlendState(mOpaqueBlendState.Get(), NULL, 0xFFFFFFFF);
		mContext->OMSetRenderTargets(1, mMainView.GetAddressOf(), mDSView.Get());
		mScenePPQuad->GetContext().Apply();
		mScenePPQuad->Render();
	}

	 
#pragma endregion Private Functions

#pragma region Public Functions

	GhostList<matrix4>::GhostNode* Renderer::AddOpaqueNode(RenderShape *_node) {
		return mOpaqueSet.AddShape(*_node);
	}

	GhostList<matrix4>::GhostNode * Renderer::AddTransparentNode(RenderShape * _node)
	{
		return mTransparentSet.AddShape(*_node);
	}

	bool Renderer::iInitialize(HWND _Window, unsigned int _width, unsigned int _height, bool _vsync, int _fps, bool _fullscreen, float _farPlane, float _nearPlane, vr::IVRSystem * _vrsys) {
		mWindow = _Window;
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

		// Account for the second eye
		rtvWidth *= 2;

		InitializeD3DDevice();
		InitializeDXGIFactory();
		InitializeDXGISwapChain(_Window, _fullscreen, _fps, rtvWidth, rtvHeight);
		InitializeViews(rtvWidth, rtvHeight);
		InitializeBuffers();
		InitializeSceneQuad();
		InitializeStates();

#if _DEBUG
		InitializeObjectNames();
#endif
		InitializeSamplerState();
		SetStaticBuffers();
		// TODO Eventually: Give each shape a topology enum, perhaps?
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// TODO Eventually: Actually assign input layouts for each render shape.
		InputLayoutManager::Instance().ApplyLayout(eVERT_POSNORMTEX);

		mUseVsync = _vsync;


		if (!mVrSystem) {
			mVPLeftData.projection.matrix = DirectX::XMMatrixPerspectiveFovRH(70, (float)_height / (float)_width, 0.1f, 1000);
			mVPLeftData.view = VRInputManager::GetInstance().GetPlayerPosition().Transpose().Invert();
			mVPLeftData.projection = mVPLeftData.projection.Transpose();
			mVPRightData = mVPLeftData;
		}

		UpdateGSBuffers();
		return true;
	}

	void Renderer::ClearRenderSet()
	{
		mOpaqueSet.ClearSet();
		mTransparentSet.ClearSet();
	}


	void Renderer::Render(float _deltaTime) {

		float color[4] = { 0.251f, 0.709f, 0.541f, 1 };

		// Setup the Scene Render Target 
		mRendererLock.lock();
		//RenderShadowMaps(_deltaTime);
		mContext->OMSetRenderTargets(1, mSceneView.GetAddressOf(), mDSView.Get());
		mContext->ClearRenderTargetView(mSceneView.Get(), color);
		mContext->ClearDepthStencilView(mDSView.Get(), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
		mContext->PSSetConstantBuffers(0, 1, mLBuffer.GetAddressOf());
		mContext->PSSetShaderResources(3, 1, mShadowSRV[0].GetAddressOf());
		mContext->PSSetShaderResources(4, 1, mShadowSRV[1].GetAddressOf());
		mContext->PSSetSamplers(3, 1, mSSamplerState.GetAddressOf());
		mContext->VSSetConstantBuffers(1, 1, mPLBufferS.GetAddressOf());

		if (nullptr == mVrSystem) {
			RenderNoVR(_deltaTime);
		}
		else {
			// In VR, we need to apply the post processing before we submit textures to the compositor.
			RenderVR(_deltaTime);
		}

		//mContext->ClearDepthStencilView(mDSView.Get(), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
		//// Regardless of VR or not, we need to render the Screen quad so we only see one eye on screen.
		//mSceneScreenQuad->GetContext().Apply();
		//mSceneScreenQuad->Render();

		// Remove the Scene Shader Resource View from the input pipeline, as once the next render
		// call happens, it will be bound to the output pipeline, as wel as the input pipeline,
		// which will result in DirectX not setting it as an output.
		ID3D11ShaderResourceView *nullSRV = nullptr;
		mContext->PSSetShaderResources(eTEX_DIFFUSE, 1, &nullSRV);


		mChain->Present(mUseVsync ? 1 : 0, 0);
		mRendererLock.unlock();
	}

#pragma endregion Public Functions

#pragma endregion Instance Functions

}