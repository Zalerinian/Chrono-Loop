#pragma once
#include "Component.h"
#include "../Rendering/RendererDefines.h"
#include "../Rendering/RenderShape.h"

namespace Epoch
{
	struct RenderShape;

	class MeshComponent : public Component
	{
		RenderShape* mShape;
		bool mVisible = true;

	public:
		MeshComponent(const char *_path);
		void Update();
		void Destroy();
		void SetVisible(bool _vis);
		void AddTexture(const char *_path, TextureType _type);
		inline bool IsVisible() { return mVisible; };
		inline Triangle *GetTriangles() { return mShape->GetTriangles(); }
		inline size_t GetTriangleCount() { return mShape->GetTriangleCount(); }
		inline RenderContext& GetContext() { return mShape->GetContext(); }
		void SetRasterState(RasterState _t);
	};

}
