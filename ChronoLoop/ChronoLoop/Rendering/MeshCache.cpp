#include "MeshCache.h"
#include "../Common/Logger.h"

namespace Epoch {

	MC* MeshCache::sInstance = nullptr;

	MC& MeshCache::GetInstance() {
		if (nullptr == sInstance) {
			sInstance = new MC;
		}
		return *sInstance;
	}

	void MeshCache::DestroyInstance() {
		if (nullptr != sInstance) {
			delete sInstance;
			sInstance = nullptr;
		}
	}
	MC::MC() {}

	MC::~MC() {
		while (mMeshCache.begin() != mMeshCache.end()) {
			mMeshCache.erase(mMeshCache.begin());
		}
	}


	Mesh* MC::GetMesh(std::string _path) {
		Mesh* CachedMesh = mMeshCache[_path];
		if (CachedMesh != nullptr) {
			SystemLogger::Debug() << _path << " was already loaded by the mesh cache! Returning existing pointer!" << std::endl;
			return CachedMesh;
		}
		SystemLogger::Debug() << _path << " was not already loaded by the mesh cache! Creating a new mesh..." << std::endl;
		CachedMesh = new Mesh(_path.c_str());
		mMeshCache[_path] = CachedMesh;
		return CachedMesh;
	}
} // Epoch Namespace