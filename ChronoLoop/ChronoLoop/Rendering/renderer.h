#pragma once
#include <d3d11.h>
#include <memory>
#include <vector>
#include <openvr.h>

namespace RenderEngine {

	struct RenderNode;
	
	class Renderer {
	
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
	
		// Static members
		static Renderer* sInstance;
		static void InitializeD3DDevice();
		static void InitializeDXGIFactory();
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