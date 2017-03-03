//#include "stdafx.h"
#include "RasterizerStateManager.h"
#include "Renderer.h"

namespace Epoch {

	RasterizerStateManager* RasterizerStateManager::sInstance = nullptr;

	RasterizerStateManager::RasterizerStateManager() {
		mStates = new ID3D11RasterizerState*[eRS_MAX];
		mCurrentState = eRS_MAX;

		D3D11_RASTERIZER_DESC desc;

		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = FALSE;
		desc.DepthBias = 0;
		desc.SlopeScaledDepthBias = 0.0f;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = TRUE;
		desc.ScissorEnable = FALSE;
		desc.MultisampleEnable = FALSE;
		desc.AntialiasedLineEnable = FALSE;

		ID3D11RasterizerState *state;
		(*Renderer::Instance()->iGetDevice())->CreateRasterizerState(&desc, &state);
		mStates[eRS_FILLED] = state;

		desc.FillMode = D3D11_FILL_WIREFRAME;
		(*Renderer::Instance()->iGetDevice())->CreateRasterizerState(&desc, &state);
		mStates[eRS_WIREFRAME] = state;

		desc.FrontCounterClockwise = TRUE;
		(*Renderer::Instance()->iGetDevice())->CreateRasterizerState(&desc, &state);
		mStates[eRS_CCW_WIREFRAME] = state;

		desc.FillMode = D3D11_FILL_SOLID;
		(*Renderer::Instance()->iGetDevice())->CreateRasterizerState(&desc, &state);
		mStates[eRS_CCW] = state;

		desc.FrontCounterClockwise = FALSE;
		(*Renderer::Instance()->iGetDevice())->CreateRasterizerState(&desc, &state);
		mStates[eRS_NO_CULL] = state;


		// TODO: Everything is counter-clockwise by default. Is this an export option in Blender?
		// For now everything gets inverted to display correctly (bool in RenderShape constructor)
		this->ApplyState(eRS_FILLED);
	}

	RasterizerStateManager::~RasterizerStateManager() {
		for (unsigned int i = 0; i < eRS_MAX; ++i) {
			mStates[i]->Release();
		}
		delete[] mStates;
	}

	RasterizerStateManager * RasterizerStateManager::Instance() {
		if (nullptr == sInstance) {
			sInstance = new RasterizerStateManager;
		}
		return sInstance;
	}

	void RasterizerStateManager::DestroyInstance() {
		delete sInstance;
		sInstance = nullptr;
	}


	// Instance Functions

	bool RasterizerStateManager::ApplyState(RasterState rs) {
		if (rs < eRS_FILLED || rs >= eRS_MAX) {
			return false;
		}
		if (rs == mCurrentState) {
			return true;
		}
		(*Renderer::Instance()->iGetContext())->RSSetState(mStates[rs]);
		return true;
	}

} // Namespace
