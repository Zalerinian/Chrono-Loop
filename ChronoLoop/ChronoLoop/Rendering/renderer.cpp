
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
#include "ShaderManager.h"
#include "../Common/Settings.h"
#include "../Core/LevelManager.h"
#include <regex>

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
		//mContext->UpdateSubresource(mVPBuffer.Get(), 0, nullptr, buffers, 0, 0);

		D3D11_MAPPED_SUBRESOURCE map;
		memset(&map, 0, sizeof(map));
		HRESULT MHR = mContext->Map(mVPBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		memcpy(map.pData, buffers, sizeof(ViewProjectionBuffer) * 2);
		mContext->Unmap(mVPBuffer.Get(), 0);

		memset(&map, 0, sizeof(map));
		MHR = mContext->Map(mHeadPosBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		vec4f& PlayerHeadPos = VRInputManager::GetInstance().GetPlayerView().Position;
		memcpy(map.pData, &PlayerHeadPos, sizeof(vec4f));
		mContext->Unmap(mHeadPosBuffer.Get(), 0);
	}

	void Renderer::UpdateLBuffers() {

	}
	Renderer::Renderer() {
		
	}

	Renderer::Renderer::~Renderer() {
		if (mScenePPQuad) {
			delete mScenePPQuad;
		}
		if (mSceneScreenQuad) {
			delete mSceneScreenQuad;
		}
		if (mDeferredCombiner) {
			delete mDeferredCombiner;
		}
	}

	void Renderer::ProcessGSET(void * _console, std::wstring _arguments) {
		size_t argIndex = _arguments.find(L" ");
		if (argIndex == std::wstring::npos) {
			((CommandConsole*)_console)->DisplaySet(L"The GSET command requires arguments.");
		}
		wstring subcommand = _arguments.substr(0, argIndex);
		wstring args = _arguments.substr(argIndex + 1);
		if (subcommand == L"TOGGLE") {
			if (args == L"GLOW") {
				Instance()->mEnabledFeatures[eRendererFeature_Glow].flip();
			} else if (args == L"SUPERGLOW") {
				Instance()->mEnabledFeatures[eRendererFeature_SuperGlow].flip();
			} else if (args == L"BLOOM") {
				Instance()->mEnabledFeatures[eRendererFeature_Bloom].flip();
			}
		} else if (subcommand == L"SET") {
			wregex valueFinder(L"(\\w)+\\s+(\\d+(\\.\\d+)?)", regex::ECMAScript | regex::icase);
			match_results<const wchar_t*> finds;
			regex_match(args.c_str(), finds, valueFinder);
			if (finds.size() > 2) {
				// A float value has 3 matches: the property name, the whole float value, and the decimal portion (with the '.' preceeding)
				if (finds[1] == L"GLOWSIGMA") {
					float glowSigma = stof(finds[2]);
				}
			}
		}
	}

	void Renderer::ProcessGQUERY(void *_console, std::wstring _arguments) {
		unsigned int frameCount = 1;
		if (_arguments.size() > 0) {
			frameCount = std::wcstoul(_arguments.c_str(), nullptr, 10);
		}
		if (frameCount < 1) {
			SystemLogger::Error() << "Could not find valid number in gQuery arguments. Not preparing the frame set." << std::endl;
			return;
		}
		Instance()->InitializeQueries(frameCount);
	}

	void Renderer::RenderBlurStage(BlurStage _s, float _dx, float _dy) {
		mBlurData.stage = _s;
		mBlurData.dx = _dx;
		mBlurData.dy = _dy;
		mContext->UpdateSubresource(mBlurStageBuffer.Get(), 0, nullptr, &mBlurData, 0, 0);
		mScenePPQuad->Render(1);
	}

	void Renderer::ToggleBlurTextureSet(unsigned int _texturesPerSet, ID3D11RenderTargetView ** _rtvs, ID3D11ShaderResourceView ** _srvs) {
		ID3D11RenderTargetView *noRTV = nullptr;
		mContext->OMSetRenderTargets(1, &noRTV, nullptr); // Clear render targets so they're no longer bound to the pipeline.
		ID3D11ShaderResourceView **noSRV = new ID3D11ShaderResourceView*[_texturesPerSet];
		memset(noSRV, 0, sizeof(int*) * _texturesPerSet);
		mContext->PSSetShaderResources(0, _texturesPerSet, noSRV); // Unbind shader resource views from the pipeline.

		if (mBlurTextureSet == BlurTextureSet_Ping) {
			mBlurTextureSet = BlurTextureSet_Pong;
			mContext->OMSetRenderTargets(_texturesPerSet, _rtvs + _texturesPerSet, nullptr); // Render to the Pong set
			mContext->PSSetShaderResources(0, _texturesPerSet, _srvs); // Sample from the Ping set
		} else {
			mBlurTextureSet = BlurTextureSet_Ping;
			mContext->OMSetRenderTargets(_texturesPerSet, _rtvs, nullptr); // Render to the Ping set
			mContext->PSSetShaderResources(0, _texturesPerSet, _srvs + _texturesPerSet); // Sameple from the Pong set
		}
		delete[] noSRV;
	}

	void Renderer::SetBlurTexturesDrawback(unsigned int _texturesPerSet, ID3D11RenderTargetView ** _drawbacks, ID3D11ShaderResourceView ** _srvs) {
		ID3D11RenderTargetView *noRTV = nullptr;
		mContext->OMSetRenderTargets(1, &noRTV, nullptr); // Clear render targets so they're no longer bound to the pipeline.
		ID3D11ShaderResourceView **noSRV = new ID3D11ShaderResourceView*[_texturesPerSet];
		memset(noSRV, 0, sizeof(int*) * _texturesPerSet);
		mContext->PSSetShaderResources(0, _texturesPerSet, noSRV); // Unbind shader resource views from the pipeline.
		mContext->OMSetRenderTargets(_texturesPerSet, _drawbacks, nullptr); // Render to the RTVs created for the textures we're blurring.

		if (mBlurTextureSet == BlurTextureSet_Ping) {
			mBlurTextureSet = BlurTextureSet_Pong;
			mContext->PSSetShaderResources(0, _texturesPerSet, _srvs); // Sample from the Ping set
		} else {
			mBlurTextureSet = BlurTextureSet_Ping;
			mContext->PSSetShaderResources(0, _texturesPerSet, _srvs + _texturesPerSet); // Sameple from the Pong set
		}
		delete[] noSRV;
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
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
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

		CD3D11_TEXTURE2D_DESC t2d(DXGI_FORMAT_R16G16B16A16_FLOAT, _width, _height, 1, 1, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
		ThrowIfFailed(mDevice->CreateTexture2D(&t2d, nullptr, mAlbedoTexture.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateTexture2D(&t2d, nullptr, mPositionTexture.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateTexture2D(&t2d, nullptr, mNormalTexture.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateTexture2D(&t2d, nullptr, mSpecularTexture.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateTexture2D(&t2d, nullptr, mBloomTexture.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateTexture2D(&t2d, nullptr, mGlowTexture.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateTexture2D(&t2d, nullptr, mPostProcessTexture.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateTexture2D(&t2d, nullptr, mSuperGlowTexture.GetAddressOf()));

		// Render target view in order to draw to the texture.
		HRESULT hr = mDevice->CreateRenderTargetView(mPostProcessTexture.Get(), nullptr, mPostProcessRTV.GetAddressOf());
		ThrowIfFailed(mDevice->CreateRenderTargetView(mBloomTexture.Get(), nullptr, mBloomRTV.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateRenderTargetView(mGlowTexture.Get(), nullptr, mGlowRTV.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateRenderTargetView(mSuperGlowTexture.Get(), nullptr, mSuperGlowRTV.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateRenderTargetView(mAlbedoTexture.Get(), nullptr, mAlbedoRTV.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateRenderTargetView(mPositionTexture.Get(), nullptr, mPositionRTV.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateRenderTargetView(mNormalTexture.Get(), nullptr, mNormalRTV.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateRenderTargetView(mSpecularTexture.Get(), nullptr, mSpecularRTV.GetAddressOf()));


		// Shader resource view for using the texture to draw the post quad.
		ThrowIfFailed(mDevice->CreateShaderResourceView(mPostProcessTexture.Get(), NULL, mPostProcessSRV.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateShaderResourceView(mBloomTexture.Get(), NULL, mBloomSRV.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateShaderResourceView(mGlowTexture.Get(), NULL, mGlowSRV.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateShaderResourceView(mSuperGlowTexture.Get(), NULL, mSuperGlowSRV.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateShaderResourceView(mAlbedoTexture.Get(), NULL, mAlbedoSRV.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateShaderResourceView(mPositionTexture.Get(), NULL, mPositionSRV.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateShaderResourceView(mNormalTexture.Get(), NULL, mNormalSRV.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateShaderResourceView(mSpecularTexture.Get(), NULL, mSpecularSRV.GetAddressOf()));

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

		AttachPrimaryViewports();
		AttachPrimaryRTVs();
	}

	void Renderer::InitializeBuffers() {
		ID3D11Buffer* pBuff;
		D3D11_SUBRESOURCE_DATA InitialData;

		HRESULT buffRes = 0;

		// View-Projction buffer
		CD3D11_BUFFER_DESC desc(sizeof(ViewProjectionBuffer) * 2, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		buffRes = mDevice->CreateBuffer(&desc, nullptr, &pBuff);
		mVPBuffer.Attach(pBuff);

		desc.ByteWidth = sizeof(vec4f);
		mDevice->CreateBuffer(&desc, nullptr, mHeadPosBuffer.GetAddressOf());

		// Global Matrix Buffer
		desc.ByteWidth = sizeof(matrix4);
		matrix4 identity;
		InitialData.pSysMem = &identity;
		ThrowIfFailed(mDevice->CreateBuffer(&desc, &InitialData, mGlobalMatrixBuffer.GetAddressOf()));

		// Solid color outline buffer
		desc.ByteWidth = sizeof(vec4f);
		ThrowIfFailed(mDevice->CreateBuffer(&desc, nullptr, mOutlineColorBuffer.GetAddressOf()));

		// Model position buffer
#if ENABLE_INSTANCING
		desc.ByteWidth = sizeof(matrix4) * 256;
#else
		desc.ByteWidth = sizeof(matrix4);
#endif
		buffRes = mDevice->CreateBuffer(&desc, nullptr, &pBuff);
		mPositionBuffer.Attach(pBuff);

		// Simulated Instance ID Buffer
		// This buffer is used for when Instancing is disabled for pixel debugging.
		// It is necessary because pixel shaders rely on the instance ID to access
		// instance-specific buffer data to draw it correctly. The simulated instance
		// ID stored in this buffer will let them access the buffer correctly.
		vec4i initialSimID(0, 0, 0, 0);
		InitialData.pSysMem = &initialSimID;
		desc.ByteWidth = sizeof(vec4i);
		buffRes = mDevice->CreateBuffer(&desc, &InitialData, mSimInstanceBuffer.GetAddressOf());

		//Light buffers
		desc.ByteWidth = sizeof(Light) * 3;
		buffRes = mDevice->CreateBuffer(&desc, nullptr, &pBuff);
		mLBuffer.Attach(pBuff);


		// Blur data buffer
		CD3D11_BUFFER_DESC blurBuffer(sizeof(BlurData), D3D11_BIND_CONSTANT_BUFFER);
		mDevice->CreateBuffer(&blurBuffer, nullptr, mBlurStageBuffer.GetAddressOf());
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
		sDesc.BorderColor[0] = 1;
		sDesc.BorderColor[1] = 1;
		sDesc.BorderColor[2] = 1;
		sDesc.BorderColor[3] = 1;

		ThrowIfFailed(mDevice->CreateSamplerState(&sDesc, mSamplerState.GetAddressOf()));
		mContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());
	}

	void Renderer::InitializeObjectNames() {
#if _DEBUG
		SetD3DName(mDevice.Get(), "Rendering Device");
		SetD3DName(mContext.Get(), "Device Context");
		SetD3DName(mChain.Get(), "Swapchain");
		SetD3DName(mFactory.Get(), "DXGI Factory");
		SetD3DName(mMainView.Get(), "Window Render Target");
		SetD3DName(mPostProcessRTV.Get(), "Post Processing Render Target");
		SetD3DName(mBloomRTV.Get(), "Bloom Render Target");
		SetD3DName(mDSView.Get(), "Main Depth-Stencil View");
		SetD3DName(mMainViewTexture.Get(), "Window Render Texture");
		SetD3DName(mDepthBuffer.Get(), "Main Depth Buffer");
		SetD3DName(mPostProcessTexture.Get(), "Post Processing Texture");
		SetD3DName(mBloomTexture.Get(), "Bloom Blur Texture");
		SetD3DName(mSamplerState.Get(), "Wrapping Sampler State");
		SetD3DName(mTransparentState.Get(), "Transparent Depth-Stencil State");
		SetD3DName(mOpaqueState.Get(), "Opaque Depth-Stencil State");
		SetD3DName(mPostProcessSRV.Get(), "Scene Texture SRV");
		SetD3DName(mBloomSRV.Get(), "Bloom Shader Resource View");
		SetD3DName(mOpaqueBlendState.Get(), "Opaque Blend State");
		SetD3DName(mTransparentBlendState.Get(), "Transparent Blend State");
		SetD3DName(mAdditiveBlendState.Get(), "Additive Blending State");

		SetD3DName(mVPBuffer.Get(), "View-Projection Constant Buffer");
		SetD3DName(mPositionBuffer.Get(), "Model Constant Buffer");
		SetD3DName(mSimInstanceBuffer.Get(), "Simulated Instance ID Constant Buffer");
		SetD3DName(mLBuffer.Get(), "Light Data Buffer");
		SetD3DName(mBlurStageBuffer.Get(), "Blur Data Buffer");

		SetD3DName(mGlowTexture.Get(), "Glow Texture");
		SetD3DName(mGlowRTV.Get(), "Glow RTV");
		SetD3DName(mGlowSRV.Get(), "Glow SRV");
		SetD3DName(mSuperGlowTexture.Get(), "SuperGlow Texture");
		SetD3DName(mSuperGlowSRV.Get(), "SuperGlow RTV");
		SetD3DName(mSuperGlowRTV.Get(), "SuperGlow SRV");

		// The G-Buffer
		SetD3DName(mAlbedoTexture.Get(), "GBuffer Albedo Texture");
		SetD3DName(mPositionTexture.Get(), "GBuffer Position Texture");
		SetD3DName(mNormalTexture.Get(), "GBuffer Normal Texture");
		SetD3DName(mSpecularTexture.Get(), "GBuffer Specular");
		SetD3DName(mAlbedoSRV.Get(), "Albedo SRV");
		SetD3DName(mPositionSRV.Get(), "Position SRV");
		SetD3DName(mNormalSRV.Get(), "Normal SRV");
		SetD3DName(mSpecularSRV.Get(), "Specular SRV");
		SetD3DName(mAlbedoRTV.Get(), "Albedo RTV");
		SetD3DName(mPositionRTV.Get(), "Position RTV");
		SetD3DName(mNormalRTV.Get(), "Normal RTV");
		SetD3DName(mSpecularRTV.Get(), "Specular RTV");


		SetD3DName(mLSStencilFront.Get(), "LightShadow-Stencil Front");
		SetD3DName(mLSStencilBack.Get(), "LightShadow-Stencil Back");
		SetD3DName(mLSStencilRender.Get(), "LightShadow-Stencil Render");
		SetD3DName(mMotionStateFindObject.Get(), "Motion State Find");
		SetD3DName(mMotionStateReverseDepth.Get(), "Motion State Reverse");

		//SetD3DName(.Get(), "");
#endif
	}

	void Renderer::InitializeSceneQuad() {
		mScenePPQuad = new RenderShape("../Resources/VerticalPlane.obj", true, ePS_POSTPROCESS, eVS_NDC, eGS_PosNormTex_NDC);
		mScenePPQuad->GetContext().mTextures[eTEX_DIFFUSE] = mPostProcessSRV;
		mScenePPQuad->GetContext().mTextures[eTEX_REGISTER4] = mBloomSRV;
		mScenePPQuad->GetContext().mTextures[eTEX_REGISTER5] = mGlowSRV;
		mScenePPQuad->GetContext().mTextures[eTEX_REGISTER6] = mSuperGlowSRV;
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
		mScenePPQuad->GetContext().mPixelCBuffers[ePB_REGISTER1].Attach(ColorRatioBuffer);

		mSceneScreenQuad = new RenderShape("../Resources/VerticalPlaneHalfU.obj", true, ePS_PURETEXTURE, eVS_NDC, eGS_PosNormTex_NDC);
		mSceneScreenQuad->GetContext().mTextures[eTEX_DIFFUSE] = mPostProcessSRV;
		mSceneScreenQuad->mContext.mRasterState = eRS_FILLED;

		mDeferredCombiner = new RenderShape("../Resources/VerticalPlane.obj", true, ePS_DEFERRED, eVS_NDC, eGS_PosNormTex_NDC);
		mDeferredCombiner->mContext.mTextures[0] = mAlbedoSRV;
		mDeferredCombiner->mContext.mTextures[1] = mPositionSRV;
		mDeferredCombiner->mContext.mTextures[2] = mNormalSRV;
		mDeferredCombiner->mContext.mTextures[3] = mSpecularSRV;
		mDeferredCombiner->mContext.mPixelCBuffers[0] = mLBuffer;
		mDeferredCombiner->mContext.mRasterState = eRS_FILLED;
	}

	void Renderer::SetStaticBuffers() {
		mContext->VSSetConstantBuffers(0, 1, mPositionBuffer.GetAddressOf());
		mContext->VSSetConstantBuffers(1, 1, mSimInstanceBuffer.GetAddressOf());
		mContext->VSSetConstantBuffers(2, 1, mGlobalMatrixBuffer.GetAddressOf());
		mContext->GSSetConstantBuffers(0, 1, mVPBuffer.GetAddressOf());
		mContext->PSSetConstantBuffers(0, 1, mHeadPosBuffer.GetAddressOf());
	}

	void Renderer::InitializeStates() {
		ID3D11DepthStencilState *opaqueState, *transparentState;
		D3D11_DEPTH_STENCIL_DESC opaqueDepth, transparentDepth, topmostDepth, motionFind, motionReverse, lsFrontStencil, lsBackStencil, lsRenderStencil;
		memset(&opaqueDepth, 0, sizeof(opaqueDepth));
		opaqueDepth.DepthEnable = TRUE;
		opaqueDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		opaqueDepth.DepthFunc = D3D11_COMPARISON_LESS;

		lsFrontStencil = opaqueDepth;
		transparentDepth = opaqueDepth;
		transparentDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		topmostDepth = transparentDepth;
		topmostDepth.DepthEnable = false;
		ThrowIfFailed(mDevice->CreateDepthStencilState(&topmostDepth, mTopmostState.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateDepthStencilState(&opaqueDepth, &opaqueState));
		ThrowIfFailed(mDevice->CreateDepthStencilState(&transparentDepth, &transparentState));
		mOpaqueState.Attach(opaqueState);
		mTransparentState.Attach(transparentState);

		motionFind = opaqueDepth;
		motionFind.StencilEnable = TRUE;
		motionFind.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		motionFind.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		motionFind.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
		motionFind.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		motionFind.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		motionFind.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		motionFind.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		motionFind.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		motionFind.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		motionFind.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		motionFind.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

		motionReverse = motionFind;
		motionReverse.DepthFunc = D3D11_COMPARISON_GREATER;
		motionReverse.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER;
		//motionReverse.BackFace.StencilFunc = D3D11_COMPARISON_LESS;
		motionReverse.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		motionReverse.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		motionReverse.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

		ThrowIfFailed(mDevice->CreateDepthStencilState(&motionFind, mMotionStateFindObject.GetAddressOf()));
		ThrowIfFailed(mDevice->CreateDepthStencilState(&motionReverse, mMotionStateReverseDepth.GetAddressOf()));

		lsFrontStencil.DepthEnable = TRUE;
		lsFrontStencil.StencilEnable = TRUE;
		lsFrontStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		lsFrontStencil.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		lsFrontStencil.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		lsFrontStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		lsFrontStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		lsFrontStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		lsFrontStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		lsFrontStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		lsFrontStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		lsFrontStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		lsFrontStencil.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		ThrowIfFailed(mDevice->CreateDepthStencilState(&lsFrontStencil, mLSStencilFront.GetAddressOf()));

		lsBackStencil = lsFrontStencil;
		lsBackStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;

		ThrowIfFailed(mDevice->CreateDepthStencilState(&lsBackStencil, mLSStencilBack.GetAddressOf()));

		lsRenderStencil = lsFrontStencil;
		lsRenderStencil.DepthEnable = FALSE;
		lsRenderStencil.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		lsRenderStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		lsRenderStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		lsRenderStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

		ThrowIfFailed(mDevice->CreateDepthStencilState(&lsRenderStencil, mLSStencilRender.GetAddressOf()));



		ID3D11BlendState *opaqueBS, *transparentBS;
		D3D11_BLEND_DESC opaqueBlend, transparentBlend, additiveBlend;
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

		additiveBlend = transparentBlend;
		additiveBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		additiveBlend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		additiveBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		additiveBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		additiveBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		additiveBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		ThrowIfFailed(mDevice->CreateBlendState(&additiveBlend, mAdditiveBlendState.GetAddressOf()));
	}

	void Renderer::InitializeQueries(unsigned int _queryCount) {
		if (_queryCount < 1) {
			return;
		}
		SystemLogger::Debug() << "Capturing performance of " << _queryCount << " frame" << (_queryCount > 1 ? "s" : "") << "..." << std::endl;
		mQuerySet.Prepare(mDevice, mContext, 0, _queryCount);
	}

	void Renderer::AttachPrimaryViewports() {
		D3D11_VIEWPORT viewports[] = { mLeftViewport, mRightViewport, mFullViewport };
		mContext->RSSetViewports(ARRAYSIZE(viewports), viewports);
	}

	void Renderer::AttachPrimaryRTVs() {
		ID3D11RenderTargetView *RTVS[] = { mAlbedoRTV.Get(), mPositionRTV.Get(), mNormalRTV.Get(), mSpecularRTV.Get(), mGlowRTV.Get(), mSuperGlowRTV.Get() };
		mContext->OMSetRenderTargets(sizeof(RTVS) / sizeof(RTVS[0]), RTVS, mDSView.Get());
	}

	void Renderer::UpdateCamera(float const _moveSpd, float const _rotSpd, float _delta) {
		if (GetActiveWindow() != mWindow) {
			return;
		}
		if (!CommandConsole::Instance().willTakeInput()) {
			//w
			if (GetAsyncKeyState('W')) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateNewTranslation(0, 0, -_moveSpd * _delta) * VRInputManager::GetInstance().GetPlayerPosition();
			}
			//s
			if (GetAsyncKeyState('S')) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateNewTranslation(0, 0, _moveSpd * _delta) * VRInputManager::GetInstance().GetPlayerPosition();
			}
			//a
			if (GetAsyncKeyState('A')) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateNewTranslation(-_moveSpd * _delta, 0, 0) * VRInputManager::GetInstance().GetPlayerPosition();
			}
			//d
			if (GetAsyncKeyState('D')) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateNewTranslation(_moveSpd * _delta, 0, 0) * VRInputManager::GetInstance().GetPlayerPosition();
			}
			// Q
			if (GetAsyncKeyState('Q')) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateNewZRotation(_rotSpd * _delta) * VRInputManager::GetInstance().GetPlayerPosition();
			}
			// E
			if (GetAsyncKeyState('E')) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateNewZRotation(-_rotSpd * _delta) * VRInputManager::GetInstance().GetPlayerPosition();
			}
			//x
			if (GetAsyncKeyState(VK_CONTROL)) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateNewTranslation(0, -_moveSpd * _delta, 0) * VRInputManager::GetInstance().GetPlayerPosition();
			}
			
			if (GetAsyncKeyState(VK_SPACE)) {
				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateNewTranslation(0, _moveSpd * _delta, 0) * VRInputManager::GetInstance().GetPlayerPosition();
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

				VRInputManager::GetInstance().GetPlayerPosition() = matrix4::CreateNewXRotation(dy) * matrix4::CreateNewYRotation(dx) * VRInputManager::GetInstance().GetPlayerPosition();

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
	}

	void Renderer::RenderVR(float _delta) {
		UpdateViewProjection();
		UpdateGSBuffers();
		UpdateLBuffers();
		ParticleSystem::Instance()->Render();
		ProcessRenderSet();

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
	}


	void Renderer::RenderNoVR(float _delta) {
		UpdateCamera(3, 2, _delta);
		UpdateGSBuffers();
		if (mQuerySet.Valid()) {
			mQuerySet.Query("Update Geometry Buffers");
		}
		UpdateLBuffers();
		if (mQuerySet.Valid()) {
			mQuerySet.Query("Update Light Buffers");
		}
		ParticleSystem::Instance()->Render();
		if (mQuerySet.Valid()) {
			mQuerySet.Query("Render Particles");
		}
		ProcessRenderSet();
		RenderScreenQuad();
	}

	void Renderer::ProcessRenderSet() {
		// TODO: Make a ShaderLimits.h file that has macros for the number of instances and other such arrays a shader supports.

		std::vector<matrix4> positions;
		positions.reserve(256);

		// Remove empty lists
		mOpaqueSet.Prune();
		mTransparentSet.Prune();
		mTopmostSet.Prune();
		mMotionSet.Prune();

		// Go through opaque objects first
		mContext->OMSetDepthStencilState(mOpaqueState.Get(), 1);
		mContext->OMSetBlendState(mOpaqueBlendState.Get(), NULL, 0xFFFFFFFF);
		for (auto it = mOpaqueSet.Begin(); it != mOpaqueSet.End(); ++it) {
			(*it)->mPositions.GetData(positions);
			if (positions.size() > 0) {
#if ENABLE_INSTANCING
				unsigned int offset = 0;
				positions.reserve((positions.size() / 256 + 1) * 256);
				while (positions.size() - offset <= positions.size()) {
					(*it)->mShape.GetContext().Apply(mCurrentContext);
					mCurrentContext.SimpleClone((*it)->mShape.GetContext());

					D3D11_MAPPED_SUBRESOURCE map;
					memset(&map, 0, sizeof(map));
					HRESULT MHR = mContext->Map(mPositionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
					memcpy(map.pData, positions.data() + offset, sizeof(matrix4) * min(positions.size() - offset, 256));
					mContext->Unmap(mPositionBuffer.Get(), 0);
					//mContext->UpdateSubresource(mPositionBuffer.Get(), 0, nullptr, &positions[i], 0, 0);

					(*it)->mShape.Render((UINT)positions.size() - offset);
					offset += 256;
				}
#else
				(*it)->mShape.GetContext().Apply(mCurrentContext);
				mCurrentContext.SimpleClone((*it)->mShape.GetContext());
				vec4i SimInstanceID(0, 0, 0, 0);
				for (unsigned int i = 0; i < positions.size(); ++i) {
					SimInstanceID.x = i;

					D3D11_MAPPED_SUBRESOURCE map;
					memset(&map, 0, sizeof(map));
					HRESULT MHR = mContext->Map(mSimInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
					memcpy(map.pData, &SimInstanceID, sizeof(vec4i));
					mContext->Unmap(mSimInstanceBuffer.Get(), 0);

					//mContext->UpdateSubresource(mSimInstanceBuffer.Get(), 0, nullptr, &SimInstanceID, 0, 0);

					memset(&map, 0, sizeof(map));
					MHR = mContext->Map(mPositionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
					memcpy(map.pData, &positions[i], sizeof(matrix4));
					mContext->Unmap(mPositionBuffer.Get(), 0);
					//mContext->UpdateSubresource(mPositionBuffer.Get(), 0, nullptr, &positions[i], 0, 0);
					(*it)->mShape.Render(1); // Without instancing, the instance count doesn't matter, but we're only drawing one :)
				}
#endif
			}
		}

		if (mQuerySet.Valid()) {
			mQuerySet.Query("Opaque Objects");
		}

		// Draw the topmost objects. These don't use the depth buffer at all, 
		mContext->OMSetBlendState(mOpaqueBlendState.Get(), NULL, 0xFFFFFFFF);
		for (unsigned int passIndex = 0; passIndex < 2; ++passIndex) {
			if (passIndex == 0) {
				mContext->OMSetDepthStencilState(mMotionStateFindObject.Get(), 1);
			} else {
				mContext->OMSetDepthStencilState(mMotionStateReverseDepth.Get(), 1); // Make sure the stencil buffer value is less than 1 to pass.
				Settings::GetInstance().SetInt("PixelShaderOverride", ePS_SOLIDCOLOR);
				D3D11_MAPPED_SUBRESOURCE map;
				matrix4 scale = matrix4::CreateNewScale(1.5f, 1.5f, 1.5f);
				mContext->Map(mGlobalMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
				memcpy(map.pData, &scale, sizeof(scale));
				mContext->Unmap(mGlobalMatrixBuffer.Get(), 0);

				vec4f redLit(1, 0, 0, 0);
				mContext->Map(mOutlineColorBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
				memcpy(map.pData, &redLit, sizeof(redLit));
				mContext->Unmap(mOutlineColorBuffer.Get(), 0);
			}

			for (auto it = mMotionSet.Begin(); it != mMotionSet.End(); ++it) {
				(*it)->mPositions.GetData(positions);
				if (positions.size() > 0) {
#if ENABLE_INSTANCING
					unsigned int offset = 0;
					positions.reserve((positions.size() / 256 + 1) * 256);
					while (positions.size() - offset <= positions.size()) {
						(*it)->mShape.GetContext().Apply(mCurrentContext);
						mCurrentContext.SimpleClone((*it)->mShape.GetContext());
						if (passIndex == 1) {
							mContext->PSSetConstantBuffers(1, 1, mOutlineColorBuffer.GetAddressOf());
						}

						D3D11_MAPPED_SUBRESOURCE map;
						memset(&map, 0, sizeof(map));
						HRESULT MHR = mContext->Map(mPositionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
						memcpy(map.pData, positions.data() + offset, sizeof(matrix4) * min(positions.size() - offset, 256));
						mContext->Unmap(mPositionBuffer.Get(), 0);
						//mContext->UpdateSubresource(mPositionBuffer.Get(), 0, nullptr, &positions[i], 0, 0);

						(*it)->mShape.Render((UINT)positions.size() - offset);
						offset += 256;
					}
#else
					(*it)->mShape.GetContext().Apply(mCurrentContext);
					mCurrentContext.SimpleClone((*it)->mShape.GetContext());
					if (passIndex == 1) {
						mContext->PSSetConstantBuffers(1, 1, mOutlineColorBuffer.GetAddressOf());
					}
					vec4i SimInstanceID(0, 0, 0, 0);
					for (unsigned int i = 0; i < positions.size(); ++i) {
						SimInstanceID.x = i;

						D3D11_MAPPED_SUBRESOURCE map;
						memset(&map, 0, sizeof(map));
						HRESULT MHR = mContext->Map(mSimInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
						memcpy(map.pData, &SimInstanceID, sizeof(vec4i));
						mContext->Unmap(mSimInstanceBuffer.Get(), 0);

						//mContext->UpdateSubresource(mSimInstanceBuffer.Get(), 0, nullptr, &SimInstanceID, 0, 0);

						memset(&map, 0, sizeof(map));
						MHR = mContext->Map(mPositionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
						memcpy(map.pData, &positions[i], sizeof(matrix4));
						mContext->Unmap(mPositionBuffer.Get(), 0);
						//mContext->UpdateSubresource(mPositionBuffer.Get(), 0, nullptr, &positions[i], 0, 0);
						(*it)->mShape.Render(1); // Without instancing, the instance count doesn't matter, but we're only drawing one :)
					}
#endif
				}
			}
		}


		{
			// Anonymous scope so I can use the generic variable name 'map'
			D3D11_MAPPED_SUBRESOURCE map;
			matrix4 identity;
			mContext->Map(mGlobalMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
			memcpy(map.pData, &identity, sizeof(identity));
			mContext->Unmap(mGlobalMatrixBuffer.Get(), 0);
			Settings::GetInstance().SetInt("PixelShaderOverride", ePS_MAX);
		}


		if (mQuerySet.Valid()) {
			mQuerySet.Query("Outlined Objects");
		}


		mContext->OMSetDepthStencilState(mTopmostState.Get(), 1);
		//No blend state is set, because top-most objects also support alpha blending, and that state was set in the block above.
		for (auto it = mTopmostSet.Begin(); it != mTopmostSet.End(); ++it) {
			(*it)->mPositions.GetData(positions);
			if (positions.size() > 0) {
#if ENABLE_INSTANCING
				unsigned int offset = 0;
				positions.reserve((positions.size() / 256 + 1) * 256);
				while (positions.size() - offset <= positions.size()) {
					(*it)->mShape.GetContext().Apply(mCurrentContext);
					mCurrentContext.SimpleClone((*it)->mShape.GetContext());

					D3D11_MAPPED_SUBRESOURCE map;
					memset(&map, 0, sizeof(map));
					HRESULT MHR = mContext->Map(mPositionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
					memcpy(map.pData, positions.data() + offset, sizeof(matrix4) * min(positions.size() - offset, 256));
					mContext->Unmap(mPositionBuffer.Get(), 0);
					//mContext->UpdateSubresource(mPositionBuffer.Get(), 0, nullptr, &positions[i], 0, 0);

					(*it)->mShape.Render((UINT)positions.size() - offset);
					offset += 256;
				}
#else
				(*it)->mShape.GetContext().Apply(mCurrentContext);
				mCurrentContext.SimpleClone((*it)->mShape.GetContext());
				vec4i SimInstanceID(0, 0, 0, 0);
				for (unsigned int i = 0; i < positions.size(); ++i) {
					SimInstanceID.x = i;

					D3D11_MAPPED_SUBRESOURCE map;
					memset(&map, 0, sizeof(map));
					HRESULT MHR = mContext->Map(mSimInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
					memcpy(map.pData, &SimInstanceID, sizeof(vec4i));
					mContext->Unmap(mSimInstanceBuffer.Get(), 0);

					//mContext->UpdateSubresource(mSimInstanceBuffer.Get(), 0, nullptr, &SimInstanceID, 0, 0);

					memset(&map, 0, sizeof(map));
					MHR = mContext->Map(mPositionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
					memcpy(map.pData, &positions[i], sizeof(matrix4));
					mContext->Unmap(mPositionBuffer.Get(), 0);
					//mContext->UpdateSubresource(mPositionBuffer.Get(), 0, nullptr, &positions[i], 0, 0);
					(*it)->mShape.Render(1); // Without instancing, the instance count doesn't matter, but we're only drawing one :)
				}
#endif
			}
		}

		if (mQuerySet.Valid()) {
			mQuerySet.Query("Topmost Objects");
		}
	}


	void Renderer::RenderScreenQuad() {
		{
			vec4i SimulatedIID(0, 0, 0, 0);
			D3D11_MAPPED_SUBRESOURCE map;
			HRESULT MHR = mContext->Map(mSimInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
			memcpy(map.pData, &SimulatedIID, sizeof(vec4i));
			mContext->Unmap(mSimInstanceBuffer.Get(), 0);
		}

		// Blur the bloom texture so that it actually bleeds on the screen
		if (mEnabledFeatures[eRendererFeature_SuperGlow]) {
			BlurTextures(mSuperGlowTexture.GetAddressOf(), 1, 2.0f, 0.4f);
			if (mQuerySet.Valid()) {
				mQuerySet.Query("SuperGlow");
			}
		}
		if (mEnabledFeatures[eRendererFeature_Glow]) {
			if (mEnabledFeatures[eRendererFeature_Bloom]) {
				RenderForBloom();
				if (mQuerySet.Valid()) {
					mQuerySet.Query("Render for bloom");
				}
			}
			BlurTextures(mBloomTexture.GetAddressOf(), 1, 2.0f, 0.4f);
			if (mQuerySet.Valid()) {
				mQuerySet.Query("Regular Blur");
			}
		}

		mContext->OMSetBlendState(mOpaqueBlendState.Get(), NULL, 0xFFFFFFFF);



		//mContext->RSSetViewports(1, &mFullViewport);
		mContext->OMSetRenderTargets(1, mPostProcessRTV.GetAddressOf(), nullptr);
		mDeferredCombiner->GetContext().Apply(mCurrentContext);
		mDeferredCombiner->Render(1);
		mCurrentContext.SimpleClone(mDeferredCombiner->GetContext());
		if (mQuerySet.Valid()) {
			mQuerySet.Query("Data Combination");
		}

		RenderLightVolumes();
		if (mQuerySet.Valid()) {
			mQuerySet.Query("Light Volumes");
		}

		RenderTransparentObjects();
		if (mQuerySet.Valid()) {
			mQuerySet.Query("Transparent Objects");
		}


		mContext->OMSetDepthStencilState(mOpaqueState.Get(), 1);

		ID3D11ShaderResourceView *unbind[] = { nullptr, nullptr, nullptr, nullptr };
		mContext->OMSetRenderTargets(1, mMainView.GetAddressOf(), nullptr);
		mContext->PSSetShaderResources(0, 4, unbind);

		mScenePPQuad->GetContext().Apply(mCurrentContext);
		mScenePPQuad->Render();

		ID3D11ShaderResourceView *noGlow = nullptr;
		mContext->PSSetShaderResources(eTEX_REGISTER5, 1, &noGlow);
		mContext->PSSetShaderResources(eTEX_REGISTER6, 1, &noGlow);

		mCurrentContext.SimpleClone(mScenePPQuad->GetContext());
		AttachPrimaryViewports();
	}

	void Renderer::RenderTransparentObjects() {
		mContext->OMSetRenderTargets(1, mPostProcessRTV.GetAddressOf(), mDSView.Get());

		std::vector<matrix4> positions;
		mContext->OMSetDepthStencilState(mTransparentState.Get(), 1);
		mContext->OMSetBlendState(mTransparentBlendState.Get(), NULL, 0xFFFFFFFF);
		for (auto it = mTransparentSet.Begin(); it != mTransparentSet.End(); ++it) {
			(*it)->mPositions.GetData(positions);
			if (positions.size() > 0) {
#if ENABLE_INSTANCING
				unsigned int offset = 0;
				positions.reserve((positions.size() / 256 + 1) * 256);
				while (positions.size() - offset <= positions.size()) {
					(*it)->mShape.GetContext().Apply(mCurrentContext);
					mCurrentContext.SimpleClone((*it)->mShape.GetContext());

					D3D11_MAPPED_SUBRESOURCE map;
					HRESULT MHR = mContext->Map(mPositionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
					memcpy(map.pData, positions.data() + offset, sizeof(matrix4) * min(positions.size() - offset, 256));
					mContext->Unmap(mPositionBuffer.Get(), 0);
					//mContext->UpdateSubresource(mPositionBuffer.Get(), 0, nullptr, &positions[i], 0, 0);

					(*it)->mShape.Render((UINT)positions.size() - offset);
					offset += 256;
				}
#else
				(*it)->mShape.GetContext().Apply(mCurrentContext);
				mCurrentContext.SimpleClone((*it)->mShape.GetContext());
				vec4i SimulatedIID(0, 0, 0, 0);
				for (unsigned int i = 0; i < positions.size(); ++i) {
					SimulatedIID.x = i;

					D3D11_MAPPED_SUBRESOURCE map;
					HRESULT MHR = mContext->Map(mSimInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
					memcpy(map.pData, &SimulatedIID, sizeof(vec4i));
					mContext->Unmap(mSimInstanceBuffer.Get(), 0);

					//mContext->UpdateSubresource(mSimInstanceBuffer.Get(), 0, nullptr, &SimInstanceID, 0, 0);

					MHR = mContext->Map(mPositionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
					memcpy(map.pData, &positions[i], sizeof(matrix4));
					mContext->Unmap(mPositionBuffer.Get(), 0);
					//mContext->UpdateSubresource(mPositionBuffer.Get(), 0, nullptr, &positions[i], 0, 0);
					(*it)->mShape.Render(1); // Without instancing, the instance count doesn't matter, but we're only drawing one :)
				}
#endif
			}
		}

	}

	void Renderer::RenderLightVolumes() {
		mContext->OMSetRenderTargets(1, mPostProcessRTV.GetAddressOf(), mDSView.Get());
		mContext->OMSetBlendState(mAdditiveBlendState.Get(), NULL, 0xFFFFFFFF);
		std::vector<matrix4> positions;
		for (auto it = mLightSet.Begin(); it != mLightSet.End(); ++it) {
			(*it)->mPositions.GetData(positions);
			if (positions.size() == 0) {
				continue;
			}

			vec4i SimulatedIID(0, 0, 0, 0);
			for (unsigned int i = 0; i < positions.size(); ++i) {
				(*it)->mShape.GetContext().Apply(mCurrentContext);
				mCurrentContext.SimpleClone((*it)->mShape.GetContext());
				for (unsigned int passIndex = 0; passIndex < 3; ++passIndex) {
					if (passIndex == 0) {
						mContext->ClearDepthStencilView(mDSView.Get(), D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 0, 0);
						RasterizerStateManager::Instance()->ApplyState(eRS_CCW);
						ShaderManager::Instance()->ApplyPShader(ePS_NONE);
						mContext->OMSetDepthStencilState(mLSStencilBack.Get(), 1);
					} else if (passIndex == 1) {
						RasterizerStateManager::Instance()->ApplyState(eRS_FILLED);
						ShaderManager::Instance()->ApplyPShader(ePS_NONE);
						mContext->OMSetDepthStencilState(mLSStencilFront.Get(), 0);
					} else {
						RasterizerStateManager::Instance()->ApplyState(eRS_FILLED);
						ShaderManager::Instance()->ApplyPShader(ePS_LIGHTING);
						mContext->OMSetDepthStencilState(mLSStencilRender.Get(), 0);
						(*it)->mShape.mContext.Apply(mCurrentContext);
						ShaderManager::Instance()->ApplyVShader(eVS_NDC);
						ShaderManager::Instance()->ApplyGShader(eGS_PosNormTex_NDC);
						ShaderManager::Instance()->ApplyPShader(ePS_LIGHTING);
						mCurrentContext.mVertexShaderFormat = eVS_NDC;
						mCurrentContext.mGeoShaderFormat = eGS_PosNormTex_NDC;
						mCurrentContext.mPixelShaderFormat = ePS_LIGHTING;

						ID3D11ShaderResourceView *Deferred[] = { mAlbedoSRV.Get(), mPositionSRV.Get(), mNormalSRV.Get(), mSpecularSRV.Get() };
						mContext->PSSetShaderResources(0, 4, Deferred);
						mDeferredCombiner->Render(1);
						// Terminate the final loop early, because we don't want to render each volume twice. We render them once to get the pixels
						// the pixels that the light applies to (it would fail drawing because the face is behind an object), and then we use a simple
						// quad to apply the actual light.
						continue; 
					}
					
					if (passIndex == 0) {

						SimulatedIID.x = i;

						D3D11_MAPPED_SUBRESOURCE map;
						HRESULT MHR = mContext->Map(mSimInstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
						memcpy(map.pData, &SimulatedIID, sizeof(vec4i));
						mContext->Unmap(mSimInstanceBuffer.Get(), 0);

						//mContext->UpdateSubresource(mSimInstanceBuffer.Get(), 0, nullptr, &SimInstanceID, 0, 0);

						MHR = mContext->Map(mPositionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
						memcpy(map.pData, &positions[i], sizeof(matrix4));
						mContext->Unmap(mPositionBuffer.Get(), 0);
					}
					//mContext->UpdateSubresource(mPositionBuffer.Get(), 0, nullptr, &positions[i], 0, 0);
					(*it)->mShape.Render(1); // Without instancing, the instance count doesn't matter, but we're only drawing one :)
				}
			}
		}
		Settings::GetInstance().SetInt("RasterizerStateOverride", eRS_MAX);
		Settings::GetInstance().SetInt("PixelShaderOverride", ePS_MAX);
	}

	void Renderer::RenderForBloom() {
		ID3D11ShaderResourceView *null[] = { nullptr, nullptr, nullptr };
		mContext->PSSetShaderResources(eTEX_REGISTER4, 2, null); // Remove Glow and Bloom textures
		mContext->OMSetRenderTargets(1, mBloomRTV.GetAddressOf(), nullptr);
		mContext->RSSetViewports(1, &mFullViewport);

		ShaderManager::Instance()->ApplyPShader(ePS_BLOOM);
		ShaderManager::Instance()->ApplyVShader(eVS_BLUR);
		ShaderManager::Instance()->ApplyGShader(eGS_None);
		ID3D11ShaderResourceView *srvs[] = { mPostProcessSRV.Get(), mGlowSRV.Get(), mSuperGlowSRV.Get() };
		mContext->PSSetShaderResources(0, 3, srvs);
		mScenePPQuad->Render(1);
		mContext->PSSetShaderResources(0, 3, null); // Remove scene and glow textures


		AttachPrimaryRTVs();
		AttachPrimaryViewports();
		mCurrentContext.Apply();
	}

	void Renderer::ReportFrameQueries() {
		//for (unsigned int i = 0; i < mQueries.size(); ++i) {
		//	HRESULT reportReady = mContext->GetData(mQueries[i].Disjoint, nullptr, 0, 0);
		//	if (reportReady == S_FALSE) {
		//		// Print query reports only when all reports are ready.
		//		return;
		//	}
		//}

		//D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointData;
		//D3D11_QUERY_DATA_PIPELINE_STATISTICS pipelineStats;
		//UINT64 gpuFrameBegin = 0;
		//UINT64 gpuClearing = 0;
		//UINT64 gpuGBufferUpdates = 0;
		//UINT64 gpuLBufferUpdates = 0;
		//UINT64 gpuParticleSystem = 0;
		//UINT64 gpuOpaqueObjects = 0;
		//UINT64 gpuOutlinedObjects = 0;
		//UINT64 gpuTopmostObjects = 0;
		//UINT64 gpuSuperGlow = 0;
		//UINT64 gpuBloomCheck = 0;
		//UINT64 gpuGlow = 0;
		//UINT64 gpuDataCombination = 0;
		//UINT64 gpuTransparentObjects = 0;
		//UINT64 gpuScreenQuad = 0;
		//UINT64 gpuVRSubmit = 0;
		//UINT64 gpuFrameEnd = 0;
		//UINT64 numSamples = 0;

		//for (unsigned int i = 0; i < mQueries.size(); ++i) {
		//	// Collect the data
		//	mContext->GetData(mQueries[i].Disjoint, &disjointData, sizeof(disjointData), 0);
		//	if (disjointData.Disjoint == TRUE) {
		//		// Something occured to make the GPU's frequency change, so the data is not accurate.
		//		SystemLogger::GetLog() << "Frame " << (i + 1) << " of " << mQueries.size() << ": Frame was disjoint." << std::endl;
		//		continue;
		//	}

		//	mContext->GetData(mQueries[i].gpuFrameBegin, &gpuFrameBegin, sizeof(UINT64), 0);
		//	mContext->GetData(mQueries[i].gpuClearing, &gpuClearing, sizeof(UINT64), 0);
		//	mContext->GetData(mQueries[i].gpuGBufferUpdates, &gpuGBufferUpdates, sizeof(UINT64), 0);
		//	mContext->GetData(mQueries[i].gpuLBufferUpdates, &gpuLBufferUpdates, sizeof(UINT64), 0);
		//	mContext->GetData(mQueries[i].gpuParticleSystem, &gpuParticleSystem, sizeof(UINT64), 0);
		//	mContext->GetData(mQueries[i].gpuOpaqueObjects, &gpuOpaqueObjects, sizeof(UINT64), 0);
		//	mContext->GetData(mQueries[i].gpuOutlinedObjects, &gpuOutlinedObjects, sizeof(UINT64), 0);
		//	mContext->GetData(mQueries[i].gpuTopmostObjects, &gpuTopmostObjects, sizeof(UINT64), 0);
		//	mContext->GetData(mQueries[i].gpuSuperGlow, &gpuSuperGlow, sizeof(UINT64), 0);
		//	mContext->GetData(mQueries[i].gpuBloomCheck, &gpuBloomCheck, sizeof(UINT64), 0);
		//	mContext->GetData(mQueries[i].gpuGlow, &gpuGlow, sizeof(UINT64), 0);
		//	mContext->GetData(mQueries[i].gpuDataCombination, &gpuDataCombination, sizeof(UINT64), 0);
		//	mContext->GetData(mQueries[i].gpuTransparentObjects, &gpuTransparentObjects, sizeof(UINT64), 0);
		//	mContext->GetData(mQueries[i].gpuVRSubmit, &gpuVRSubmit, sizeof(UINT64), 0);
		//	mContext->GetData(mQueries[i].gpuFrameEnd, &gpuFrameEnd, sizeof(UINT64), 0);

		//	float frequency = ((float)(disjointData.Frequency));
		//	float gpuFrameTime = (gpuFrameEnd - gpuFrameBegin) / frequency * 1000.f;

		//	// Print the data
		//	SystemLogger::GetLog() << "Frame " << (i + 1) << " of " << mQueries.size() << ":\n" <<
		//		"\tGPU Time: " << gpuFrameTime << std::endl;
		//}

		//mQueries.clear();
	}

#pragma endregion Private Functions

#pragma region Public Functions

	GhostList<matrix4>::GhostNode* Renderer::AddOpaqueNode(RenderShape &_node) {
		return mOpaqueSet.AddShape(_node);
	}

	GhostList<matrix4>::GhostNode* Renderer::AddTransparentNode(RenderShape &_node) {
		return mTransparentSet.AddShape(_node);
	}

	GhostList<matrix4>::GhostNode * Renderer::AddTopmostNode(RenderShape & _node) {
		return mTopmostSet.AddShape(_node);
	}

	GhostList<matrix4>::GhostNode * Renderer::AddMotionNode(RenderShape & _node) {
		return mMotionSet.AddShape(_node);
	}

	GhostList<matrix4>::GhostNode * Renderer::AddLightNode(RenderShape & _node) {
		return mLightSet.AddShape(_node);
	}

	void Renderer::RemoveOpaqueNode(RenderShape & _node) {
		mOpaqueSet.RemoveShape(_node);
	}

	void Renderer::RemoveTransparentNode(RenderShape & _node) {
		mTransparentSet.RemoveShape(_node);
	}

	void Renderer::RemoveTopmostNode(RenderShape & _node) {
		mTopmostSet.RemoveShape(_node);
	}

	void Renderer::RemoveMotionNode(RenderShape & _node) {
		mMotionSet.RemoveShape(_node);
	}

	void Renderer::RemoveLightNode(RenderShape & _node) {
		mLightSet.RemoveShape(_node);
	}

	void Renderer::UpdateOpaqueNodeBuffer(RenderShape & _node, ConstantBufferType _t, unsigned int _index) {
		RenderList* list = mOpaqueSet.GetListForShape(_node);
		if (list == nullptr) {
			SystemLogger::Error() << "Could not update opaque node: The given shape had no associated render list." << std::endl;
			return;
		}
		switch (_t) {
			case eCB_VERTEX:
				list->UpdateBuffer(_t, _node.GetContext().mVertexCBuffers[_index], _index, _node.mVBIndex);
				break;
			case eCB_PIXEL:
				list->UpdateBuffer(_t, _node.GetContext().mPixelCBuffers[_index], _index, _node.mPBIndex);
				break;
			case eCB_GEO:
				list->UpdateBuffer(_t, _node.GetContext().mGeometryCBuffers[_index], _index, _node.mGBIndex);
				break;
			default:
				break;
		}
	}

	void Renderer::UpdateTransparentNodeBuffer(RenderShape & _node, ConstantBufferType _t, unsigned int _index) {
		RenderList* list = mTransparentSet.GetListForShape(_node);
		if (list == nullptr) {
			SystemLogger::Error() << "Could not update transparent node: The given shape had no associated render list." << std::endl;
			return;
		}
		switch (_t) {
			case eCB_VERTEX:
				list->UpdateBuffer(_t, _node.GetContext().mVertexCBuffers[_index], _index, _node.mVBIndex);
				break;
			case eCB_PIXEL:
				list->UpdateBuffer(_t, _node.GetContext().mPixelCBuffers[_index], _index, _node.mPBIndex);
				break;
			case eCB_GEO:
				list->UpdateBuffer(_t, _node.GetContext().mGeometryCBuffers[_index], _index, _node.mGBIndex);
				break;
			default:
				break;
		}
	}

	void Renderer::UpdateTopmostNodeBuffer(RenderShape & _node, ConstantBufferType _t, unsigned int _index) {
		RenderList* list = mTopmostSet.GetListForShape(_node);
		if (list == nullptr) {
			SystemLogger::Error() << "Could not update topmost node: The given shape had no associated render list." << std::endl;
			return;
		}
		switch (_t) {
			case eCB_VERTEX:
				list->UpdateBuffer(_t, _node.GetContext().mVertexCBuffers[_index], _index, _node.mVBIndex);
				break;
			case eCB_PIXEL:
				list->UpdateBuffer(_t, _node.GetContext().mPixelCBuffers[_index], _index, _node.mPBIndex);
				break;
			case eCB_GEO:
				list->UpdateBuffer(_t, _node.GetContext().mGeometryCBuffers[_index], _index, _node.mGBIndex);
				break;
			default:
				break;
		}
	}

	void Renderer::UpdateMotionNodeBuffer(RenderShape & _node, ConstantBufferType _t, unsigned int _index) {
		RenderList* list = mMotionSet.GetListForShape(_node);
		if (list == nullptr) {
			SystemLogger::Error() << "Could not update motion node: The given shape had no associated render list." << std::endl;
			return;
		}
		switch (_t) {
			case eCB_VERTEX:
				list->UpdateBuffer(_t, _node.GetContext().mVertexCBuffers[_index], _index, _node.mVBIndex);
				break;
			case eCB_PIXEL:
				list->UpdateBuffer(_t, _node.GetContext().mPixelCBuffers[_index], _index, _node.mPBIndex);
				break;
			case eCB_GEO:
				list->UpdateBuffer(_t, _node.GetContext().mGeometryCBuffers[_index], _index, _node.mGBIndex);
				break;
			default:
				break;
		}
	}

	void Renderer::UpdateLightNodeBuffer(RenderShape & _node, ConstantBufferType _t, unsigned int _index) {
		RenderList* list = mLightSet.GetListForShape(_node);
		if (list == nullptr) {
			SystemLogger::Error() << "Could not update light node: The given shape had no associated render list." << std::endl;
			return;
		}
		switch (_t) {
			case eCB_VERTEX:
				list->UpdateBuffer(_t, _node.GetContext().mVertexCBuffers[_index], _index, _node.mVBIndex);
				break;
			case eCB_PIXEL:
				list->UpdateBuffer(_t, _node.GetContext().mPixelCBuffers[_index], _index, _node.mPBIndex);
				break;
			case eCB_GEO:
				list->UpdateBuffer(_t, _node.GetContext().mGeometryCBuffers[_index], _index, _node.mGBIndex);
				break;
			default:
				break;
		}
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
		InitializeSceneQuad(); // Must be initilized after all the views and buffers, as it assigns some.
		InitializeStates();

		mEnabledFeatures.set(eRendererFeature_Bloom);
		mEnabledFeatures.set(eRendererFeature_Glow);
		mEnabledFeatures.set(eRendererFeature_SuperGlow);
		CommandConsole::Instance().AddCommand(L"GSET", &Renderer::ProcessGSET);
		CommandConsole::Instance().AddCommand(L"GQUERY", &Renderer::ProcessGQUERY);

		InitializeSamplerState();
		SetStaticBuffers();
		// TODO Eventually: Give each shape a topology enum, perhaps?
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// TODO Eventually: Actually assign input layouts for each render shape.
		InputLayoutManager::Instance().ApplyLayout(eVERT_POSNORMTANTEX);

		mUseVsync = _vsync;


		if (!mVrSystem) {
			mVPLeftData.projection.matrix = DirectX::XMMatrixPerspectiveFovRH(70, (float)_height / (float)_width, 0.1f, 1000);
			mVPLeftData.view = VRInputManager::GetInstance().GetPlayerPosition().Transpose().Invert();
			mVPLeftData.projection = mVPLeftData.projection.Transpose();
			mVPRightData = mVPLeftData;
		}

		UpdateGSBuffers();
#if _DEBUG
		InitializeObjectNames();
#endif
		return true;
	}

	void Renderer::ClearRenderSet() {
		mOpaqueSet.ClearSet();
		mTransparentSet.ClearSet();
		mTopmostSet.ClearSet();
		mMotionSet.ClearSet();
		mLightSet.ClearSet();
	}

	bool Renderer::BlurTextures(ID3D11Texture2D **_textures, unsigned int _numTextures, float _sigma, float _downsample) {
		if (_numTextures < 1) {
			return false;
		}

		D3D11_TEXTURE2D_DESC texDesc, downDesc;
		ID3D11Texture2D **downTex = new ID3D11Texture2D*[_numTextures * 2];
		ID3D11ShaderResourceView **downSRV = new ID3D11ShaderResourceView*[_numTextures * 2];
		ID3D11RenderTargetView **downRTV = new ID3D11RenderTargetView*[_numTextures * 2];
		ID3D11ShaderResourceView **upSRV = new ID3D11ShaderResourceView*[_numTextures * 2];
		ID3D11RenderTargetView **upRTV = new ID3D11RenderTargetView*[_numTextures * 2];
		for (unsigned int i = 0; i < _numTextures; ++i) {
			_textures[i]->GetDesc(&texDesc);
			for (unsigned int j = 0; j < 2; ++j) {
				downDesc = texDesc;
				downDesc.Width = (UINT)(texDesc.Width * _downsample);
				downDesc.Height = (UINT)(texDesc.Height * _downsample);

				mDevice->CreateTexture2D(&downDesc, nullptr, &downTex[i + _numTextures * j]);
				mDevice->CreateRenderTargetView(downTex[i + _numTextures * j], nullptr, &downRTV[i + _numTextures * j]);
				mDevice->CreateShaderResourceView(downTex[i + _numTextures * j], nullptr, &downSRV[i + _numTextures * j]);

				SetD3DName(downTex[i], (std::string("Blur Downsample Texture ") + std::to_string(i + _numTextures * j)).c_str());
				SetD3DName(downSRV[i], (std::string("Blur Downsample SRV ") + std::to_string(i + _numTextures * j)).c_str());
				SetD3DName(downRTV[i], (std::string("Blur Downsample RTV ") + std::to_string(i + _numTextures * j)).c_str());
			}

			// SRVs and RTVs for the original textures.
			mDevice->CreateRenderTargetView(_textures[i], nullptr, &upRTV[i]);
			mDevice->CreateShaderResourceView(_textures[i], nullptr, &upSRV[i]);
			upRTV[i + _numTextures] = upRTV[i];
			upSRV[i + _numTextures] = upSRV[i];
			SetD3DName(upSRV[i], (std::string("Blur Upsample SRV ") + std::to_string(i)).c_str());
			SetD3DName(upRTV[i], (std::string("Blur Upsample RTV ") + std::to_string(i)).c_str());
			D3D11_VIEWPORT blurport;
			blurport.MinDepth = 0;
			blurport.MaxDepth = 1;
			blurport.TopLeftX = 0;
			blurport.TopLeftY = 0;
			blurport.Width = downDesc.Width * 1.0f;
			blurport.Height = downDesc.Height * 1.0f;
			mContext->RSSetViewports(1, &blurport);
		}

		// Prepare the pipeline
		ShaderManager::Instance()->ApplyPShader(ePS_BLUR);
		ShaderManager::Instance()->ApplyVShader(eVS_BLUR);
		ShaderManager::Instance()->ApplyGShader(eGS_None);
		mContext->PSSetConstantBuffers(ePB_REGISTER1 + ePB_OFFSET, 1, mBlurStageBuffer.GetAddressOf());
		mBlurData.sigma = _sigma;


		for (unsigned int pass = 0; pass < _numTextures; pass += D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) {
			unsigned int passTextureCount = min(_numTextures - pass, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT);

			// Downsample
			ToggleBlurTextureSet(passTextureCount, downRTV, upSRV); // ping
			RenderBlurStage(BLUR_STAGE_SAMPLE, 0, 0);

			//// horz blur
			ToggleBlurTextureSet(passTextureCount, downRTV, downSRV); // pong
			RenderBlurStage(BLUR_STAGE_BLUR, 1, 0);

			//vert blur
			ToggleBlurTextureSet(passTextureCount, downRTV, downSRV); // ping
			RenderBlurStage(BLUR_STAGE_BLUR, 0, 1);

			//// upsample
			mContext->RSSetViewports(1, &mFullViewport);
			ToggleBlurTextureSet(passTextureCount, upRTV, downSRV); // pong
			RenderBlurStage(BLUR_STAGE_SAMPLE, 0, 0);
		}

		for (unsigned int i = 0; i < _numTextures; ++i) {
			for (unsigned int j = 0; j < 2; ++j) {
				downTex[i + _numTextures * j]->Release();
				downSRV[i + _numTextures * j]->Release();
				downRTV[i + _numTextures * j]->Release();
			}

			upSRV[i]->Release();
			upRTV[i]->Release();
		}
		delete[] downTex;
		delete[] downSRV;
		delete[] downRTV;
		delete[] upSRV;
		delete[] upRTV;

		AttachPrimaryViewports();
		AttachPrimaryRTVs();
		mCurrentContext.Apply(); // Reapply the current pipeline
		return true;
	}


	void Renderer::Render(float _deltaTime) {
		if (mQuerySet.Valid()) {
			mQuerySet.Begin();
		}

		float color[4] = { 0.251f, 0.709f, 0.541f, 1 };
		float black[4] = { 0, 0, 0, 0 };

		// Setup the Scene Render Target
		mRendererLock.lock();
		AttachPrimaryRTVs();
		mContext->ClearRenderTargetView(mPostProcessRTV.Get(), color);
		mContext->ClearRenderTargetView(mBloomRTV.Get(), black);
		mContext->ClearRenderTargetView(mAlbedoRTV.Get(), black);
		mContext->ClearRenderTargetView(mPositionRTV.Get(), black);
		mContext->ClearRenderTargetView(mNormalRTV.Get(), black);
		mContext->ClearRenderTargetView(mSpecularRTV.Get(), black);
		mContext->ClearRenderTargetView(mGlowRTV.Get(), black);
		mContext->ClearRenderTargetView(mSuperGlowRTV.Get(), black);
		mContext->ClearDepthStencilView(mDSView.Get(), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
		if (mQuerySet.Valid()) {
			mQuerySet.Query("RTV Clearing");
		}

		if (nullptr == mVrSystem) {
			RenderNoVR(_deltaTime);
		} else {
			// In VR, we need to apply the post processing before we submit textures to the compositor.
			RenderVR(_deltaTime);
		}

		CommandConsole::Instance().Render();

		// Remove the Scene Shader Resource View from the input pipeline, as once the next render
		// call happens, it will be bound to the output pipeline, as well as the input pipeline,
		// which will result in DirectX not setting it as an output.
		ID3D11ShaderResourceView *nullSRV = nullptr;
		mContext->PSSetShaderResources(eTEX_DIFFUSE, 1, &nullSRV);


		mChain->Present(0, 0);
		if (mQuerySet.Valid()) {
			mQuerySet.End();
			SystemLogger::Debug() << "Captured frame " << mQuerySet.GetCurrentIndex() << " of " << mQuerySet.GetSize() << std::endl;
		}
		if (mQuerySet.Ready()) {
			mQuerySet.Display();
			mQuerySet.Clear();
		}
		mRendererLock.unlock();
	}

#pragma endregion Public Functions

#pragma endregion Instance Functions

}