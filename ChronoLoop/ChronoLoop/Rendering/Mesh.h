#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <openvr.h>
//#include "stdafx.h"
#include "Structures.h"
#include "RendererDefines.h"
#include "../Common/FileIO.h"

struct Triangle
{
	vec4f* Vertex[3];
	vec4f Normal;
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

template<typename T>
void MeshFormat<T>::CheckFormat()
{
	if (typeid(T) == typeid(VertexPos))
		mFormat = RenderEngine::eVERT_POS;
	else if (typeid(T) == typeid(VertexPosColor))
		mFormat = RenderEngine::eVERT_POSCOLOR;
	else if (typeid(T) == typeid(VertexPosTex))
		mFormat = RenderEngine::eVERT_POSTEX;
	else if (typeid(T) == typeid(VertexPosNormTex))
		mFormat = RenderEngine::eVERT_POSNORMTEX;
	else if (typeid(T) == typeid(VertexPosNormTanTex))
		mFormat = RenderEngine::eVERT_POSNORMTANTEX;
	else if (typeid(T) == typeid(VertexPosBoneWeight))
		mFormat = RenderEngine::eVERT_POSBONEWEIGHT;
	else if (typeid(T) == typeid(VertexPosBoneWeightNormTex))
		mFormat = RenderEngine::eVERT_POSBONEWEIGHTNORMTEX;
	else if (typeid(T) == typeid(VertexPosBoneWeightNormTanTex))
		mFormat = RenderEngine::eVERT_POSBONEWEIGHTNORMTANTEX;
}

template<typename T>
MeshFormat<T>::MeshFormat()
{
	CheckFormat();
}

template<typename T>
MeshFormat<T>::MeshFormat(const char * path)
{
	CheckFormat();
	this->Load(path);
}
template<typename T>
MeshFormat<T>::~MeshFormat()
{
	this->Clear();
}

template<typename T>
void MeshFormat<T>::loadShaders(char * pixel, char * vertex)
{
	char *bytecode = nullptr;
	int bytelength;
	FileIO::LoadBytes(pixel, &bytecode, bytelength);
	(*RenderEngine::Renderer::Instance()->GetDevice())->CreatePixelShader(bytecode, bytelength, nullptr, &pShader);
	delete[] bytecode;
	FileIO::LoadBytes(vertex, &bytecode, bytelength);
	(*RenderEngine::Renderer::Instance()->GetDevice())->CreateVertexShader(bytecode, bytelength, nullptr, &vShader);
}

template<typename T>
bool MeshFormat<T>::Load(vr::RenderModel_t * _model)
{
	for (unsigned int i = 0; i < _model->unVertexCount; i++)
	{
		T temp;
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

template<typename T>
Triangle * MeshFormat<T>::GetTriangles()
{
	if (mTriangles.size() != mIndicies.size() / 3)
	{
		for (unsigned int i = 0; i < mIndicies.size() / 3; i++)
		{
			Triangle temp;
			if (mFormat == RenderEngine::eVERT_POSNORMTEX || mFormat == RenderEngine::eVERT_POSNORMTANTEX || mFormat == RenderEngine::eVERT_POSBONEWEIGHTNORMTEX || mFormat == RenderEngine::eVERT_POSBONEWEIGHTNORMTANTEX)
			{
				temp.Normal = (mUniqueVerts[mIndicies[(i * 3) + 0]].Normal + mUniqueVerts[mIndicies[(i * 3) + 1]].Normal + mUniqueVerts[mIndicies[(i * 3) + 2]].Normal) / 2;
			}
			temp.Vertex[0] = &mUniqueVerts[mIndicies[(i * 3) + 0]].Position;
			temp.Vertex[1] = &mUniqueVerts[mIndicies[(i * 3) + 1]].Position;
			temp.Vertex[2] = &mUniqueVerts[mIndicies[(i * 3) + 2]].Position;
		}
	}
	return mTriangles.data();
}

//bool MeshFormat<T>::Load(std::vector<Vertex>* vecArray, std::vector<Bone>* boneArray, Animation * anim)
//{
//	this->Clear();
//	m_anim = *anim;
//	for (int i = 0; i < vecArray->size(); i++)
//	{
//		Indicies.push_back(i);
//		DirectX::XMFLOAT4 pos(vecArray->at(i).pos);
//		DirectX::XMFLOAT4 norm(vecArray->at(i).norms);
//		//DirectX::XMFLOAT4 norm(1, 1, 1, 1);
//		DirectX::XMFLOAT4 uv(vecArray->at(i).uvs);
//		AnimatedVert temp;
//		temp.position = pos;
//		temp.normal = norm;
//		temp.uv = uv;
//		float weights[4];
//		u_int indices[4];
//		for (int j = 0; j < 4; j++)
//		{
//			if (j < vecArray->at(i).blendWeights.size())
//			{
//				weights[j] = vecArray->at(i).blendWeights[j];
//				indices[j] = vecArray->at(i).boneIndices[j];
//			}
//			else
//			{
//				weights[j] = 0;
//				indices[j] = 0;
//			}
//		}
//		temp.blendWeights = DirectX::XMFLOAT4(weights);
//		temp.boneIndices = DirectX::XMUINT4(indices);
//		UniqueVerts.push_back(temp);
//	}
//	totalVerts = totalUnique = vecArray->size();
//
//	for (int i = 0; i < boneArray->size(); ++i)
//	{
//		Bones.push_back(&boneArray->at(i));
//	}
//	return true;
//}

//bool MeshFormat<T>::LoadBin(char* path)
//{
//	LoadFBX(path, false);
//	std::ifstream file;
//	std::string fileThing(path); 
//	size_t lastindex = fileThing.find_last_of(".");
//	std::string rawname = fileThing.substr(0, lastindex);
//	rawname.append(".bin");
//	file.open(rawname.c_str(), std::ios_base::binary);
//	if (file.is_open())
//	{
//		Skeleton skeleton;
//
//		//Number of Verts
//		unsigned int numVerts;
//		file.read((char*)&numVerts, sizeof(unsigned int));
//
//		//For every Vert
//		for (unsigned int i = 0; i < numVerts; ++i)
//		{
//			AnimatedVert vert;
//			//Pos
//			float tempP[4];
//			file.read((char*)&tempP, sizeof(float[4]));
//			vert.position = DirectX::XMFLOAT4(tempP);
//
//			//UVs
//			float tempUV[4];
//			file.read((char*)&tempUV, sizeof(float[2]));
//			vert.uv = DirectX::XMFLOAT4(tempUV);
//			vert.uv.z = 0;
//			vert.uv.w = 0;
//
//			//Norms
//			float tempN[4];
//			file.read((char*)&tempN, sizeof(float[4]));
//			DirectX::XMFLOAT4 norms(tempN);
//			vert.normal = norms;
//
//			//Number of Blend Weights
//			unsigned int numWeights;
//			file.read((char*)&numWeights, sizeof(unsigned int));
//
//			//For every Blend Weight
//			float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
//			for (unsigned int b = 0; b < numWeights && b < 4; ++b)
//			{
//				//Weights
//				file.read((char*)&weights[b], sizeof(float));
//			}
//			vert.blendWeights = XMFLOAT4(weights);
//			//Number of Bone Indices
//			unsigned int numIndices;
//			file.read((char*)&numIndices, sizeof(unsigned int));
//
//			//For every Bone Index
//			unsigned int indices[4] = { 0, 0, 0, 0 };
//			for (unsigned int b = 0; b < numIndices && b < 4; ++b)
//			{
//				//Weights
//				file.read((char*)&indices[b], sizeof(unsigned int));
//			}
//			vert.boneIndices = XMUINT4(indices);
//
//			if (FirstRun)
//			{
//				UniqueVerts.push_back(vert);
//				Indicies.push_back(i);
//			}
//		}
//		
//		//Number of Bones
//		unsigned int numBones;
//		file.read((char*)&numBones, sizeof(unsigned int));
//		
//		//For every Bone
//		for (unsigned int i = 0; i < numBones; ++i)
//		{
//			Bone* bone = new Bone();
//
//			//Bones index
//			file.read((char*)&bone->myIndex, sizeof(int));
//
//			//Bone's parent index
//			file.read((char*)&bone->parentIndex, sizeof(int));
//
//			//Bind Pos
//			file.read((char*)&bone->transform, sizeof(DirectX::XMFLOAT4X4));
//
//			//Number of Keyframes
//			unsigned int numKeys;
//			file.read((char*)&numKeys, sizeof(unsigned int));
//			bone->keyframes = new std::vector<KeyFrame*>();
//
//			//For each Keyframe
//			for(unsigned int k = 0; k < numKeys; ++k)
//			{
//				//Keyframes
//				KeyFrame* temp = new KeyFrame();
//				file.read((char*)temp, sizeof(KeyFrame));
//				bone->keyframes->push_back(temp);
//			}
//			skeleton.Bones.push_back(bone);
//			skeleton.m_anim.bones.push_back(bone);
//		}
//
//		//Animation Time
//		file.read((char*)&skeleton.m_anim.loopTime, sizeof(float));
//
//		for (unsigned int i = 0; i < numBones; ++i)
//		{
//			if (skeleton.m_anim.bones.at(i)->parentIndex > -1)
//			{
//				skeleton.m_anim.bones.at(i)->Parent = skeleton.m_anim.bones.at(skeleton.m_anim.bones.at(i)->parentIndex);
//				skeleton.m_anim.bones.at(skeleton.m_anim.bones.at(i)->parentIndex)->children.push_back(skeleton.m_anim.bones.at(i));
//			}
//			else if (skeleton.m_anim.bones.at(i)->parentIndex == -1)
//				skeleton.m_anim.bones.at(i)->Parent = nullptr;
//		}
//
//		NumSkeletons++;
//		skeletons.push_back(skeleton);
//		file.close();
//		FirstRun = false;
//		return true;
//	}
//
//	return false;
//}

template<typename T>
void MeshFormat<T>::Clear()
{
	mUniqueVerts.clear();
	mIndicies.clear();
	mTriangles.clear();
}

template<typename T>
void MeshFormat<T>::Invert()
{
	for (unsigned int i = 0; i < mIndicies.size() / 3; i++)
	{
		int temp = mIndicies[(i * 3)];
		mIndicies[(i * 3)] = mIndicies[(i * 3) + 2];
		mIndicies[(i * 3) + 2] = temp;
	}
}

//void MeshFormat<T>::MakePlane()
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

//void MeshFormat<T>::MakeViewPlane()
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

typedef MeshFormat<VertexPosNormTex> Mesh;