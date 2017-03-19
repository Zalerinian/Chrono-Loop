texture2D tDiffuse : register(t0);
SamplerState diffuseFilter : register(s0);

cbuffer Ratios : register(b1) {
	float4 desaturationColor;
	float4 tintColor;
	float2 ratios;
	float2 __padding;
	float2 fullRatios;
}

struct PSI {
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
};

float4 main(PSI input) : SV_TARGET
{
	float4 diffuseColor = tDiffuse.Sample(diffuseFilter, input.texCoord.xy);

	diffuseColor.rgb = lerp(diffuseColor.rgb, dot(diffuseColor.rgb, desaturationColor.rgb), ratios.x);

	float tintRatio = (ratios.y / fullRatios.y);
	float4 fullColor = (float4(1, 1, 1, 1) * (1 - tintRatio));

	diffuseColor.rgb *= fullColor + (tintColor.rgb * tintRatio);
	diffuseColor.a = 1;


	return diffuseColor;
}