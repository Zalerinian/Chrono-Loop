#include "VertexLayouts.hlsli"

cbuffer ModelBuffer : register(b0) {
	matrix model[256];
}

cbuffer VPLight : register(b1)
{
    matrix view;
    matrix proj;
}

struct PSI
{
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
	float4 wpos : WORLDPOS;
    float4 shadowPos : SHADOW;
};

PSI main(VERTEX_POSNORMTEX input, uint id : SV_InstanceID) {

	PSI output;
	output.wpos = input.position;
	output.position = mul(input.position, model[id]);
	output.normal = mul(input.normal, model[id]);
	output.texCoord = input.texCoord;

    output.shadowPos = mul(output.wpos, model[id]);
    output.shadowPos = mul(output.shadowPos, view);
    output.shadowPos = mul(output.shadowPos, proj);
	return output;
}