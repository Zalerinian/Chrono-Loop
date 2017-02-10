#pragma once
#include <d3d11.h>
#include <memory>
#include <vector>
#include <openvr.h>
#include "RenderContext.h"
#include "Mesh.h"
#include "../Common/Math.h"
#include "RenderSet.h"
#include "RenderShape.h"

class InputLayoutManager;

namespace RenderEngine {

	class Renderer {
	private:
		struct MyBuffer {
			matrix4 model, view, projection;
		} constantData;

		friend InputLayoutManager;
		// D3D11 Variables
		// Instance members
		std::shared_ptr<ID3D11Device*> mDevice;
		std::shared_ptr<ID3D11DeviceContext*> mContext;
		std::shared_ptr<IDXGISwapChain*> mChain;
		std::shared_ptr<IDXGIFactory1*> mFactory;
		std::shared_ptr<ID3D11RenderTargetView*> mDebugScreen, mLeftEye, mRightEye;
		std::shared_ptr<ID3D11ShaderResourceView*> mTexture;
		std::shared_ptr<ID3D11Texture2D*> mLeftTexture, mRightTexture;
		std::shared_ptr<ID3D11DepthStencilView*> mDSView, mVRDSView;
		std::shared_ptr<ID3D11Texture2D*> mDepthBuffer;
		D3D11_VIEWPORT mViewport;
		vr::IVRSystem* mVrSystem;
		std::shared_ptr<HWND> mWindow;
		RenderContext mCurrentContext;
		RenderSet mRenderSet;
		ID3D11Buffer* constantBluffer;
		RenderShape mControllerModel;
		vr::RenderModel_t *mControllerRM;

		static Renderer* sInstance;

		void InitializeD3DDevice();
		void InitializeDXGIFactory();
		void InitializeDXGISwapChain(HWND &_win, bool _fullscreen, int _fps,
																	int _width, int _height);
		void InitializeViews(int _width, int _height);
		void ThrowIfFailed(HRESULT hr);


		void DoAllTheBootlegThingsForTheDemo();

		matrix4 mEyePosLeft, mEyePosRight, mEyeProjLeft, mEyeProjRight, mHMDPos;
		vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];	

		matrix4 GetEye(vr::EVREye e);
		matrix4 GetProjection(vr::EVREye e);
		void GetMVP(vr::EVREye e, MyBuffer &data, matrix4 world);
		void UpdateTrackedPositions();

		void RenderVR();
		void RenderNoVR();

		Renderer();
		~Renderer();
	public:
		static Renderer* Instance();
		static void DestroyInstance();

		// Instance Functions
		bool Initialize(HWND Window, unsigned int width, unsigned int height,
			bool vsync, int fps, bool fullscreen, float farPlane, float nearPlane,
			vr::IVRSystem* vrsys);

		void AddNode(RenderNode *node);
		void Render();
		std::shared_ptr<ID3D11Device*> GetDevice();
		std::shared_ptr<ID3D11DeviceContext*> GetContext();
		std::shared_ptr<IDXGISwapChain*> GetChain();
		std::shared_ptr<IDXGIFactory1*> GetFactory();
		std::shared_ptr<ID3D11RenderTargetView*> GetRTView();
		std::shared_ptr<ID3D11DepthStencilView*> GetDSView();
	};

}