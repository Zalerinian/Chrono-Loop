#include "stdafx.h"
#include "renderer.h"
#include <d3d11.h>
#include <openvr.h>
#include <iostream>
#include "Mesh.h"
#include "InputLayoutManager.h"
#include "RenderShape.h"

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

	void Renderer::ThrowIfFailed(HRESULT hr) {
		if (FAILED(hr)) {
			throw "Something has gone catastrophically wrong!";
		}
	}

	void Renderer::DoAllTheBootlegThingsForTheDemo()
	{
		
	}

	Renderer::Renderer() {}

	Renderer::Renderer::~Renderer() {
		(*mContext)->Release();
		(*mDSView)->Release();
		(*mDepthBuffer)->Release();
		if (mDebugScreen.get() != nullptr) {
			(*mDebugScreen)->Release();
		}
		if (mLeftEye.get() != nullptr) {
			(*mLeftEye)->Release();
		}
		if (mRightEye.get() != nullptr) {
			(*mRightEye)->Release();
		}
		(*mFactory)->Release();
		(*mChain)->Release();
		(*mDevice)->Release();
		mContext.reset();
		mDSView.reset();
		mDepthBuffer.reset();
		mDebugScreen.reset();
		mLeftEye.reset();
		mRightEye.reset();
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
		scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
		mDebugScreen = make_shared<ID3D11RenderTargetView*>(rtv);

		if (mVrSystem != nullptr) {

			uint32_t texWidth, texHeight;
			mVrSystem->GetRecommendedRenderTargetSize(&texWidth, &texHeight);

			std::cout << "According to VR, the view of our headset is " << texWidth << "x" << texHeight << std::endl;

			D3D11_TEXTURE2D_DESC TextureDescription;
			memset(&TextureDescription, 0, sizeof(TextureDescription));
			TextureDescription.Width = texWidth;
			TextureDescription.Height = texHeight;
			TextureDescription.MipLevels = 1;
			TextureDescription.ArraySize = 1;
			TextureDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			TextureDescription.SampleDesc.Count = 1;
			TextureDescription.Usage = D3D11_USAGE_DEFAULT;
			TextureDescription.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

			ID3D11Texture2D *leftTex, *rightTex;
			ThrowIfFailed((*mDevice)->CreateTexture2D(&TextureDescription, nullptr, &leftTex));
			ThrowIfFailed((*mDevice)->CreateTexture2D(&TextureDescription, nullptr, &rightTex));
			mLeftTexture = make_shared<ID3D11Texture2D*>(leftTex);
			mRightTexture = make_shared<ID3D11Texture2D*>(rightTex);

			ID3D11RenderTargetView *leftView, *rightView;
			ThrowIfFailed((*mDevice)->CreateRenderTargetView(leftTex, NULL, &leftView));
			ThrowIfFailed((*mDevice)->CreateRenderTargetView(rightTex, NULL, &rightView));
			mLeftEye = make_shared<ID3D11RenderTargetView*>(leftView);
			mRightEye = make_shared<ID3D11RenderTargetView*>(rightView);

			D3D11_TEXTURE2D_DESC depthStencilDesc;
			depthStencilDesc.Width = texWidth;
			depthStencilDesc.Height = texHeight;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.ArraySize = 1;
			depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
			depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
			depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilDesc.CPUAccessFlags = 0;
			depthStencilDesc.MiscFlags = 0;
			ID3D11Texture2D *depthTexture;
			ID3D11DepthStencilView *depthView;
			ThrowIfFailed((*mDevice)->CreateTexture2D(&depthStencilDesc, NULL, &depthTexture));
			ThrowIfFailed((*mDevice)->CreateDepthStencilView(depthTexture, NULL, &depthView));
			mVRDSView = make_shared<ID3D11DepthStencilView*>(depthView);

		}
		else {
			std::cout << "What's a VR?" << std::endl;
		}

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

	bool Renderer::Initialize(HWND _Window, unsigned int _width, unsigned int _height, bool _vsync, int _fps, bool _fullscreen, float _farPlane, float _nearPlane, vr::IVRSystem * _vrsys) {
		mWindow = make_shared<HWND>(_Window);
		mVrSystem = _vrsys;

		if (_vrsys) {
			for (unsigned int i = 0; i < vr::VRRenderModels()->GetRenderModelCount(); ++i) {
				char name[1024];
				vr::VRRenderModels()->GetRenderModelName(i, name, 1024);
				std::cout << "We've got a nice little " << name << " model here (" << i << ")" << std::endl;
			}
		}

		InitializeD3DDevice();
		InitializeDXGIFactory();
		InitializeDXGISwapChain(_Window, _fullscreen, _fps, _width, _height);
		InitializeViews(_width, _height);

		constantData.projection = Math::MatrixTranspose(Math::Projection((float)_width / (float)_height, 180, _nearPlane, _farPlane));
		constantData.view = Math::MatrixTranspose(Math::MatrixTranslation(0, -1, 5)).Inverse();
		constantData.model.matrix = DirectX::XMMatrixIdentity();
		// Eye gets changd per render
		RenderShape *box = new RenderShape(Mesh("../Resources/Box.obj"));
		box->LoadShaders("BasicVertexShader.cso", "BasicPixelShader.cso");
		AddNode(box);

		CD3D11_BUFFER_DESC desc(sizeof(MyBuffer), D3D11_BIND_CONSTANT_BUFFER);
		(*mDevice)->CreateBuffer(&desc, nullptr, &constantBluffer);

		return true;
	}

	void Renderer::AddNode(RenderNode *node) {
		mRenderSet.AddNode(node);
	}

	void Renderer::Render() {
		float color[4] = { 0.3f, 0.3f, 1, 1 };
		(*mContext)->ClearRenderTargetView((*mDebugScreen), color);
		if (mVrSystem != nullptr) {
			(*mContext)->ClearRenderTargetView((*mLeftEye), color);
			(*mContext)->ClearRenderTargetView((*mRightEye), color);
		}


		matrix4 temp;
		if (mVrSystem) {
			temp = Math::FromMatrix(mVrSystem->GetEyeToHeadTransform(vr::EVREye::Eye_Left));
		} else {
			temp = Math::Identity();
		}

		constantData.eye.matrix = temp.Inverse().matrix;
		(*mContext)->UpdateSubresource(constantBluffer, 0, NULL, &constantData, 0, 0);
		(*mContext)->OMSetRenderTargets(1, mDebugScreen.get(), (*mDSView));
		(*mContext)->ClearDepthStencilView((*mDSView), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);

		UINT strideGround = sizeof(VertexPos);
		UINT offsetGround = 0;
		(*mContext)->IASetInputLayout(InputLayoutManager::Instance().GetInputLayout(eVERT_POS)); 
		(*mContext)->IASetIndexBuffer(((RenderShape*)mRenderSet.GetHead())->mIndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Indicies are shorts
		(*mContext)->IASetVertexBuffers(0, 1, &((RenderShape*)mRenderSet.GetHead())->mVertexBuffer, &strideGround, &offsetGround);
		(*mContext)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		(*mContext)->VSSetShader(((RenderShape*)mRenderSet.GetHead())->vShader, nullptr, 0);
		(*mContext)->VSSetConstantBuffers(0, 1, &constantBluffer);
		/// Pixel Shader(s)
		(*mContext)->PSSetShader(((RenderShape*)mRenderSet.GetHead())->pShader, nullptr, 0);

		(*mContext)->DrawIndexed((UINT)((RenderShape*)mRenderSet.GetHead())->mIndexCount, 0, 0);

		if (mVrSystem != nullptr) {
			if (mVrSystem) {
				(*mContext)->OMSetRenderTargets(1, mLeftEye.get(), (*mVRDSView));
				(*mContext)->ClearDepthStencilView((*mVRDSView), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
				(*mContext)->DrawIndexed((UINT)((RenderShape*)mRenderSet.GetHead())->mIndexCount, 0, 0);

				temp = Math::FromMatrix(mVrSystem->GetEyeToHeadTransform(vr::EVREye::Eye_Left));
				constantData.eye.matrix = temp.Inverse().matrix;
				(*mContext)->UpdateSubresource(constantBluffer, 0, NULL, &constantData, 0, 0);
				(*mContext)->VSSetConstantBuffers(0, 1, &constantBluffer);
				(*mContext)->OMSetRenderTargets(1, mRightEye.get(), (*mVRDSView));
				(*mContext)->ClearDepthStencilView((*mVRDSView), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
				(*mContext)->DrawIndexed((UINT)((RenderShape*)mRenderSet.GetHead())->mIndexCount, 0, 0);
			}


			vr::TrackedDevicePose_t pose;
			mVrSystem->GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin::TrackingUniverseStanding,
				0,
				&pose,
				1);

			vr::VRCompositor()->CompositorBringToFront();
			vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
			auto e = vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, nullptr, 0);
			for (int i = 0; i < 2; i++) {
				vr::Texture_t tex = { i == 0 ? (void*)(*mLeftTexture) : (void*)(*mRightTexture), vr::TextureType_DirectX, vr::ColorSpace_Auto };
				vr::VRCompositor()->Submit(i == 0 ? vr::EVREye::Eye_Left : vr::EVREye::Eye_Right, &tex);
			}
		}



		(*mChain)->Present(0, 0);
	}

	std::shared_ptr<ID3D11Device*> Renderer::GetDevice() {
		return mDevice;
	}

	std::shared_ptr<ID3D11DeviceContext*> Renderer::GetContext() {
		return mContext;
	}

	std::shared_ptr<IDXGISwapChain*> Renderer::GetChain() {
		return mChain;
	}

	std::shared_ptr<IDXGIFactory1*> Renderer::GetFactory() {
		return mFactory;
	}

	std::shared_ptr<ID3D11RenderTargetView*> Renderer::GetRTView() {
		return mDebugScreen;
	}

	std::shared_ptr<ID3D11DepthStencilView*> Renderer::GetDSView() {
		return mDSView;
	}

#pragma endregion Instance Functions

}