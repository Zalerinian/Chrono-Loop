#include "VertexLayouts.hlsli"

cbuffer ModelBuffer : register(b0)
{
    matrix model[256];
} 

cbuffer VPLight : register(b1)
{
	matrix view;
	matrix proj;
}

struct PS
{
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
    float4 wpos : WORLDPOS;
};


PS main(VERTEX_POSNORMTEX input, uint id : SV_InstanceID)
{
	PS output;
	output.position = input.position;
    output.wpos = input.position;
    output.position = mul(output.position, model[id]);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);

    output.normal = input.normal;
    output.texCoord = input.texCoord;
	return output;
}

void SMPS(PS input) {}