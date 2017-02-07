#pragma once
#include <d3d11.h>
#include <vector>

struct Shader {
	const char *path;
	std::vector<byte> bytecode;
	std::vector<ID3D11Buffer*> buffers;
protected:
	Shader();
};

struct PixelShader : Shader {
	ID3D11PixelShader *shader = nullptr;
	PixelShader(const char *_path);
};

struct VertexShader : Shader {
	ID3D11VertexShader *shader = nullptr;
	ID3D11InputLayout *layout;
	VertexShader(const char *_path, D3D11_INPUT_ELEMENT_DESC **_layout);
};