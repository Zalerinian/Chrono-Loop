#include "VertexLayouts.hlsli"

float4 main(VERTEX_POSNORMTANTEX input) : SV_POSITION
{
	return input.position;
}