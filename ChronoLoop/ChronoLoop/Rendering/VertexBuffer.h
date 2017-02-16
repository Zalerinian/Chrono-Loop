#pragma once
#include <map>
#include <string>
#include <d3d11.h>
#include "renderer.h"


namespace RenderEngine {
	template<typename T>
	class VertexBuffer
	{
	private:
		std::map<std::string, unsigned int> mOffsets;
		ID3D11Buffer* mVertexBuffer;
	public:
		VertexBuffer();
		~VertexBuffer();
		unsigned int AddVerts(std::string _name, const T *_verts, unsigned int _numVerts);
		ID3D11Buffer *const GetVertexBuffer();
	};

	template<typename T>
	VertexBuffer<T>::VertexBuffer()
	{
		mVertexBuffer = nullptr;
	}

	template<typename T>
	VertexBuffer<T>::~VertexBuffer()
	{
		mVertexBuffer->Release();
	}

	template<typename T>
	unsigned int VertexBuffer<T>::AddVerts(std::string _name, const T * _verts, unsigned int _numVerts)
	{
		unsigned int offset = 0;
		if (mOffsets.count(_Name) > 0)
			offset = mOffsets.at(_Name);
		else
		{
			if (mVertexBuffer)
			{
				D3D11_BUFFER_DESC desc;
				mVertexBuffer->GetDesc(&desc);

				D3D11_SUBRESOURCE_DATA initData;
				unsigned int oldSize = desc.ByteWidth / sizeof(T);
				unsigned int newSize = oldSize + _numVerts;
				offset = oldSize;
				initData.pSysMem = new T[newSize];
				memcpy((char *)(initData.pSysMem) + desc.ByteWidth, _Indices, sizeof(T) * _NumIndices);
				desc.ByteWidth = sizeof(T) * _NumIndices;
				ID3D11Buffer *newBuffer;
				(*Renderer::Instance()->GetDevice())->CreateBuffer(&desc, &initData, &newBuffer);
				(*Renderer::Instance()->GetContext())->CopySubresourceRegion(newBuffer, 0, 0, 0, 0, mVertexBuffer, 0, 0);
				mVertexBuffer->Release();
				mVertexBuffer = newBuffer;
				delete[] initData.pSysMem;
			}
			else
			{
				D3D11_BUFFER_DESC desc;
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.ByteWidth = sizeof(t) * _numVerts;
				desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;
				desc.StructureByteStride = 0;

				D3D11_SUBRESOURCE_DATA initData;
				initData.pSysMem = _verts;
				(*Renderer::Instance()->GetDevice())->CreateBuffer(&desc, &initData, &mVertexBuffer);
			}
			mOffsets[_Name] = offset;
		}
		return offset;
	}
	template<typename T>
	ID3D11Buffer * const VertexBuffer<T>::GetVertexBuffer()
	{
		return mVertexBuffer;
	}
}