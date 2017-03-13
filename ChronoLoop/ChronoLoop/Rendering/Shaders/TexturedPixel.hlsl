#include "VertexLayouts.hlsli"

texture2D tDiffuse : register(t0);
SamplerState diffuseFilter : register(s0);

texture2D tNormal : register(t1);
SamplerState normalFilter : register(s1);

texture2D tSpecular : register(t2);
SamplerState specularFilter : register(s2);

struct PSI
{
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
};

float4 main(PSI input) : SV_TARGET
{
	float4 diffuseColor = tDiffuse.Sample(diffuseFilter, input.texCoord.xy);
	//float4 diffuseColor = float4(1, 1, 1, 1);
	clip(diffuseColor.a - 0.25);
	return diffuseColor;
}