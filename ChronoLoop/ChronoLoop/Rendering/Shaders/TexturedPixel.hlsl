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
	float4 eyePos : HEADPOS;
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
    
	Light lights[3] = { One, Two, Three };
	float3 lighting = float3(0, 0, 0);
	float specIntensity = 0;
	[unroll]
	for (uint lightIndex = 0; lightIndex < 3; ++lightIndex) {
		Light l = lights[lightIndex];
		if (l.type == 1) {
			// Directional Light
			lighting += ApplyDirectionalLight(l.dir, input.normal.xyz, l.color, diffuseColor);
			specIntensity += GetSpecularIntensity(l.pos, input.eyePos.xyz, input.wpos.xyz, normalize(input.normal.xyz));
		} else if (l.type == 2) {
			// Point light
			lighting += ApplyPointLight(l.pos, input.wpos, input.normal.xyz, l.color, diffuseColor);
			specIntensity += GetSpecularIntensity(l.pos, input.eyePos.xyz, input.wpos.xyz, normalize(input.normal.xyz));
		} else if (l.type == 4) {
			//Spot light
			lighting += ApplySpotLight(input.normal.xyz, l.pos, input.wpos, l.cdir, l.ratio, l.color, diffuseColor);
			specIntensity += GetSpotlightSpecularIntensity(l.pos, l.ratio, l.cdir, input.eyePos.xyz, input.wpos.xyz, normalize(input.normal.xyz));
		}
	}


	output.diffuse = ((float4(lighting.xyz, 1) + float4(.05, .05, .05, 0)) * diffuseColor) + float4((specularColor).xyz * specIntensity, 0);
	output.bloom = emissiveColor;
	return output;
}

//Empty object, light component
//Renderer