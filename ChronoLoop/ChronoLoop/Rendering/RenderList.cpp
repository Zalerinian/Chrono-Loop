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
		auto& ctx = Renderer::Instance()->GetContext();
		auto& dev = Renderer::Instance()->GetDevice();

		// If the buffer doesn't exist, just copy the _copy buffer.
		if (_toGrow.Get() == nullptr) {
			D3D11_BUFFER_DESC desc;
			_copy->GetDesc(&desc);
			dev->CreateBuffer(&desc, nullptr, _toGrow.GetAddressOf());
			ctx->CopyResource(_toGrow.Get(), _copy.Get());
			SetD3DName(_toGrow.Get(), _name.c_str());
			Renderer::Instance()->GetRendererLock().unlock();
			mIdMap[mMasterId] = 0;
			return mMasterId++;
		}



		D3D11_BUFFER_DESC desc;
		_toGrow->GetDesc(&desc);

		unsigned int OldCount = desc.ByteWidth / sizeof(BufferWidth);


		desc.ByteWidth += sizeof(BufferWidth);
		ID3D11Buffer *newBuffer;
		dev->CreateBuffer(&desc, nullptr, &newBuffer);
		SetD3DName(newBuffer, _name.c_str());

		// Coy over the data that was already there.
		ctx->CopySubresourceRegion(newBuffer, 0, 0, 0, 0, _toGrow.Get(), 0, 0);

		// Copy in the data we wanted to add.
		ctx->CopySubresourceRegion(newBuffer, 0, desc.ByteWidth - sizeof(BufferWidth), 0, 0, _copy.Get(), 0, 0);

		_toGrow = newBuffer;
		Renderer::Instance()->GetRendererLock().unlock();
		mIdMap[mMasterId] = OldCount;
		return mMasterId++;
	}

	unsigned int RenderList::EnlargeBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& _toGrow, std::string _name, BufferWidth _filler) {
		Renderer::Instance()->GetRendererLock().lock();
		auto& ctx = Renderer::Instance()->GetContext();
		auto& dev = Renderer::Instance()->GetDevice();

		if (_toGrow.Get() == nullptr) {
			D3D11_SUBRESOURCE_DATA iData;
			iData.pSysMem = &_filler;
			CD3D11_BUFFER_DESC desc(sizeof(_filler), D3D11_BIND_CONSTANT_BUFFER);
			dev->CreateBuffer(&desc, &iData, _toGrow.GetAddressOf());
			SetD3DName(_toGrow.Get(), _name.c_str());
			Renderer::Instance()->GetRendererLock().unlock();
			mIdMap[mMasterId] = 0;
			return mMasterId++;
		}

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
		mIdMap[mMasterId] = OldCount;
		return mMasterId++;
	}

	void RenderList::Cut(Microsoft::WRL::ComPtr<ID3D11Buffer>& _toCut, std::string _name, unsigned int index) {
		Renderer::Instance()->GetRendererLock().lock();
		auto& ctx = Renderer::Instance()->GetContext();
		auto& dev = Renderer::Instance()->GetDevice();

		if (_toCut.Get() == nullptr) {
			Renderer::Instance()->GetRendererLock().unlock();
			return;
		}

		D3D11_BUFFER_DESC desc;
		_toCut->GetDesc(&desc);

		unsigned int OldCount = desc.ByteWidth / sizeof(BufferWidth);

		index = GetBufferIndex(index);

		if (index < 0 || index >= OldCount) {
			SystemLogger::Debug() << "Can't cut buffer, invalid index given: Given: " << index << " Max: " << (OldCount - 1) << std::endl;
			Renderer::Instance()->GetRendererLock().unlock();
			return;
		}

		desc.ByteWidth -= sizeof(BufferWidth);
		if (desc.ByteWidth <= 0) {
			_toCut = nullptr;
			mIdMap.erase(index);
			Renderer::Instance()->GetRendererLock().unlock();
			return;
		}


		ID3D11Buffer *newBuffer;
		HRESULT hr = dev->CreateBuffer(&desc, nullptr, &newBuffer);
		SetD3DName(newBuffer, _name.c_str());
		SystemLogger::Debug() << "Cutting index " << index << " out of '" << mShape.mName << "'  (" << OldCount << ")... ";

		bool idsNeedUpdating = true;
		// Copy in the data we wanted to add.
		if (index == 0) {
			SystemLogger::GetLog() << "The shape being removed was the first one!" << std::endl;
			// The one to be removed is the first element
			D3D11_BOX dataBoundary;
			dataBoundary.left = sizeof(BufferWidth);
			dataBoundary.top = 0;
			dataBoundary.front = 0;
			dataBoundary.right = (OldCount) * sizeof(BufferWidth); // The right boundary is excluded, so it should be just outside the valid range.
			dataBoundary.back = 1;
			dataBoundary.bottom = 1;
			ctx->CopySubresourceRegion(newBuffer, 0, 0, 0, 0, _toCut.Get(), 0, &dataBoundary);
		} else if (index == OldCount - 1) {
			SystemLogger::GetLog() << "The shape being removed was the last one!" << std::endl;
			idsNeedUpdating = false;

			// The one to be removed is the last element
			D3D11_BOX dataBoundary;
			dataBoundary.left = 0;
			dataBoundary.top = 0;
			dataBoundary.front = 0;
			dataBoundary.right = (OldCount - 1) * sizeof(BufferWidth); // The right boundary is excluded, so it should be just outside the valid range.
			dataBoundary.back = 1;
			dataBoundary.bottom = 1;
			ctx->CopySubresourceRegion(newBuffer, 0, 0, 0, 0, _toCut.Get(), 0, &dataBoundary);
		} else {
			SystemLogger::GetLog() << "The shape being removed was in the middle!" << std::endl;
			// The data to remove is in the middle, we need to do 2 copies.
			D3D11_BOX leftBoundary;
			leftBoundary.left = 0;
			leftBoundary.top = 0;
			leftBoundary.front = 0;
			leftBoundary.right = (index - 1) * sizeof(BufferWidth); // The right boundary is excluded, so it should be just outside the valid range.
			leftBoundary.back = 1;
			leftBoundary.bottom = 1;
			ctx->CopySubresourceRegion(newBuffer, 0, 0, 0, 0, _toCut.Get(), 0, &leftBoundary);

			D3D11_BOX rightBoundary;
			rightBoundary.left = (index) * sizeof(BufferWidth);
			rightBoundary.top = 0;
			rightBoundary.front = 0;
			rightBoundary.right = (OldCount - 1) * sizeof(BufferWidth); // The right boundary is excluded, so it should be just outside the valid range.
			rightBoundary.back = 1;
			rightBoundary.bottom = 1;
			ctx->CopySubresourceRegion(newBuffer, 0, (index) * sizeof(BufferWidth), 0, 0, _toCut.Get(), 0, &rightBoundary);

		}
		_toCut = newBuffer;

		// Go through the ID Map and decrement any id higher than the given one
		if (idsNeedUpdating) {
			// If the removed element is the last item in the map, then we don't need to update anything because nothing is after it.
			for (auto it = mIdMap.begin(); it != mIdMap.end(); ++it) {
				if (it->second > index) {
					--it->second;
				}
			}
		}
		Renderer::Instance()->GetRendererLock().unlock();
	}

	void RenderList::UpdateBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& _toUpdate, Microsoft::WRL::ComPtr<ID3D11Buffer>& _data, unsigned int index) {
		if (_toUpdate.Get() == nullptr) {
			SystemLogger::Error() << "Could not update a buffer, because there *is*no buffer." << std::endl;
			return;
		}
		
		D3D11_BUFFER_DESC desc;
		_toUpdate->GetDesc(&desc);
		unsigned int count = desc.ByteWidth / sizeof(BufferWidth);
		
		unsigned int BufferIndex = GetBufferIndex(index);
		if (BufferIndex < 0 || BufferIndex >= count) {
			SystemLogger::Error() << "Cannot update a buffer because somehow, an index is out of range!" << std::endl;
			return;
		}

		Renderer::Instance()->GetContext()->CopySubresourceRegion(_toUpdate.Get(), 0, sizeof(BufferWidth) * BufferIndex, 0, 0, _data.Get(), 0, nullptr);
	}

	unsigned int RenderList::GetBufferIndex(unsigned int _uid) {
		return mIdMap[_uid];
	}

	RenderList::RenderList(RenderShape & _reference) {
		mShape = _reference;

		// Clears all the buffers back to nullptr, because initially we'd have copies of the object's buffers,
		// but the shape associated with the RenderList is a "master" shape, and should not have the buffer data
		// of any individual shape.
		for (int i = 0; i < eVB_MAX; ++i) {
			mShape.mContext.mVertexCBuffers[i] = nullptr;
		}

		for (int i = 0; i < ePB_MAX; ++i) {
			mShape.mContext.mPixelCBuffers[i] = nullptr;
		}

		for (int i = 0; i < eGB_MAX; ++i) {
			mShape.mContext.mGeometryCBuffers[i] = nullptr;
		}
	}

	GhostList<matrix4>::GhostNode* RenderList::Push(RenderShape& _shape) {
		BufferWidth NullData;
		
		NullData.e1.xAxis.Set(24, 24, 24, 24);
		NullData.e1.yAxis.Set(25, 25, 25, 25);
		NullData.e1.zAxis.Set(42, 42, 42, 42);
		NullData.e1.Position.Set(7, 7, 7, 7);
		NullData.e2.xAxis.Set(7, 7, 7, 7);
		NullData.e2.yAxis.Set(42, 42, 42, 42);
		NullData.e2.zAxis.Set(25, 25, 25, 25);
		NullData.e2.Position.Set(24, 24, 24, 24);

		int vbIndex = -1, pbIndex = -1, gbIndex = -1;
		// Prepare the vertex buffers.
		std::string bufferName;
		for (int i = 0; i < eVB_MAX; ++i) {
			int nIdx = -1;
			bufferName = mShape.mName + std::string(" master vertex buffer at index ") + std::to_string(i);
			if (_shape.mContext.mVertexCBuffers[i].Get()) {
				// This shape has data. Copy it in.
				nIdx = EnlargeBuffer(mShape.mContext.mVertexCBuffers[i], bufferName, _shape.mContext.mVertexCBuffers[i]);
			} else {
				// This shape has no data, but in order to maintain proper ordering, we need to put in a blank space.
				nIdx = EnlargeBuffer(mShape.mContext.mVertexCBuffers[i], bufferName, NullData);
			}


			if (vbIndex < 0) {
				vbIndex = nIdx;
			} else {
				if (GetBufferIndex(nIdx) != GetBufferIndex(vbIndex)) {
					SystemLogger::Error() << "A different index than expected (" << GetBufferIndex(vbIndex) << ") was returned for Vertex Constant Buffer " << i << " for " << mShape.GetName() << ". " << GetBufferIndex(nIdx) << " was received." << std::endl;
				}
			}
		}

		// Prepare the pixel buffers
		for (int i = 0; i < ePB_MAX; ++i) {
			int nIdx = -1;
			bufferName = mShape.mName + std::string(" master pixel buffer at index ") + std::to_string(i);
			if (_shape.mContext.mPixelCBuffers[i].Get()) {
				// This shape has data. Copy it in.
				nIdx = EnlargeBuffer(mShape.mContext.mPixelCBuffers[i], bufferName, _shape.mContext.mPixelCBuffers[i]);
			} else {
				// This shape has no data, but in order to maintain proper ordering, we need to put in a blank space.
				nIdx = EnlargeBuffer(mShape.mContext.mPixelCBuffers[i], bufferName, NullData);
			}


			if (pbIndex < 0) {
				pbIndex = nIdx;
			} else {
				if (GetBufferIndex(pbIndex) != GetBufferIndex(nIdx)) {
					SystemLogger::Error() << "A different index than expected (" << GetBufferIndex(pbIndex) << ") was returned for Pixel Constant Buffer " << i << " for " << mShape.GetName() << ". " << GetBufferIndex(nIdx) << " was received." << std::endl;
				}
			}
		}

		// Prepare the geometry buffers
		for (int i = 0; i < eGB_MAX; ++i) {
			int nIdx = -1;
			bufferName = mShape.mName + std::string(" master geometry buffer at index ") + std::to_string(i);
			if (_shape.mContext.mGeometryCBuffers[i].Get()) {
				// This shape has data. Copy it in.
				nIdx = EnlargeBuffer(mShape.mContext.mGeometryCBuffers[i], bufferName, _shape.mContext.mGeometryCBuffers[i]);
			} else {
				// This shape has no data, but in order to maintain proper ordering, we need to put in a blank space.
				nIdx = EnlargeBuffer(mShape.mContext.mGeometryCBuffers[i], bufferName, NullData);
			}


			if (gbIndex < 0) {
				gbIndex = nIdx;
			} else {
				if (GetBufferIndex(gbIndex) != GetBufferIndex(nIdx)) {
					SystemLogger::Error() << "A different index than expected (" << GetBufferIndex(gbIndex) << ") was returned for Geometry Constant Buffer " << i << " for " << mShape.GetName() << ". " << GetBufferIndex(nIdx) << " was received." << std::endl;
				}
			}
		}

		if (GetBufferIndex(vbIndex) != GetBufferIndex(pbIndex)) {
			SystemLogger::Error() << "The Vertex Buffer Index [" << vbIndex << "] and Pixel Buffer index [" << pbIndex << "] are not the same for " << mShape.mName << "." << std::endl;
		}
		if (GetBufferIndex(pbIndex) != GetBufferIndex(gbIndex)) {
			SystemLogger::Error() << "The Pixel Buffer Index [" << pbIndex << "] and Geometry Buffer index [" << gbIndex << "] are not the same for " << mShape.mName << "." << std::endl;
		}

		_shape.mVBIndex = vbIndex;  // Unless something is wrong, these all have the same buffer indices.
		_shape.mPBIndex = pbIndex;  // Unless something is wrong, these all have the same buffer indices.
		_shape.mGBIndex = gbIndex;  // Unless something is wrong, these all have the same buffer indices.

		return mPositions.Push(_shape.mPosition);
	}

	void RenderList::Pop(RenderShape & _shape) {
		// Cut vertex buffers
		std::string bufferName;
		for (int i = 0; i < eVB_MAX; ++i) {
			bufferName = mShape.mName + std::string(" master vertex buffer at index ") + std::to_string(i);
			Cut(mShape.mContext.mVertexCBuffers[i], bufferName, _shape.mVBIndex);
		}

		for (int i = 0; i < ePB_MAX; ++i) {
			bufferName = mShape.mName + std::string(" master pixel buffer at index ") + std::to_string(i);
			Cut(mShape.mContext.mPixelCBuffers[i], bufferName, _shape.mPBIndex);
		}

		for (int i = 0; i < eGB_MAX; ++i) {
			bufferName = mShape.mName + std::string(" master geometry buffer at index ") + std::to_string(i);
			Cut(mShape.mContext.mGeometryCBuffers[i], bufferName, _shape.mGBIndex);
		}
	}

	void RenderList::UpdateBuffer(ConstantBufferType _t, Microsoft::WRL::ComPtr<ID3D11Buffer> _data, unsigned int _bufferIndex, unsigned int _dataIndex) {
		switch (_t) {
			case eCB_VERTEX:
				UpdateBuffer(mShape.mContext.mVertexCBuffers[_bufferIndex], _data, _dataIndex);
				break;
			case eCB_PIXEL:
				UpdateBuffer(mShape.mContext.mPixelCBuffers[_bufferIndex], _data, _dataIndex);
				break;
			case eCB_GEO:
				UpdateBuffer(mShape.mContext.mGeometryCBuffers[_bufferIndex], _data, _dataIndex);
				break;
			default:
				SystemLogger::Error() << "Invalid buffer type passed to UpdateBuffer: " << _t << std::endl;
				break;
		}
	}

}
