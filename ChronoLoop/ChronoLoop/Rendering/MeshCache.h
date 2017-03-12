#pragma once
#include "Mesh.h"
#include <unordered_map>
#include <string>

namespace Epoch {

	class MC {
		friend class MeshCache;
		std::unordered_map<std::string, Mesh*> mMeshCache;
		MC();
		~MC();
	public:
		Mesh* GetMesh(std::string _path);
	};


	class MeshCache {
		static MC* sInstance;

	public:
		static MC& GetInstance();
		static void DestroyInstance();
	};

}