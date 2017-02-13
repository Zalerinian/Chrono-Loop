#pragma once
#include "RenderNode.h"
#include "RenderContext.h"
#include "RendererDefines.h"
#include <memory>
#include "Mesh.h"

struct ID3D11Buffer;

namespace RenderEngine {

	struct RenderShape : public RenderNode {
		std::shared_ptr<ID3D11Buffer*> mVertexBuffer = nullptr, mIndexBuffer = nullptr;
		unsigned int                   mIndexCount = 0;

		RenderShape();
		RenderShape(Mesh& _mesh);
		void Load(Mesh& _mesh);
		void SetShaders(PixelShaderFormat pf, VertexShaderFormat vf);

		inline RenderContext &GetContext() { return mContext; }

		void AddTexture(const char* _path, TextureType position);
		void AddTexture(const wchar_t* _path, TextureType position);
		void Render();

	protected:
		RenderContext mContext;
	};

}