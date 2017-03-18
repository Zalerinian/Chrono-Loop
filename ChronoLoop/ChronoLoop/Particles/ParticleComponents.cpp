#include "ParticleComponents.h"
#include "../Rendering\renderer.h"
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
		mSize = mSSize = mFSize = _size;
		mPos = _pos;
		mSColor = mEColor = mCurColor = _color;
		mVelocity = vec4f(0, 0, 0, 0);
		mActive = true;
	}
	Particle::Particle(int _life, float _size, float _msize, vec4f _pos, vec4f _scolor, vec4f _ecolor)
	{
		mLife = mMaxLife = _life;
		mSize = mSSize = _size;
		mFSize = _msize;
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
		mSSize = _other.mSSize;
		mFSize = _other.mFSize;
		mPos = _other.mPos;
		mSColor = _other.mSColor;
		mEColor = _other.mEColor;
		mCurColor = _other.mCurColor;
		mActive = _other.mActive;

		return *this;
	}

	//Base Particle Emitter---------------------------------------------

	ParticleEmitter::ParticleEmitter(int _totalp, int _maxp, int _persec, vec4f _pos)
	{
		mTotalParticles = _totalp;
		mMaxParticles = _maxp;
		mPerSec = _persec;
		mPos = _pos;
		mActive = true;
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
		return (int)mGParticles.size();
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
		if (mActive == false)
			return;
		CleanUpParticles();
		EmitParticles();
		for (Particle* p : mParticles)
			UpdateParticle(p, _delta);
		UpdateBuffers();
	}

	void ParticleEmitter::UpdateParticle(Particle* _p, float _delta)
	{
		//TODO: I have no clue, particle stuff
		if (_p->mLife <= 0 || _p->mSize <= 0)
			_p->mActive = false;
		if (_p->mActive)
		{
			_p->mPrevPos = _p->mPos;
			_p->mPos = _p->mPos + (_p->mVelocity * _delta);
			_p->mSize += (_p->mFSize - _p->mSSize) * (1 - ((float)_p->mLife / (float)_p->mMaxLife)) / 10.0f;
			_p->mCurColor += (_p->mEColor - _p->mSColor) *  (1 - ((float)_p->mLife / (float)_p->mMaxLife)) / 10.0f;
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
			if (total >= mTotalParticles && mTotalParticles != -1 && mParticles.size() == 0)
			{
				mActive = false;
				break;
			}
			if (mParticles.size() < mMaxParticles && (total < mTotalParticles || mTotalParticles == -1))
			{
				float x, y, z;
				x = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (-3.0f))));
				y = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (-3.0f))));
				z = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (-3.0f))));
				Particle* p = new Particle();
				*p = *mBase;
				p->mPos = vec4f(0, 0, 0, 1);
				p->mPos += mPos;
				p->mSize = .1f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.25f - .1f)));

				x = -3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - -3)));
				y = -3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - -3)));
				z = -3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3 - -3)));
				p->mVelocity = vec4f(x, y, z, 0);
				mParticles.push_back(p);
				total++;
			}
		}
	}

	//Volume Emitter------------------------------------------------------

	VolumeEmitter::VolumeEmitter(int _totalp, int _maxp, int _persec, vec4f _pos) : ParticleEmitter(_totalp, _maxp, _persec, _pos)
	{

	}

	void VolumeEmitter::UpdateParticle(Particle* _p, float _delta)
	{
		switch (mBoundingVolume)
		{
		case Epoch::VolumeEmitter::eNONE:
			break;
		case Epoch::VolumeEmitter::eAABB:
		{
			BindToAABB(_p);
		}
		break;
		case Epoch::VolumeEmitter::eSPHERE:
		{
			BindToSPHR(_p);
		}
		break;
		case Epoch::VolumeEmitter::eCYLINDER:
		{
			BindToCYL(_p);
		}
		break;
		default:
			break;
		}
		ParticleEmitter::UpdateParticle(_p, _delta);
	}

	void VolumeEmitter::SetBoundingVolume(float _l, float _w, float _h)
	{
		mBoundingVolume = eVolume::eAABB;
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
		mBoundingVolume = eVolume::eSPHERE;
		mSphr.radius = _r;
	}
	void VolumeEmitter::SetBoundingVolume(float _r, float _h, int _dir)
	{
		mBoundingVolume = eVolume::eCYLINDER;

		float d = _h / 2.0f;
		mCylndr.radius = _r;
		mCylndr.length = _h;
		mCylndr.orientation = (_dir < 0 || _dir > 2) ? 0 : _dir;
		mCylndr.top = mCylndr.bottom = mPos;
		switch (mCylndr.orientation)
		{
		case 0:
		{
			mCylndr.top.y += d;
			mCylndr.bottom.y -= d;
		}
		break;
		case 1:
		{
			mCylndr.top.x += d;
			mCylndr.bottom.x -= d;
		}
		break;
		case 2:
		{
			mCylndr.top.z += d;
			mCylndr.bottom.z -= d;
		}
		break;
		default:
			break;
		}
	}

	void VolumeEmitter::BindToAABB(Particle* _p)
	{
		float x = _p->mPos.x, y = _p->mPos.y, z = _p->mPos.z;

		if (x < mRect.min.x || x > mRect.max.x)
		{
			_p->mVelocity.x += (_p->mVelocity.x < 0) ? -.1f : .1f;
			_p->mVelocity.x *= -1;
		}
		if (y < mRect.min.y || y > mRect.max.y)
		{
			_p->mVelocity.y += (_p->mVelocity.y < 0) ? -.1f : .1f;
			_p->mVelocity.y *= -1;
		}
		if (z < mRect.min.z || z > mRect.max.z)
		{
			_p->mVelocity.z += (_p->mVelocity.z < 0) ? -.1f : .1f;
			_p->mVelocity.z *= -1;
		}
	}
	void VolumeEmitter::BindToSPHR(Particle* _p)
	{
		float r = mSphr.radius;
		float d = (_p->mPos - mPos).Magnitude();

		if (d > r)
			_p->mVelocity *= -1;

	}
	void VolumeEmitter::BindToCYL(Particle* _p)
	{
		//TODO: Is broken
		float offset = mCylndr.length / 2.0f;
		float r = mCylndr.radius;
		vec4f dir = (mCylndr.top - mCylndr.bottom);
		float rat = dir.Dot(_p->mPos);
		if (rat < 0)
			rat = 0;
		else if (rat > 1)
			rat = 1;
		vec4f point = mCylndr.bottom + (dir * rat);
		float d = (point - _p->mPos).Magnitude();

		if (d > r)
			_p->mVelocity *= -1;

		/*switch (mCylndr.orientation)
		{
		case 0:
		{
			if (_p->mPos.y > mCylndr.top.y || _p->mPos.y < mCylndr.bottom.y)
				_p->mVelocity.y *= -1;
		}
		break;
		case 1:
		{
			if (_p->mPos.x > mCylndr.top.x || _p->mPos.x < mCylndr.bottom.x)
				_p->mVelocity.x *= -1;
		}
		break;
		case 2:
		{
			if (_p->mPos.z > mCylndr.top.z || _p->mPos.z < mCylndr.bottom.z)
				_p->mVelocity.z *= -1;
		}
		break;
		default:
			break;
		}*/
	}

	//Radial ----------------------

	RadialEmitter::RadialEmitter(int _totalp, int _maxp, int _persec, vec4f _pos) : ParticleEmitter(_totalp, _maxp, _persec, _pos)
	{

	}

	void RadialEmitter::UpdateParticle(Particle* _p, float _delta)
	{
		ParticleEmitter::UpdateParticle(_p, _delta);
		_p->mVelocity.y += .0001f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.001 - .0001)));;
	}

	void RadialEmitter::EmitParticles()
	{
		static int total = 0;

		for (int i = 0; i < mPerSec; i++)
		{
			if (total >= mTotalParticles && mTotalParticles != -1 && mParticles.size() == 0)
			{
				mActive = false;
				break;
			}
			if (mParticles.size() < mMaxParticles && (total < mTotalParticles || mTotalParticles == -1))
			{
				float x, y, z;
				x = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (-3.0f))));
				y = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (-3.0f))));
				z = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (-3.0f))));
				Particle* p = new Particle();
				*p = *mBase;
				p->mPos = vec4f(0, 0, 0, 1);
				p->mPos += mPos;
				//p->mSize = .1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.25 - .1)));

				x = -.05f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.05f - -.05f)));
				y = -.05f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.05f - -.05f)));
				z = -.05f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.05f - -.05f)));
				p->mVelocity = vec4f(x, 0, z, 0);
				mParticles.push_back(p);
				total++;
			}
		}
	}


	Patrick::Patrick(int _totalp, int _maxp, int _persec, vec4f _pos) : ParticleEmitter(_totalp, _maxp, _persec, _pos)
	{

	}

	void Patrick::UpdateParticle(Particle* _p, float _delta)
	{
		ParticleEmitter::UpdateParticle(_p, _delta);
	}

	void Patrick::EmitParticles()
	{
		static int total = 0;

		for (int i = 0; i < mPerSec; i++)
		{
			if (total >= mTotalParticles && mTotalParticles != -1 && mParticles.size() == 0)
			{
				mActive = false;
				break;
			}
			if (mParticles.size() < mMaxParticles && (total < mTotalParticles || mTotalParticles == -1))
			{
				float x, y, z;
				x = -1.25f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.25f - (-1.25f))));
				y = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (-3.0f))));
				z = -1.25f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.25f - (-1.25f))));
				Particle* p = new Particle();
				*p = *mBase;
				p->mPos = vec4f(x, 0, z, 1);
				p->mPos += mPos;
				//p->mSize = .1f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.25f - .1f)));

				x = -.05f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.05f - -.05f)));
				y = y1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (y2 - y1)));
				z = -.05f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.05f - -.05f)));
				p->mVelocity = vec4f(0, y, 0, 0);
				mParticles.push_back(p);
				total++;
			}
		}
	}
}