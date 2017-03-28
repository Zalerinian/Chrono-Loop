#include "../RenderShaderDefines.hlsli"

cbuffer VPBuffer : register(b0) {
	matrix view;
	matrix proj;
}

cbuffer ModelBuffer : register(b1) {
	matrix model;
}

struct VertexShaderInput {
	float4 pos : POSITION;
};

struct PixelShaderInput {
	float4 pos : SV_POSITION;
};

PixelShaderInput main(VertexShaderInput input) {
	PixelShaderInput output;
	float4 pos = input.pos;
	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, proj);
	output.pos = pos;
	return output;
}