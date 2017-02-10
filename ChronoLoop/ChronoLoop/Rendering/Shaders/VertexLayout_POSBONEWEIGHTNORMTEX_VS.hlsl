#include "VertexLayouts.hlsli"

float4 main(VERTEX_POSBONEWEIGHTNORMTEX input) : SV_POSITION
{
	return input.position;
}