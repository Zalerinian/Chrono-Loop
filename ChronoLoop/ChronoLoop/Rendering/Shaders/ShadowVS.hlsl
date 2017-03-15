#include "VertexLayouts.hlsli"

cbuffer VPLight : register(b0)
{
	matrix view;
	matrix proj;
}

struct PS
{
	float4 position :	SV_POSITION;
	//float4 normal	:	NORMAL0;
	//float4 texCoord :	COLOR;
};


PS main(VERTEX_POSNORMTEX input)
{
	PS output;
	output.position = input.position;
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);
	return output;
}

void SMPS(PS input) {}