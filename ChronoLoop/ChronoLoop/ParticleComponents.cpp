#include "ParticleComponents.h"
#include "Rendering\renderer.h"
#include <d3d11.h>

namespace Epoch
{

	//Particle----------------------------------------------------------

	Particle::Particle()
	{
		mLife = 1000;
		mSize = 1.0f;
		mPos = vec4f(0, 0, 0, 0);
		mVelocity = vec4f(0, 0, 0, 0);
		mSColor = mEColor = vec4f(1, 1, 1, 1);
		mActive = true;
	}
	Particle::Particle(int _life, float _size, vec4f _pos, vec4f _color)
	{
		mLife = _life;
		mSize = _size;
		mPos = _pos;
		mSColor = mEColor = _color;
		mVelocity = vec4f(0, 0, 0, 0);
		mActive = true;
	}
	Particle::Particle(int _life, float _size, vec4f _pos, vec4f _scolor, vec4f _ecolor)
	{
		mLife = _life;
		mSize = _size;
		mPos = _pos;
		mSColor = _scolor;
		mEColor = _ecolor;
		mVelocity = vec4f(0, 0, 0, 0);
		mActive = true;
	}

	Particle& Particle::operator=(const Particle& _other)
	{
		mLife = _other.mLife;
		mSize = _other.mSize;
		mPos = _other.mPos;
		mSColor = _other.mSColor;
		mEColor = _other.mEColor;
		mActive = _other.mActive;

		return *this;
	}

	//Base Particle Emitter---------------------------------------------

	ParticleEmitter::ParticleEmitter(int _numParticles, int _totalp, int _maxp, int _persec, vec4f _pos)
	{
		mParticles.resize(_numParticles);
		mTotalParticles = _totalp;
		mMaxParticles = _maxp;
		mPerSec = _persec;
		mPos = _pos;
		CreateBuffers();
	}

	ParticleEmitter::~ParticleEmitter()
	{
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
		mGParticles.resize(mMaxParticles);
		mGParticles.reserve(mMaxParticles);
		for (int i = 0; i < mGParticles.size(); i++)
		{
			GSParticle vp;
			vp.pos = vec4f(7, 1, 5, 7);
			vp.size = 0;
			mGParticles[i] = vp;
		}

		D3D11_BUFFER_DESC vDesc;
		D3D11_SUBRESOURCE_DATA vData;

		vDesc.Usage = D3D11_USAGE_DYNAMIC;
		vDesc.ByteWidth = sizeof(GSParticle) * mMaxParticles;
		vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vDesc.MiscFlags = 0;
		vDesc.StructureByteStride = 0;

		vData.pSysMem = &mGParticles[0];
		vData.SysMemPitch = 0;
		vData.SysMemSlicePitch = 0;

		Renderer::Instance()->iGetDevice()->CreateBuffer(&vDesc, &vData, &mVBuffer);
	}

	void ParticleEmitter::CreateTextureResource()
	{
		//TODO: UNCOMMENT THIS -Patrick
		//TextureManager::Instance()->iGetTexture2D(mTName, &tv, &text);
	}

	ID3D11Buffer* ParticleEmitter::GetVertexBuffer()
	{
		return mVBuffer;
	}

	ID3D11ShaderResourceView* ParticleEmitter::GetTexture()
	{
		return tv.Get();
	}

	int ParticleEmitter::GetVertexCount()
	{
		return mGParticles.size();
	}
	void ParticleEmitter::SetTexture(const char* _tex)
	{
		mTName = _tex;
		CreateTextureResource();
	}

	void ParticleEmitter::SetParticle(Particle* _p)
	{
		mBase = _p;
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
		if (_p->mLife <= 0)
			_p->mActive = false;
		if (_p->mActive)
		{
			_p->mPos = _p->mPos + _p->mVelocity;
			_p->mLife--;
		}
	}

	void ParticleEmitter::UpdateBuffers()
	{
		D3D11_MAPPED_SUBRESOURCE mRes;
		//mGParticles.clear();
		//mGParticles.resize(mMaxParticles);
		//ZeroMemory(mGParticles.data(), sizeof(GSParticle) * mGParticles.size());

		GSParticle gps;
		int i = 0;
		for (auto iter = mParticles.begin(); iter != mParticles.end(); ++iter)
		{
			if ((*iter)->mActive)
			{
				gps.pos = (*iter)->mPos;
				gps.size = (*iter)->mSize;
				mGParticles[i] = gps;
			}
			else
			{
				gps.pos = vec4f(0, 0, 0, 0);
				gps.size = 0;
				mGParticles[i] = gps;
			}
			i++;
		}
		for (; i < mGParticles.size(); i++)
		{
			gps.pos = vec4f(0, 0, 0, 0);
			gps.size = 0;
			mGParticles[i] = gps;
		}

		Renderer::Instance()->iGetContext()->Map(mVBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mRes);
		
		memcpy(mRes.pData, mGParticles.data(), sizeof(GSParticle) * mGParticles.size());
		//Graphics 2 slides
		Renderer::Instance()->iGetContext()->Unmap(mVBuffer, 0);
	}

	void ParticleEmitter::CleanUpParticles()
	{
		auto i = mGParticles.begin();
		for (auto iter = mParticles.begin(); iter != mParticles.end() && i != mGParticles.end(); ++iter, ++i)
		{
			if ((*iter)->mActive == false)
			{
				(*i).pos = vec4f(0, 0, 0, 0);
				(*i).size = 0;
				Particle* temp = (*iter);
				iter = mParticles.erase(iter);
				if (iter == mParticles.end())
					break;
				//delete temp;
			}
		}
	}

	void ParticleEmitter::EmitParticles()
	{
		static int total = 0;

		for (int i = 0; i < 3; i++)
		{
			if (mParticles.size() < mMaxParticles && (total < mTotalParticles || mTotalParticles == -1))
			{
				//TODO: Relative to emitter
				float x, y, z;
				x = fmodf((float)rand(), 6.0) - 3.0;
				y = fmodf((float)rand(), 6.0) - 3.0;
				z = fmodf((float)rand(), 6.0) - 3.0;

				Particle* p = new Particle();
				*p = *mBase;
				p->mPos = vec4f(x, y, z, 1);
				p->mPos += mPos;

				x = fmodf((float)rand(), 6.0) - 3.0;
				y = fmodf((float)rand(), 6.0) - 3.0;
				z = fmodf((float)rand(), 6.0) - 3.0;

				p->mVelocity = vec4f(0, .001, 0, 0);
				mParticles.push_back(p);
			}
		}
	}
}