#pragma once
#include <vector>
#include <DirectXMath.h>
#include <fstream>

struct Vertex
{
	float pos[4];
	float uvs[2];
	float norms[4];
	std::vector<float> blendWeights;
	std::vector<int> boneIndices;
	unsigned int controlPointIndice;
};

struct KeyFrame
{
	float keyTime;
	DirectX::XMFLOAT4X4 transform;
};

struct Bone
{
	int myIndex;
	int parentIndex;
	std::vector<int> childrenIndices;
	Bone* Parent;
	std::vector<Bone*> children;
	DirectX::XMFLOAT4X4 transform;
	//float Translation[4];
	//float Rotation[4];
	std::vector<KeyFrame*>* keyframes;
};

struct Animation
{
	float loopTime;
	std::vector<Bone*> bones;
};

extern "C"
{
	__declspec(dllexport) void InitFBXLoader();
	__declspec(dllexport) void LoadFBX(char* path, bool Override);
}