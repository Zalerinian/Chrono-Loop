#pragma once
#include <d3d11.h>
#include <memory>

#define EXPORT __declspec(dllexport)
extern "C" {

class EXPORT Renderer {
	static Renderer* sInstance;
	// D3D11 Variables
	std::shared_ptr<ID3D11Device> device;
	std::shared_ptr<ID3D11DeviceContext> context;


	Renderer();
public:
	static Renderer* Instance();
	static void Render();
};

class RenderNode {
	friend Renderer;
	RenderNode();
};
}
