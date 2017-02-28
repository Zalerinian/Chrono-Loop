#include "Mesh.h"
#include "Structures.h"

template<>
bool MeshFormat<VertexPos>::Load(const char * path)
{
	this->Clear();
	mName = path;
	std::vector<VertexPos> Verts;
	std::vector<unsigned int> Ind;
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
			else if (line[1] == ' ') {
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
				VertexPos temp;
				temp.Position = verts[vertexIndex[i] - 1];
				temp.Position.w = 1;
				Ind.push_back((unsigned int)Verts.size());
				Verts.push_back(temp);
			}
		}
	}
	mUniqueVerts = Verts;
	mIndicies = Ind;
	file.close();
	return true;
}

template<>
bool MeshFormat<VertexPosTex>::Load(const char * path)
{
	this->Clear();
	mName = path;
	std::vector<VertexPosTex> Verts;
	std::vector<unsigned int> Ind;
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
			else if (line[1] == ' ') {
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
				VertexPosTex temp;
				temp.Position = verts[vertexIndex[i] - 1];
				temp.Position.w = 1;
				temp.UV = uvs[uvIndex[i] - 1];
				temp.UV.z = 0;
				temp.UV.w = 0;
				Ind.push_back((unsigned int)Verts.size());
				Verts.push_back(temp);
			}
		}
	}
	mUniqueVerts = Verts;
	mIndicies = Ind;
	file.close();
	return true;
}

template<>
bool MeshFormat<VertexPosColor>::Load(const char * path)
{
	this->Clear();
	mName = path;
	std::vector<VertexPosColor> Verts;
	std::vector<unsigned int> Ind;
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
			else if (line[1] == ' ') {
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
				VertexPosColor temp;
				temp.Position = verts[vertexIndex[i] - 1];
				temp.Position.w = 1;
				temp.Color = vec4f(0, 0, 0, 0);
				Ind.push_back((unsigned int)Verts.size());
				Verts.push_back(temp);
			}
		}
	}
	mUniqueVerts = Verts;
	mIndicies = Ind;
	file.close();
	return true;
}

template<>
bool MeshFormat<VertexPosNormTex>::Load(const char * path)
{
	this->Clear();
	mName = path;
	std::vector<VertexPosNormTex> Verts;
	std::vector<unsigned int> Ind;
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
			else if (line[1] == ' ') {
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
			Triangle t;
			for (int i = 0; i < 3; i++)
			{
				VertexPosNormTex temp;
				temp.Position = verts[vertexIndex[i] - 1];
				temp.Position.w = 1;
				temp.UV = uvs[uvIndex[i] - 1];
				temp.UV.z = 0;
				temp.UV.w = 0;
				temp.Normal = norms[normalIndex[i] - 1];
				temp.Normal.w = 0;
				Ind.push_back((unsigned int)Verts.size());
				t.Vertex[i] = temp.Position;
				Verts.push_back(temp);
			}
		}
	}
	mUniqueVerts = Verts;
	mIndicies = Ind;
	file.close();
	return true;
}

template<>
bool MeshFormat<VertexPosNormTanTex>::Load(const char * path)
{
	this->Clear();
	mName = path;
	std::vector<VertexPosNormTanTex> Verts;
	std::vector<unsigned int> Ind;
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
			else if (line[1] == ' ') {
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
				VertexPosNormTanTex temp;
				temp.Position = verts[vertexIndex[i] - 1];
				temp.Position.w = 1;
				temp.UV = uvs[uvIndex[i] - 1];
				temp.UV.z = 0;
				temp.UV.w = 0;
				temp.Normal = norms[normalIndex[i] - 1];
				temp.Normal.w = 0;
				temp.Tangent = vec4f();
				temp.Determinant = 0;
				Ind.push_back((unsigned int)Verts.size());
				Verts.push_back(temp);
			}
		}
	}
	mUniqueVerts = Verts;
	mIndicies = Ind;
	file.close();
	return true;
}

template<>
bool MeshFormat<VertexPosBoneWeightNormTanTex>::Load(const char * path)
{
	this->Clear();
	mName = path;
	std::vector<VertexPosBoneWeightNormTanTex> Verts;
	std::vector<unsigned int> Ind;
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
			else if (line[1] == ' ') {
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
				VertexPosBoneWeightNormTanTex temp;
				temp.Position = verts[vertexIndex[i] - 1];
				temp.Position.w = 1;
				temp.UV = uvs[uvIndex[i] - 1];
				temp.UV.z = 0;
				temp.UV.w = 0;
				temp.Normal = norms[normalIndex[i] - 1];
				temp.Normal.w = 0;
				temp.Tangent = vec4f();
				temp.Determinant = 0;
				temp.BoneIndices = vec4i(0, 0, 0, 0);
				temp.Weights = vec4f(1, 0, 0, 0);
				Ind.push_back((unsigned int)Verts.size());
				Verts.push_back(temp);
			}
		}
	}
	mUniqueVerts = Verts;
	mIndicies = Ind;
	file.close();
	return true;
}

template<>
bool MeshFormat<VertexPosBoneWeightNormTex>::Load(const char * path)
{
	this->Clear();
	mName = path;
	std::vector<VertexPosBoneWeightNormTex> Verts;
	std::vector<unsigned int> Ind;
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
			else if (line[1] == ' ') {
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
				VertexPosBoneWeightNormTex temp;
				temp.Position = verts[vertexIndex[i] - 1];
				temp.Position.w = 1;
				temp.UV = uvs[uvIndex[i] - 1];
				temp.UV.z = 0;
				temp.UV.w = 0;
				temp.Normal = norms[normalIndex[i] - 1];
				temp.Normal.w = 0;
				temp.BoneIndices = vec4i(0, 0, 0, 0);
				temp.Weights = vec4f(1, 0, 0, 0);
				Ind.push_back((unsigned int)Verts.size());
				Verts.push_back(temp);
			}
		}
	}
	mUniqueVerts = Verts;
	mIndicies = Ind;
	file.close();
	return true;
}

template<>
bool MeshFormat<VertexPosBoneWeight>::Load(const char * path)
{
	this->Clear();
	mName = path;
	std::vector<VertexPosBoneWeight> Verts;
	std::vector<unsigned int> Ind;
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
			else if (line[1] == ' ') {
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
				VertexPosBoneWeight temp;
				temp.Position = verts[vertexIndex[i] - 1];
				temp.Position.w = 1;
				temp.BoneIndices = vec4i(0, 0, 0, 0);
				temp.Weights = vec4f(1, 0, 0, 0);
				Ind.push_back((unsigned int)Verts.size());
				Verts.push_back(temp);
			}
		}
	}
	mUniqueVerts = Verts;
	mIndicies = Ind;
	file.close();
	return true;
}