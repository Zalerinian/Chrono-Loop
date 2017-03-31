#pragma once
#include "RenderNode.h"
#include "RenderContext.h"
#include "RendererDefines.h"
#include "Mesh.h"
#include "../Common/Math.h"


namespace Epoch {

	struct RenderShape : public RenderNode {
		friend class RenderSet;
		unsigned int mIndexCount = 0, mVertexOffset = 0, mIndexOffset = 0;

		matrix4 mPosition;

		RenderShape();
		RenderShape(Mesh& _mesh);
		RenderShape(const RenderShape& _other);
		RenderShape(const char* _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs);
		RenderShape(const char* _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs, GeometryShaderFormat _gs);
		~RenderShape();
		void Load(Mesh *_mesh);
		void Load(const char* _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs);
		void Load(const char* _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs, GeometryShaderFormat _gs);
		void SetShaders(PixelShaderFormat pf, VertexShaderFormat vf);
		void SetShaders(PixelShaderFormat pf, VertexShaderFormat vf, GeometryShaderFormat _gf);
		void SetVertexShader(VertexShaderFormat _vf);
		void SetPixelShader(PixelShaderFormat _pf);
		void SetGeometryShader(GeometryShaderFormat _gf);
		VertexShaderFormat GetVertexShader();
		PixelShaderFormat GetPixelShader();
		GeometryShaderFormat GetGeometryShader();



		inline RenderContext &GetContext() { return mContext; }
		inline RenderContext GetContext() const { return mContext; }

		RenderShape& AddTexture(const char* _path, TextureType _position);
		RenderShape& AddTexture(const wchar_t* _path, TextureType _position);
		inline Triangle* GetTriangles() { return mMesh->GetTriangles(); }
		inline size_t GetTriangleCount() { return mMesh->GetNumTriangles(); }

		void Render(UINT _instanceCount = 1) const;

		bool operator==(const RenderShape& _other) const;

	protected:
		RenderContext mContext;
		Mesh* mMesh;
	};

}