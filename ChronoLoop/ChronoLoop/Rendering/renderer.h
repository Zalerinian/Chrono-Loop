#pragma once
#include <d3d11.h>
#include "../Common/Math.h"
#include <memory>
#include <vector>
#include <openvr.h>
#include "Mesh.h"
#include "RenderSet.h"
#include "RenderShape.h"

class InputLayoutManager;

namespace RenderEngine {

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
		D3D11_VIEWPORT mViewport;
		std::shared_ptr<HWND> mWindow;

		vr::IVRSystem* mVrSystem;
		RenderSet mRenderSet;
		std::shared_ptr<ID3D11Buffer*> mVPBuffer, mPositionBuffer;
		RenderShape mControllerModel, mBox, mPlane;
		bool mUseVsync = false;


		// These are instance functions
		void InitializeD3DDevice();
		void InitializeDXGIFactory();
		void InitializeDXGISwapChain(HWND &_win, bool _fullscreen, int _fps,
																	int _width, int _height);
		void InitializeViews(int _width, int _height);
		void InitializeBuffers();
		void InitializeObjectNames();
		void SetStaticBuffers();
		void ThrowIfFailed(HRESULT hr);

		matrix4 mEyePosLeft, mEyePosRight, mEyeProjLeft, mEyeProjRight, mHMDPos, mDebugCameraPos;
		vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];

#if _DEBUG
		// Shit for the debug camera.
		POINT mMouseOrigin;
		bool mIsMouseDown = false;
#endif

		matrix4 GetEye(vr::EVREye e);
		matrix4 GetProjection(vr::EVREye e);
		void GetMVP(vr::EVREye e, ViewProjectionBuffer &data);
		void UpdateTrackedPositions();

		void RenderVR();
		void UpdateCamera(float const moveSpd, float const rotSpd, float delta);
		void RenderNoVR();
		void ProcessRenderSet();

		Renderer();
		~Renderer();
	public:
		static Renderer* Instance();
		static void DestroyInstance();

		// Instance Functions
		bool Initialize(HWND Window, unsigned int width, unsigned int height,
			bool vsync, int fps, bool fullscreen, float farPlane, float nearPlane,
			vr::IVRSystem* vrsys);

		void AddNode(RenderShape *node);
		void Render();
		inline std::shared_ptr<ID3D11Device*> GetDevice() { return mDevice; }
		inline std::shared_ptr<ID3D11DeviceContext*> GetContext() { return mContext; }
		inline std::shared_ptr<IDXGISwapChain*> GetChain() { return mChain; }
		inline std::shared_ptr<IDXGIFactory1*> GetFactory() { return mFactory; }
		inline std::shared_ptr<ID3D11RenderTargetView*> GetRTView() { return mMainView; }
		inline std::shared_ptr<ID3D11DepthStencilView*> GetDSView() { return mDSView; }
	};

}