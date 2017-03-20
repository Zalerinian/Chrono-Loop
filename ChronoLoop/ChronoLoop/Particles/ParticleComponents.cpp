#include "ParticleComponents.h"
#include "../Rendering\renderer.h"
#include <d3d11.h>

namespace Epoch
{

#pragma region Particles

	Particle& Particle::Init(const Particle& _p)
	{
		Particle* p = new Particle();
		*p = _p;

		return *p;
	}
	Particle& Particle::Init(int _life, float _ss, float _es, float _so, float _eo, float _xr, float _yr, float _zr, vec4f _pos, vec4f _vel, vec4f _scol, vec4f _ecol)
	{
		Particle* p = new Particle();
		p->mLife = p->mTotalLife = _life;
		p->mSize = p->mStartSize = _ss;
		p->mEndSize = _es;
		p->mOpacity = p->mStartOpacity = _so;
		p->mEndOpacity = _eo;
		p->mXRadial = _xr;
		p->mYRadial = _yr;
		p->mZRadial = _zr;
		p->mPos = _pos;
		p->mVelocity = _vel;
		p->mStartColor = _scol;
		p->mEndColor = _ecol;

		return *p;
	}

	Particle& Particle::operator=(const Particle& _other)
	{
		mLife = _other.mLife;
		mTotalLife = _other.mTotalLife;
		mSize = _other.mSize;
		mStartSize = _other.mStartSize;
		mEndSize = _other.mEndSize;
		mOpacity = _other.mOpacity;
		mStartOpacity = _other.mStartOpacity;
		mEndOpacity = _other.mEndOpacity;
		mXRadial = _other.mXRadial;
		mYRadial = _other.mYRadial;
		mZRadial = _other.mZRadial;
		mPos = _other.mPos;
		mPrevPos = _other.mPrevPos;
		mColor = _other.mColor;
		mStartColor = _other.mStartColor;
		mEndColor = _other.mEndColor;
		mVelocity = _other.mVelocity;
		mAlive = _other.mAlive;

		return *this;
	}

	bool Particle::IsActive() const
	{
		return mAlive;
	}
	float Particle::GetLifeRatio() const
	{
		return ((float)mLife / (float)mTotalLife);
	}
	int Particle::GetLife() const
	{
		return mLife;
	}
	float Particle::GetSize() const
	{
		return mSize;
	}
	void Particle::GetRadials(float* _x, float* _y, float* _z)
	{
		_x = &mXRadial;
		_y = &mYRadial;
		_z = &mZRadial;
	}
	vec4f* Particle::GetPos()
	{
		return &mPos;
	}
	vec4f* Particle::GetPrevPos()
	{
		return &mPrevPos;
	}
	vec4f Particle::GetVelocity() const
	{
		return mVelocity;
	}
	vec4f Particle::GetColor() const
	{
		return mColor;
	}

	void Particle::SetLife(int _life)
	{
		mLife = mTotalLife = _life;
	}
	void Particle::SetPos(vec4f _pos)
	{
		mPos = _pos;
		mPos.w = 1;
	}
	void Particle::SetPos(float _x, float _y, float _z)
	{
		mPos.x = _x;
		mPos.y = _y;
		mPos.z = _z;
		mPos.w = 1;
	}
	void Particle::SetSize(float _ssize, float _esize)
	{
		mSize = mStartSize = _ssize;
		mEndSize = _esize;
	}
	void Particle::SetRadials(float _x, float _y, float _z)
	{
		mXRadial = _x;
		mYRadial = _y;
		mZRadial = _z;
	}
	void Particle::SetVelocity(vec4f _vel)
	{
		mVelocity = _vel;
		mVelocity.w = 0;
	}
	void Particle::SetVelocity(float _x, float _y, float _z)
	{
		mVelocity.x = _x;
		mVelocity.y = _y;
		mVelocity.z = _z;
		mVelocity.w = 0;
	}
	void Particle::SetColors(vec4f _scol, vec4f _ecol)
	{
		mColor = mStartColor = _scol;
		mEndColor = _ecol;
	}
	void Particle::DecLife()
	{
		mLife--;
	}
	void Particle::Kill()
	{
		mAlive = false;
	}

	Particle& Particle::operator--()
	{
		--mLife;
		return *this;
	}
	Particle Particle::operator--(int i)
	{
		mLife--;
		return *this;
	}

	void Particle::InterpSize()
	{
		mSize += (mEndSize - mStartSize) * (1.0f - GetLifeRatio());
	}
	void Particle::InterpColor()
	{
		mColor += (mEndColor - mStartColor) * (1.0f - GetLifeRatio());
	}

#pragma endregion

#pragma region Base Emitter

	ParticleEmitter::ParticleEmitter(int _totalp, int _maxp, int _persec, vec4f _pos)
	{
		mTotalParticles = _totalp;
		mMaxParticles = _maxp;
		mPerSec = _persec;
		mPos = _pos;
		mActive = true;
		mEnabled = false;
		CreateBuffers();
	}

	ParticleEmitter::~ParticleEmitter()
	{
		delete mBase;
		mTName = nullptr;
		mBase = nullptr;
		for (auto i = mParticles.begin(); i != mParticles.end(); i++)
		{
			Particle* t = *i;
			i = mParticles.erase(i);
			if (i == mParticles.end())
				break;
			delete t;
		}
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
		if (mEnabled == false)
			return;
		CleanUpParticles();
		EmitParticles();
		for (auto p = mParticles.begin(); p != mParticles.end(); p++)
		{
			UpdateParticle(*p, _delta);
		}
		UpdateBuffers();
	}

	void ParticleEmitter::UpdateParticle(Particle* _p, float _delta)
	{
		//TODO: I have no clue, particle stuff
		if (_p->GetLife() <= 0 || _p->GetSize() <= 0)
			_p->Kill();
		if (_p->IsActive())
		{
			*_p->GetPrevPos() = *_p->GetPos();
			*_p->GetPos() = *_p->GetPos() + (_p->GetVelocity() * _delta);
			_p->InterpSize();
			_p->InterpColor();
			(*_p)--;
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
			if ((*iter)->IsActive())
			{
				gps.pos = *(*iter)->GetPos();
				gps.size = (*iter)->GetSize();
				gps.col = (*iter)->GetColor();
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
			if ((*iter)->IsActive() == false)
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
				Particle* p = &Particle::Init(*mBase);
				float x, y, z;
				x = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (-3.0f))));
				y = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (-3.0f))));
				z = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (-3.0f))));
				p->SetPos(0, 0, 0);
				*p->GetPos() = *p->GetPos() + mPos;

				x = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - -3.0f)));
				y = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - -3.0f)));
				z = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - -3.0f)));
				p->SetVelocity(x, 0, z);

				mParticles.push_back(p);
				total++;
			}
		}
	}

#pragma endregion

#pragma region Volume Emitter

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
		float x = _p->GetPos()->x, y = _p->GetPos()->y, z = _p->GetPos()->z;

		if (x < mRect.min.x || x > mRect.max.x)
		{
			vec4f tvel = _p->GetVelocity();
			tvel.x += (_p->GetVelocity().x < 0) ? -.1f : .1f;
			tvel.x *= -1.0f;
			_p->SetVelocity(tvel);
		}
		if (y < mRect.min.y || y > mRect.max.y)
		{
			vec4f tvel = _p->GetVelocity();
			tvel.y += (_p->GetVelocity().y < 0) ? -.1f : .1f;
			tvel.y *= -1.0f;
			_p->SetVelocity(tvel);
		}
		if (z < mRect.min.z || z > mRect.max.z)
		{
			vec4f tvel = _p->GetVelocity();
			tvel.z += (_p->GetVelocity().z < 0) ? -.1f : .1f;
			tvel.z *= -1.0f;
			_p->SetVelocity(tvel);
		}
	}
	void VolumeEmitter::BindToSPHR(Particle* _p)
	{
		float r = mSphr.radius;
		float d = (*_p->GetPos() - mPos).Magnitude();

		vec4f tvel = _p->GetVelocity();
		if (d > r)
			tvel *= -1;
		_p->SetVelocity(tvel);

	}
	void VolumeEmitter::BindToCYL(Particle* _p)
	{
		//TODO: Is broken
		float offset = mCylndr.length / 2.0f;
		float r = mCylndr.radius;
		vec4f dir = (mCylndr.top - mCylndr.bottom);
		float rat = dir.Dot(*_p->GetPos());
		if (rat < 0)
			rat = 0;
		else if (rat > 1)
			rat = 1;
		vec4f point = mCylndr.bottom + (dir * rat);
		float d = (point - *_p->GetPos()).Magnitude();

		vec4f tvel = _p->GetVelocity();
		if (d > r)
			tvel *= -1;
		_p->SetVelocity(tvel);

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

#pragma endregion

#pragma region Radial

	RadialEmitter::RadialEmitter(int _totalp, int _maxp, int _persec, vec4f _pos) : ParticleEmitter(_totalp, _maxp, _persec, _pos)
	{

	}

	void RadialEmitter::UpdateParticle(Particle* _p, float _delta)
	{
		vec4f tvel = _p->GetVelocity();
		_p->mYRadial += .01f;
		//tvel.y += .01f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.1f - .01f)));
		//_p->SetVelocity(tvel);
		tvel = *_p->GetPos();
		tvel.y = 0;
		tvel.w = 0;
		//float radius = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (0.0f))));
		float radius = (tvel - mPos).Magnitude();

		(*_p->GetPos()).x = radius * cos(_p->mYRadial) + mPos.x;
		(*_p->GetPos()).z = radius * sin(-_p->mYRadial) + mPos.z;
		ParticleEmitter::UpdateParticle(_p, _delta);

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
				Particle* p = &Particle::Init(*mBase);
				float x, y, z;
				x = -.05f - mPos.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.05f + mPos.x - (-.05f - mPos.x))));
				y = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (-3.0f))));
				z = -.05f - mPos.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.05f + mPos.x - (-.05f - mPos.z))));
				p->SetPos(0, 0, 0);
				*p->GetPos() = *p->GetPos() + mPos;
				p->SetRadials(0, y, 0);
				x = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - -3.0f)));
				y = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - -3.0f)));
				z = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - -3.0f)));
				p->SetVelocity(x, 1, z);

				mParticles.push_back(p);
				total++;
			}
		}
	}

#pragma endregion

#pragma region Teleport

	IDC::IDC(int _totalp, int _maxp, int _persec, vec4f _pos) : ParticleEmitter(_totalp, _maxp, _persec, _pos)
	{

	}

	void IDC::UpdateParticle(Particle* _p, float _delta)
	{
		ParticleEmitter::UpdateParticle(_p, _delta);
	}

	void IDC::EmitParticles()
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
				Particle* p = &Particle::Init(*mBase);
				float x, y, z;
				x = -1.25f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.25f - (-1.25f))));
				y = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (-3.0f))));
				z = -1.25f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.25f - (-1.25f))));
				p->SetPos(x, 0, z);
				*p->GetPos() = *p->GetPos() + mPos;
				x = -.05f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.05f - -.05f)));
				y = y1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (y2 - y1)));
				z = -.05f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.05f - -.05f)));
				p->SetVelocity(0, y, 0);

				mParticles.push_back(p);
				total++;
			}
		}
	}

#pragma endregion
}