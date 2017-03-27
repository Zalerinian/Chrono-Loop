#include "../RenderShaderDefines.hlsli"

texture2D tDiffuse : register(t0);
SamplerState diffuseFilter : register(s0);

struct PSI {
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
};

float4 main(PSI input) : SV_TARGET
{
	return tDiffuse.Sample(diffuseFilter, input.texCoord.xy);
}