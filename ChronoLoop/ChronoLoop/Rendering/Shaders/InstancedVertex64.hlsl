#include "VertexLayouts.hlsli"

cbuffer VPBuffer : register(b0) {
	matrix view;
	matrix proj;
}

cbuffer ModelBuffer : register(b1) {
	matrix model;
}



struct PSI {
	float4 position : SV_POSITION;
	float4 normal   : NORMAL0;
	float4 texCoord : COLOR;
};

PSI main(VERTEX_POSNORMTEX input) {
	PSI r;
	r.position = input.position;
	r.normal = input.normal;
	r.texCoord = input.texCoord;
	return r;
}