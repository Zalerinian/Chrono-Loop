#include "stdafx.h"
#include "RenderShape.h"
#include "renderer.h"
#include "Mesh.h"
#include <d3d11.h>
#include "InputLayoutManager.h"

RenderEngine::RenderShape::RenderShape() {
	mNext = nullptr;
}

RenderEngine::RenderShape::RenderShape(Mesh & _mesh) {
	this->Load(_mesh);
	mNext = nullptr;
}

void RenderEngine::RenderShape::Load(Mesh & _mesh) {
	mIndexBuffer = nullptr;
	mVertexBuffer = nullptr;
	auto device = *Renderer::Instance()->GetDevice().get();
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	auto verts = _mesh.GetVerts();
	vertexBufferData.pSysMem = verts;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc((UINT)_mesh.VertSize() * sizeof(VertexPosNormTex), D3D11_BIND_VERTEX_BUFFER);
	HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &mVertexBuffer);
	//Index Buffer
	mIndexCount = (unsigned int)_mesh.VertSize();
	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = _mesh.GetIndicies();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc((UINT)_mesh.IndicieSize() * sizeof(unsigned short), D3D11_BIND_INDEX_BUFFER);
	result = device->CreateBuffer(&indexBufferDesc, nullptr, &mIndexBuffer);
	auto context = *Renderer::Instance()->GetContext();
	context->UpdateSubresource(mIndexBuffer, 0, NULL, _mesh.GetIndicies(), 0, 0);
}

void RenderEngine::RenderShape::LoadShaders(char * vertex, char * pixel) {
	char *bytecode = nullptr;
	int bytelength;
	RenderEngine::InputLayoutManager::LoadShader(pixel, &bytecode, bytelength);
	(*RenderEngine::Renderer::Instance()->GetDevice())->CreatePixelShader(bytecode, bytelength, nullptr, &pShader);
	delete[] bytecode;
	RenderEngine::InputLayoutManager::LoadShader(vertex, &bytecode, bytelength);
	(*RenderEngine::Renderer::Instance()->GetDevice())->CreateVertexShader(bytecode, bytelength, nullptr, &vShader);
}