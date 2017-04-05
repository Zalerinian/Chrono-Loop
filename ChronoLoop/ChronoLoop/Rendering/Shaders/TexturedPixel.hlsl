#include "VertexLayouts.hlsli"
#include "LightFunctions.hlsli"
//#include "../RenderShaderDefines.hlsli"

texture2D tDiffuse : register(t0);
SamplerState diffuseFilter : register(s0);

texture2D tNormal : register(t1);
SamplerState normalFilter : register(s1);

texture2D tSpecular : register(t2);
SamplerState specularFilter : register(s2);

texture2D Shadow : register(t3);
texture2D Shadow2 : register(t4);
SamplerComparisonState ShadowFilter : register(s3);

struct Light
{
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

cbuffer _Light : register(b0)
{
    Light One, Two, Three;
};

struct PSI
{
    float4 position : SV_POSITION;
    float4 normal : NORMAL0;
    float4 texCoord : COLOR;
    float4 wpos : WORLDPOS;
    float4 shadowPos : SHADOW;
};

float near = .1f, far = 1000.0f;

float4 main(PSI input) : SV_TARGET
{
    float4 color = float4(0, 0, 0, 0);
    float4 l1, l2, l3;
    float4 diffuseColor = tDiffuse.Sample(diffuseFilter, float4(input.texCoord.xy, 0, 0));
	//float4 diffuseColor = float4(1, 1, 1, 1);
    clip(diffuseColor.a - 0.25);
    
    l1 = ApplyDirectionalLight(One.dir, input.normal, One.color, diffuseColor) * saturate(One.type & 1);
    l1 += ApplyPointLight(One.pos, input.wpos, input.normal, One.color, diffuseColor) * saturate(One.type & 2);
    l1 += ApplySpotLight(input.normal, One.pos, input.wpos, One.cdir, One.ratio, One.color, diffuseColor) * saturate(One.type & 4);

    l2 = ApplyDirectionalLight(Two.dir, input.normal, Two.color, diffuseColor) * saturate(Two.type & 1);
    l2 += ApplyPointLight(Two.pos, input.wpos, input.normal, Two.color, diffuseColor) * saturate(Two.type & 2);
    l2 += ApplySpotLight(input.normal, Two.pos, input.wpos, Two.cdir, Two.ratio, Two.color, diffuseColor) * saturate(Two.type & 4);

    l3 = ApplyDirectionalLight(Three.dir, input.normal, Three.color, diffuseColor) * saturate(Three.type & 1);
    l3 += ApplyPointLight(Three.pos, input.wpos, input.normal, Three.color, diffuseColor) * saturate(Three.type & 2);
    l3 += ApplySpotLight(input.normal, Three.pos, input.wpos, Three.cdir, Three.ratio, Three.color, diffuseColor) * saturate(Three.type & 4);

    color = l1 + l2 + l3;

    return (saturate(color) + float4(.05, .05, .05, 0)) * diffuseColor;

    input.shadowPos.xyz /= input.shadowPos.w;

    float l = length(input.shadowPos);
    input.shadowPos /= l;

    float dx = 1.0f / 1366;
    float dy = 1.0f / 720;
    float depth = input.shadowPos.z - .01f, percent = 0.0f;
    const float2 offsets[9] =
    {
        float2(-dx, -dy), float2(0.0f, -dy), float2(dx, -dy),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx, +dy), float2(0.0f, +dy), float2(dx, +dy)
    };
    float dep, sdep;
    if (input.shadowPos.z >= 0.0f)
    {
        float2 front;
        front.x = (input.shadowPos.x / (1.0f + input.shadowPos.z)) * .5 + .5;
        front.y = 1.0f - ((input.shadowPos.y / (1.0f + input.shadowPos.z)) * .5 + .5);

        sdep = (l - near) / (far - near);
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
        dep = percent;
    }
    else
    {
        float2 end;
        end.x = (input.shadowPos.x / (1.0f + input.shadowPos.z)) * .5 + .5;
        end.y = 1.0f - ((input.shadowPos.y / (1.0f + input.shadowPos.z)) * .5 + .5);

        sdep = (l - near) / (far - near);
        percent += Shadow2.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[0], 0, 0), depth).r;
        percent += Shadow2.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[1], 0, 0), depth).r;
        percent += Shadow2.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[2], 0, 0), depth).r;
        percent += Shadow2.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[3], 0, 0), depth).r;
        percent += Shadow2.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[4], 0, 0), depth).r;
        percent += Shadow2.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[5], 0, 0), depth).r;
        percent += Shadow2.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[6], 0, 0), depth).r;
        percent += Shadow2.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[7], 0, 0), depth).r;
        percent += Shadow2.SampleCmpLevelZero(ShadowFilter, float4(input.shadowPos.xy + offsets[8], 0, 0), depth).r;
        percent /= 9.0f;
        dep = percent;
    }

    if ((dep + 0.0f) < sdep)
    {
        return (saturate(color) + float4(.05, .05, .05, 0)) * diffuseColor;
    }
    else
    {
        
        float rat = dot(normalize(input.normal), normalize(One.pos - input.wpos.xyz));
        return (saturate(percent * color * rat) + float4(.05, .05, .05, 0)) * diffuseColor;
    }

}

//Empty object, light component
//Renderer