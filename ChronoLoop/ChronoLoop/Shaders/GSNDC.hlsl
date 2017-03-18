struct GSOutput {
	float4 position : SV_POSITION;
	float4 normal : NORMAL0;
	float4 texCoord : COLOR;
	float4 wpos : WORLDPOS;
	uint viewport : SV_ViewportArrayIndex;
};

struct GSInput {
	float4 position : SV_POSITION;
	float4 normal : NORMAL0;
	float4 texCoord : COLOR;
	float4 wpos : WORLDPOS;
};

[maxvertexcount(3)]
void main(triangle GSInput input[3], inout TriangleStream<GSOutput> TriStream) {
	GSOutput output;

	[unroll]
	for (uint i = 0; i < 3; ++i) {
		output.position = input[i].position;
		output.normal = input[i].normal;
		output.texCoord = input[i].texCoord;
		output.wpos = input[i].wpos;
		output.viewport = 2;
		TriStream.Append(output);
	}
}