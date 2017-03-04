#include "VertexBufferManager.h"


namespace Epoch {

	VertexBufferManager* VertexBufferManager::sInstance = nullptr;

	VertexBufferManager::VertexBufferManager()
	{
	}


	VertexBufferManager::~VertexBufferManager()
	{
	}
	VertexBufferManager & VertexBufferManager::Instance()
	{
		if (!sInstance)
			sInstance = new VertexBufferManager();
		return *sInstance;
	}
	void VertexBufferManager::Shutdown()
	{
		if (sInstance)
		{
			delete sInstance;
		}
	}
	ID3D11Buffer * VertexBufferManager::GetBuffer(VertFormat _format)
	{
		if (sInstance)
		{
			switch (_format)
			{
			case eVERT_POS:
				return sInstance->mPosBuffer.GetVertexBuffer();
				break;
			case eVERT_POSCOLOR:
				return sInstance->mPosColorBuffer.GetVertexBuffer();
				break;
			case eVERT_POSTEX:
				return sInstance->mPosTexBuffer.GetVertexBuffer();
				break;
			case eVERT_POSNORMTEX:
				return sInstance->mPosNormTexBuffer.GetVertexBuffer();
				break;
			case eVERT_POSNORMTANTEX:
				return sInstance->mPosNormTanTexBuffer.GetVertexBuffer();
				break;
			case eVERT_POSBONEWEIGHT:
				return sInstance->mPosBoneWeightBuffer.GetVertexBuffer();
				break;
			case eVERT_POSBONEWEIGHTNORMTEX:
				return sInstance->mPosBoneWeightNormTexBuffer.GetVertexBuffer();
				break;
			case eVERT_POSBONEWEIGHTNORMTANTEX:
				return sInstance->mPosBoneWeightNormTanTexBuffer.GetVertexBuffer();
				break;
			default:
				return nullptr;
				break;
			}
		}
		return nullptr;
	}
}