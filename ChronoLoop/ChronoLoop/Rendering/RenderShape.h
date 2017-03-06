#pragma once
#include "RenderNode.h"
#include "RenderContext.h"
#include "RendererDefines.h"
#include "Mesh.h"
#include "../Common/Math.h"


namespace Epoch {

	struct RenderShape : public RenderNode {
		friend class RenderSet;
		friend struct std::less<RenderShape>;
		unsigned int mIndexCount = 0, mVertexOffset = 0, mIndexOffset = 0;

		matrix4 mPosition;

		RenderShape();
		RenderShape(Mesh& _mesh);
		RenderShape(const RenderShape& _other);
		RenderShape(const char* _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs);
		~RenderShape();
		void Load(Mesh& _mesh);
		void Load(const char* _path, bool _invert, PixelShaderFormat _ps, VertexShaderFormat _vs);
		void SetShaders(PixelShaderFormat pf, VertexShaderFormat vf);

		inline RenderContext &GetContext() { return mContext; }
		inline RenderContext GetContext() const { return mContext; }

		RenderShape& AddTexture(const char* _path, TextureType _position);
		RenderShape& AddTexture(const wchar_t* _path, TextureType _position);
		inline Triangle* GetTriangles() { return mMesh.GetTriangles(); }
		inline size_t GetTriangleCount() { return mMesh.GetNumTriangles(); }

		void Render() const;

		bool operator==(const RenderShape& _other) const;

	protected:
		RenderContext mContext;
		Mesh mMesh;
	};

}

namespace std {
	template<>
	struct less<Epoch::RenderShape> {
		bool operator()(const Epoch::RenderShape& _lhs, const Epoch::RenderShape& _rhs) const {
			// TODO: Create a good sorting algorithm for render shapes. Currently, the objects are not sorted.
			return _lhs.mVertexOffset < _rhs.mVertexOffset;
		}
	};

	template <>
	struct hash<Epoch::RenderShape> {
		size_t operator()(const Epoch::RenderShape& _shape) const {
			return (size_t)_shape.mVertexOffset;
		}
	};
}