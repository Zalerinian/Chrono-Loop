#include "VertexLayouts.hlsli"
#include "LightFunctions.hlsli"

texture2D tDiffuse : register(t0);
SamplerState diffuseFilter : register(s0);

texture2D tNormal : register(t1);
SamplerState normalFilter : register(s1);

texture2D tSpecular : register(t2);
SamplerState specularFilter : register(s2);

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

struct PSI
{
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
	float4 wpos : WORLDPOS;
}; 

float4 main(PSI input) : SV_TARGET
{
	float4 color = float4(0,0,0,0);
	float4 l1, l2, l3;
	float4 diffuseColor = tDiffuse.Sample(diffuseFilter, input.texCoord.xy);
	//float4 diffuseColor = float4(1, 1, 1, 1);
	clip(diffuseColor.a - 0.25);

	l1 = ApplyDirectionalLight(dir, input.normal, dcol, diffuseColor);
	l2 = ApplyPointLight(ppos, input.wpos, input.normal, pcol, diffuseColor);
	l3 = ApplySpotLight(input.normal, slpos, input.wpos, cdir, cratio, slcol, diffuseColor);

	color = l1 + l2 + l3;

	return saturate(color) * diffuseColor;
}

//Empty object, light component
//Renderer