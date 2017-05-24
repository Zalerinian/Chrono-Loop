#include "Structs.hlsli"

cbuffer EyePos_ : register(b0) {
	float3 EyePos;
}

cbuffer SolidColor_ : register(b1) {
	float4 Color;
	float  isLit;
}

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
	output.diffuse = Color;
	output.normal = float4(input.normal.xyz, isLit);
	output.position = input.position;
	return output;
}