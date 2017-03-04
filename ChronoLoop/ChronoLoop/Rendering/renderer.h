#pragma once
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include "../Common/Math.h"
#include <memory>
#include <vector>
#include <openvr.h>
#include <d2d1_1.h>
#include "Mesh.h"
#include "RenderSet.h"
#include "RenderShape.h"

class InputLayoutManager;

namespace Epoch {

	class Renderer {
	private:
		struct ViewProjectionBuffer {
			matrix4 view, projection;
		} mVPData;


		static Renderer* sInstance;

		friend InputLayoutManager;
		// Instance members
		// D3D11 Variables
		std::shared_ptr<ID3D11Device*> mDevice;
		std::shared_ptr<ID3D11DeviceContext*> mContext;
		std::shared_ptr<IDXGISwapChain*> mChain;
		std::shared_ptr<IDXGIFactory1*> mFactory;
		std::shared_ptr<ID3D11RenderTargetView*> mMainView;
		std::shared_ptr<ID3D11Texture2D*> mMainViewTexture;
		std::shared_ptr<ID3D11DepthStencilView*> mDSView;
		std::shared_ptr<ID3D11Texture2D*> mDepthBuffer;
		std::shared_ptr<ID3D11SamplerState*> mSamplerState;
		D3D11_VIEWPORT mViewport;
		std::shared_ptr<HWND> mWindow;

		vr::IVRSystem* mVrSystem;
		RenderSet mRenderSet;
		std::shared_ptr<ID3D11Buffer*> mVPBuffer, mPositionBuffer;
		bool mUseVsync = false;


		//Pat Added
		//DirectWrite Drawing componets

		void InitializeD3DDevice();
		void InitializeDXGIFactory();
		void InitializeDXGISwapChain(HWND &_win, bool _fullscreen, int _fps,
																	int _width, int _height);
		int mFps = 0;
		float mFrameTime = 0;

		void InitializeViews(int _width, int _height);
		void InitializeBuffers();
		void InitializeSamplerState();
		void InitializeObjectNames();
		void SetStaticBuffers();

		matrix4 mEyePosLeft, mEyePosRight, mEyeProjLeft, mEyeProjRight, mHMDPos, mDebugCameraPos;

		POINT mMouseOrigin;
		bool mIsMouseDown = false;

		matrix4 GetEye(vr::EVREye e);
		matrix4 GetProjection(vr::EVREye e);
		void GetMVP(vr::EVREye e, ViewProjectionBuffer &data);
		
		void RenderVR(float _delta);
		void UpdateCamera(float const moveSpd, float const rotSpd, float delta);
		void RenderNoVR(float _delta);
		void ProcessRenderSet();

		Renderer();
		~Renderer();
	public:
		static Renderer* Instance();
		static void DestroyInstance();
		// Instance Functions
		bool iInitialize(HWND Window, unsigned int width, unsigned int height,
			bool vsync, int fps, bool fullscreen, float farPlane, float nearPlane,
			vr::IVRSystem* vrsys);
		void ThrowIfFailed(HRESULT hr);
		void AddNode(RenderShape *_node);
		void RemoveNode(RenderShape *_node);
		void Render(float _deltaTime);

		//Draws text in 0 to 1 space

		inline std::shared_ptr<ID3D11Device*> iGetDevice() { return mDevice; }
		inline std::shared_ptr<ID3D11DeviceContext*> iGetContext() { return mContext; }
		inline std::shared_ptr<IDXGISwapChain*> iGetChain() { return mChain; }
		inline std::shared_ptr<IDXGIFactory1*> iGetFactory() { return mFactory; }
		inline std::shared_ptr<ID3D11RenderTargetView*> iGetRTView() { return mMainView; }
		inline std::shared_ptr<ID3D11DepthStencilView*> iGetDSView() { return mDSView; }
		inline std::shared_ptr<ID3D11Texture2D*> iGetRTViewTexture() { return mMainViewTexture; }
		inline std::shared_ptr<HWND> iGetWindow() { return mWindow; }
	};
}
