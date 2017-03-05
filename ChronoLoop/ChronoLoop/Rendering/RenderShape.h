#pragma once
#include "RenderNode.h"
#include "RenderContext.h"
#include "RendererDefines.h"
#include <memory>
#include "Mesh.h"
#include "../Common/Math.h"

struct ID3D11Buffer;

namespace Epoch {

	struct RenderShape : public RenderNode {
		friend class RenderSet;
		std::shared_ptr<ID3D11Buffer*> mVertexBuffer, mIndexBuffer;
		unsigned int                   mIndexCount = 0, mVertexOffset = 0, mIndexOffset = 0;

		matrix4 mPosition;

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
		inline Triangle* GetTriangles() { return mMesh.GetTriangles(); }
		inline size_t GetTriangleCount() { return mMesh.GetNumTriangles(); }
		
		void Render();

	protected:
		RenderContext mContext;
		Mesh mMesh;
	};

}