#pragma once
#include "Component.h"
#include "../Rendering/RendererDefines.h"
#include "../Rendering/RenderShape.h"
#include "../Common/GhostList.h"

#define DESTROY_NODE(x) { \
	if(x != nullptr) { \
		delete x; \
		x = nullptr; \
	} \
}

namespace Epoch
{
	struct RenderShape;

	class MeshComponent : public Component
	{
	protected:
		GhostList<matrix4>::GhostNode* mNode = nullptr;
		RenderShape* mShape = nullptr;
		bool mVisible = true;

		virtual void CreateNode();
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
		void SetVertexShader(VertexShaderFormat _vf);
		void SetPixelShader(PixelShaderFormat _pf);
		void SetGeometryShader(GeometryShaderFormat _gf);
		RenderShape* GetShape();
		void ForceReinsertion();

		bool CanCreateNode();
	};

}
