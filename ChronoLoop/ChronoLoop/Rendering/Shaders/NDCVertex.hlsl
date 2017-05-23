#include "VertexLayouts.hlsli"
#include "../RenderShaderDefines.hlsli"

cbuffer InstanceID : register(b1)
{
    uint IID;
    uint3 padding;
}

struct PSI {
    float4 position : SV_POSITION;
    float4 normal : NORMAL0;
    float4 texCoord : COLOR;
    float4 wpos : WORLDPOS;
    float4 tangent : TANGENT;
    uint IID : CL_IID;
};


PSI main(VERTEX_POSNORMTANTEX input) {
	PSI output;
	output.position = input.position;
	output.normal = input.normal;
	output.texCoord = input.texCoord;
	output.wpos = input.position;
    output.tangent = input.tangent;
	output.IID = IID;
	return output;
}