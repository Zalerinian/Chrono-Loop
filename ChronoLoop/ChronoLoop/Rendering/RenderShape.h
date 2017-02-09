#pragma once
#include "RenderNode.h"

struct ID3D11Buffer;
class Mesh;
struct ID3D11PixelShader;
struct ID3D11VertexShader;

namespace RenderEngine {

	class RenderShape : public RenderNode {
	public:
		ID3D11PixelShader		*pShader;
		ID3D11VertexShader	*vShader;
		ID3D11Buffer				*mVertexBuffer;
		ID3D11Buffer				*mIndexBuffer;
		unsigned int				mIndexCount;

		RenderShape();
		RenderShape(Mesh& _mesh);
		void Load(Mesh& _mesh);
		void LoadShaders(char* vertex, char* pixel);


	};

}