//#include "../RenderShaderDefines.hlsli"

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
	float3x3 TBN : TBN;
	uint IID : CL_IID;
	uint viewport : SV_ViewportArrayIndex;
};

struct GSInput
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL0;
	float4 texCoord : COLOR;
	float4 wpos : WORLDPOS;
	float4 tangent : TANGENT;
	uint IID : CL_IID;
};

[maxvertexcount(6)]
void main(triangle GSInput input[3], inout TriangleStream<GSOutput> TriStream)
{
	GSOutput output;
    float3 bitangent;
    float3x3 tbn;
	[unroll]
	for (uint i = 0; i < 3; ++i)
	{
        bitangent = cross(input[i].tangent.xyz, input[i].normal.xyz);
        tbn = float3x3(input[i].tangent.x, bitangent.x, input[i].normal.x, input[i].tangent.y, bitangent.y, input[i].normal.y, input[i].tangent.z, bitangent.z, input[i].normal.z);

		output.position = mul(mul(input[i].position, eyes[0].view), eyes[0].projection);
		output.normal = input[i].normal;
		output.texCoord = input[i].texCoord;
		output.wpos = input[i].wpos;
		output.IID = input[i].IID;
		output.viewport = 0;
		output.TBN = tbn;
		TriStream.Append(output);
	}

	TriStream.RestartStrip();

	[unroll]
	for (uint j = 0; j < 3; ++j)
	{
        bitangent = cross(input[j].tangent.xyz, input[j].normal.xyz);
        tbn = float3x3(input[j].tangent.x, bitangent.x, input[j].normal.x, input[j].tangent.y, bitangent.y, input[j].normal.y, input[j].tangent.z, bitangent.z, input[j].normal.z);
		output.position = mul(mul(input[j].position, eyes[1].view), eyes[1].projection);
		output.normal = input[j].normal;
		output.texCoord = input[j].texCoord;
		output.wpos = input[j].wpos;
		output.IID = input[j].IID;
		output.viewport = 1;
		output.TBN = tbn;
		TriStream.Append(output);
	}
	//TriStream.RestartStrip();
}