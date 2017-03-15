#include "VertexLayouts.hlsli"
#include "LightFunctions.hlsli"

texture2D tDiffuse : register(t0);
SamplerState diffuseFilter : register(s0);

texture2D tNormal : register(t1);
SamplerState normalFilter : register(s1);

texture2D tSpecular : register(t2);
SamplerState specularFilter : register(s2);

struct DLight {
	float4 dir;
	float4 color;
};

struct PLight {
	float4 pos;
	float4 color;
};

struct SLight {
	float4 dir;
	float4 pos;
	float4 color;
	float  ratio;
};

cbuffer _DLight : register(b0)
{
	DLight DirLight;
	//float4 dir;
	//float4 dcol;
};

cbuffer _PLight : register(b1)
{
	PLight PointLight;
	//float4 ppos; 
	//float4 pcol;
};

cbuffer _SLight : register(b2)
{
	SLight SpotLight;
	//float4 cdir;
	//float4 slpos;
	//float4 slcol;
	//float cratio;
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

	l1 = ApplyDirectionalLight(DirLight.dir, input.normal, DirLight.color, diffuseColor);
	l2 = ApplyPointLight(PointLight.pos, input.wpos, input.normal, PointLight.color, diffuseColor);
	l3 = ApplySpotLight(input.normal, SpotLight.pos, input.wpos, SpotLight.dir, SpotLight.ratio, SpotLight.color, diffuseColor);

	color = l1 + l2 + l3;

	return (saturate(color) + float4(.25,.25,.25,.25))  * diffuseColor;
}

//Empty object, light component
//Renderer