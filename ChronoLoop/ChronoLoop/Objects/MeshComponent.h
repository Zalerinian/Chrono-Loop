#pragma once
#include "Component.h"
#include "../Rendering/RendererDefines.h"
#include "../Rendering/RenderShape.h"
#include "../Common/GhostList.h"

namespace Epoch
{
	struct RenderShape;

	class MeshComponent : public Component
	{
		GhostList<matrix4>::GhostNode* mNode;
		RenderShape* mShape;
		bool mVisible = true;

	public:
		MeshComponent(const char *_path);
		virtual void Update() override;
		virtual void Destroy() override;
		MeshComponent* SetVisible(bool _vis);
		MeshComponent* AddTexture(const char *_path, TextureType _type);
		inline bool IsVisible() { return mVisible; };
		inline Triangle *GetTriangles() { return mShape->GetTriangles(); }
		inline size_t GetTriangleCount() { return mShape->GetTriangleCount(); }
		inline RenderContext& GetContext() { return mShape->GetContext(); }
		void SetRasterState(RasterState _t);
	};

}
