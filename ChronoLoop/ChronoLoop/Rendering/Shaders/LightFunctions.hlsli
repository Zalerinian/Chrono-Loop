
#ifndef LIGHTFUNCS_HLSLI
#define LIGHTFUNCS_HLSLI


// TODO: Change the light color to a Float 3, as lights shouldn't change the alpha of a pixel.
float4 ApplyPointLight(float3 _lpos, float4 _spos, float4 _snorm, float3 _lcol, float4 _scol)
{
    float4 result;
    float4 dir = normalize(float4(_lpos.xyz, 0) - _spos);
    float ratio = clamp(dot(dir, _snorm), 0, 1);
    result = float4((ratio * _lcol).rgb, _scol.a);

    return result;
}

float4 ApplyDirectionalLight(float3 _dir, float4 _snorm, float3 _lcol, float4 _scol)
{
    float4 result;

    float ratio = clamp(dot(-_dir, _snorm), 0, 1);
    result = float4((ratio * _lcol).rgb, _scol.a);

    return result;
}

float4 ApplySpotLight(float4 _snorm, float3 _lpos, float4 _spos, float3 _cdir, float _cratio, float3 _lcol, float4 _scol)
{
    float4 result;
    float4 dir = normalize(float4(_lpos.xyz, 0) - _spos);
    float surfaceRatio = clamp(dot(-dir, _cdir), 0, 1);
    int spotfactor = (surfaceRatio > _cratio) ? 1 : 0;
    float ratio = clamp(dot(dir, _snorm), 0, 1);
    result = float4((spotfactor * ratio * _lcol).rgb, _scol.a);

    return result;
}

#endif //LIGHTFUNCS_HLSLI