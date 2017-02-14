//#include "stdafx.h"
#include "Mesh.h"
#include <ppltasks.h>
#include <fstream>
#include <string>
#include "Renderer.h"
#include "../Common/FileIO.h"
#include <openvr.h>

using namespace Concurrency;

Mesh::Mesh()
{
}

Mesh::Mesh(const char * path)
{
	this->Load(path);
}
Mesh::~Mesh()
{
	this->Clear();
}

bool Mesh::Load(const char * path)
{
	this->Clear();
	std::vector<VertexPosNormTex> Verts;
	std::vector<unsigned short> Ind;
	int index = 1;
	std::ifstream file;
	file.open(path, std::ios::in);
	if (!file.is_open())
		return false;
	std::string line;
	std::vector<vec4f> verts;
	std::vector<vec4f> norms;
	std::vector<vec4f> uvs;
	while (std::getline(file, line))
	{
		if (line[0] == 'v') {
			if (line[1] == 't') {
				vec4f uv;
				sscanf_s(line.c_str(), "vt %f %f\n", &uv.x, &uv.y);
				uv.y = 1 - uv.y;
				uvs.push_back(uv);
			}
			else if (line[1] == 'n') {
				vec4f normal;
				sscanf_s(line.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
				norms.push_back(normal);
			}
			else if(line[1] == ' ') {
				vec4f vertex;
				sscanf_s(line.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				verts.push_back(vertex);
			}
		}
		else if (line[0] == 'f') {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				return false;
			}
			for (int i = 0; i < 3; i++)
			{
				VertexPosNormTex temp;
				temp.Position = verts[vertexIndex[i] - 1];
				temp.Position.w = 1;
				//temp.color = DirectX::XMFLOAT4(i == 0, i == 1, i == 3);
				temp.Normal = norms[normalIndex[i] - 1];
				temp.UV = uvs[uvIndex[i] - 1];
				temp.UV.z = 0;
				Ind.push_back((unsigned short)Verts.size());
				Verts.push_back(temp);
			}
		}
	}
	mUniqueVerts = Verts;
	mIndicies = Ind;
	file.close();
	return true;
}

bool Mesh::Load(vr::RenderModel_t * _model)
{
	for (unsigned int i = 0; i < _model->unVertexCount; i++)
	{
		VertexPosNormTex temp;
		for (int j = 0; j < 3; j++)
			temp.Position.xyzw[j] = _model->rVertexData[i].vPosition.v[j];
		temp.Position.w = 1;
		
		mUniqueVerts.push_back(temp);
	}
	for (unsigned int i = 0; i < _model->unTriangleCount * 3; i++)
	{
		mIndicies.push_back(_model->rIndexData[i]);
	}
	return true;
}

Triangle * Mesh::GetTriangles()
{
	if (mTriangles.size() != mIndicies.size() / 3)
	{
		for (unsigned int i = 0; i < mIndicies.size() / 3; i++)
		{
			Triangle temp;
			temp.Normal = (mUniqueVerts[mIndicies[(i * 3) + 0]].Normal + mUniqueVerts[mIndicies[(i * 3) + 1]].Normal + mUniqueVerts[mIndicies[(i * 3) + 2]].Normal) / 2;
			temp.Vertex[0] = mUniqueVerts[mIndicies[(i * 3) + 0]].Position;
			temp.Vertex[1] = mUniqueVerts[mIndicies[(i * 3) + 1]].Position;
			temp.Vertex[2] = mUniqueVerts[mIndicies[(i * 3) + 2]].Position;
		}
	}
	return mTriangles.data();
}

void Mesh::Clear()
{
	mUniqueVerts.clear();
	mIndicies.clear();
	mTriangles.clear();
}

void Mesh::Invert()
{
	for (unsigned int i = 0; i < mIndicies.size() / 3; i++)
	{
		int temp = mIndicies[(i * 3)];
		mIndicies[(i * 3)] = mIndicies[(i * 3) + 2];
		mIndicies[(i * 3) + 2] = temp;
	}
}

//void Mesh::MakePlane()
//{
//	this->Clear();
//
//	static const AnimatedVert cubeVertices[] =
//	{
//		{ XMFLOAT4(-50.0f,  0.0f, -50.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0, 0, 0, 0), XMUINT4(0, 0, 0, 0) },
//		{ XMFLOAT4(-50.0f,  0.0f,  50.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0, 0, 0, 0), XMUINT4(0, 0, 0, 0) },
//		{ XMFLOAT4(50.0f,  0.0f, -50.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0, 0, 0, 0), XMUINT4(0, 0, 0, 0) },
//		{ XMFLOAT4(50.0f,  0.0f,  50.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0, 0, 0, 0), XMUINT4(0, 0, 0, 0) },
//	};
//	for (int i = 0; i < 6; i++)
//		UniqueVerts.push_back(cubeVertices[i]);
//	totalUnique = UniqueVerts.size();
//
//	static const unsigned short cubeIndices[] =
//	{
//		0,3,2, // +y
//		0,1,3
//	};
//	for (int i = 0; i < 6; i++)
//		Indicies.push_back(cubeIndices[i]);
//	totalVerts = Indicies.size();
//}

//void Mesh::MakeViewPlane()
//{
//	this->Clear();
//
//	static const VertexPosNormTex cubeVertices[] =
//	{
//		{ XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0, 0, 0, 0), XMUINT4(0, 0, 0, 0) },
//		{ XMFLOAT4(-1.0f,  1.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0, 0, 0, 0), XMUINT4(0, 0, 0, 0) },
//		{ XMFLOAT4(1.0f,  -1.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0, 0, 0, 0), XMUINT4(0, 0, 0, 0) },
//		{ XMFLOAT4(1.0f,   1.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0, 0, 0, 0), XMUINT4(0, 0, 0, 0) },
//	};
//	for (int i = 0; i < 6; i++)
//		UniqueVerts.push_back(cubeVertices[i]);
//	totalUnique = UniqueVerts.size();
//
//	static const unsigned short cubeIndices[] =
//	{
//		0,3,2, // +y
//		0,1,3
//	};
//	for (int i = 0; i < 6; i++)
//		Indicies.push_back(cubeIndices[i]);
//	totalVerts = Indicies.size();
//}

VertexPosNormTex * Mesh::GetVerts()
{
	return mUniqueVerts.data();
}

size_t Mesh::VertSize()
{
	return mUniqueVerts.size();
}

unsigned short * Mesh::GetIndicies()
{
	return mIndicies.data();
}

size_t Mesh::IndicieSize()
{
	return mIndicies.size();
}