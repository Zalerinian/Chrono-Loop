#pragma once
#pragma comment (lib, "d3D11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include <d3D11.h>
#include <directxmath.h>
#include <vector>
#include <fstream>
#include "stdafx.h"
#include "renderer.h"

struct Skeleton
{
	std::vector<Bone*> Bones;
	Animation m_anim;
};

class Mesh
{
private:

	std::vector<AnimatedVert> UniqueVerts;
	std::vector<unsigned short> Indicies;
	//std::vector<Bone*> Bones;
	wchar_t *Image;
	size_t totalVerts;
	size_t totalUnique;
	Renderer::Node* m_node;
	bool FirstRun = true;

public:

	int currSkeleton = 0;
	int NumSkeletons = 0;
	Mesh();
	Mesh(char *path);
	Mesh(char *path, wchar_t *path2);
	virtual ~Mesh();
	bool Load(char *path);
	//bool Load(std::vector<Vertex>* vecArray, std::vector<Bone>* boneArray, Animation * anim);
	bool LoadBin(char *path);
	void Clear();
	void Invert();
	void MakePlane();
	void MakeViewPlane();
	AnimatedVert *GetVerts();
	size_t VertSize();
	unsigned short *GetIndicies();
	size_t IndicieSize();
	wchar_t *ImagePath();
	void CreateNode();
	void SetConstantBuffer(ModelViewProjectionConstantBuffer* cb) { m_node->SetConstantBuffer(cb); };
	void SetImagePath(wchar_t* path) { Image = path; };
	void Translate(float x, float y, float z) { m_node->Translate(x, y, z); };
	void SetPos(float x, float y, float z) { m_node->Position(x, y, z); };
	void Scale(float x, float y, float z) { m_node->Scale(x, y, z); };
	void Rotate(float x, float y, float z) { m_node->Rotate(x, y, z); };
	void ToggleRender() { if (m_node) m_node->ToggleRender(); };
};

