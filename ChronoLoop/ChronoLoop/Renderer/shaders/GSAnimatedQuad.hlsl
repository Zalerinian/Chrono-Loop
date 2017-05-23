//#include "../RenderShaderDefines.hlsli"

struct EyeData {
	matrix view;
	matrix projection;
};

struct AnimationData {
	uint framesWide;
	uint framesTall;
	uint currentFrame;
	uint frameCount;
	// Register Break
	float4 padding[7];
};

cbuffer ViewData : register(b0) {
	EyeData eyes[2];
}

cbuffer _AnimationData : register(b2) {
	AnimationData animationData[64];
};

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

[maxvertexcount(6)]
void main(triangle GSInput input[3], inout TriangleStream<GSOutput> TriStream) {
	GSOutput output;
	AnimationData anim = animationData[input[0].IID];
	uint y = anim.currentFrame / anim.framesWide; // y: The current row of the animation sheet
	uint x = anim.currentFrame % anim.framesWide; // x: The current column of the animation sheet
	float ty = 1.0 / (float)anim.framesTall, tx = 1.0 / (float)anim.framesWide; // Texel X/Y: The width/height of a single frame
	float ox = x * tx, oy = y * ty; // Offset X/Y: The width/height of the current frame

    float3 bitangent = cross(input[0].tangent.xyz, input[0].normal.xyz);
    float3x3 tbn = float3x3(input[0].tangent.x, bitangent.x, input[0].normal.x, input[0].tangent.y, bitangent.y, input[0].normal.y, input[0].tangent.z, bitangent.z, input[0].normal.z);

	[unroll]
	for (uint i = 0; i < 3; ++i) {
		float4 inTex = input[i].texCoord;

		output.position = mul(mul(input[i].position, eyes[0].view), eyes[0].projection);
		output.normal = input[i].normal;
		output.texCoord = float4(ox + inTex.x * tx, oy + inTex.y * ty, 0, 0);
		output.wpos = input[i].wpos;
		output.IID = input[i].IID;
		output.viewport = 0;
        output.TBN = tbn;
		TriStream.Append(output);
	}

	TriStream.RestartStrip();

	[unroll]
	for (uint j = 0; j < 3; ++j) {
		float4 inTex = input[j].texCoord;


		output.position = mul(mul(input[j].position, eyes[1].view), eyes[1].projection);
		output.normal = input[j].normal;
		output.texCoord = float4(ox + inTex.x * tx, oy + inTex.y * ty, 0, 0);
		output.wpos = input[j].wpos;
		output.IID = input[j].IID;
		output.viewport = 1;
        output.TBN = tbn;
		TriStream.Append(output);
	}
	TriStream.RestartStrip();
}