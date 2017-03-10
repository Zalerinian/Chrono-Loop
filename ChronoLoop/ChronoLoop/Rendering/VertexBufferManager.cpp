#include "VertexBufferManager.h"


namespace Epoch {

	VertexBufferManager* VertexBufferManager::sInstance = nullptr;

	VertexBufferManager::VertexBufferManager() {}


	VertexBufferManager::~VertexBufferManager() {}

	VertexBufferManager & VertexBufferManager::Instance() {
		if (!sInstance)
			sInstance = new VertexBufferManager();
		return *sInstance;
	}
	
	void VertexBufferManager::Shutdown() {
		if (sInstance) {
			delete sInstance;
		}
	}
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBufferManager::GetBuffer(VertFormat _format) {
		if (sInstance) {
			switch (_format) {
				case eVERT_POS:
					return sInstance->mPosBuffer.GetBuffer();
					break;
				case eVERT_POSCOLOR:
					return sInstance->mPosColorBuffer.GetBuffer();
					break;
				case eVERT_POSTEX:
					return sInstance->mPosTexBuffer.GetBuffer();
					break;
				case eVERT_POSNORMTEX:
					return sInstance->mPosNormTexBuffer.GetBuffer();
					break;
				case eVERT_POSNORMTANTEX:
					return sInstance->mPosNormTanTexBuffer.GetBuffer();
					break;
				case eVERT_POSBONEWEIGHT:
					return sInstance->mPosBoneWeightBuffer.GetBuffer();
					break;
				case eVERT_POSBONEWEIGHTNORMTEX:
					return sInstance->mPosBoneWeightNormTexBuffer.GetBuffer();
					break;
				case eVERT_POSBONEWEIGHTNORMTANTEX:
					return sInstance->mPosBoneWeightNormTanTexBuffer.GetBuffer();
					break;
				default:
					return nullptr;
					break;
			}
		}
		return nullptr;
	}
}