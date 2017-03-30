
cbuffer GSMatrix : register(b0)
{
	matrix view;
	matrix proj;
}
cbuffer Model : register(b1)
{
	matrix model;
}

struct GSParticle
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float size : SIZE;
	float3 pad : USELESS;
};


GSParticle main(GSParticle input)
{
	GSParticle output;

	float4 pos = input.pos;
	
	//pos = mul(pos, model);
	//pos = mul(pos, view);
	//pos = mul(pos, proj);

	output.pos = float4(pos.xyz, 1);
	output.col = input.col;
	output.size = input.size;
	output.pad = input.pad;

	return output;
}