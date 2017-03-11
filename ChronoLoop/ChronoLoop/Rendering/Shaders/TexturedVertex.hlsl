#include "VertexLayouts.hlsli"

cbuffer ModelBuffer : register(b0) {
	matrix model[256];
}

struct PSI
{
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
};

PSI main(VERTEX_POSNORMTEX input, uint id : SV_InstanceID) {
	PSI output;
	output.position = mul(input.position, model[id]);
	output.normal = mul(input.normal, model[id]);
	output.texCoord = input.texCoord;
	return output;
}