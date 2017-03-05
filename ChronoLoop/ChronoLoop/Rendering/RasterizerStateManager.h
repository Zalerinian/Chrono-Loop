#pragma once
#include <d3d11.h>
#include "RendererDefines.h"

namespace Epoch {
	

	class RasterizerStateManager {
		static RasterizerStateManager* sInstance;
		
		ID3D11RasterizerState **mStates;
		RasterState mCurrentState;

		RasterizerStateManager();
		~RasterizerStateManager();
	public:
		static RasterizerStateManager* Instance();
		static void DestroyInstance();

		// Instance Functions
		bool ApplyState(RasterState rs);
	};
}