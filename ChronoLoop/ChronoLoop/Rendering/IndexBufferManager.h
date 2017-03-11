#pragma once
#include <string>
#include <unordered_map>
#include <wrl/client.h>

struct ID3D11Buffer;

namespace Epoch {

	class IndexBufferManager
	{
		static IndexBufferManager *sInstance;
		std::unordered_map<std::string, unsigned int> mOffsets;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		IndexBufferManager();
		~IndexBufferManager();
	public:
		static IndexBufferManager& GetInstance();
		static void DestroyInstance();
		static Microsoft::WRL::ComPtr<ID3D11Buffer> GetBuffer();
		static unsigned int AddToBuffer(std::string _Name, const unsigned int* _Indices, unsigned int _NumIndices);
	};
}