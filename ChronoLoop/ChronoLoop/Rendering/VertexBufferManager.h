#pragma once
#include "VertexBuffer.h"
#include "Structures.h"
#include "RendererDefines.h"
#include <typeinfo>
namespace Epoch {

	class VertexBufferManager {
	private:
		VertexBuffer<VertexPos>						mPosBuffer;
		VertexBuffer<VertexPosColor>				mPosColorBuffer;
		VertexBuffer<VertexPosTex>					mPosTexBuffer;
		VertexBuffer<VertexPosNormTex>				mPosNormTexBuffer;
		VertexBuffer<VertexPosNormTanTex>			mPosNormTanTexBuffer;
		VertexBuffer<VertexPosBoneWeight>			mPosBoneWeightBuffer;
		VertexBuffer<VertexPosBoneWeightNormTex>	mPosBoneWeightNormTexBuffer;
		VertexBuffer<VertexPosBoneWeightNormTanTex> mPosBoneWeightNormTanTexBuffer;

		static VertexBufferManager* sInstance;
		VertexBufferManager();
		~VertexBufferManager();
	public:
		static VertexBufferManager& Instance();
		static void Shutdown();
		static ID3D11Buffer* GetBuffer(VertFormat _format);

		template<class T>
		static VertexBuffer<T>* GetInternalBuffer() {
			if (!sInstance) {
				return nullptr;
			}
			return &sInstance->mPosNormTexBuffer;
		}
	};
}