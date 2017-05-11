#include "../RenderShaderDefines.hlsli"
#include "Structs.hlsli"

texture2D tDiffuse : register(t0);
SamplerState diffuseFilter : register(s0);

struct PSI {
	float4 position : SV_POSITION;
	float4 normal : NORMAL0;
	float4 texCoord : COLOR;
	float4 wpos : WORLDPOS;
	float4 shadowPos : SHADOW;
	float4 eyePos : HEADPOS;
	uint IID : CL_IID;
	uint viewport : SV_ViewportArrayIndex;
};

MRTOutput main(PSI input) {
	MRTOutput output = GetMRTOutput();
	output.diffuse = tDiffuse.Sample(diffuseFilter, input.texCoord.xy);
	clip(output.diffuse.a - 0.25);

	output.normal = float4(normalize(input.normal.xyz), 0);
	output.position = input.wpos;
	return output;
}