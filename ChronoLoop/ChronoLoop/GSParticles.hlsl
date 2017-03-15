struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float4 uv : UV;
	float4 wpos : WPOS;
	float4 normal : NORMAL;
	uint viewport : SV_ViewportArrayIndex;
};

struct GSParticle
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float size : SIZE;
	float3 pad : USELESS;

};

struct EyeData {
	matrix view;
	matrix proj;
};

cbuffer _EyeData : register(b0) {
	EyeData eyes[2];
}

[maxvertexcount(8)]
void main(
	point GSParticle input[1],
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	float4 temp[4];
	temp[0] = float4(input[0].pos) + float4(-input[0].size, input[0].size, 0, 0);
	temp[1] = float4(input[0].pos) + float4(input[0].size, input[0].size, 0, 0);
	temp[2] = float4(input[0].pos) + float4(-input[0].size, -input[0].size, 0, 0);
	temp[3] = float4(input[0].pos) + float4(input[0].size, -input[0].size, 0, 0);
	element.col = input[0].col;
	element.viewport = 0;
	
	matrix vpos = eyes[0].view / determinant(eyes[0].view);
	element.normal = vpos[3];

	element.wpos = temp[0];
	element.pos = mul(mul(temp[0], eyes[0].view), eyes[0].proj);
	element.uv = float4(0, 0, 0, 0);

	output.Append(element);

	element.wpos = temp[1];
	element.pos = mul(mul(temp[1], eyes[0].view), eyes[0].proj);
	element.uv = float4(1, 0, 0, 0);

	output.Append(element);

	element.wpos = temp[2];
	element.pos = mul(mul(temp[2], eyes[0].view), eyes[0].proj);
	element.uv = float4(0, 1, 0, 0);

	output.Append(element);

	element.wpos = temp[3];
	element.pos = mul(mul(temp[3], eyes[0].view), eyes[0].proj);
	element.uv = float4(1, 1, 0, 0);

	output.Append(element);

	output.RestartStrip();
	
	element.viewport = 1;

	vpos = eyes[1].view / determinant(eyes[1].view);
	element.normal = vpos[3];

	element.wpos = temp[0];
	element.pos = mul(mul(temp[0], eyes[1].view), eyes[1].proj);
	element.uv = float4(0, 0, 0, 0);

	output.Append(element);

	element.wpos = temp[1];
	element.pos = mul(mul(temp[1], eyes[1].view), eyes[1].proj);
	element.uv = float4(1, 0, 0, 0);

	output.Append(element);

	element.wpos = temp[2];
	element.pos = mul(mul(temp[2], eyes[1].view), eyes[1].proj);
	element.uv = float4(0, 1, 0, 0);

	output.Append(element);

	element.wpos = temp[3];
	element.pos = mul(mul(temp[3], eyes[1].view), eyes[1].proj);
	element.uv = float4(1, 1, 0, 0);

	output.Append(element);
	
	output.RestartStrip();

}