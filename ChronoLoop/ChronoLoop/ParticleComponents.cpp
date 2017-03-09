#include "ParticleComponents.h"
#include "Rendering\renderer.h"
#include <d3d11.h>

namespace Epoch
{

	//Particle----------------------------------------------------------

	Particle::Particle()
	{
		mLife = mMaxLife = 1000;
		mSize = 1.0f;
		mPos = vec4f(0, 0, 0, 0);
		mVelocity = vec4f(0, 0, 0, 0);
		mSColor = mEColor = mCurColor = vec4f(1, 1, 1, 1);
		mActive = true;
	}
	Particle::Particle(int _life, float _size, vec4f _pos, vec4f _color)
	{
		mLife = mMaxLife = _life;
		mSize = _size;
		mPos = _pos;
		mSColor = mEColor = mCurColor= _color;
		mVelocity = vec4f(0, 0, 0, 0);
		mActive = true;
	}
	Particle::Particle(int _life, float _size, vec4f _pos, vec4f _scolor, vec4f _ecolor)
	{
		mLife = mMaxLife = _life;
		mSize = _size;
		mPos = _pos;
		mSColor = mCurColor = _scolor;
		mEColor = _ecolor;
		mVelocity = vec4f(0, 0, 0, 0);
		mActive = true;
	}

	Particle& Particle::operator=(const Particle& _other)
	{
		mLife = _other.mLife;
		mMaxLife = _other.mMaxLife;
		mSize = _other.mSize;
		mPos = _other.mPos;
		mSColor = _other.mSColor;
		mEColor = _other.mEColor;
		mCurColor = _other.mCurColor;
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
		mTName = nullptr;
		mBase = nullptr;
		for (Particle* p : mParticles)
			delete p;
		mParticles.clear();
		mTotalParticles = mMaxParticles = 0;
		mVBuffer->Release();

	}

	void ParticleEmitter::CreateBuffers()
	{
		//TODO: Fix this (ask BURNSIDE)
		mGParticles.resize(mMaxParticles);
		mGParticles.reserve(mMaxParticles);
		for (int i = 0; i < mGParticles.size(); i++)
		{
			GSParticle vp;
			vp.pos = vec4f(0, 0, 0, 0);
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

		Renderer::Instance()->GetDevice()->CreateBuffer(&vDesc, &vData, &mVBuffer);
	}

	void ParticleEmitter::CreateTextureResource()
	{
		//TODO: UNCOMMENT THIS -Patrick
		TextureManager::Instance()->iGetTexture2D(mTName, &tv, &text);
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
			_p->mCurColor += (_p->mEColor - _p->mSColor) *  (1 - ((float)_p->mLife / (float)_p->mMaxLife)) / 100.0f;
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
				gps.col = (*iter)->mCurColor;
				mGParticles[i] = gps;
			}
			else
			{
				gps.pos = vec4f(0, 0, 0, 0);
				gps.size = 0;
				gps.col = vec4f(0, 0, 0, 0);
				mGParticles[i] = gps;
			}
			i++;
		}
		for (; i < mGParticles.size(); i++)
		{
			gps.pos = vec4f(0, 0, 0, 0);
			gps.size = 0;
			gps.col = vec4f(0, 0, 0, 0);
			mGParticles[i] = gps;
		}

		Renderer::Instance()->GetContext()->Map(mVBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mRes);

		memcpy(mRes.pData, mGParticles.data(), sizeof(GSParticle) * mGParticles.size());
		//Graphics 2 slides
		Renderer::Instance()->GetContext()->Unmap(mVBuffer, 0);
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
				delete temp;
			}
		}
	}

	void ParticleEmitter::EmitParticles()
	{
		static int total = 0;

		for (int i = 0; i < mPerSec; i++)
		{
			if (mParticles.size() < mMaxParticles && (total < mTotalParticles || mTotalParticles == -1))
			{
				float x, y, z;
				x = -3.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0 - (-3.0))));
				y = -3.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0 - (-3.0))));
				z = -3.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0 - (-3.0))));
				Particle* p = new Particle();
				*p = *mBase;
				p->mPos = vec4f(0, 0, 0, 1);
				p->mPos += mPos;
				p->mSize = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0 - 0)));

				x = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.025 - 0)));
				y = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.025 - 0)));
				z = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.025 - 0)));
				p->mVelocity = vec4f(x, y, z, 0);
				mParticles.push_back(p);
			}
		}
	}

	//Volume Emitter------------------------------------------------------

	void VolumeEmitter::UpdateParticle(Particle* _p)
	{

	}

	void VolumeEmitter::SetBoundingVolume(float _l, float _w, float _h)
	{
		vec4f MIN, MAX;
		MIN = MAX = mPos;
		MIN.x += _l / 2.0f;
		MIN.y += _h / 2.0f;
		MIN.z += _w / 2.0f;

		MAX.x -= _l / 2.0f;
		MAX.y -= _h / 2.0f;
		MAX.z -= _w / 2.0f;

		mRect.min = MIN;
		mRect.max = MAX;
	}
	void VolumeEmitter::SetBoundingVolume(float _r)
	{
		mSphr.radius = _r;
	}
	void VolumeEmitter::SetBoundingVolume()
	{

	}

}