#include "VertexLayouts.hlsli"
#include "LightFunctions.hlsli"
#include "Structs.hlsli"

texture2D tDiffuse : register(t0);
SamplerState diffuseFilter : register(s0);

texture2D tNormal : register(t1);
SamplerState normalFilter : register(s1);

texture2D tSpecular : register(t2);
SamplerState specularFilter : register(s2);

texture2D tEmissive : register(t3);

struct Light {
	int type;
	float3 p1;

	float3 pos;
	float3 dir;
	float3 cdir;
	float3 color;
	float colorAlpha;

	float ratio;
	float3 p2;
};

cbuffer _Light : register(b0) {
	Light One, Two, Three;
};

struct PSI {
	float4 position : SV_POSITION;
	float4 normal : NORMAL0;
	float4 texCoord : COLOR;
	float4 wpos : WORLDPOS;
	float4 shadowPos : SHADOW;
};

MRTOutput main(PSI input) {
	MRTOutput output = GetMRTOutput();
	float4 l1, l2, l3;
	float4 diffuseColor = tDiffuse.Sample(diffuseFilter, input.texCoord.xy);

	clip(diffuseColor.a - 0.25);
	float4 emissiveColor = tEmissive.Sample(diffuseFilter, input.texCoord.xy);
    
	l1 = ApplyDirectionalLight(One.dir, input.normal.xyz, One.color, diffuseColor) * saturate(One.type & 1);
	l1 += ApplyPointLight(One.pos, input.wpos, input.normal.xyz, One.color, diffuseColor) * saturate(One.type & 2);
	l1 += ApplySpotLight(input.normal.xyz, One.pos, input.wpos, One.cdir, One.ratio, One.color, diffuseColor) * saturate(One.type & 4);

	l2 = ApplyDirectionalLight(Two.dir, input.normal.xyz, Two.color, diffuseColor) * saturate(Two.type & 1);
	l2 += ApplyPointLight(Two.pos, input.wpos, input.normal.xyz, Two.color, diffuseColor) * saturate(Two.type & 2);
	l2 += ApplySpotLight(input.normal.xyz, Two.pos, input.wpos, Two.cdir, Two.ratio, Two.color, diffuseColor) * saturate(Two.type & 4);

	l3 = ApplyDirectionalLight(Three.dir, input.normal.xyz, Three.color, diffuseColor) * saturate(Three.type & 1);
	l3 += ApplyPointLight(Three.pos, input.wpos, input.normal.xyz, Three.color, diffuseColor) * saturate(Three.type & 2);
	l3 += ApplySpotLight(input.normal.xyz, Three.pos, input.wpos, Three.cdir, Three.ratio, Three.color, diffuseColor) * saturate(Three.type & 4);

	output.diffuse = (((l1 + l2 + l3) + float4(.05, .05, .05, 0)) * diffuseColor);
	output.bloom = emissiveColor;
	return output;
}

//Empty object, light component
//Renderer