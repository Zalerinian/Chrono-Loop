
struct MRTOutput {
	float4 diffuse : SV_TARGET0;
	float4 position : SV_TARGET1;
	float4 normal : SV_TARGET2;
	float4 specular : SV_TARGET3;
	float4 glow : SV_TARGET4;
	float4 superGlow : SV_TARGET5;
};

MRTOutput GetMRTOutput() {
	MRTOutput o;
	o.diffuse = float4(0, 0, 0, 0);
	o.position = float4(0, 0, 0, 0);
	o.normal = float4(0, 0, 0, 0);
	o.specular = float4(0, 0, 0, 0);
	o.glow = float4(0, 0, 0, 0);
	o.superGlow = float4(0, 0, 0, 0);
	return o;
}