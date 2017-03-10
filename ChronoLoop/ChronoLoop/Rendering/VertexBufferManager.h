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
		static Microsoft::WRL::ComPtr<ID3D11Buffer> GetBuffer(VertFormat _format);

		template <class T>
		static VertexBuffer<T>* GetVertexBuffer() {
			// This generic function is invalid, and is only used so that specialzations can be made.
			return nullptr;
		}

		template<> static VertexBuffer<VertexPos>* GetVertexBuffer<VertexPos>() {
			return &sInstance->mPosBuffer;
		}

		template<> static VertexBuffer<VertexPosColor>* GetVertexBuffer<VertexPosColor>() {
			return &sInstance->mPosColorBuffer;
		}

		template<> static VertexBuffer<VertexPosTex>* GetVertexBuffer<VertexPosTex>() {
			return &sInstance->mPosTexBuffer;
		}

		template<> static VertexBuffer<VertexPosNormTex>* GetVertexBuffer<VertexPosNormTex>() {
			return &sInstance->mPosNormTexBuffer;
		}

		template<> static VertexBuffer<VertexPosNormTanTex>* GetVertexBuffer<VertexPosNormTanTex>() {
			return &sInstance->mPosNormTanTexBuffer;
		}

		template<> static VertexBuffer<VertexPosBoneWeight>* GetVertexBuffer<VertexPosBoneWeight>() {
			return &sInstance->mPosBoneWeightBuffer;
		}

		template<> static VertexBuffer<VertexPosBoneWeightNormTex>* GetVertexBuffer<VertexPosBoneWeightNormTex>() {
			return &sInstance->mPosBoneWeightNormTexBuffer;
		}

		template<> static VertexBuffer<VertexPosBoneWeightNormTanTex>* GetVertexBuffer<VertexPosBoneWeightNormTanTex>() {
			return &sInstance->mPosBoneWeightNormTanTexBuffer;
		}
	};
}