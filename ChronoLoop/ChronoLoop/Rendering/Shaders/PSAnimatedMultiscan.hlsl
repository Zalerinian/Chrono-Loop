struct PSI {
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
	float4 wpos : WORLDPOS;
	float3x3 TBN : TBN;
	uint IID : CL_IID;
};

struct MultiScanData {
	float DiffuseAlpha;
	float MultiscanAlpha;
	float MultiscanVOffset;
	float ScanlineAlpha;
	// Register Break
	float ScanlineVOffset;
	float3 padding;
	// Register Break
	float4 padding2[6];
};

cbuffer _MultiScanData : register(b1) {
	MultiScanData dataBuffer[64];
}

texture2D tDiffuse : register(t0);
texture2D tMultiscan : register(t4);
texture2D tScanline : register(t5);
SamplerState texFilter : register(s0);


float4 main(PSI input) : SV_TARGET
{
	float4 diffuseColor = tDiffuse.Sample(texFilter, input.texCoord.xy);
	clip(diffuseColor.a);
	
	// These are assigned because it makes debugging easy ( '-')b
	float multiAlpha = dataBuffer[input.IID].MultiscanAlpha;
	float multiOffset = dataBuffer[input.IID].MultiscanVOffset;
	float scanAlpha = dataBuffer[input.IID].ScanlineAlpha;
	float scanOffset = fmod(dataBuffer[input.IID].ScanlineVOffset, 2.5);

	float4 multiColor = tMultiscan.Sample(texFilter, float2(input.texCoord.x, input.texCoord.y - multiOffset)); // Minus offset, because we want the texture to move down, and negative is down (think regular coordinate system)
	float4 ScanColor = tScanline.Sample(texFilter, float2(input.texCoord.x, saturate((input.texCoord.y - scanOffset) * 3)));
	//ScanColor *= scanOffset > 0.8 ? 1 : 0;

	return float4(diffuseColor.rgb + (multiColor.rgb * multiAlpha) + (ScanColor.rbg * scanAlpha), diffuseColor.a * dataBuffer[input.IID].DiffuseAlpha);
}