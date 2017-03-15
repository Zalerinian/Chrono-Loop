struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float4 uv : UV;
};

struct GSParticle
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float size : SIZE;
	float3 pad : USELESS;

};

[maxvertexcount(4)]
void main(
	point GSParticle input[1],
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	float4 temp[8];
	temp[0] = float4(input[0].pos) + float4(-input[0].size, input[0].size, 0, 0);
	temp[1] = float4(input[0].pos) + float4(input[0].size, input[0].size, 0, 0);
	temp[2] = float4(input[0].pos) + float4(-input[0].size, -input[0].size, 0, 0);
	temp[3] = float4(input[0].pos) + float4(input[0].size, -input[0].size, 0, 0);
	element.col = input[0].col;

	element.pos = temp[0];
	element.uv = float4(0, 0, 0, 0);

	output.Append(element);

	element.pos = temp[1];
	element.uv = float4(1, 0, 0, 0);

	output.Append(element);

	element.pos = temp[2];
	element.uv = float4(0, 1, 0, 0);

	output.Append(element);

	element.pos = temp[3];
	element.uv = float4(1, 1, 0, 0);

	output.Append(element);
}