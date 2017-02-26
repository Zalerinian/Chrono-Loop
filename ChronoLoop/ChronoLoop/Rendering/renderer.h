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
		std::shared_ptr<ID3D11SamplerState*> mSamplerState;
		D3D11_VIEWPORT mViewport;
		std::shared_ptr<HWND> mWindow;

		vr::IVRSystem* mVrSystem;
		RenderSet mRenderSet;
		std::shared_ptr<ID3D11Buffer*> mVPBuffer, mPositionBuffer;
		bool mUseVsync = false;


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
		std::shared_ptr<ID2D1Bitmap1*>mScreenBitmap;

		void InitializeD3DDevice();
		void InitializeDXGIFactory();
		void InitializeDXGISwapChain(HWND &_win, bool _fullscreen, int _fps,
																	int _width, int _height);
		void InitializeIDWriteFactory();
		void InitializeDirect2D();
		void InitializeScreenBitmap();
		int mFps = 0;
		float mFrameTime = 0;

		void InitializeViews(int _width, int _height);
		void InitializeBuffers();
		void InitializeSamplerState();
		void InitializeObjectNames();
		void SetStaticBuffers();
		void ThrowIfFailed(HRESULT hr);

		matrix4 mEyePosLeft, mEyePosRight, mEyeProjLeft, mEyeProjRight, mHMDPos, mDebugCameraPos;

#if _DEBUG
		POINT mMouseOrigin;
		bool mIsMouseDown = false;
#endif

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
		matrix4* Renderer::GetPlayerWorldPos();
		// Instance Functions
		bool iInitialize(HWND Window, unsigned int width, unsigned int height,
			bool vsync, int fps, bool fullscreen, float farPlane, float nearPlane,
			vr::IVRSystem* vrsys);

		void AddNode(RenderShape *_node);
		void RemoveNode(RenderShape *_node);
		void Render(float _deltaTime);

		//Draws text in 0 to 1 space
		
		void DrawTextToBitmap(std::wstring _text, ID2D1Bitmap* _bitmap,float _topLeftx, float _topLefty, float _bottomRightx, float _bottomRighty);
		ID2D1Bitmap1* CreateBitmapForTexture(ID3D11Texture2D* _texture);

		inline std::shared_ptr<ID3D11Device*> iGetDevice() { return mDevice; }
		inline std::shared_ptr<ID3D11DeviceContext*> iGetContext() { return mContext; }
		inline std::shared_ptr<IDXGISwapChain*> iGetChain() { return mChain; }
		inline std::shared_ptr<IDXGIFactory1*> iGetFactory() { return mFactory; }
		inline std::shared_ptr<ID3D11RenderTargetView*> iGetRTView() { return mMainView; }
		inline std::shared_ptr<ID3D11DepthStencilView*> iGetDSView() { return mDSView; }
		inline std::shared_ptr<HWND> iGetWindow() { return mWindow; }
	};

}