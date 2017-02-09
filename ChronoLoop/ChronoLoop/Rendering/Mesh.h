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
#include "Structures.h"

struct Triangle
{
	vec4f* Vertex[3];
	vec4f Normal;
};

class Mesh
{
private:
	std::vector<Triangle> mTriangles;
	std::vector<VertexPosNormTex> mUniqueVerts;
	std::vector<unsigned short> mIndicies;
	wchar_t *mImage;
public:
	Mesh();
	Mesh(char *path);
	Mesh(char *path, wchar_t *path2);
	virtual ~Mesh();
	bool Load(char *path);
	Triangle *GetTriangles();
	size_t GetNumTriangles() { return mTriangles.size(); };
	//bool LoadBin(char *path);
	void Clear();
	void Invert();
	//void MakePlane();
	//void MakeViewPlane();
	VertexPosNormTex *GetVerts();
	size_t VertSize();
	unsigned short *GetIndicies();
	size_t IndicieSize();
	wchar_t *ImagePath();
	void SetImagePath(wchar_t* path) { mImage = path; };
};

