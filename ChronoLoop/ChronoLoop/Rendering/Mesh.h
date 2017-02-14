#pragma once
#pragma comment (lib, "d3D11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include <d3D11.h>
#include <directxmath.h>
#include <vector>
#include <fstream>
//#include "stdafx.h"
#include "Structures.h"

struct Triangle
{
	vec4f Vertex[3];
	vec4f Normal;
};
namespace vr
{
	struct RenderModel_t;
}

class Mesh
{
private:
	std::vector<Triangle> mTriangles;
	std::vector<VertexPosNormTex> mUniqueVerts;
	std::vector<unsigned short> mIndicies;
public:
	Mesh();
	Mesh(const char *path);
	virtual ~Mesh();
	bool Load(const char *path);
	bool Load(vr::RenderModel_t *_model);
	Triangle *GetTriangles();
	inline size_t GetNumTriangles() { return mTriangles.size(); };
	//bool LoadBin(char *path);
	void Clear();
	void Invert();
	//void MakePlane();
	//void MakeViewPlane();
	VertexPosNormTex *GetVerts();
	size_t VertSize();
	unsigned short *GetIndicies();
	size_t IndicieSize();
};

