#include "ParticleComponents.h"
#include "Rendering\renderer.h"
#include <d3d11.h>



//Particle----------------------------------------------------------

Particle::Particle(int _life, float _size, vec4f _pos, vec4f _color)
{
	mLife = _life;
	mSize = _size;
	mPos = _pos;
	mColor = _color;
	mActive = true;
}

//Base Particle Emitter---------------------------------------------

ParticleEmitter::ParticleEmitter(int _numParticles, int _totalp, int _maxp, int _persec, vec4f _pos)
{
	mParticles.resize(_numParticles);
	mTotalParticles = _totalp;
	mMaxParticles = _maxp;
	mPerSec = _persec;
	mPos = _pos;
	
}

ParticleEmitter::~ParticleEmitter()
{
	mTexture->Release();
	delete mBase;
	mBase = nullptr;
	for (Particle* p : mParticles)
		delete p;
	mParticles.clear();
	mTotalParticles = mMaxParticles = 0;
}

void ParticleEmitter::CreateBuffers()
{

}

void ParticleEmitter::CreateTexureResource()
{

}

ID3D11Buffer* ParticleEmitter::GetVertexBuffer()
{
	return mVBuffer;
}
ID3D11Buffer* ParticleEmitter::GetIndexBuffer()
{
	return mIBuffer;
}
ID3D11ShaderResourceView* ParticleEmitter::GetTexture()
{
	return mTextureView;
}

void ParticleEmitter::SetTexture(ID3D11Texture2D* _tex)
{

}

void ParticleEmitter::SetREye(matrix4 _world, matrix4 _view, matrix4 _proj)
{

}

void ParticleEmitter::SetLEye(matrix4 _world, matrix4 _view, matrix4 _proj)
{

}

void ParticleEmitter::SetParticle(Particle* _p)
{

}

void ParticleEmitter::Update(float _delta)
{
	CleanUpParticles();
	EmitParticles();
	for (Particle* p : mParticles)
		UpdateParticle(p);
	UpdateBuffers();
}

void ParticleEmitter::UpdateParticle(Particle* _p)
{
	//TODO: I have no clue, particle stuff
}

void ParticleEmitter::UpdateBuffers()
{

}

void ParticleEmitter::CleanUpParticles()
{

}

void ParticleEmitter::EmitParticles()
{

}