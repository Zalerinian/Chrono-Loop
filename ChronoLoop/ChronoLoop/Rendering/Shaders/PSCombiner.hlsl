#include "Structs.hlsli"
#include "LightFunctions.hlsli"

texture2D tAlbedo : register(t0);
texture2D tPosition : register(t1);
texture2D tNormal : register(t2);
texture2D tSpecular : register(t3);
SamplerState fFilter : register(s0);


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

struct PSI {
	float4 position : SV_POSITION;
	float4 normal : NORMAL0;
	float4 texCoord : COLOR;
	float4 wpos : WORLDPOS;
	uint IID : CL_IID;
};

cbuffer _Eye : register(b0) {
	float3 EyePos;
}

cbuffer _Light : register(b1) {
	Light One, Two, Three;
};

float4 main(PSI input) : SV_TARGET
{
	float4 cAlbedo = tAlbedo.Sample(fFilter, input.texCoord.xy);
	float4 cSpecular = tSpecular.Sample(fFilter, input.texCoord.xy);

	Light lights[3] = { One, Two, Three };
	float3 lighting = float3(0, 0, 0);
	float specIntensity = 0;
	[unroll]
	for (uint lightIndex = 0; lightIndex < 3; ++lightIndex) {
		Light l = lights[lightIndex];
		if (l.type == 1) {
			// Directional Light
			lighting += ApplyDirectionalLight(l.dir, input.normal.xyz, l.color, cAlbedo);
			specIntensity += GetSpecularIntensity(l.pos, EyePos, input.wpos.xyz, normalize(input.normal.xyz));
		} else if (l.type == 2) {
			// Point light
			lighting += ApplyPointLight(l.pos, input.wpos, input.normal.xyz, l.color, cAlbedo);
			specIntensity += GetSpecularIntensity(l.pos, EyePos, input.wpos.xyz, normalize(input.normal.xyz));
		} else if (l.type == 4) {
			//Spot light
			lighting += ApplySpotLight(input.normal.xyz, l.pos, input.wpos, l.cdir, l.ratio, l.color, cAlbedo);
			specIntensity += GetSpotlightSpecularIntensity(l.pos, l.ratio, l.cdir, EyePos, input.wpos.xyz, normalize(input.normal.xyz));
		}
	}

	return ((float4(lighting.xyz, 1) + float4(.05, .05, .05, 0)) * cAlbedo) + float4((cSpecular).xyz * specIntensity, 0);
}