#pragma once
#include <d3d11.h>
#include <memory>
#include <vector>
#include <openvr.h>

struct RenderNode;

class Renderer {

	class RendererInstance {
		friend Renderer;

		// D3D11 Variables
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
		RendererInstance();
		~RendererInstance();

	public:
		void Render();
	};
	static RendererInstance* sInstance;

	static void InitializeD3DDevice();
	static void InitializeDXGIFactory();
	static void InitializeDXGISwapChain(HWND &_win, bool _fullscreen, int _fps,
																			int _width, int _height);
	static void InitializeViews(int _width, int _height);
	static void ThrowIfFailed(HRESULT hr);

	Renderer();
	~Renderer();
public:
	static RendererInstance* Instance();
	static void DestroyInstance();
	static bool Initialize(HWND Window, unsigned int width, unsigned int height,
												 bool vsync, int fps, bool fullscreen, float farPlane, float nearPlane,
												 vr::IVRSystem* vrsys);
};