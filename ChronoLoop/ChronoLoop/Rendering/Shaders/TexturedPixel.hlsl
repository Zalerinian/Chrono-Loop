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

struct PSI {
	float4 position : SV_POSITION;
	float4 normal : NORMAL0;
	float4 texCoord : COLOR;
	float4 wpos : WORLDPOS;
	float4 shadowPos : SHADOW;
	float3x3 TBN : TBN;
	uint IID : CL_IID;
	uint viewport : SV_ViewportArrayIndex;
};

MRTOutput main(PSI input) {
	MRTOutput output = GetMRTOutput();
	float4 l1, l2, l3;
	float4 diffuseColor = tDiffuse.Sample(diffuseFilter, input.texCoord.xy);

	clip(diffuseColor.a - 0.25);
	float4 emissiveColor = tEmissive.Sample(diffuseFilter, input.texCoord.xy);
	float4 specularColor = tSpecular.Sample(diffuseFilter, input.texCoord.xy);

	float3 norm = tNormal.Sample(diffuseFilter, input.texCoord.xy);
	if (length(norm) < 1) {
		norm = normalize(input.normal.xyz);
	} else {
		norm = normalize(norm * 2 - 1);
		norm = normalize(mul(input.TBN, norm));
	}


	output.diffuse = diffuseColor;
	output.position = input.wpos;
	output.normal = float4(norm, 1);
	output.specular = specularColor;
	output.glow = emissiveColor;
	return output;
}

//Empty object, light component
//Renderer