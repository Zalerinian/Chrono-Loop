#include "VertexLayouts.hlsli"

float4 main(VERTEX_POSBONEWEIGHT input) : SV_POSITION
{
	return input.position;
}