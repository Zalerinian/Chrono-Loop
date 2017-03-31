#include "VertexLayouts.hlsli"
#include "../RenderShaderDefines.hlsli"

cbuffer ModelBuffer : register(b0) {
	matrix model[256];
}

cbuffer IID : register(b1) {
	uint SimIID;
}

struct PSI {
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
	float4 wpos : WORLDPOS;
	float4 shadowPos : SHADOW;
	uint IID : CL_IID;
};

// Instancing interferes with the Graphic's Debugger's ability to create a pixel history, which is
// useful in determining why something looks the way it does. By disabling instancing, we can
// debug these issues much more easily.
#if ENABLE_INSTANCING
PSI main(VERTEX_POSNORMTEX input, uint id : SV_IID) {
#else
PSI main(VERTEX_POSNORMTEX input) {
#endif
	PSI output;
	//matrix light = view / determinant(view);
	//float4 lpos = light[3];

#if ENABLE_INSTANCING
	output.position = mul(input.position, model[id]);
	output.wpos = output.position;
	output.normal = mul(input.normal, model[id]);
	output.IID = id;
#else
	output.position = mul(input.position, model[0]);
	output.wpos = output.position;
	output.normal = mul(input.normal, model[0]);
	output.IID = SimIID;
#endif
	output.texCoord = input.texCoord;

	//output.shadowPos = mul(output.wpos, view);
	output.shadowPos = float4(0, 0, 0, 0);
	return output;
}