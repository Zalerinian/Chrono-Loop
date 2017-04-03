//#include "../Effects.hlsli"

struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float4 uv : UV;
	float4 wpos : WPOS;
	float4 normal : NORMAL;
};

//TODO: VERY IMPORTANT-- ONLY SUPPORTS STRIPS OF SPRITES FOR ANIMATION
cbuffer TextureData : register(b0)
{
    float offset[4];
}
texture2D text2D0 : register(t0);
texture2D text2D1 : register(t1);
texture2D text2D2 : register(t2);
SamplerState samp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
    
    float4 col = text2D0.Sample(samp, float2(input.uv.x + offset[0], input.uv.y));
    col += text2D1.Sample(samp, float2(input.uv.x + offset[1], input.uv.y));
    col += text2D2.Sample(samp, float2(input.uv.x + offset[2], input.uv.y));

	clip(col.a - .25);
	col *= input.col;

	return col;
}