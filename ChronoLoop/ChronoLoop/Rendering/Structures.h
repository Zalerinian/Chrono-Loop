#pragma once
#include "stdafx.h"
#include "../Common/Math.h"

struct VertexPos
{
	vec3f	Position;
};
struct VertexPosColor
{
	vec3f	Position;
	vec4f	Color;
};
struct VertexPosTex
{
	vec3f	Position;
	vec2f	UV;
};
struct VertexPosNormTex
{
	vec3f	Position;
	vec3f	Normal;
	vec2f	UV;
};
struct VertexPosNormTanTex
{
	vec3f	Position;
	vec3f	Normal;
	vec2f	UV;
	vec3f	Tangent;
	float	Determinant;
};
struct VertexPosBoneWeight
{
	vec3f	Position;
	vec4i	BoneIndices;
	vec4f	Weights;
};
struct VertexPosBoneWeightNormTex
{
	vec3f	Position;
	vec4i	BoneIndices;
	vec4f	Weights;
	vec3f	Normal;
	vec2f	UV;
};
struct VertexPosBoneWeightNormTanTex
{
	vec3f	Position;
	vec3f	Normal;
	vec2f	UV;
	vec4i	BoneIndices;
	vec4f	Weights;
	vec3f	Tangent;
	float	Determinant;
};