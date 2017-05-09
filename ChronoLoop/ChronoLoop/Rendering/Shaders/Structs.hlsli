
struct MRTOutput {
	float4 diffuse : SV_TARGET0;
	float4 bloom : SV_TARGET1;
};

MRTOutput GetMRTOutput() {
	MRTOutput o;
	o.bloom = float4(0, 0, 0, 1);
	o.diffuse = float4(0, 0, 0, 0);
	return o;
}