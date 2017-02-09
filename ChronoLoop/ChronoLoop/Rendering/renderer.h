#pragma once
#include <d3d11.h>
#include <memory>
#include <vector>
#include <openvr.h>
#include "RenderContext.h"
#include "Mesh.h"
#include "../Common/Math.h"

class InputLayoutManager;

namespace RenderEngine {
	
	class Renderer {
		struct MyBuffer {
			matrix4 model, view, eye, projection;
		} constantData;

		friend InputLayoutManager;
		// D3D11 Variables
		// Instance members
		std::shared_ptr<ID3D11Device*> mDevice;
		std::shared_ptr<ID3D11DeviceContext*> mContext;
		std::shared_ptr<IDXGISwapChain*> mChain;
		std::shared_ptr<IDXGIFactory1*> mFactory;
		std::shared_ptr<ID3D11RenderTargetView*> mDebugScreen, mLeftEye, mRightEye;
		std::shared_ptr<ID3D11Texture2D*> mLeftTexture, mRightTexture;
		std::shared_ptr<ID3D11DepthStencilView*> mDSView, mVRDSView;
		std::shared_ptr<ID3D11Texture2D*> mDepthBuffer;
		D3D11_VIEWPORT mViewport;
		vr::IVRSystem* mVrSystem;
		std::shared_ptr<HWND> mWindow;
		RenderContext mCurrentContext;
		std::vector<RenderNode*> mNodes;
		std::vector<Mesh> mMeshes;
		ID3D11Buffer* constantBluffer, *boxVertex, *boxIndex;
	
		static Renderer* sInstance;

		void InitializeD3DDevice();
		void InitializeDXGIFactory();
		void InitializeDXGISwapChain(HWND &_win, bool _fullscreen, int _fps,
																	int _width, int _height);
		void InitializeViews(int _width, int _height);
		void ThrowIfFailed(HRESULT hr);


		void DoAllTheBootlegThingsForTheDemo();

	
		Renderer();
		~Renderer();
	public:
		static Renderer* Instance();
		static void DestroyInstance();
	
	
		// Instance Functions
		bool Initialize(HWND Window, unsigned int width, unsigned int height,
			bool vsync, int fps, bool fullscreen, float farPlane, float nearPlane,
			vr::IVRSystem* vrsys);

		void Render();
		std::shared_ptr<ID3D11Device*> GetDevice();
		std::shared_ptr<ID3D11DeviceContext*> GetContext();
		std::shared_ptr<IDXGISwapChain*> GetChain();
		std::shared_ptr<IDXGIFactory1*> GetFactory();
		std::shared_ptr<ID3D11RenderTargetView*> GetRTView();
		std::shared_ptr<ID3D11DepthStencilView*> GetDSView();
	};

}