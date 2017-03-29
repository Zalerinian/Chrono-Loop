#include "VertexLayouts.hlsli"
#include "LightFunctions.hlsli"
#include "../RenderShaderDefines.hlsli"

texture2D tDiffuse : register(t0);
SamplerState diffuseFilter : register(s0);

texture2D tNormal : register(t1);
SamplerState normalFilter : register(s1);

texture2D tSpecular : register(t2);
SamplerState specularFilter : register(s2);

texture2D Shadow : register(t3);
SamplerComparisonState ShadowFilter : register(s3);

struct Light
{
    int type;
    float3 p1;
    float4 pos;
    float4 dir;
    float4 cdir;
    float4 color;
    float ratio;
    float3 p2;
};

cbuffer _Light : register(b0)
{
    Light DirLight;
    Light PointLight;
    Light SpotLight;
};

struct PSI
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL0;
    float4 texCoord : COLOR;
    float4 wpos : WORLDPOS;
    float4 shadowPos : SHADOW;
};

float4 main(PSI input) : SV_TARGET
{
    float4 color = float4(0, 0, 0, 0);
    float4 l1, l2, l3;
    float4 diffuseColor = tDiffuse.Sample(diffuseFilter, float4(input.texCoord.xy,0,0));
	//float4 diffuseColor = float4(1, 1, 1, 1);
    clip(diffuseColor.a - 0.25);

    l1 = ApplyDirectionalLight(DirLight.dir, input.normal, DirLight.color, diffuseColor);
    l2 = ApplyPointLight(PointLight.pos, input.wpos, input.normal, PointLight.color, diffuseColor);
    l3 = ApplySpotLight(input.normal, SpotLight.pos, input.wpos, SpotLight.dir, SpotLight.ratio, SpotLight.color, diffuseColor);

    color = l1 + l2 + l3;

    input.shadowPos.xyz /= input.shadowPos.w;

    if (input.shadowPos.x + .01f < -1.0f || input.shadowPos.x + .01f > 1.0f ||
       input.shadowPos.y + .01f < -1.0f || input.shadowPos.y + .01f > 1.0f ||
       input.shadowPos.z + .01f < 0.0f || input.shadowPos.z + .01f > 1.0f)
        return (saturate(color) + float4(.25, .25, .25, .25)) * diffuseColor;
    float dx = 1.0f / 1366;
    float dy = 1.0f / 720;
    float depth = input.shadowPos.z - .01f, percent = 0.0f;
    const float2 offsets[9] =
    {
        float2(-dx, -dy), float2(0.0f, -dy), float2(dx, -dy),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx, +dy), float2(0.0f, +dy), float2(dx, +dy)
    };

    percent += Shadow.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[0], 0, 0), depth).r;
    percent += Shadow.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[1], 0, 0), depth).r;
    percent += Shadow.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[2], 0, 0), depth).r;
    percent += Shadow.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[3], 0, 0), depth).r;
    percent += Shadow.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[4], 0, 0), depth).r;
    percent += Shadow.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[5], 0, 0), depth).r;
    percent += Shadow.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[6], 0, 0), depth).r;
    percent += Shadow.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[7], 0, 0), depth).r;
    percent += Shadow.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[8], 0, 0), depth).r;
    percent /= 9.0f;
    

    float rat = dot(normalize(input.normal), normalize(PointLight.pos - input.wpos.xyz));
    return (saturate(percent * color * rat) + float4(.05, .05, .05, .05)) * diffuseColor;
}

//Empty object, light component
//Renderer