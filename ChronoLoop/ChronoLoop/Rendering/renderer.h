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
#include <wrl/client.h>

namespace Epoch {

	class Renderer {
	private:
		struct ViewProjectionBuffer {
			matrix4 view, projection;
		} mVPLeftData, mVPRightData;
		static Renderer* sInstance;
		//TODO: Light buffers
		DirectionalLight mDLData;
		PointLight mPLData;
		SpotLight mSLData;

		// Instance members
		// D3D11 Variables
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mChain;
		Microsoft::WRL::ComPtr<IDXGIFactory1> mFactory;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mMainView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mMainViewTexture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDSView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthBuffer;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
		D3D11_VIEWPORT mLeftViewport, mRightViewport;
		HWND mWindow;

		vr::IVRSystem* mVrSystem;
		RenderSet mRenderSet;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVPBuffer, mPositionBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mDLBuffer, mPLBuffer, mSLBuffer;
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
		void UpdateViewProjection();
		void UpdateGSBuffers();
		void UpdateLBuffers();
		
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
		GhostList<matrix4>::GhostNode* AddNode(RenderShape *_node);
		void Render(float _deltaTime);

		//Draws text in 0 to 1 space

		inline Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return mDevice; }
		inline Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() { return mContext; }
		inline Microsoft::WRL::ComPtr<IDXGISwapChain> GetChain() { return mChain; }
		inline Microsoft::WRL::ComPtr<IDXGIFactory1> GetFactory() { return mFactory; }
		inline Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetRTView() { return mMainView; }
		inline Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDSView() { return mDSView; }
		inline Microsoft::WRL::ComPtr<ID3D11Texture2D> GetRTViewTexture() { return mMainViewTexture; }
		inline HWND GetWindow() { return mWindow; }
	};
}
