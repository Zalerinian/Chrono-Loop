
#ifndef LIGHTFUNCS_HLSLI
#define LIGHTFUNCS_HLSLI


// TODO: Change the light color to a Float 3, as lights shouldn't change the alpha of a pixel.
float4 ApplyPointLight(float3 _lpos, float4 _spos, float3 _snorm, float3 _lcol, float4 _scol)
{
    //TODO: Dissipation
    float4 result;
    float3 lvec = _lpos.xyz - _spos.xyz;
    float att = 1.0f - saturate((length(lvec.xyz) / 1.0f));
    float3 dir = normalize(lvec);
    float ratio = saturate(dot(dir, _snorm));
    result = float4((ratio * _lcol).rgb, _scol.a);

    return result;
}

float4 ApplyDirectionalLight(float3 _dir, float3 _snorm, float3 _lcol, float4 _scol)
{
    float4 result;

    float ratio = saturate(dot(-_dir, _snorm));
    result = float4((ratio * _lcol).rgb, _scol.a);

    return result;
}

float4 ApplySpotLight(float3 _snorm, float3 _lpos, float4 _spos, float3 _cdir, float _cratio, float3 _lcol, float4 _scol)
{
    float4 result;
    float3 dir = normalize(_lpos.xyz - _spos.xyz);
    float surfaceRatio = saturate(dot(-dir, _cdir));
    int spotfactor = (surfaceRatio > _cratio) ? 1 : 0;
    float ratio = saturate(dot(dir, _snorm));
    result = float4((spotfactor * ratio * _lcol).rgb, _scol.a);

    return result;
}

#endif //LIGHTFUNCS_HLSLI