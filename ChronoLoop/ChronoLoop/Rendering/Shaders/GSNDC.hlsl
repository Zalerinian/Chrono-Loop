//#include "../RenderShaderDefines.hlsli"


struct GSOutput {
    float4 position : SV_POSITION;
    float4 normal : NORMAL0;
    float4 texCoord : COLOR;
    float4 wpos : WORLDPOS;
    float3x3 TBN : TBN;
    uint IID : CL_IID;
    uint viewport : SV_ViewportArrayIndex;
};

struct GSInput {
    float4 position : SV_POSITION;
    float4 normal : NORMAL0;
    float4 texCoord : COLOR;
    float4 wpos : WORLDPOS;
    float4 tangent : TANGENT;
    uint IID : CL_IID;
};

[maxvertexcount(3)]
void main(triangle GSInput input[3], inout TriangleStream<GSOutput> TriStream) {
	GSOutput output;
    float3 bitangent;
    float3x3 tbn;
	// This geometry shader will just output one copy of the input to the full-screen viewport. There currently
	// 3 viewports always bound to the pipeline.
	// 0 is the left-eye viewport.
	// 1 is the right-eye viewport.
	// 2 is a viewport that spans the entire window. This is pretty much used for the post processing quad to be able
	// to be drawn to the whole screen instead of half, like the eyes are.

	[unroll]
	for (uint i = 0; i < 3; ++i) {
        bitangent = cross(input[i].tangent.xyz, input[i].normal.xyz);
        tbn = float3x3(input[i].tangent.x, bitangent.x, input[i].normal.x, input[i].tangent.y, bitangent.y, input[i].normal.y, input[i].tangent.z, bitangent.z, input[i].normal.z);
		output.position = input[i].position;
		output.normal = input[i].normal;
		output.texCoord = input[i].texCoord;
		output.wpos = input[i].wpos;
		output.IID = input[i].IID;
        output.TBN = tbn;
		output.viewport = 2;
		TriStream.Append(output);
	}
}