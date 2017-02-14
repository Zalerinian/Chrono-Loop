#pragma once
#include <string>
#include <vector>
//#include "stdafx.h"
#include "Structures.h"
#include "RendererDefines.h"

struct Triangle
{
	vec4f* Vertex[3];
	vec4f Normal;
};
namespace vr
{
	struct RenderModel_t;
};
template<typename T>
class MeshFormat
{
private:
	std::vector<Triangle> mTriangles;
	std::vector<T> mUniqueVerts;
	std::vector<unsigned short> mIndicies;
	RenderEngine::VertFormat mFormat;
	void CheckFormat();
public:
	// Super temporary shit
	ID3D11PixelShader *pShader;
	ID3D11VertexShader *vShader;

	MeshFormat();
	MeshFormat(const char *path);
	virtual ~MeshFormat();
	void loadShaders(char *pixel, char* vertex);
	bool Load(const char *path);
	bool Load(vr::RenderModel_t *_model);
	Triangle *GetTriangles();
	inline size_t GetNumTriangles() { return mTriangles.size(); };
	//bool LoadBin(char *path);
	void Clear();
	void Invert();
	//void MakePlane();
	//void MakeViewPlane();
	inline T *GetVerts() { return mUniqueVerts.data(); };
	inline size_t VertSize() { return mUniqueVerts.size(); };
	inline unsigned short *GetIndicies() { return mIndicies.data(); };
	inline size_t IndicieSize() { return mIndicies.size(); };
};

typedef MeshFormat<VertexPosNormTex> Mesh;

