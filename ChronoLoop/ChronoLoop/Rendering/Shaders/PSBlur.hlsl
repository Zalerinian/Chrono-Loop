struct PSI {
	float4 position : SV_POSITION;
	float4 texCoord : COLOR;
};

struct PSO {
	float4 out1 : SV_TARGET0;
	float4 out2 : SV_TARGET1;
	float4 out3 : SV_TARGET2;
	float4 out4 : SV_TARGET3;
	float4 out5 : SV_TARGET4;
	float4 out6 : SV_TARGET5;
	float4 out7 : SV_TARGET6;
	float4 out8 : SV_TARGET7;
};

cbuffer BlurStages : register(b0) {
	uint stage;
	uint kernelWidth;
	uint kernelHeight;
	float padding;
}

texture2D tTex0 : register(t0);
texture2D tTex1 : register(t1);
texture2D tTex2 : register(t2);
texture2D tTex3 : register(t3);
texture2D tTex4 : register(t4);
texture2D tTex5 : register(t5);
texture2D tTex6 : register(t6);
texture2D tTex7 : register(t7);
SamplerState sFilter : register(s0);

uint STAGE_SAMPLE = 0; // downsampling and upsampling don't require any special instructions, so they fall under the same stage.
uint STAGE_HORZ = 1;
uint STAGE_VERT = 2;

void CalculatePascalRow(uint KernelSize, out uint elementCount, out float weights[64]) {
	// This can not support a kernel size greater than 30.
	elementCount = KernelSize + 1;
	float rowTotal = 1;
	float elements[64];
	elements[0] = 1;
	for (uint i = 1; i <= KernelSize; ++i) {
		float denom = i;
		elements[i] = (uint)(elements[i - 1] * ((KernelSize + 1 - i) / denom));
		rowTotal += elements[i];
	}
	for (uint j = 0; j < KernelSize; ++j) {
		weights[j] = elements[i] / rowTotal;
	}
}

PSO main(PSI input)
{
	texture2D textures[8] = { tTex0, tTex1, tTex2, tTex3, tTex4, tTex5, tTex6, tTex7 };
	// This is valid code. It interprets the PSO structure as an array of 8 float4's, and
	// allows us to index into it to assign values, just as we could do something similar in C++
	//PSO output;
	//((float4[8])output)[5] = float4(1, 1, 1, 1);

	PSO output;
	[unroll]
	for (int tex = 0; tex < 8; ++tex) {
		// Sample stage - Just shrink/grow the texture based on the size of the quad. Nothing fancy here.
		if (stage == STAGE_SAMPLE) {
			((float4[8])output)[tex] = textures[tex].Sample(sFilter, input.texCoord.xy);
		} else if (stage == STAGE_HORZ) {
			// Horizontal blur stage.
			float tWidth = 0, tHeight = 0;
			textures[tex].GetDimensions(tWidth, tHeight);

			float weights[64];
			uint count = 0;
			CalculatePascalRow(kernelWidth, count, weights);

			float4 color = textures[tex].Sample(sFilter, input.texCoord.xy) * weights[0];
			// This loop cannot be unrolled because it is based on a constant buffer value, not a compile-time constant.
			for (uint x = 1; x < count; ++x) {
				color += textures[tex].Sample(sFilter, input.texCoord.xy + float2(x / tWidth, 0));
			}
			((float4[8])output)[tex] = color;
		} else if (stage == STAGE_VERT) {
			// Vertical blur stage.
			float tWidth = 0, tHeight = 0;
			textures[tex].GetDimensions(tWidth, tHeight);

			float weights[64];
			uint count = 0;
			CalculatePascalRow(kernelHeight, count, weights);

			float4 color = textures[tex].Sample(sFilter, input.texCoord.xy) * weights[0];
			// This loop cannot be unrolled because it is based on a constant buffer value, not a compile-time constant.
			for (uint y = 1; y < count; ++y) {
				color += textures[tex].Sample(sFilter, input.texCoord.xy + float2(0, y / tHeight));
			}
			((float4[8])output)[tex] = color;
		} else {
			((float4[8])output)[tex] = float4(1, 0, 0, 1);
		}
	}


	return output;
}