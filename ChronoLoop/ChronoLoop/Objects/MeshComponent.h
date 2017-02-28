#pragma once
#include "Component.h"
#include "../Rendering/RendererDefines.h"
#include  "../Rendering/RenderShape.h"

namespace RenderEngine {
	struct RenderShape;
}

class MeshComponent : public Component {
	RenderEngine::RenderShape* mShape;
	bool mVisible;

public:
	MeshComponent(const char *_path);
	void Update();
	void Destroy();
	void SetVisible(bool _vis);
	void AddTexture(const char *_path, RenderEngine::TextureType _type);
	inline bool IsVisible() { return mVisible; };
	inline Triangle *GetTriangles() { return mShape->GetTriangles(); }
	inline size_t GetTriangleCount() { return mShape->GetTriangleCount(); }
};
