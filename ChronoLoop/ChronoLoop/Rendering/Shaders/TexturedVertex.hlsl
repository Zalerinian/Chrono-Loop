#include "VertexLayouts.hlsli"

cbuffer VPBuffer : register(b0) {
	matrix view;
	matrix proj;
}

cbuffer ModelBuffer : register(b1) {
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
	float4 pos = input.position;
	pos = mul(pos, model[id]);
	pos = mul(pos, view);
	pos = mul(pos, proj);
	output.position = pos;
	output.normal = mul(input.normal, model[id]);
	output.texCoord = input.texCoord;
	return output;
}