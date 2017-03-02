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
	//TODO: Fix this (ask BURNSIDE)
	D3D11_BUFFER_DESC vDesc, iDesc;
	D3D11_SUBRESOURCE_DATA vData, iData;

	vDesc.Usage = D3D11_USAGE_DYNAMIC;
	vDesc.ByteWidth = sizeof(GSParticle) * mMaxParticles;
	vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vDesc.MiscFlags = 0;
	vDesc.StructureByteStride = 0;

	vData.pSysMem = mGParticles.data();
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;

	(*RenderEngine::Renderer::Instance()->iGetDevice())->CreateBuffer(&vDesc, &vData, &mVBuffer);
}

void ParticleEmitter::CreateTexureResource()
{
	RenderEngine::TextureManager::Instance()->iGetTexture2D(mTName, &std::make_shared<ID3D11ShaderResourceView*>(mTextureView), &std::make_shared<ID3D11Texture2D*>(mTexture));
}

ID3D11Buffer* ParticleEmitter::GetVertexBuffer()
{
	return mVBuffer;
}

ID3D11ShaderResourceView* ParticleEmitter::GetTexture()
{
	return mTextureView;
}

void ParticleEmitter::SetTexture(const char* _tex)
{
	mTName = _tex;
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
	D3D11_MAPPED_SUBRESOURCE mRes;
	mGParticles.clear();
	mGParticles.resize(mParticles.size());

	GSParticle gps;
	for (auto iter = mParticles.begin(); iter != mParticles.end(); iter++)
	{
		gps.pos = (*iter)->mPos;
		gps.size = (*iter)->mSize;
		mGParticles.push_back(gps);
	}

	(*RenderEngine::Renderer::Instance()->iGetContext())->Map(mVBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mRes);

	memcpy(mRes.pData, mGParticles.data(), sizeof(GSParticle) * mGParticles.size());
	//Graphics 2 slides
	(*RenderEngine::Renderer::Instance()->iGetContext())->Unmap(mVBuffer, 0);
}

void ParticleEmitter::CleanUpParticles()
{

}

void ParticleEmitter::EmitParticles()
{

}