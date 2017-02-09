cbuffer MVEP : register(b0) {
	matrix model;
	matrix view;
	matrix eye;
	matrix projection;
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
	pos = mul(pos, eye);
	pos = mul(pos, projection);
	output.pos = pos;
	return output;
}