texture2D tDiffuse : register(t0);
SamplerState diffuseFilter : register(s0);

cbuffer Ratios : register(b1) {
	float4 color;
	float K;
}

struct PSI {
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
};

float4 main(PSI input) : SV_TARGET
{
	float4 diffuseColor = tDiffuse.Sample(diffuseFilter, input.texCoord.xy);

	diffuseColor.rgb = lerp(diffuseColor.rgb, dot(diffuseColor.rgb, color.rgb), K);
	diffuseColor.a = 1;

	return diffuseColor;
}