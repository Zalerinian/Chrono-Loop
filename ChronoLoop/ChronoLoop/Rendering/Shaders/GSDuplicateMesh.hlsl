		struct EyeData
{
	matrix view;
	matrix projection;
};

cbuffer ViewData : register(b0)
{
	EyeData eyes[2];
}

struct GSOutput
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL0;
	float4 texCoord : COLOR;
	float4 wpos : WORLDPOS;
	uint viewport : SV_ViewportArrayIndex;
};

struct GSInput
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL0;
	float4 texCoord : COLOR;
	float4 wpos : WORLDPOS;
};

[maxvertexcount(6)]
void main(triangle GSInput input[3], inout TriangleStream<GSOutput> TriStream)
{
	GSOutput output;
	// TODO: Does this do instancing....???
	// Triangle 1
	[unroll]
	for (uint i = 0; i < 3; ++i)
	{
		output.position = mul(mul(input[i].position, eyes[0].view), eyes[0].projection);
		output.normal = input[i].normal;
		output.texCoord = input[i].texCoord;
		output.wpos = input[i].wpos;
		output.viewport = 0;
		TriStream.Append(output);
	}

	TriStream.RestartStrip();

	[unroll]
	for (uint j = 0; j < 3; ++j)
	{
		output.position = mul(mul(input[j].position, eyes[1].view), eyes[1].projection);
		output.normal = input[j].normal;
		output.texCoord = input[j].texCoord;
		output.wpos = input[j].wpos;
		output.viewport = 1;
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}