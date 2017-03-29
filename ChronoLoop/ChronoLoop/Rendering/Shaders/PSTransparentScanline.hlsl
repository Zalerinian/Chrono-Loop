#include "VertexLayouts.hlsli"
#include "LightFunctions.hlsli"
//#include "../RenderShaderDefines.hlsli"

texture2D tDiffuse : register(t0);
SamplerState diffuseFilter : register(s0);

texture2D tNormal : register(t1);
SamplerState normalFilter : register(s1);

texture2D tSpecular : register(t2);
SamplerState specularFilter : register(s2);

texture2D tMultiscan : register(t3);
SamplerState fMultiscan : register(s3);

texture2D tScanline : register(t4);
SamplerState fScanline : register(s4);

cbuffer Ratios : register(b1) {
	float  alpha;
	float  MultiscanOffset;
	float2 ScanlineRatio;
}

struct PSI {
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
	float4 wpos : WORLDPOS;
};

float4 main(PSI input) : SV_TARGET
{
	float4 diffuseColor = tDiffuse.Sample(diffuseFilter, input.texCoord.xy);
	float4 multiscanColor = tMultiscan.Sample(fMultiscan, input.texCoord.xy);

	float scanlineAlpha = saturate((1 - (input.texCoord.y + ScanlineRatio.x)) / ScanlineRatio.y);

	float4 scanlineColor = tScanline.Sample(fScanline, input.texCoord.xy) * scanlineAlpha;
	scanlineColor *= scanlineColor.a;

	float4 combinedColor = float4(diffuseColor.rgb + multiscanColor.rgb + scanlineColor.rgb, diffuseColor.a * alpha);

	return combinedColor;
}

//Empty object, light component
//Renderer