texture2D tDiffuse : register(t0);
texture2D tGlow : register(t1);
texture2D tSuperGlow : register(t2);
SamplerState diffuseFilter : register(s0);

struct PSI {
	float4 position : SV_POSITION;
	float4 texCoord : COLOR;
};

float4 main(PSI input) : SV_Target {
	float4 output = float4(0, 0, 0, 0);
	float4 diffuse = tDiffuse.Sample(diffuseFilter, input.texCoord.xy);

	float brightness = dot(diffuse.rgb, float3(0.2126, 0.7152, 0.0722));
	output += diffuse * saturate(brightness - 1); // Faster than an if statement, as brightness is a dynamic value.
	output += tGlow.Sample(diffuseFilter, input.texCoord.xy);
	output += tSuperGlow.Sample(diffuseFilter, input.texCoord.xy);
	return output;
}