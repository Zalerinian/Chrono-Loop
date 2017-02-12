#pragma once
#include "RenderNode.h"
#include "RenderContext.h"
#include "RendererDefines.h"
#include <memory>

struct ID3D11Buffer;
class Mesh;

namespace RenderEngine {

	struct RenderShape : public RenderNode {
		std::shared_ptr<ID3D11Buffer*> mVertexBuffer = nullptr, mIndexBuffer = nullptr;
		unsigned int                   mIndexCount = 0;

		RenderShape();
		RenderShape(Mesh& _mesh);
		void Load(Mesh& _mesh);
		void SetShaders(PixelShaderFormat pf, VertexShaderFormat vf);

		inline RenderContext &GetContext() { return mContext; }

		void Render();

	protected:
		RenderContext mContext;
	};

}