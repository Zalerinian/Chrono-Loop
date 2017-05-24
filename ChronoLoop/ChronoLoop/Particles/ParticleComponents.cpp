#include "ParticleComponents.h"
#include "../Rendering\renderer.h"
#include <d3d11.h>
#include "../Core\TimeManager.h"

namespace Epoch
{

#pragma region Particles

	//Broken i guess???
	Particle& Particle::Init(const Particle& _p)
	{
		Particle* p = new Particle();
		*p = _p;

		return *p;
	}
	Particle& Particle::Init(int _life, float _ss, float _es, float _so, float _eo, float _xr, float _yr, float _zr, vec3f _pos, vec3f _vel, vec4f _scol, vec4f _ecol)
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
	vec3f* Particle::GetPos()
	{
		return &mPos;
	}
	vec3f* Particle::GetPrevPos()
	{
		return &mPrevPos;
	}
	vec3f Particle::GetVelocity() const
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
	void Particle::SetPos(vec3f _pos)
	{
		mPos = _pos;
	}
	void Particle::SetPos(float _x, float _y, float _z)
	{
		mPos.x = _x;
		mPos.y = _y;
		mPos.z = _z;
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
	void Particle::SetVelocity(vec3f _vel)
	{
		mVelocity = _vel;
	}
	void Particle::SetVelocity(float _x, float _y, float _z)
	{
		mVelocity.x = _x;
		mVelocity.y = _y;
		mVelocity.z = _z;
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

	ParticleEmitter::ParticleEmitter(int _totalp, int _maxp, int _persec, vec3f _pos)
	{
		mTotalParticles = _totalp;
		mMaxParticles = _maxp;
		mPerSec = _persec;
		mPos = _pos;
		mActive = true;
		mEnabled = false;
		mReuse = false;

		SetPosBounds(vec3f(), vec3f());
		SetVelBounds(vec3f(-2, -2, -2), vec3f(2, 2, 2));

		CreateBuffers();
	}

	ParticleEmitter::~ParticleEmitter()
	{
		delete mBase;
		mTName[0] = mTName[1] = mTName[2] = nullptr;
		mBase = nullptr;
		for (auto i = mParticles.begin(); i != mParticles.end(); ++i)
		{
			delete *i;
		}
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

		//TODO: Make pixel shader buffer
		for (int i = 0; i < 4; i++)
		{
			mPSData.xoff[i] = 0;
			mPSData.yoff[i] = 0;
		}
		vDesc.Usage = D3D11_USAGE_DEFAULT;
		vDesc.ByteWidth = sizeof(mPSData);
		vDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		vDesc.CPUAccessFlags = 0;
		vDesc.MiscFlags = 0;

		vData.pSysMem = &mPSData;
		Renderer::Instance()->GetDevice()->CreateBuffer(&vDesc, &vData, &mPBuffer);

		for (int i = 0; i < 3; i++)
		{
			mTextures[i].mAnimated = false;
			mTextures[i].mOffset = 0;
			mTextures[i].mFrames = 0;
		}
	}

	void ParticleEmitter::CreateTextureResource()
	{
		//TODO: UNCOMMENT THIS -Patrick
		//TextureManager::Instance()->iGetTexture2D(mTName, &tv, &text);
	}

	ID3D11Buffer* ParticleEmitter::GetVertexBuffer()
	{
		return mVBuffer.Get();
	}
	ID3D11Buffer* ParticleEmitter::GetPixelBuffer()
	{
		return mPBuffer.Get();
	}


	ID3D11ShaderResourceView* ParticleEmitter::GetTexture(int _index)
	{
		return mTextures[_index].tv.Get();
	}

	int ParticleEmitter::GetVertexCount()
	{
		return (int)mGParticles.size();
	}
	void ParticleEmitter::SetTexture(const char* _tex, int _index)
	{
		mTName[_index] = _tex;
		mTextures[_index].mType = 0;
		mPSData.xoff[_index] = 0;
		TextureManager::Instance()->iGetTexture2D(mTName[_index], &mTextures[_index].tv, &mTextures[_index].text);

	}
	void ParticleEmitter::SetTexture(const char* _tex, bool _animated, int _frames, float _offset,  int _index)
	{
		mTName[_index] = _tex;
		mTextures[_index].mType = 2;
		mTextures[_index].mAnimated = _animated;
		mTextures[_index].mOffset = _offset;
		mTextures[_index].mFrames = _frames;
		TextureManager::Instance()->iGetTexture2D(mTName[_index], &mTextures[_index].tv, &mTextures[_index].text);
	}
	void ParticleEmitter::SetTexture(const char* _tex, bool _wrap, float _speedx, float _speedy, int _index)
	{
		mTName[_index] = _tex;
		mTextures[_index].mType = 3;
		mTextures[_index].mWrap = _wrap;
		mTextures[_index].mSpeed[0] = _speedx;
		mTextures[_index].mSpeed[1] = _speedy;
		TextureManager::Instance()->iGetTexture2D(mTName[_index], &mTextures[_index].tv, &mTextures[_index].text);
	}

	void ParticleEmitter::SetParticle(Particle* _p)
	{
		mBase = _p;
	}

	void ParticleEmitter::SetPosBounds(vec3f _min, vec3f _max)
	{
		mMinPX = _min.x;
		mMinPY = _min.y;
		mMinPZ = _min.z;

		mMaxPX = _max.x;
		mMaxPY = _max.y;
		mMaxPZ = _max.z;
	}
	void ParticleEmitter::SetVelBounds(vec3f _min, vec3f _max)
	{
		mMinVX = _min.x;
		mMinVY = _min.y;
		mMinVZ = _min.z;

		mMaxVX = _max.x;
		mMaxVY = _max.y;
		mMaxVZ = _max.z;
	}
	void ParticleEmitter::SetPos(vec3f _pos)
	{
		mPos = _pos;
	}
	void ParticleEmitter::Update(float _delta)
	{
		if (mEnabled == false)
		{
			CleanUpParticles();
			for (auto p = mParticles.begin(); p != mParticles.end(); p++)
			{
				UpdateParticle(*p, _delta);
			}
			UpdateBuffers();
			return;
		}
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
		Renderer::Instance()->GetRendererLock().lock();
		Renderer::Instance()->GetContext()->Map(mVBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mRes);

		memcpy(mRes.pData, mGParticles.data(), sizeof(GSParticle) * mGParticles.size());
		//Graphics 2 slides
		Renderer::Instance()->GetContext()->Unmap(mVBuffer.Get(), 0);

		for (int i = 0; i < 3; i++)
		{
			if (mTextures[i].mType == 1)
				continue;
			if (mTextures[i].mType == 3)
			{
				mPSData.xoff[i] += mTextures[i].mSpeed[0];
				mPSData.yoff[i] += mTextures[i].mSpeed[1];
			}
			else
			{
				mPSData.xoff[i] += mTextures[i].mOffset;
				if (mPSData.xoff[i] > mTextures[i].mOffset * mTextures[i].mFrames)
					mPSData.xoff[i] = 0;
			}

		}

		Renderer::Instance()->GetContext()->UpdateSubresource(mPBuffer.Get(), 0, nullptr, &mPSData, 0, 0);
		Renderer::Instance()->GetRendererLock().unlock();


	}

	void ParticleEmitter::CleanUpParticles()
	{
		auto i = mGParticles.begin();
		for (auto iter = mParticles.begin(); iter != mParticles.end() && i != mGParticles.end(); ++iter, ++i)
		{
			if ((*iter)->IsActive() == false)
			{
				delete (*iter);
				iter = mParticles.erase(iter);
				if (iter == mParticles.end())
					break;
			}
		}
	}

	void ParticleEmitter::EmitParticles()
	{

		for (int i = 0; i < mPerSec; i++)
		{
			if (total >= mTotalParticles && mTotalParticles != -1 && mParticles.size() == 0)
			{
				if (mReuse)
				{
					Reset();
					mEnabled = false;
					break;
				}
				else
				{
					mActive = false;
					break;
				}
			}
			if (mParticles.size() < mMaxParticles && (total < mTotalParticles || mTotalParticles == -1))
			{
				//Particle* p = &Particle::Init(*mBase);
				Particle* p = new Particle();
				p->emitterType = 0xFFEEDDCC;
				(*p) = (*mBase);
				float x, y, z;
				x = (mMinPX + mPos.x) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((mMaxPX + mPos.x) - (mMinPX + mPos.x))));
				y = (mMinPY + mPos.y) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((mMaxPY + mPos.y) - (mMinPY + mPos.y))));
				z = (mMinPZ + mPos.z) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((mMaxPZ + mPos.z) - (mMinPZ + mPos.z))));
				p->SetPos(x, y, z);

				x = mMinVX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVX - mMinVX)));
				y = mMinVY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVY - mMinVY)));
				z = mMinVZ + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVZ - mMinVZ)));
				p->SetVelocity(x, y, z);

				mParticles.push_back(p);
				total++;
			}
		}
	}

	void ParticleEmitter::Clear()
	{
		for (auto it = mParticles.begin(); it != mParticles.end(); ++it) {
			delete *it;
		}
		mParticles.clear();
	}

	void ParticleEmitter::Reset()
	{
		Clear();
		total = 0;
	}

#pragma endregion

#pragma region Volume Emitter

	VolumeEmitter::VolumeEmitter(int _totalp, int _maxp, int _persec, vec3f _pos) : ParticleEmitter(_totalp, _maxp, _persec, _pos)
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

	RadialEmitter::RadialEmitter(int _totalp, int _maxp, int _persec, vec3f _pos) : ParticleEmitter(_totalp, _maxp, _persec, _pos)
	{

	}

	void RadialEmitter::UpdateParticle(Particle* _p, float _delta)
	{
		//vec4f tvel = _p->GetVelocity();
		//_p->mYRadial += .01f;
		////tvel.y += .01f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.1f - .01f)));
		////_p->SetVelocity(tvel);
		//tvel = *_p->GetPos();
		//tvel.y = 0;
		//tvel.w = 0;
		////float radius = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (0.0f))));
		//float radius = (tvel - mPos).Magnitude();
		//
		//(*_p->GetPos()).x = radius * cos(_p->mYRadial) + mPos.x;
		//(*_p->GetPos()).z = radius * sin(-_p->mYRadial) + mPos.z;
		ParticleEmitter::UpdateParticle(_p, _delta);

	}

	void RadialEmitter::EmitParticles()
	{
		for (int i = 0; i < mPerSec; i++)
		{
			if (total >= mTotalParticles && mTotalParticles != -1 && mParticles.size() == 0)
			{
				if (mReuse)
				{
					Reset();
					mEnabled = false;
					break;
				}
				else
				{
					mActive = false;
					break;
				}
			}
			if (mParticles.size() < mMaxParticles && (total < mTotalParticles || mTotalParticles == -1))
			{
				//Particle* p = &Particle::Init(*mBase);
				Particle* p = new Particle();
				p->emitterType = 0x000000FF;
				(*p) = (*mBase);
				float x, y, z;
				x = (mMinPX + mPos.x) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((mMaxPX + mPos.x) - (mMinPX + mPos.x))));
				y = (mMinPY + mPos.y) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((mMaxPY + mPos.y) - (mMinPY + mPos.y))));
				z = (mMinPZ + mPos.z) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((mMaxPZ + mPos.z) - (mMinPZ + mPos.z))));
				p->SetPos(mPos.x, mPos.y, mPos.z);

				p->SetRadials(0, y, 0);
				x = mMinVX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVX - mMinVX)));
				y = mMinVY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVY - mMinVY)));
				z = mMinVZ + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVZ - mMinVZ)));
				p->SetVelocity(x, 0, z);

				mParticles.push_back(p);
				total++;
			}
		}
	}

#pragma endregion

#pragma region Teleport

	TeleportEffect::TeleportEffect(int _totalp, int _maxp, int _persec, vec3f _pos) : ParticleEmitter(_totalp, _maxp, _persec, _pos)
	{

	}

	void TeleportEffect::UpdateParticle(Particle* _p, float _delta)
	{
		ParticleEmitter::UpdateParticle(_p, _delta);
	}

	void TeleportEffect::EmitParticles()
	{
		for (int i = 0; i < mPerSec; i++)
		{
			if (total >= mTotalParticles && mTotalParticles != -1 && mParticles.size() == 0)
			{
				if (mReuse)
				{
					Reset();
					mEnabled = false;
					break;
				}
				else
				{
					mActive = false;
					break;
				}
			}
			if (mParticles.size() < mMaxParticles && (total < mTotalParticles || mTotalParticles == -1))
			{
				//Particle* p = &Particle::Init(*mBase);
				Particle* p = new Particle();
				p->emitterType = 0x0000FF00;
				(*p) = (*mBase);
				float x, y, z;
				x = (mMinPX + mPos.x) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((mMaxPX + mPos.x) - (mMinPX + mPos.x))));
				y = (mMinPY + mPos.y) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((mMaxPY + mPos.y) - (mMinPY + mPos.y))));
				z = (mMinPZ + mPos.z) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((mMaxPZ + mPos.z) - (mMinPZ + mPos.z))));
				//x = -1.25f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.25f - (-1.25f))));
				//y = -3.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0f - (-3.0f))));
				//z = -1.25f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.25f - (-1.25f))));
				p->SetPos(x, mPos.y, z);

				x = mMinVX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVX - mMinVX)));
				y = mMinVY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVY - mMinVY)));
				z = mMinVZ + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVZ - mMinVZ)));
				p->SetVelocity(0, y, 0);

				mParticles.push_back(p);
				total++;
			}
		}
	}

#pragma endregion

#pragma region Sparks

	Sparks::Sparks(int _totalp, int _maxp, int _persec, vec3f _pos) : ParticleEmitter(_totalp, _maxp, _persec, _pos)
	{

	}

	void Sparks::UpdateParticle(Particle* _p, float _delta)
	{
		//TODO: Special update
		// x = x, y = sin(t), z = z // t -> yradial
		_p->SetVelocity(vec3f(_p->GetVelocity().x, cos(_p->mYRadial) * 2.25f, _p->GetVelocity().z));
		_p->mYRadial += static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / .3));
		ParticleEmitter::UpdateParticle(_p, _delta );
	}

	void Sparks::EmitParticles()
	{
		timer += TimeManager::Instance()->GetDeltaTime();

		if ((int)timer % mTime != 0)
			return;

		for (int i = 0; i < mPerSec; i++)
		{
			if (total >= mTotalParticles && mTotalParticles != -1 && mParticles.size() == 0)
			{
				if (mReuse)
				{
					Reset();
					mEnabled = false;
					break;
				}
				else
				{
					mActive = false;
					break;
				}
			}
			if (mParticles.size() < mMaxParticles && (total < mTotalParticles || mTotalParticles == -1))
			{
				//Particle* p = &Particle::Init(*mBase);
				Particle* p = new Particle();
				p->emitterType = 0x10FF0002;
				(*p) = (*mBase);
				float x, y, z;
				x = (mMinPX + mPos.x) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((mMaxPX + mPos.x) - (mMinPX + mPos.x))));
				y = (mMinPY + mPos.y) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((mMaxPY + mPos.y) - (mMinPY + mPos.y))));
				z = (mMinPZ + mPos.z) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((mMaxPZ + mPos.z) - (mMinPZ + mPos.z))));
				p->SetPos(x, y, z);
				p->SetLife((rand() % 51) + 50);


				vec3f sc, ec;
				x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0));
				y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0));
				z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0));
				sc = vec3f(x, y, x);												
				x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0));
				y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0));
				z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.0));
				ec = vec3f(x, y, x);

				//p->SetColors(sc, sc);

				x = mMinVX + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVX - mMinVX)));
				y = mMinVY + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVY - mMinVY)));
				z = mMinVZ + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (mMaxVZ - mMinVZ)));
				p->SetVelocity(x, 0, z);
				p->mYRadial = 0;

				mParticles.push_back(p);
				total++;
			}
		}
	}

#pragma endregion

}