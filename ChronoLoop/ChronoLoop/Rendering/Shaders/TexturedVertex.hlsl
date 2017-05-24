#include "VertexLayouts.hlsli"
#include "../RenderShaderDefines.hlsli"

cbuffer ModelBuffer : register(b0) {
	matrix model[256];
}

cbuffer IID : register(b1) {
	uint SimIID;
}

cbuffer GlobalMatrix_ : register(b2) {
	matrix GlobalMatrix;
}

struct PSI {
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
	float4 wpos : WORLDPOS;
	float4 tangent : TANGENT;
	uint IID : CL_IID;
};

// Instancing interferes with the Graphic's Debugger's ability to create a pixel history, which is
// useful in determining why something looks the way it does. By disabling instancing, we can
// debug these issues much more easily.
#if ENABLE_INSTANCING
PSI main(VERTEX_POSNORMTANTEX input, uint id : SV_InstanceID) {
#else
PSI main(VERTEX_POSNORMTANTEX input) {
#endif
	PSI output;
	//matrix light = view / determinant(view);
	//float4 lpos = light[3];

#if ENABLE_INSTANCING
	output.position = mul(mul(input.position, GlobalMatrix), model[id]);
	output.wpos = output.position;
	output.normal = mul(mul(input.normal, GlobalMatrix), model[id]);
	output.IID = id;
#else
	output.position = mul(mul(input.position, GlobalMatrix), model[0]);
	output.wpos = output.position;
	output.normal = mul(mul(input.normal, GlobalMatrix), model[0]);
	output.IID = SimIID;
#endif
	output.tangent = input.tangent;
	output.texCoord = input.texCoord;
	return output;
}