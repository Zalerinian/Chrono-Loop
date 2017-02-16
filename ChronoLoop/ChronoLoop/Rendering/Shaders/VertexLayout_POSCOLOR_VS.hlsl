#include "VertexLayouts.hlsli"

float4 main(VERTEX_POSCOLOR input) : SV_POSITION
{
	return input.position;
}