#pragma once
//#include "stdafx.h"
#include <string>
#include <vector>
#include <fstream>
#include <openvr.h>
#include "Structures.h"
#include "RendererDefines.h"
#include "../Common/FileIO.h"
#include <string>

namespace Epoch
{

	struct Triangle
	{
		vec3f Vertex[3];
		vec3f Normal;
		vec3f Centeroid;
	};

	template<typename T>
	class MeshFormat
	{
	private:
		std::vector<Triangle> mTriangles;
		std::vector<T> mUniqueVerts;
		std::vector<unsigned int> mIndicies;
		VertFormat mFormat;
		std::string mName;
		void CheckFormat();
	public:
		MeshFormat();
		MeshFormat(const char *path);
		virtual ~MeshFormat();
		void loadShaders(char *pixel, char* vertex);
		bool Load(const char *path);
		bool Load(vr::RenderModel_t *_model);
		bool Load(std::vector<VertexPosNormTanTex>&, std::vector<unsigned int>&);
		void Update(std::vector<VertexPosNormTanTex>&, std::vector<unsigned int>&);
		Triangle *GetTriangles();
		inline size_t GetNumTriangles() { return mTriangles.size(); };
		//bool LoadBin(char *path);
		void Clear();
		void Invert();
		//void MakePlane();
		//void MakeViewPlane();
		inline T *GetVerts() { return mUniqueVerts.data(); };
		inline size_t VertSize() { return mUniqueVerts.size(); };
		inline unsigned int *GetIndicies() { return mIndicies.data(); };
		inline size_t IndicieSize() { return mIndicies.size(); };
		inline std::string GetName() { return mName; }
	};

	template<typename T>
	void MeshFormat<T>::CheckFormat()
	{
		if (typeid(T) == typeid(VertexPos))
			mFormat = eVERT_POS;
		else if (typeid(T) == typeid(VertexPosColor))
			mFormat = eVERT_POSCOLOR;
		else if (typeid(T) == typeid(VertexPosTex))
			mFormat = eVERT_POSTEX;
		else if (typeid(T) == typeid(VertexPosNormTex))
			mFormat = eVERT_POSNORMTEX;
		else if (typeid(T) == typeid(VertexPosNormTanTex))
			mFormat = eVERT_POSNORMTANTEX;
		else if (typeid(T) == typeid(VertexPosBoneWeight))
			mFormat = eVERT_POSBONEWEIGHT;
		else if (typeid(T) == typeid(VertexPosBoneWeightNormTex))
			mFormat = eVERT_POSBONEWEIGHTNORMTEX;
		else if (typeid(T) == typeid(VertexPosBoneWeightNormTanTex))
			mFormat = eVERT_POSBONEWEIGHTNORMTANTEX;
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

	template <class T>
	Triangle * MeshFormat<T>::GetTriangles()
	{
		if (mTriangles.size() != mIndicies.size() / 3)
		{
			for (unsigned int i = 0; i < mIndicies.size() / 3; i++)
			{
				Triangle temp;
				if (mFormat == eVERT_POSNORMTEX || mFormat == eVERT_POSNORMTANTEX || mFormat == eVERT_POSBONEWEIGHTNORMTEX || mFormat == eVERT_POSBONEWEIGHTNORMTANTEX)
				{
					temp.Normal = (mUniqueVerts[mIndicies[(i * 3) + 0]].Normal + mUniqueVerts[mIndicies[(i * 3) + 1]].Normal + mUniqueVerts[mIndicies[(i * 3) + 2]].Normal) / 2;
				}
				temp.Vertex[0] = mUniqueVerts[mIndicies[(i * 3) + 0]].Position;
				temp.Vertex[1] = mUniqueVerts[mIndicies[(i * 3) + 1]].Position;
				temp.Vertex[2] = mUniqueVerts[mIndicies[(i * 3) + 2]].Position;
				temp.Centeroid = (temp.Vertex[0] + temp.Vertex[1] + temp.Vertex[2]) / 3;
				mTriangles.push_back(temp);
			}
		}
		return mTriangles.data();
	}

	template<typename T>
	void MeshFormat<T>::Clear()
	{
		mName = "";
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

	typedef MeshFormat<VertexPosNormTanTex> Mesh;

}