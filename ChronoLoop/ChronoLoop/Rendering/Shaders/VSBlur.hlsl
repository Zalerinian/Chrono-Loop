#include "VertexLayouts.hlsli"

struct PSI {
	float4 position : SV_POSITION;
	float4 texCoord : COLOR;
};

PSI main(VERTEX_POSNORMTEX input)
{
	PSI output;
	output.position = input.position;
	output.texCoord = input.texCoord;
	return output;
}