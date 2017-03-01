struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 uv : UV;
};

struct GSParticle
{
	float4 pos : POSITION;
	float size : SIZE;
	float3 pad : USELESS;
	
};
cbuffer GSMatrix : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

[maxvertexcount(4)]
void main(
	point GSParticle input[1],
	inout TriangleStream< GSOutput > output
)
{
//Do 4 times
	GSOutput element;
	float4 temp[4];
	temp[0] = float4(input[0].pos.xyz, 1) + float4(input[0].size, input[0].size, 0, 0);
	temp[1] = float4(input[0].pos.xyz, 1) + float4(input[0].size, -input[0].size, 0, 0);
	temp[2] = float4(input[0].pos.xyz, 1) + float4(-input[0].size, -input[0].size, 0, 0);
	temp[3] = float4(input[0].pos.xyz, 1) + float4(-input[0].size, input[0].size, 0, 0);
	[unroll]
	for (int f = 0; f < 4; f++)
	{
		//world
		temp[f] = mul(temp[f], world);
		//view
		temp[f] = mul(temp[f], view);
		//projection
		temp[f] = mul(temp[f], proj);

		element.pos = temp[f];
		//TODO: uv stuff
		element.uv = float4(0, 0, 0, 0);

		output.Append(element);
	}

}