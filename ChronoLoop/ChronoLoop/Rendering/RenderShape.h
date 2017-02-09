#pragma once
#include "RenderNode.h"

struct ID3D11Buffer;
class Mesh;

namespace RenderEngine {

	class RenderShape : RenderNode {
		ID3D11Buffer*				mVertexBuffer;
		ID3D11Buffer*				mIndexBuffer;
		unsigned int				mIndexCount;

	public:
		RenderShape();
		RenderShape(Mesh& _mesh);

		void Load(Mesh& _mesh);
	};

}