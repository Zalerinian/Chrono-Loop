#include "Structs.hlsli"
#include "LightFunctions.hlsli"

struct PSI {
    float4 position : SV_POSITION;
    float4 normal : NORMAL0;
    float4 texCoord : COLOR;
    float4 wpos : WORLDPOS;
    float3x3 TBN : TBN;
    uint IID : CL_IID;
    uint viewport : SV_ViewportArrayIndex;
};

texture2D tAlbedo : register(t0);
texture2D tPosition : register(t1);
texture2D tNormal : register(t2);
texture2D tSpecular : register(t3);
SamplerState fFilter : register(s0);

struct Light
{
    float4 color;
    float4 position;
    float4 padding[6];
};

cbuffer _Eye : register(b0)
{
	float3 EyePos;
}

cbuffer LightProperties : register(b1)
{
    Light lights[256];
}

float4 main(PSI input) : SV_TARGET
{

    float screenW = 0, screenH = 0;
    tAlbedo.GetDimensions(screenW, screenH);

    float2 texCoord = float2(input.position.x / screenW, input.position.y / screenH);

    float4 cAlbedo = tAlbedo.Sample(fFilter, texCoord);
    float4 cSpecular = tSpecular.Sample(fFilter, texCoord);
    float4 pixelNormal = tNormal.Sample(fFilter, texCoord);
    float3 pixelPosition = tPosition.Sample(fFilter, texCoord);


    float3 color = ApplyPointLight(lights[input.IID].position.xyz, pixelPosition.xyz, pixelNormal.xyz, lights[input.IID].color.rgb, cAlbedo.rgba) * cAlbedo.rgb;
    float4 specularIntensity = GetSpecularIntensity(lights[input.IID].position.xyz, EyePos, pixelPosition.xyz, pixelNormal.xyz);
    cSpecular.rgb *= cSpecular.a;

	float lightRatio = pixelNormal.w < 1 ? 0 : 1;

    return float4((lightRatio * (color)).rgb + (cSpecular.rgb * specularIntensity * lights[input.IID].color.rgb), 0); // Additive blending, so don't add alpha
}