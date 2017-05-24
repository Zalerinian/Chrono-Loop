
#ifndef LIGHTFUNCS_HLSLI
#define LIGHTFUNCS_HLSLI


// TODO: Change the light color to a Float 3, as lights shouldn't change the alpha of a pixel.
float3 ApplyPointLight(float3 _lpos, float3 _spos, float3 _snorm, float3 _lcol, float4 _scol) {
	float3 lvec = _lpos - _spos;
	float att = 1.0f - saturate((length(lvec.xyz) / 1.0f));
	float3 dir = normalize(lvec);
	float ratio = saturate(dot(dir, _snorm));
	return (ratio * _lcol).rgb;
}

float3 ApplyDirectionalLight(float3 _dir, float3 _snorm, float3 _lcol, float4 _scol) {
	float ratio = saturate(dot(-_dir, _snorm));
	return (ratio * _lcol).rgb;
}

float3 ApplySpotLight(float3 _snorm, float3 _lpos, float4 _spos, float3 _cdir, float _cratio, float3 _lcol, float4 _scol) {
	float3 dir = normalize(_lpos.xyz - _spos.xyz);
	float surfaceRatio = saturate(dot(-dir, _cdir));
	int spotfactor = (surfaceRatio > _cratio) ? 1 : 0;
	float ratio = saturate(dot(dir, _snorm));
	return (spotfactor * ratio * _lcol).rgb;
}

float GetSpecularIntensity(float3 _lightPos, float3 _eyePos, float3 _surfacePos, float3 _surfaceNormal) { 
	float3 IncidentVector = normalize(_surfacePos - _lightPos);
	float3 reflection = normalize(reflect(IncidentVector, _surfaceNormal));
	float3 surfaceToEye = normalize(_eyePos - _surfacePos);
	return pow(saturate(dot(reflection, surfaceToEye)), 96);
}

float GetSpotlightSpecularIntensity(float3 _lightPos, float _coneRatio, float3 _coneDir, float3 _eyePos, float3 _surfacePos, float3 _surfaceNormal) {
	float3 IncidentVector = normalize(_surfacePos - _lightPos);
	float surfaceRatio = saturate(dot(IncidentVector, _coneDir));
	float spotFactor = (surfaceRatio > _coneRatio) ? 1 : 0;
	float3 reflection = normalize(reflect(IncidentVector, _surfaceNormal));
	float3 surfaceToEye = normalize(_eyePos - _surfacePos);
	return pow(saturate(dot(reflection, surfaceToEye)), 96 * 5) * spotFactor;
}

#endif //LIGHTFUNCS_HLSLI