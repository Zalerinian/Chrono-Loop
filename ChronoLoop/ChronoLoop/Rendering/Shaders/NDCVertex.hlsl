#include "VertexLayouts.hlsli"
#include "../RenderShaderDefines.hlsli"

struct PSI {
	float4 position : SV_POSITION;
	float4 normal : NORMAL0;
	float4 texCoord : COLOR;
	float4 wpos : WORLDPOS;
	float  instanceID : CL_InstanceID;
};


PSI main(VERTEX_POSNORMTEX input) {
	PSI output;
	output.position = input.position;
	output.normal = input.normal;
	output.texCoord = input.texCoord;
	output.wpos = input.position;
	output.instanceID = 0;
	return output;
}