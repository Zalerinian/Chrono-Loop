#pragma once
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <memory>
#include <vector>
#include <openvr.h>
#include <d2d1_1.h>

class InputLayoutManager;

namespace RenderEngine {

	struct RenderNode;
	
	class Renderer {
		friend InputLayoutManager;
		// D3D11 Variables
		// Instance members
		std::shared_ptr<ID3D11Device*> mDevice;
		std::shared_ptr<ID3D11DeviceContext*> mContext;
		std::shared_ptr<IDXGISwapChain*> mChain;
		std::shared_ptr<IDXGIFactory1*> mFactory;
		std::shared_ptr<ID3D11RenderTargetView*> mRTView;
		std::shared_ptr<ID3D11DepthStencilView*> mDSView;
		std::shared_ptr<ID3D11Texture2D*> mDepthBuffer;
		D3D11_VIEWPORT mViewport;
		std::shared_ptr<HWND> mWindow;
		std::vector<RenderNode*> mNodes;

		//Pat Added
		//DirectWrite Drawing componets
		std::shared_ptr<ID2D1Factory1*> mTextFactory;
		std::shared_ptr<ID2D1Device*> mDevice2D;
		std::shared_ptr<IDXGIDevice*> mGIDevice;
		std::shared_ptr<ID2D1DeviceContext*>mContext2D;
		std::shared_ptr<IDWriteFactory*> mDWrite;
		std::shared_ptr<IDWriteTextFormat*>mTextformat;
		std::shared_ptr<ID2D1DCRenderTarget*> m2DRenderTarget;
		std::shared_ptr<ID2D1SolidColorBrush*>mBrush;
	
		// Static members
		static Renderer* sInstance;
		static void InitializeD3DDevice();
		static void InitializeDXGIFactory();
		//PatAdded
		static void InitializeIDWriteFactory();

		static void InitializeDXGISwapChain(HWND &_win, bool _fullscreen, int _fps,
																				int _width, int _height);
		static void InitializeViews(int _width, int _height);
		static void ThrowIfFailed(HRESULT hr);
	
		Renderer();
		~Renderer();
	public:
		static Renderer* Instance();
		static void DestroyInstance();
		static bool Initialize(HWND Window, unsigned int width, unsigned int height,
													 bool vsync, int fps, bool fullscreen, float farPlane, float nearPlane,
													 vr::IVRSystem* vrsys);
	
	
		// Instance Functions
		void Render();
		std::shared_ptr<ID3D11Device*> GetDevice();
		std::shared_ptr<ID3D11DeviceContext*> GetContext();
		std::shared_ptr<IDXGISwapChain*> GetChain();
		std::shared_ptr<IDXGIFactory1*> GetFactory();
		std::shared_ptr<ID3D11RenderTargetView*> GetRTView();
		std::shared_ptr<ID3D11DepthStencilView*> GetDSView();
	};

}