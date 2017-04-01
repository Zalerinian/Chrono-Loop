#pragma once
#include "RenderShape.h"
#include "../Common/GhostList.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace Epoch {

	struct matrix4;

	struct BufferWidth {
		matrix4 e1, e2;
	};

	class RenderList {
		// returns the index into the buffer that the given data occupies. this number should be stored in the shape so that
		// whe it is removed later, it can be cut out of these buffers.
		unsigned int EnlargeBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& _toGrow, std::string _name, Microsoft::WRL::ComPtr<ID3D11Buffer> _copy);
		unsigned int EnlargeBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& _toGrow, std::string _name, BufferWidth _filler);
		void Cut(Microsoft::WRL::ComPtr<ID3D11Buffer>& _toCut, std::string _name, unsigned int index);
		void UpdateBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& _toUpdate, Microsoft::WRL::ComPtr<ID3D11Buffer>& _data, unsigned int index);

		std::unordered_map<unsigned int, unsigned int> mIdMap;
		unsigned int mMasterId = 0;
		unsigned int GetBufferIndex(unsigned int _uid);

	public:
		RenderShape mShape;
		GhostList<matrix4> mPositions;

		RenderList(RenderShape& _reference);

		GhostList<matrix4>::GhostNode* Push(RenderShape& _shape);
		void Pop(RenderShape& _shape);
		void UpdateBuffer(ConstantBufferType _t, Microsoft::WRL::ComPtr<ID3D11Buffer> _data, unsigned int _bufferIndex, unsigned int _dataIndex);
	};

} // Epoch Namespace