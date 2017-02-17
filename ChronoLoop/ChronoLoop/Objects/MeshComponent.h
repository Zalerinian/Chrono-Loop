#pragma once
#include "Component.h"
#include "../Rendering/RendererDefines.h"

namespace RenderEngine {
	struct RenderShape;
}

class MeshComponent : public Component {
	RenderEngine::RenderShape* mShape;
	bool mVisible;

public:
	// TODO: Add textures to this class.
	MeshComponent(const char *_path);
	void Update();
	void Destroy();
	void SetVisible(bool _vis);
	void AddTexture(const char *_path, RenderEngine::TextureType _type);
};
