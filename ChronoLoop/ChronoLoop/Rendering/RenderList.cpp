#include "RenderList.h"
#include "../Rendering/RendererDefines.h"
#include "../Rendering/Renderer.h"
#include "../Common/Common.h"
#include <d3d11.h>


using namespace DirectX;
using namespace Microsoft::WRL;

namespace Epoch {



	unsigned int RenderList::EnlargeBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& _toGrow, std::string _name, Microsoft::WRL::ComPtr<ID3D11Buffer> _copy) {
		Renderer::Instance()->GetRendererLock().lock();
		ComPtr<ID3D11DeviceContext> ctx = Renderer::Instance()->GetContext();
		ComPtr<ID3D11Device> dev = Renderer::Instance()->GetDevice();

		D3D11_BUFFER_DESC desc;
		_toGrow->GetDesc(&desc);

		unsigned int OldCount = desc.ByteWidth / sizeof(BufferWidth);


		desc.ByteWidth += sizeof(BufferWidth);
		ID3D11Buffer *newBuffer;
		dev->CreateBuffer(&desc, nullptr, &newBuffer);
		SetD3DName(newBuffer, _name.c_str());

		// Coy over the data that was already there.
		ctx->CopyResource(newBuffer, _toGrow.Get());

		// Copy in the data we wanted to add.
		ctx->CopySubresourceRegion(newBuffer, desc.ByteWidth - sizeof(BufferWidth), 0, 0, 0, _copy.Get(), 0, 0);

		_toGrow = newBuffer;
		Renderer::Instance()->GetRendererLock().unlock();
		return OldCount;
	}

	unsigned int RenderList::EnlargeBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& _toGrow, std::string _name, BufferWidth _filler) {
		Renderer::Instance()->GetRendererLock().lock();
		ComPtr<ID3D11DeviceContext> ctx = Renderer::Instance()->GetContext();
		ComPtr<ID3D11Device> dev = Renderer::Instance()->GetDevice();

		D3D11_BUFFER_DESC desc;
		_toGrow->GetDesc(&desc);

		unsigned int OldCount = desc.ByteWidth / sizeof(BufferWidth);
		D3D11_SUBRESOURCE_DATA InitialData;
		InitialData.pSysMem = new BufferWidth[OldCount + 1];
		// Copies the new data to the end of the data to initialize the buffer with. The old data is then
		// copied over the garbage.
		memcpy(((char*)InitialData.pSysMem) + desc.ByteWidth, &_filler, sizeof(_filler)); 
		desc.ByteWidth += sizeof(BufferWidth);
		ID3D11Buffer *newBuffer;
		dev->CreateBuffer(&desc, &InitialData, &newBuffer);
		SetD3DName(newBuffer, _name.c_str());

		// Copy in the original data back to the front of the buffer.
		ctx->CopySubresourceRegion(newBuffer, 0, 0, 0, 0, _toGrow.Get(), 0, 0);

		delete[] InitialData.pSysMem;
		
		_toGrow = newBuffer;
		Renderer::Instance()->GetRendererLock().unlock();
		return OldCount;
	}

	void RenderList::Cut(unsigned int index) {

	}


	RenderList::RenderList(RenderShape & _reference) {
		mShape = _reference;

		// Matrix4 is the same size as XMMATRIX (it has a XMMATRIX stored as part of a union).
		// No given buffer for an object shall exceed the size of 2 matrices (128 bytes)
		CD3D11_BUFFER_DESC bufferDesc(sizeof(XMMATRIX) * 2, D3D11_BIND_CONSTANT_BUFFER);
		D3D11_SUBRESOURCE_DATA Empty;

		BufferWidth NullData;

		memset(&NullData, 0, sizeof(NullData));

		Empty.pSysMem = &NullData;

		// Prepare the vertex buffers.
		for (int i = 0; i < eVB_MAX; ++i) {
			Renderer::Instance()->GetDevice()->CreateBuffer(&bufferDesc, nullptr, mShape.mContext.mVertexCBuffers[i].GetAddressOf());
		}

		// Prepare the pixel buffers
		for (int i = 0; i < ePB_MAX; ++i) {
			Renderer::Instance()->GetDevice()->CreateBuffer(&bufferDesc, nullptr, mShape.mContext.mPixelCBuffers[i].GetAddressOf());
		}

		// Prepare the geometry buffers
		for (int i = 0; i < eGB_MAX; ++i) {
			Renderer::Instance()->GetDevice()->CreateBuffer(&bufferDesc, nullptr, mShape.mContext.mGeometryCBuffers[i].GetAddressOf());
		}
	}



	GhostList<matrix4>::GhostNode* RenderList::Push(RenderShape& _shape) {
		BufferWidth NullData;
		
		NullData.e1.xAxis.Set(24, 24, 24, 24);
		NullData.e1.yAxis.Set(25, 25, 25, 25);
		NullData.e1.zAxis.Set(42, 42, 42, 42);
		NullData.e1.Position.Set(7, 7, 7, 7);
		NullData.e2.Position.Set(7, 7, 7, 7);
		NullData.e2.zAxis.Set(42, 42, 42, 42);
		NullData.e2.yAxis.Set(25, 25, 25, 25);
		NullData.e2.xAxis.Set(24, 24, 24, 24);


		// Prepare the vertex buffers.
		std::string bufferName;
		for (int i = 0; i < eVB_MAX; ++i) {
			bufferName = mShape.mName + std::string(" master vertex buffer ") + std::to_string(i);
			if (_shape.mContext.mVertexCBuffers[i].Get()) {
				// This shape has data. Copy it in.
				EnlargeBuffer(mShape.mContext.mVertexCBuffers[i], bufferName, _shape.mContext.mVertexCBuffers[i]);
			} else {
				// This shape has no data, but in order to maintain proper ordering, we need to put in a blank space.
				EnlargeBuffer(mShape.mContext.mVertexCBuffers[i], bufferName, NullData);
			}
		}

		// Prepare the pixel buffers
		for (int i = 0; i < ePB_MAX; ++i) {
			bufferName = mShape.mName + std::string(" master pixel buffer ") + std::to_string(i);
			if (_shape.mContext.mPixelCBuffers[i].Get()) {
				// This shape has data. Copy it in.
				EnlargeBuffer(mShape.mContext.mPixelCBuffers[i], bufferName, _shape.mContext.mPixelCBuffers[i]);
			} else {
				// This shape has no data, but in order to maintain proper ordering, we need to put in a blank space.
				EnlargeBuffer(mShape.mContext.mPixelCBuffers[i], bufferName, NullData);
			}
		}

		// Prepare the geometry buffers
		for (int i = 0; i < eGB_MAX; ++i) {
			bufferName = mShape.mName + std::string(" master geometry buffer ") + std::to_string(i);
			if (_shape.mContext.mGeometryCBuffers[i].Get()) {
				// This shape has data. Copy it in.
				EnlargeBuffer(mShape.mContext.mGeometryCBuffers[i], bufferName, _shape.mContext.mGeometryCBuffers[i]);
			} else {
				// This shape has no data, but in order to maintain proper ordering, we need to put in a blank space.
				EnlargeBuffer(mShape.mContext.mGeometryCBuffers[i], bufferName, NullData);
			}
		}




		return mPositions.Push(_shape.mPosition);
	}

}
