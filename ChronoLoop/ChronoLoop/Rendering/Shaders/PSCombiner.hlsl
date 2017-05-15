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
	float4 worldPos = tPosition.Sample(fFilter, input.texCoord.xy);
	float4 pixelNormal = tNormal.Sample(fFilter, input.texCoord.xy);
	float4 cSpecular = tSpecular.Sample(fFilter, input.texCoord.xy);
	float4 appliedSpecular = float4(0, 0, 0, 0);

	Light lights[3] = { One, Two, Three };
	float3 lighting = float3(0, 0, 0);
	[unroll]
	for (uint lightIndex = 0; lightIndex < 3; ++lightIndex) {
		Light l = lights[lightIndex];
		if (l.type == 1) {
			// Directional Light
			lighting += ApplyDirectionalLight(l.dir, pixelNormal.xyz, l.color, cAlbedo);
			appliedSpecular.rgb += GetSpecularIntensity(l.pos, EyePos, worldPos.xyz, pixelNormal.xyz) * l.color * cSpecular.rgb * cSpecular.a;
		} else if (l.type == 2) {
			// Point light
			lighting += ApplyPointLight(l.pos, worldPos, pixelNormal.xyz, l.color, cAlbedo);
			appliedSpecular.rgb += GetSpecularIntensity(l.pos, EyePos, worldPos.xyz, pixelNormal.xyz) * l.color * cSpecular.rgb * cSpecular.a;
		} else if (l.type == 4) {
			//Spot light
			lighting += ApplySpotLight(pixelNormal.xyz, l.pos, worldPos, l.cdir, l.ratio, l.color, cAlbedo);
			appliedSpecular.rgb += GetSpotlightSpecularIntensity(l.pos, l.ratio, l.cdir, EyePos, worldPos.xyz, pixelNormal.xyz) * l.color * cSpecular.rgb * cSpecular.a;
		}
	}
	// Add in ambient light.
	lighting.xyz += dot(abs(pixelNormal.xyz), float3(0.05, 0.05, 0.05));

	if (pixelNormal.w < 1.0f) {
		// In order to fit the data in the same 4 images, the W component
		// of the normal indicates if a pixel is affected by lighting. A
		// value of 0 means the pixel will not be lit, and to just take
		// the full diffuse color.
		lighting.xyz = dot(abs(pixelNormal.xyz), float3(1, 1, 1));
	}

	return ((float4(lighting.xyz, 1) * cAlbedo) + appliedSpecular);
}