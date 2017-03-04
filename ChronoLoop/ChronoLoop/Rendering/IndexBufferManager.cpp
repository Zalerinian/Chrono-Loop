#include "IndexBufferManager.h"
#include "renderer.h"
#include <d3d11.h>
#include "../Common/Common.h"

namespace Epoch {

	IndexBufferManager* IndexBufferManager::sInstance = nullptr;

	IndexBufferManager::IndexBufferManager()
	{
		mIndexBuffer = nullptr;
	}


	IndexBufferManager::~IndexBufferManager()
	{
		mIndexBuffer->Release();
	}

	IndexBufferManager & IndexBufferManager::Instance()
	{
		if (!sInstance)
			sInstance = new IndexBufferManager();
		return *sInstance;
	}
	void IndexBufferManager::Shutdown()
	{
		if (sInstance)
			delete sInstance;
	}
	ID3D11Buffer * IndexBufferManager::GetBuffer()
	{
		if (sInstance)
			return sInstance->mIndexBuffer;
		return nullptr;
	}
	unsigned int IndexBufferManager::AddToBuffer(std::string _Name, const unsigned int * _Indices, unsigned int _NumIndices)
	{
		if (!sInstance)
			Instance();
		unsigned int offset = 0;
		if (sInstance->mOffsets.count(_Name) > 0)
			offset = sInstance->mOffsets.at(_Name);
		else
		{
			if (sInstance->mIndexBuffer)
			{
				D3D11_BUFFER_DESC desc;
				sInstance->mIndexBuffer->GetDesc(&desc);

				D3D11_SUBRESOURCE_DATA initData;
				unsigned int oldSize = desc.ByteWidth / sizeof(unsigned int);
				unsigned int newSize = oldSize + _NumIndices;
				offset = oldSize;
				initData.pSysMem = new unsigned int[newSize];
				memcpy((char *)(initData.pSysMem) + desc.ByteWidth, _Indices, sizeof(unsigned int) * _NumIndices);
				desc.ByteWidth += sizeof(unsigned int) * _NumIndices;
				ID3D11Buffer *newBuffer;
				Renderer::Instance()->GetDevice()->CreateBuffer(&desc, &initData, &newBuffer);
				Renderer::Instance()->GetContext()->CopySubresourceRegion(newBuffer, 0, 0, 0, 0, sInstance->mIndexBuffer, 0, 0);
				sInstance->mIndexBuffer->Release();
				sInstance->mIndexBuffer = newBuffer;
				std::string name = "The Index Buffer, mk" + std::to_string(sInstance->mOffsets.size());
				SetD3DName(sInstance->mIndexBuffer, name.c_str());
				delete[] initData.pSysMem;
			}
			else
			{
				D3D11_BUFFER_DESC desc;
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.ByteWidth = sizeof(unsigned int) * _NumIndices;
				desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;
				desc.StructureByteStride = 0;

				D3D11_SUBRESOURCE_DATA initData;
				initData.pSysMem = _Indices;
				Renderer::Instance()->GetDevice()->CreateBuffer(&desc, &initData, &sInstance->mIndexBuffer);
				SetD3DName(sInstance->mIndexBuffer, "The Index Buffer");
			}
			sInstance->mOffsets[_Name] = offset;
		}
		return offset;
	}
}