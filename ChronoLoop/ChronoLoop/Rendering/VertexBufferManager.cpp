#include "VertexBufferManager.h"


namespace RenderEngine {

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
			case RenderEngine::eVERT_POS:
				return sInstance->mPosBuffer.GetVertexBuffer();
				break;
			case RenderEngine::eVERT_POSCOLOR:
				return sInstance->mPosColorBuffer.GetVertexBuffer();
				break;
			case RenderEngine::eVERT_POSTEX:
				return sInstance->mPosTexBuffer.GetVertexBuffer();
				break;
			case RenderEngine::eVERT_POSNORMTEX:
				return sInstance->mPosNormTexBuffer.GetVertexBuffer();
				break;
			case RenderEngine::eVERT_POSNORMTANTEX:
				return sInstance->mPosNormTanTexBuffer.GetVertexBuffer();
				break;
			case RenderEngine::eVERT_POSBONEWEIGHT:
				return sInstance->mPosBoneWeightBuffer.GetVertexBuffer();
				break;
			case RenderEngine::eVERT_POSBONEWEIGHTNORMTEX:
				return sInstance->mPosBoneWeightNormTexBuffer.GetVertexBuffer();
				break;
			case RenderEngine::eVERT_POSBONEWEIGHTNORMTANTEX:
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