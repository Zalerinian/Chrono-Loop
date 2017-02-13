#include "VertexLayouts.hlsli"

cbuffer MVEP : register(b0) {
	matrix model;
	matrix view;
	matrix proj;
	matrix viewproj;
}

struct PSI
{
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
};

PSI main(VERTEX_POSNORMTEX input) {
	PSI output;
	float4 pos = input.position;
	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, proj);
	output.position = pos;
	output.normal = mul(input.normal, model);
	output.texCoord = input.texCoord;
	return output;
}