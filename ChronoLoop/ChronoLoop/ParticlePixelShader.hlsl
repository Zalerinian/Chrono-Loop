#include "Rendering/Shaders/LightFunctions.hlsli"

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

cbuffer DirectionalLight : register(b0)
{
	float4 dir;
	float4 dcol;
};
cbuffer PointLight : register(b1)
{
	float4 ppos;
	float4 pcol;
};
cbuffer SpotLight : register(b2)
{
	float4 cdir;
	float4 slpos;
	float4 slcol;
	float cratio;
};

float4 main(GSOutput input) : SV_TARGET
{
	float4 color = float4(0,0,0,0);
	float4 l1, l2, l3;

	float4 col = text2D.Sample(samp, input.uv.xy);
	clip(col.a - .25);
	col *= input.col;

	/*l1 = ApplyDirectionalLight(dir, input.normal, dcol, dcol);
	l2 = ApplyPointLight(ppos, input.wpos, input.normal, pcol, dcol);
	l3 = ApplySpotLight(input.normal, slpos, input.wpos, cdir, cratio, slcol, dcol);

	color = l1 + l2 + l3;*/

	return (saturate(color) + float4(.25, .25, .25, .25)) * col;
}