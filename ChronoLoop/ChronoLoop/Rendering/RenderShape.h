#pragma once
#include "RenderNode.h"
#include "RenderContext.h"
#include "RendererDefines.h"
#include <memory>
#include "Mesh.h"

struct ID3D11Buffer;

namespace RenderEngine {

	struct RenderShape : public RenderNode {
		std::shared_ptr<ID3D11Buffer*> mVertexBuffer, mIndexBuffer;
		unsigned int                   mIndexCount = 0;

		RenderShape();
		RenderShape(Mesh& _mesh);
		RenderShape(const char* _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs);
		~RenderShape();
		void Load(Mesh& _mesh);
		void Load(const char* _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs);
		void SetShaders(PixelShaderFormat pf, VertexShaderFormat vf);

		inline RenderContext &GetContext() { return mContext; }

		RenderShape& AddTexture(const char* _path, TextureType _position);
		RenderShape& AddTexture(const wchar_t* _path, TextureType _position);
		void Render();

	protected:
		RenderContext mContext;
	};

}