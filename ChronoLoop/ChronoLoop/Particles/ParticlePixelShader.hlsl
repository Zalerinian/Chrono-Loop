
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
    float4 offsetx;
    float4 offsety;
}

texture2D text2D0 : register(t0);
texture2D text2D1 : register(t1);
texture2D text2D2 : register(t2);
SamplerState samp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
    float4 col = text2D0.Sample(samp, float2(input.uv.x + offsetx.x, input.uv.y + offsety.x));
    clip(col.a - .25);
    //col += text2D1.Sample(samp, float2(input.uv.x + offsetx.y, input.uv.y + offsety.y));
    //col += text2D2.Sample(samp, float2(input.uv.x + offsetx.z, input.uv.y + offsety.z));
    float4 col1 = text2D1.Sample(samp, float2(input.uv.x + offsetx.y, input.uv.y + offsety.y));
    float4 col2 = text2D2.Sample(samp, float2(input.uv.x + offsetx.z, input.uv.y + offsety.z));

    col *= col1 != 0 ? float4(col1.xyz, 1) : 1;
    col *= col2 != 0 ? float4(col2.xyz,1) : 1;
    col.a += col1.a + col2.a;
	
    col *= (input.col.a == 0) ? 1 : input.col;

	return saturate(col);
}