struct PSI {
	float4 position : SV_POSITION;
	float4 texCoord : COLOR;
};

cbuffer BlurStages : register(b1) {
	uint stage;
	float sigma;
	float2 direction;
}

texture2D tDiffuse : register(t0);
SamplerState sFilter : register(s0);

uint STAGE_SAMPLE = 0; // downsampling and upsampling don't require any special instructions, so they fall under the same stage.
uint STAGE_BLUR = 1;

float4 main(PSI input) : SV_TARGET {
	float4 color = float4(0, 0, 0, 0);
	float tWidth = 0, tHeight = 0;
	if (stage > 0) {
		tDiffuse.GetDimensions(tWidth, tHeight);
	}

	// Sample stage - Just shrink/grow the texture based on the size of the quad. Nothing fancy here.
	if (stage == 0) {
		color = tDiffuse.Sample(sFilter, input.texCoord.xy);
	} else if (stage == 1) {
		// Horizontal blur stage.
		int pixelCount = int(sigma * 3.0);
		float norm = 1 / (sqrt(2 * 3.14159265359) * sigma);

		float4 thisPixel = tDiffuse.Sample(sFilter, input.texCoord.xy);
		color = thisPixel;
		// This loop cannot be unrolled because it is based on a constant buffer value, not a compile-time constant.
		for (int i = 1; i < pixelCount; ++i) {
			float texel = float(i) / (dot(float2(tWidth, tHeight), direction));
			float2 uva = input.texCoord.xy + texel * direction,
				uvs = input.texCoord.xy - texel * direction;
			float weight = exp(-0.5 * float(i) * float(i) / (sigma * sigma));

			float4 clra = tDiffuse.Sample(sFilter, saturate(uva)) * weight;
			float4 clrs = tDiffuse.Sample(sFilter, saturate(uvs)) * weight;
			color += (dot(uva, direction)) <= 1.0f ? clra : (thisPixel * weight);
			color += (dot(uvs, direction)) >= 0.0f ? clrs : (thisPixel * weight);
		}
		color *= norm; // Normalize the color so it doesn't get brighter/darker.
	} else {
		color = float4(1, 0, 0, 1);
	}

	return color;
}