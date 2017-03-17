texture2D tDiffuse : register(t0);
SamplerState diffuseFilter : register(s0);

struct PSI {
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
};

float4 main(PSI input) : SV_TARGET
{
	float4 diffuseColor = tDiffuse.Sample(diffuseFilter, input.texCoord.xy);
	float K = 0;
	const float3 dsat = { 0.30, 0.59, 0.11 };

	diffuseColor.rgb = lerp(diffuseColor.rgb, dot(diffuseColor.rgb, dsat.rgb), K);
	diffuseColor.a = 1;

	return diffuseColor;
}