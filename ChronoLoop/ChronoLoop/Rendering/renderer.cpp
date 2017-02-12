//#include "stdafx.h"
#include "renderer.h"
#include <d3d11.h>
#include <openvr.h>
#include <iostream>
#include "Mesh.h"
#include "InputLayoutManager.h"
#include "RenderShape.h"
#include "../../DXTK/DirectXTexP.h"
#include "../../DXTK/ddstextureloader.h"
#include "../Input/VRInputManager.h"
#include "../Input/Controller.h"

using namespace std;

namespace RenderEngine {

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

		return matrixObj.Inverse();
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

	void Renderer::GetMVP(vr::EVREye e, MyBuffer &data, matrix4 world) {
		matrix4 hmd = (Math::FromMatrix(poses[0].mDeviceToAbsoluteTracking));

		matrix4 hmdPos = hmd.Inverse();
		if (e == vr::EVREye::Eye_Left) {
			data.model = Math::MatrixTranspose(world);
			data.view = Math::MatrixTranspose((mEyePosLeft * hmdPos));
			data.projection = Math::MatrixTranspose(mEyeProjLeft);
			data.viewproj = (mEyeProjLeft * mEyePosLeft * hmdPos);
		} else {
			data.model = Math::MatrixTranspose(world);
			data.view = Math::MatrixTranspose((mEyePosRight * hmdPos));
			data.projection = Math::MatrixTranspose(mEyeProjRight);
			data.viewproj = (mEyeProjRight * mEyePosRight * hmdPos);
		}
	}

	void Renderer::UpdateTrackedPositions() {
		vr::VRCompositor()->WaitGetPoses(poses, vr::k_unMaxTrackedDeviceCount, NULL, 0);
	}

	Renderer::Renderer() {}

	Renderer::Renderer::~Renderer() {
		(*mContext)->Release();
		(*mDSView)->Release();
		(*mDepthBuffer)->Release();
		(*mMainView)->Release();
		(*mFactory)->Release();
		(*mChain)->Release();
		(*mDevice)->Release();
		mContext.reset();
		mDSView.reset();
		mDepthBuffer.reset();
		mMainView.reset();
		mFactory.reset();
		mChain.reset();
		mDevice.reset();
	}

	void Renderer::InitializeD3DDevice() {
		UINT flags = 0;
#if _DEBUG
		flags = D3D11_CREATE_DEVICE_DEBUG;
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

		ThrowIfFailed((*mFactory)->CreateSwapChain((*mDevice),
			&scDesc,
			&chain));
		mChain = make_shared<IDXGISwapChain*>(chain);
	}

	void Renderer::InitializeViews(int _width, int _height) {
		ID3D11Resource *bbuffer;
		ID3D11RenderTargetView *rtv;
		ThrowIfFailed((*mChain)->GetBuffer(0, __uuidof(bbuffer), (void**)(&bbuffer)));
		ThrowIfFailed((*mDevice)->CreateRenderTargetView(bbuffer, NULL, &rtv));
		mMainView = make_shared<ID3D11RenderTargetView*>(rtv);

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

	void Renderer::RenderVR() {

	}

	void Renderer::RenderNoVR() {

	}

#pragma endregion Private Functions

#pragma region Public Functions

	void Renderer::AddNode(RenderNode *node) {
		// TODO: Bad. Very Bad! Get the render context from the render shape!
		mRenderSet.AddNode(node, new RenderContext());
	}

	bool Renderer::Initialize(HWND _Window, unsigned int _width, unsigned int _height, bool _vsync, int _fps, bool _fullscreen, float _farPlane, float _nearPlane, vr::IVRSystem * _vrsys) {
		mWindow = make_shared<HWND>(_Window);
		mVrSystem = _vrsys;

		int rtvWidth = _width;
		int rtvHeight = _height;

		if (mVrSystem) {
			uint32_t texWidth, texHeight;
			mVrSystem->GetRecommendedRenderTargetSize(&texWidth, &texHeight);
			std::cout << "According to VR, the view of our headset is " << texWidth << "x" << texHeight << std::endl;
			std::cout << "The screen will probably look bad. We're just using one render target view currently, and it gets set to the VR headset's recommended resolution when it's plugged in. We should account for that later." << std::endl;
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
		InitializeViews(_width, _height);

		mUseVsync = _vsync;



		// Eye gets changd per render
		Mesh cube("../Resources/Cube.obj");
		cube.Invert();
		RenderShape *box = new RenderShape(cube);
		DirectX::ScratchImage img;
		DirectX::Blob *image = new DirectX::Blob;
		UINT flags = 0;
		DirectX::LoadFromWICFile(L"../Resources/cube_texture.png", flags, nullptr, img);
		DirectX::SaveToDDSMemory(*img.GetImage(0, 0, 0), flags, *image);
		ID3D11ShaderResourceView *rtv;
		ID3D11Texture2D *tex;
		DirectX::CreateDDSTextureFromMemory((*mDevice), (const uint8_t*)image->GetBufferPointer(), image->GetBufferSize(), (ID3D11Resource**)&tex, &rtv);
		mTexture = make_shared<ID3D11ShaderResourceView*>(rtv);
		box->SetShaders(ePS_TEXTURED, eVS_TEXTURED);
		AddNode(box);

		mControllerRM = nullptr;

		CD3D11_BUFFER_DESC desc(sizeof(MyBuffer), D3D11_BIND_CONSTANT_BUFFER);
		(*mDevice)->CreateBuffer(&desc, nullptr, &constantBluffer);
		boxPosition = Math::Identity();

		return true;
	}

	void Renderer::Render() {
		float color[4] = { 0.3f, 0.3f, 1, 1 };
		(*mContext)->ClearRenderTargetView((*mMainView), color);
		if (nullptr == mVrSystem) {
			RenderNoVR();
		}
		else {
			RenderVR();
		}
		(*mChain)->Present(mUseVsync ? 1 : 0, 0);
		//matrix4 identity = boxPosition;

		//Controller& rightController = VRInputManager::Instance().GetController(false);
		//Controller& leftController = VRInputManager::Instance().GetController(true);
		//if (rightController.GetPress(vr::k_EButton_SteamVR_Trigger)) {
		//	identity = boxPosition = Math::FromMatrix(poses[rightController.GetIndex()].mDeviceToAbsoluteTracking);
		//}
		//if (leftController.GetPress(vr::k_EButton_SteamVR_Trigger)) {
		//	identity = boxPosition = Math::FromMatrix(poses[leftController.GetIndex()].mDeviceToAbsoluteTracking);
		//}



		//GetMVP(vr::EVREye::Eye_Left, constantData, identity);
		//(*mContext)->UpdateSubresource(constantBluffer, 0, NULL, &constantData, 0, 0);
		//(*mContext)->OMSetRenderTargets(1, mDebugScreen.get(), (*mDSView));
		//(*mContext)->ClearDepthStencilView((*mDSView), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);

		//UINT strideGround = sizeof(VertexPosNormTex);
		//UINT offsetGround = 0;
		//(*mContext)->IASetInputLayout(InputLayoutManager::Instance().GetInputLayout(eVERT_POSNORMTEX)); 
		//(*mContext)->IASetIndexBuffer(((RenderShape*)mRenderSet.GetHead())->mIndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Indicies are shorts
		//(*mContext)->IASetVertexBuffers(0, 1, &((RenderShape*)mRenderSet.GetHead())->mVertexBuffer, &strideGround, &offsetGround);
		//(*mContext)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//(*mContext)->VSSetShader(((RenderShape*)mRenderSet.GetHead())->vShader, nullptr, 0);
		//(*mContext)->VSSetConstantBuffers(0, 1, &constantBluffer);
		///// Pixel Shader(s)
		//(*mContext)->PSSetShader(((RenderShape*)mRenderSet.GetHead())->pShader, nullptr, 0);
		//(*mContext)->PSSetShaderResources(0, 1, mTexture.get());

		//(*mContext)->DrawIndexed((UINT)((RenderShape*)mRenderSet.GetHead())->mIndexCount, 0, 0);

		//if (mVrSystem != nullptr) {
		//	UpdateTrackedPositions();
		//	if (mControllerModel.pShader == nullptr) {
		//			auto status = vr::VRRenderModels()->LoadRenderModel_Async("vr_controller_vive_1_5", &mControllerRM);
		//			if (status == vr::VRRenderModelError_None) {
		//				Mesh controllerMesh;
		//				controllerMesh.Load(mControllerRM);
		//				controllerMesh.Invert();
		//				mControllerModel.Load(controllerMesh);
		//				mControllerModel.LoadShaders("TexturedVertex.cso", "TexturedPixel.cso");
		//			}
		//		}

		//		(*mContext)->OMSetRenderTargets(1, mLeftEye.get(), (*mVRDSView));
		//		(*mContext)->ClearDepthStencilView((*mVRDSView), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
		//		(*mContext)->DrawIndexed((UINT)((RenderShape*)mRenderSet.GetHead())->mIndexCount, 0, 0);

		//		GetMVP(vr::EVREye::Eye_Right, constantData, identity);
		//		(*mContext)->UpdateSubresource(constantBluffer, 0, NULL, &constantData, 0, 0);
		//		(*mContext)->VSSetConstantBuffers(0, 1, &constantBluffer);
		//		(*mContext)->OMSetRenderTargets(1, mRightEye.get(), (*mVRDSView));
		//		(*mContext)->ClearDepthStencilView((*mVRDSView), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
		//		(*mContext)->DrawIndexed((UINT)((RenderShape*)mRenderSet.GetHead())->mIndexCount, 0, 0);





		//		if (mControllerModel.pShader != nullptr) {

		//			// Controller 1
		//			if (leftController.GetIndex() > 0) {
		//				(*mContext)->OMSetRenderTargets(1, mLeftEye.get(), (*mVRDSView));
		//				GetMVP(vr::EVREye::Eye_Left, constantData, Math::FromMatrix(poses[leftController.GetIndex()].mDeviceToAbsoluteTracking));
		//				(*mContext)->UpdateSubresource(constantBluffer, 0, NULL, &constantData, 0, 0);
		//				(*mContext)->IASetIndexBuffer(mControllerModel.mIndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Indicies are shorts
		//				(*mContext)->IASetVertexBuffers(0, 1, &mControllerModel.mVertexBuffer, &strideGround, &offsetGround);

		//				(*mContext)->VSSetShader(mControllerModel.vShader, nullptr, 0);
		//				(*mContext)->VSSetConstantBuffers(0, 1, &constantBluffer);
		//				/// Pixel Shader(s)
		//				(*mContext)->PSSetShader(mControllerModel.pShader, nullptr, 0);
		//				(*mContext)->PSSetShaderResources(0, 1, mTexture.get());

		//				(*mContext)->DrawIndexed((UINT)mControllerModel.mIndexCount, 0, 0);

		//				(*mContext)->OMSetRenderTargets(1, mRightEye.get(), (*mVRDSView));
		//				GetMVP(vr::EVREye::Eye_Right, constantData, Math::FromMatrix(poses[leftController.GetIndex()].mDeviceToAbsoluteTracking));
		//				(*mContext)->UpdateSubresource(constantBluffer, 0, NULL, &constantData, 0, 0);
		//				(*mContext)->IASetIndexBuffer(mControllerModel.mIndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Indicies are shorts
		//				(*mContext)->IASetVertexBuffers(0, 1, &mControllerModel.mVertexBuffer, &strideGround, &offsetGround);

		//				(*mContext)->VSSetShader(mControllerModel.vShader, nullptr, 0);
		//				(*mContext)->VSSetConstantBuffers(0, 1, &constantBluffer);
		//				/// Pixel Shader(s)
		//				(*mContext)->PSSetShader(mControllerModel.pShader, nullptr, 0);
		//				(*mContext)->PSSetShaderResources(0, 1, mTexture.get());

		//				(*mContext)->DrawIndexed((UINT)mControllerModel.mIndexCount, 0, 0);
		//			}

		//			// Controller 1
		//			if (rightController.GetIndex() > 0) {
		//				(*mContext)->OMSetRenderTargets(1, mLeftEye.get(), (*mVRDSView));
		//				GetMVP(vr::EVREye::Eye_Left, constantData, Math::FromMatrix(poses[rightController.GetIndex()].mDeviceToAbsoluteTracking));
		//				(*mContext)->UpdateSubresource(constantBluffer, 0, NULL, &constantData, 0, 0);
		//				(*mContext)->IASetIndexBuffer(mControllerModel.mIndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Indicies are shorts
		//				(*mContext)->IASetVertexBuffers(0, 1, &mControllerModel.mVertexBuffer, &strideGround, &offsetGround);

		//				(*mContext)->VSSetShader(mControllerModel.vShader, nullptr, 0);
		//				(*mContext)->VSSetConstantBuffers(0, 1, &constantBluffer);
		//				/// Pixel Shader(s)
		//				(*mContext)->PSSetShader(mControllerModel.pShader, nullptr, 0);
		//				(*mContext)->PSSetShaderResources(0, 1, mTexture.get());

		//				(*mContext)->DrawIndexed((UINT)mControllerModel.mIndexCount, 0, 0);

		//				(*mContext)->OMSetRenderTargets(1, mRightEye.get(), (*mVRDSView));
		//				GetMVP(vr::EVREye::Eye_Right, constantData, Math::FromMatrix(poses[rightController.GetIndex()].mDeviceToAbsoluteTracking));
		//				(*mContext)->UpdateSubresource(constantBluffer, 0, NULL, &constantData, 0, 0);
		//				(*mContext)->IASetIndexBuffer(mControllerModel.mIndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Indicies are shorts
		//				(*mContext)->IASetVertexBuffers(0, 1, &mControllerModel.mVertexBuffer, &strideGround, &offsetGround);

		//				(*mContext)->VSSetShader(mControllerModel.vShader, nullptr, 0);
		//				(*mContext)->VSSetConstantBuffers(0, 1, &constantBluffer);
		//				/// Pixel Shader(s)
		//				(*mContext)->PSSetShader(mControllerModel.pShader, nullptr, 0);
		//				(*mContext)->PSSetShaderResources(0, 1, mTexture.get());

		//				(*mContext)->DrawIndexed((UINT)mControllerModel.mIndexCount, 0, 0);
		//			}
		//		}


		//	for (int i = 0; i < 2; i++) {
		//		vr::Texture_t tex = { i == 0 ? (void*)(*mLeftTexture) : (void*)(*mRightTexture), vr::TextureType_DirectX, vr::ColorSpace_Auto };
		//		vr::VRCompositor()->Submit(i == 0 ? vr::EVREye::Eye_Left : vr::EVREye::Eye_Right, &tex);
		//	}
		//}



		//(*mChain)->Present(0, 0);
	}

#pragma endregion Public Functions

#pragma endregion Instance Functions

}