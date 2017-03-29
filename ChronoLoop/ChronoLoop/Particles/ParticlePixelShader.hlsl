//#include "../Effects.hlsli"

struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float4 uv : UV;
	float4 wpos : WPOS;
	float4 normal : NORMAL;
};

texture2D text2D : register(t0);
SamplerState samp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	float4 col = text2D.Sample(samp, input.uv.xy);
	clip(col.a - .25);
	col *= input.col;

	return col;
}