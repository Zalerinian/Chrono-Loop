#include "stdafx.h"
#include "renderer.h"
#include <d3d11.h>
#include<d2d1_1.h>
#include <dxgi1_2.h>

using namespace std;
using namespace D2D1;

namespace RenderEngine {

	Renderer* Renderer::sInstance = nullptr;
	
	#pragma region Singleton Functions
	
	Renderer::Renderer() {}
	
	Renderer::Renderer::~Renderer() {
		(*mContext)->Release();
		(*mDSView)->Release();
		(*mDepthBuffer)->Release();
		(*mRTView)->Release();
		(*mFactory)->Release();
		(*mChain)->Release();
		(*mDevice)->Release();
		mContext.reset();
		mDSView.reset();
		mDepthBuffer.reset();
		mRTView.reset();
		mFactory.reset();
		mChain.reset();
		mDevice.reset();
	}
	
	void Renderer::InitializeD3DDevice() {
		UINT flags = 0;
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
	
		sInstance->mDevice = make_shared<ID3D11Device*>(dev);
		sInstance->mContext = make_shared<ID3D11DeviceContext*>(ctx);
	}
	
	void Renderer::InitializeDXGIFactory() {
		IDXGIFactory1 *factory;
		ThrowIfFailed(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory));
		sInstance->mFactory = make_shared<IDXGIFactory1*>(factory);

	}

	void Renderer::InitializeIDWriteFactory()
	{
		HRESULT hr;
		//create 2dfactory
		ID2D1Factory1 * factory2;
		ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory2));
		sInstance->mTextFactory = make_shared<ID2D1Factory1*>(factory2);

		//createDxgiDevice
		IDXGIDevice* DxgiDevice;
		ThrowIfFailed((*sInstance->GetDevice())->QueryInterface(__uuidof(IDXGIDevice), (void **)&DxgiDevice));
		sInstance->mGIDevice = make_shared<IDXGIDevice*>(DxgiDevice);

		//create device2d 
		ID2D1Device* Device2d;
		ThrowIfFailed(factory2->CreateDevice(*sInstance->mGIDevice, &Device2d));
		sInstance->mDevice2D = make_shared<ID2D1Device*>(Device2d);

		//create context
		ID2D1DeviceContext* context2d;
		ThrowIfFailed((*sInstance->mDevice2D)->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &context2d));
		sInstance->mContext2D = make_shared<ID2D1DeviceContext*>(context2d);


	/*	IDWriteFactory* WriteFactory;
		ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(WriteFactory), reinterpret_cast<IUnknown**>(&WriteFactory)));
		sInstance->mDWrite = make_shared<IDWriteFactory*>(WriteFactory);

		static const WCHAR fontName[] = L"Verdana";
		static const FLOAT fontSize = 50;

		IDWriteTextFormat* WriteFormat;
		ThrowIfFailed(WriteFactory->CreateTextFormat(
			fontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			fontSize,
			L"en-us",
			&WriteFormat));
		sInstance->mTextformat = make_shared<IDWriteTextFormat*>(WriteFormat);

		WriteFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		WriteFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		
		D2D1_PIXEL_FORMAT PF = {DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_IGNORE};

		D2D1_RENDER_TARGET_PROPERTIES RTProp = {
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			PF,
			0,
			0,
			D2D1_RENDER_TARGET_USAGE_NONE,
			D2D1_FEATURE_LEVEL_DEFAULT
		};

		ID2D1DCRenderTarget* rt;
		ThrowIfFailed(factory2->CreateDCRenderTarget(&RTProp, &rt));
		sInstance->m2DRenderTarget = make_shared <ID2D1DCRenderTarget*>(rt);
	
		ID2D1SolidColorBrush* brush;
		ThrowIfFailed(rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White,1.0f),&brush));
		sInstance->mBrush = make_shared<ID2D1SolidColorBrush*>(brush);*/

		
	

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
	
		ThrowIfFailed((*sInstance->mFactory)->CreateSwapChain((*sInstance->mDevice),
																&scDesc,
																&chain));
		sInstance->mChain = make_shared<IDXGISwapChain*>(chain);
	}
	
	void Renderer::InitializeViews(int _width, int _height) {
		ID3D11Resource *bbuffer;
		ID3D11RenderTargetView *rtv;
		ThrowIfFailed((*sInstance->mChain)->GetBuffer(0, __uuidof(bbuffer), (void**)(&bbuffer)));
		ThrowIfFailed((*sInstance->mDevice)->CreateRenderTargetView(bbuffer, NULL, &rtv));
		sInstance->mRTView = make_shared<ID3D11RenderTargetView*>(rtv);
	
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
		ThrowIfFailed((*sInstance->mDevice)->CreateTexture2D(&depthStencilDesc, NULL, &depthTexture));
		ThrowIfFailed((*sInstance->mDevice)->CreateDepthStencilView(depthTexture, NULL, &depthView));
		(*sInstance->mContext)->OMSetRenderTargets(1, &rtv, depthView);
	
		sInstance->mDepthBuffer = make_shared<ID3D11Texture2D*>(depthTexture);
		sInstance->mDSView = make_shared<ID3D11DepthStencilView*>(depthView);
	
	
		// Viewport
		DXGI_SWAP_CHAIN_DESC scd;
		(*sInstance->mChain)->GetDesc(&scd);
		sInstance->mViewport.Width = (FLOAT)scd.BufferDesc.Width;
		sInstance->mViewport.Height = (FLOAT)scd.BufferDesc.Height;
		sInstance->mViewport.MinDepth = 0;
		sInstance->mViewport.MaxDepth = 1;
		sInstance->mViewport.TopLeftX = 0;
		sInstance->mViewport.TopLeftY = 0;
		(*sInstance->mContext)->RSSetViewports(1, &sInstance->mViewport);
	}
	
	void Renderer::ThrowIfFailed(HRESULT hr) {
		if (FAILED(hr)) {
			throw "Something has gone catastrophically wrong!";
		}
	}
	
	Renderer* Renderer::Instance() {
		return sInstance;
	}
	
	void Renderer::DestroyInstance() {
		if (sInstance != nullptr) {
			delete sInstance;
		}
		sInstance = nullptr;
	}
	
	bool Renderer::Initialize(HWND _Window, unsigned int _width, unsigned int _height, bool _vsync, int _fps, bool _fullscreen, float _farPlane, float _nearPlane, vr::IVRSystem * _vrsys) {
		if (nullptr != sInstance) {
			return false;
		}
		sInstance = new Renderer();
		sInstance->mWindow = make_shared<HWND>(_Window);
	
		InitializeD3DDevice();
		InitializeDXGIFactory();
		//PatAdded
		InitializeIDWriteFactory();
		InitializeDXGISwapChain(_Window, _fullscreen, _fps, _width, _height);
		InitializeViews(_width, _height);
		return true;
	}
	
	#pragma endregion Singleton Functions
	
	#pragma region Instance Functions
	
	void Renderer::Render() {
		float color[4] = { 0.3f, 0.3f, 1, 1 };
		(*mContext)->ClearRenderTargetView((*mRTView), color);
		(*mContext)->OMSetRenderTargets(1, &(*mRTView), *mDSView);

		D2D1_TAG t1 = NULL;
		D2D1_TAG t2 = NULL;

		static const WCHAR sc_helloWorld[] = L"Hello, World!";

		
		// Retrieve the size of the render target.
		D2D1_SIZE_F renderTargetSize = (*m2DRenderTarget)->GetSize();

		(*m2DRenderTarget)->BeginDraw();
		
		(*m2DRenderTarget)->SetTransform(D2D1::Matrix3x2F::Identity());

		(*m2DRenderTarget)->Clear(D2D1::ColorF(D2D1::ColorF::White));

		//(*m2DRenderTarget)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		//(*m2DRenderTarget)->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
		

		(*m2DRenderTarget)->DrawText(
			sc_helloWorld,
			ARRAYSIZE(sc_helloWorld) - 1,
			(*mTextformat),
			D2D1::RectF(0, 0, renderTargetSize.width, renderTargetSize.height),
			(*mBrush)
		);

		HRESULT hr;
		hr = (*m2DRenderTarget)->EndDraw(&t1,&t2);


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
		return mRTView;
	}

	std::shared_ptr<ID3D11DepthStencilView*> Renderer::GetDSView() {
		return mDSView;
	}

	#pragma endregion Instance Functions

}