#pragma once
#include <string>
#include <unordered_map>

struct ID3D11Buffer;

namespace RenderEngine {

	class IndexBufferManager
	{
		static IndexBufferManager *sInstance;
		std::unordered_map<std::string, unsigned int> mOffsets;
		ID3D11Buffer* mIndexBuffer;
		IndexBufferManager();
		~IndexBufferManager();
	public:
		static IndexBufferManager& Instance();
		static void Shutdown();
		static ID3D11Buffer* GetBuffer();
		static unsigned int AddToBuffer(std::string _Name, const unsigned int* _Indices, unsigned int _NumIndices);
	};
}