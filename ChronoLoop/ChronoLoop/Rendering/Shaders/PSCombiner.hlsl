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
    float3x3 TBN : TBN;
    uint IID : CL_IID;
    uint viewport : SV_ViewportArrayIndex;
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
	float4 pixelNormal = tNormal.Sample(fFilter, input.texCoord.xy);

    float3 lightAmount = pixelNormal.w < 1 ? float3(1, 1, 1) : float3(0.05, 0.05, 0.05);

	return float4((cAlbedo * dot(abs(pixelNormal.xyz), lightAmount)).xyz, 1);
}