#include "VertexLayouts.hlsli"
#include "LightFunctions.hlsli"
//#include "../RenderShaderDefines.hlsli"

texture2D tDiffuse : register(t0);
SamplerState diffuseFilter : register(s0);

struct AlphaBuffer {
	float alpha;
	float3 padding;
	float4 padding2[7];
};

cbuffer _Alpha : register(b1) {
	AlphaBuffer aBuffers[256];
}

struct PSI {
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
	float4 wpos : WORLDPOS;
	float3x3 TBN : TBN;
	uint IID : CL_IID;
};

float4 main(PSI input) : SV_TARGET
{
	float4 color = float4(0,0,0,0);
	float4 diffuseColor = tDiffuse.Sample(diffuseFilter, input.texCoord.xy);
	clip(diffuseColor.a - 0.25);

	return float4(diffuseColor.rgb, diffuseColor.a * aBuffers[input.IID].alpha);
}

//Empty object, light component
//Renderer