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
	float4 tpos, tpos2;
	tpos = tpos2 = input[0].pos;
	float4 temp[4];
	temp[0] = float4(-input[0].size, input[0].size, 0, 0);
	temp[1] = float4(input[0].size, input[0].size, 0, 0);
	temp[2] = float4(-input[0].size, -input[0].size, 0, 0);
	temp[3] = float4(input[0].size, -input[0].size, 0, 0);

	element.col = input[0].col;
	element.viewport = 0;
	tpos = mul(mul(tpos, eyes[0].view), eyes[0].proj);
	matrix vpos = eyes[0].view / determinant(eyes[0].view);
	vpos[3].w = 0; // Mark particles as not being affected by light
	element.normal = vpos[3];


	element.wpos = input[0].pos + temp[0];
	element.pos = tpos + temp[0];
	element.uv = float4(0, 0, 0, 0);

	output.Append(element);

	element.wpos = input[0].pos + temp[1];
	element.pos = tpos + temp[1];
	element.uv = float4(1, 0, 0, 0);

	output.Append(element);

	element.wpos = input[0].pos + temp[2];
	element.pos = tpos + temp[2];
	element.uv = float4(0, 1, 0, 0);

	output.Append(element);

	element.wpos = input[0].pos + temp[3];
	element.pos = tpos + temp[3];
	element.uv = float4(1, 1, 0, 0);

	output.Append(element);

	
	element.viewport = 1;

	tpos2 = mul(mul(tpos2, eyes[1].view), eyes[1].proj);
	vpos = eyes[1].view / determinant(eyes[1].view);
	element.normal = vpos[3];


	element.wpos = input[0].pos + temp[0];
	element.pos = tpos2 + temp[0];
	element.uv = float4(0, 0, 0, 0);

	output.Append(element);

	element.wpos = input[0].pos + temp[1];
	element.pos = tpos2 + temp[1];
	element.uv = float4(1, 0, 0, 0);

	output.Append(element);

	element.wpos = input[0].pos + temp[2];
	element.pos = tpos2 + temp[2];
	element.uv = float4(0, 1, 0, 0);

	output.Append(element);

	element.wpos = input[0].pos + temp[3];
	element.pos = tpos2 + temp[3];
	element.uv = float4(1, 1, 0, 0);

	output.Append(element);
	

}