#pragma once
//#include "stdafx.h"
#include "../Common/Math.h"

namespace Epoch
{

	struct VertexPos
	{
		vec4f	Position;
	};
	struct VertexPosColor
	{
		vec4f	Position;
		vec4f	Color;
	};
	struct VertexPosTex
	{
		vec4f	Position;
		vec4f	UV;
	};
	struct VertexPosNormTex
	{
		vec4f	Position;
		vec4f	Normal;
		vec4f	UV;
	};
	struct VertexPosNormTanTex
	{
		vec4f	Position;
		vec4f	Normal;
		vec4f	UV;
		vec4f	Tangent;
		float	Determinant;
	};
	struct VertexPosBoneWeight
	{
		vec4f	Position;
		vec4i	BoneIndices;
		vec4f	Weights;
	};
	struct VertexPosBoneWeightNormTex
	{
		vec4f	Position;
		vec4i	BoneIndices;
		vec4f	Weights;
		vec4f	Normal;
		vec4f	UV;
		VertexPosBoneWeightNormTex() {};
	};
	struct VertexPosBoneWeightNormTanTex
	{
		vec4f	Position;
		vec4f	Normal;
		vec4f	UV;
		vec4i	BoneIndices;
		vec4f	Weights;
		vec4f	Tangent;
		float	Determinant;
		VertexPosBoneWeightNormTanTex() {};
	};

	struct Light
	{
	public:
		int Type;

	protected:
		float pad2[3];
		vec4f Position, Direction, ConeDirection, Color;
		float ConeRatio;
		float pad[3];
	};

	struct Directional : public Light
	{
		vec4f & mDirection = Direction;
		vec4f & mColor = Color;
		Directional()
		{
			Type = 1;
		}
	};
	struct Point : public Light
	{
		vec4f & mPosition = Position;
		vec4f & mColor = Color;
		Point()
		{
			Type = 2;
		}
	};
	struct Spot : public Light
	{
		vec4f & mConeDirection = ConeDirection;
		vec4f & mPosition = Position;
		vec4f & mColor = Color;
		float & mConeRatio = ConeRatio;
		Spot()
		{
			Type = 4;
		}
	};
}