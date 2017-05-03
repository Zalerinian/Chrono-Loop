struct PSI {
	float4 position : SV_POSITION;
	float4 texCoord : COLOR;
};

cbuffer BlurStages : register(b1) {
	uint stage;
	uint kernelWidth;
	uint kernelHeight;
	float padding;
}

texture2D tDiffuse : register(t0);
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

float4 main(PSI input) : SV_TARGET {
	float4 color = float4(0.647f, 0.239f, 0.0509f, 1);

	// Sample stage - Just shrink/grow the texture based on the size of the quad. Nothing fancy here.
	if (stage == 0) {
		color = tDiffuse.Sample(sFilter, input.texCoord.xy);
	} else if (stage == 1) {
		// Horizontal blur stage.
		float tWidth = 0, tHeight = 0;
		tDiffuse.GetDimensions(tWidth, tHeight);

		float weights[64];
		uint count = 0;
		CalculatePascalRow(kernelWidth, count, weights);

		color = tDiffuse.Sample(sFilter, input.texCoord.xy) * 0;
		// This loop cannot be unrolled because it is based on a constant buffer value, not a compile-time constant.
		for (uint x = 1; x < count; ++x) {
			color += tDiffuse.Sample(sFilter, input.texCoord.xy + float2(float(x) / tWidth, 0)) * (1.0f / float(kernelWidth) / 2);
			color += tDiffuse.Sample(sFilter, input.texCoord.xy - float2(float(x) / tWidth, 0)) * (1.0f / float(kernelWidth) / 2);
		}
	} else if (stage == 2) {
		// Vertical blur stage.
		float tWidth = 0, tHeight = 0;
		tDiffuse.GetDimensions(tWidth, tHeight);

		float weights[64];
		uint count = 0;
		CalculatePascalRow(kernelHeight, count, weights);

		color = tDiffuse.Sample(sFilter, input.texCoord.xy) * 0;
		// This loop cannot be unrolled because it is based on a constant buffer value, not a compile-time constant.
		for (uint y = 1; y < count; ++y) {
			color += tDiffuse.Sample(sFilter, input.texCoord.xy + float2(0, float(y) / tHeight)) * (1.0f / float(kernelHeight) / 2);
			color += tDiffuse.Sample(sFilter, input.texCoord.xy - float2(0, float(y) / tHeight)) * (1.0f / float(kernelHeight) / 2);
		}
	} else {
		color = float4(1, 0, 0, 1);
	}

	return color;
}