#include "../RenderShaderDefines.hlsli"
#include "Structs.hlsli"

texture2D tDiffuse : register(t0);
texture2D tNormal : register(t1);
SamplerState diffuseFilter : register(s0);

struct PSI {
	float4 position : SV_POSITION;
	float4 normal : NORMAL0;
	float4 texCoord : COLOR;
	float4 wpos : WORLDPOS;
	float3x3 TBN : TBN;
	uint IID : CL_IID;
	uint viewport : SV_ViewportArrayIndex;
};

MRTOutput main(PSI input) {
	MRTOutput output = GetMRTOutput();
	output.diffuse = tDiffuse.Sample(diffuseFilter, input.texCoord.xy);
	clip(output.diffuse.a - 0.25);

	float3 norm = tNormal.Sample(diffuseFilter, input.texCoord.xy);
	if (length(norm) < 1) {
		norm = normalize(input.normal.xyz);
	} else {
		norm = normalize(norm * 2 - 1);
		norm = normalize(mul(input.TBN, norm));
	}

	output.normal = float4(norm, 0);
	output.position = input.wpos;
	return output;
}