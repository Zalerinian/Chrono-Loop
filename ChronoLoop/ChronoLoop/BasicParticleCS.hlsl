
struct ParticleData
{
	float4 position;
	float4 velocity;
};


ParticleData input : register(t0);
ParticleData output : register(u0);



[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	//[DispatchThreadID.x]
	output.position = input.position + input.velocity;
}