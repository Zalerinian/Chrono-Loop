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
PSI main(VERTEX_POSNORMTEX input)
{
#endif
    PSI output;
    matrix light = view / determinant(view);
    float4 lpos = light[3];

#if ENABLE_INSTANCING
	output.position = mul(input.position, model[id]);
	output.wpos = output.position;
	output.normal = mul(input.normal, model[id]);
#else
    output.position = mul(input.position, model[0]);
    output.wpos = output.position;
    output.normal = mul(input.normal, model[0]);
#endif
    output.texCoord = input.texCoord;

    output.shadowPos = mul(output.wpos, view);
    return output;
}