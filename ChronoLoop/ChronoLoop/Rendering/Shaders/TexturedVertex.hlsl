#include "VertexLayouts.hlsli"
#include "../RenderShaderDefines.hlsli"

cbuffer ModelBuffer : register(b0) {
	matrix model[256];
}

cbuffer VPLight : register(b1)
{
    matrix view;
    matrix proj;
}

struct PSI
{
	float4 position :	SV_POSITION;
	float4 normal	:	NORMAL0;
	float4 texCoord :	COLOR;
	float4 wpos : WORLDPOS;
    float4 shadowPos : SHADOW;
};

// Instancing interferes with the Graphic's Debugger's ability to create a pixel history, which is
// useful in determining why something looks the way it does. By disabling instancing, we can
// debug these issues much more easily.
#if ENABLE_INSTANCING
PSI main(VERTEX_POSNORMTEX input, uint id : SV_InstanceID) {
#else
PSI main(VERTEX_POSNORMTEX input) {
#endif
	PSI output;
	output.wpos = input.position;
#if ENABLE_INSTANCING
	output.position = mul(input.position, model[id]);
	output.normal = mul(input.normal, model[id]);
#else
	output.position = mul(input.position, model[0]);
	output.normal = mul(input.normal, model[0]);
#endif
	output.texCoord = input.texCoord;

    output.shadowPos = mul(output.wpos, model[id]);
    output.shadowPos = mul(output.shadowPos, view);
    output.shadowPos = mul(output.shadowPos, proj);
	return output;
}