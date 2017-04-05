

struct PSI {
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
	float4 wpos : WORLDPOS;
	float4 shadowPos : SHADOW;
	uint IID : CL_IID;
};

struct MultiScanData {
	float DiffuseAlpha;
	float MultiscanAlpha;
	float MultiscanVOffset;
	float padding;
	float4 padding2[7];
};

cbuffer _MultiScanData : register(b1) {
	MultiScanData dataBuffer[64];
}

texture2D tDiffuse : register(t0);
texture2D tMultiscan : register(t4);
SamplerState texFilter : register(s0);


float4 main(PSI input) : SV_TARGET
{
	float4 diffuseColor = tDiffuse.Sample(texFilter, input.texCoord.xy);
	clip(diffuseColor.a);
	float multiAlpha = dataBuffer[input.IID].MultiscanAlpha;
	float multiOffset = dataBuffer[input.IID].MultiscanVOffset;
	float4 multiColor = tMultiscan.Sample(texFilter, float2(input.texCoord.x, input.texCoord.y + multiOffset));
	return float4(diffuseColor.rgb + (multiColor.rgb * multiAlpha), diffuseColor.a * dataBuffer[input.IID].DiffuseAlpha);
}