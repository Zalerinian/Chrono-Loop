#pragma once
#include <list>
#include <vector>
#include <string>
#include "../Rendering/TextureManager.h"
#include "../Rendering/Structures.h"
#include "../Common\FileIO.h"
#include <wrl/client.h>


struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11Buffer;
struct ID3D11InputLayout;

namespace Epoch
{
	struct GSParticle
	{
		vec4f pos;
		vec4f col;
		float size;
		float pad[3];
	};
	struct GSMatrix
	{
		matrix4 model;
	};
	struct Texture
	{
		int mType;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tv;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> text;
		union
		{
			struct
			{
				bool mWrap;
				float mSpeed[2];
			};
			struct
			{
				bool mAnimated;
				float mOffset;
				int mFrames;
			};
		};
	};

	struct Particle
	{
	private:
		//TODO: Privatize stuff
	public:
		int emitterType = -1;
		bool mAlive;
		int mLife, mTotalLife;
		float mSize, mStartSize, mEndSize;
		float mOpacity, mStartOpacity, mEndOpacity;
		float mXRadial, mYRadial, mZRadial;
		vec3f mPos, mPrevPos, mVelocity;
		vec4f mColor, mStartColor, mEndColor;

		//Constructors
		Particle() { mAlive = true; }
		Particle& operator=(const Particle& _other);


		//Initialize functions
		static Particle& Init(const Particle& _other);

		/// <summary>Initializes a particle</summary>
		///<returns>The new particle</returns>
		static Particle& Init(int _life = 250 /**< [in] Life of the particle. */,
			float _ss = 0.5f /**< [in] Starting size. */,
			float _es = 0.5f /**< [in] Ending size. */,
			float _so = 1.0f /**< [in] Starting opacity. */,
			float _eo = 1.0f /**< [in] Ending opacity. */,
			float _xr = 0.0f /**< [in] X radial (for moving x in a circular pattern). */,
			float _yr = 0.0f /**< [in] Y radial (for moving y in a circular pattern).. */,
			float _zr = 0.0f /**< [in] Z radial (for moving z in a circular pattern).. */,
			vec3f _pos = vec3f(0, 0, 0) /**< [in] Position. */,
			vec3f _vel = vec3f(0, 0, 0) /**< [in] Initial velocity. */,
			vec4f _scol = vec4f(0, 0, 0, 0) /**< [in] Starting color. */,
			vec4f _ecol = vec4f(0, 0, 0, 0) /**< [in] Ending color. */);

		//Getters

		bool IsActive() const;
		float GetLifeRatio() const;
		int GetLife() const;
		float GetSize() const;
		void GetRadials(float* _x, float* _y, float* _z);
		float* GetXRadial();
		float* GetYRadial();
		float* GetZRadial();
		vec3f* GetPos();
		vec3f* GetPrevPos();
		vec3f GetVelocity() const;
		vec4f GetColor() const;

		//Setters

		void SetLife(int _life);
		void SetPos(vec3f _pos);
		void SetPos(float _x, float _y, float _z);
		void SetSize(float _ssize, float _esize);
		void SetRadials(float _x, float _y, float _z);
		void SetVelocity(vec3f _vel);
		void SetVelocity(float _x, float _y, float _z);
		void SetColors(vec4f _scol, vec4f _ecol);
		void DecLife();
		void Kill();

		//Operator functions

		Particle& operator--();
		Particle operator--(int i);

		//Misc
		void InterpSize();
		void InterpColor();

	};

	class ParticleEmitter
	{
		const char* mDebugName = "ParticleEmit";
		long mID;
		char* mName;

	protected:
		const char* mTName[3];
		Texture mTextures[3];

		struct
		{
			float xoff[4];
			float yoff[4];
		} mPSData;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVBuffer, mPBuffer;
		//BlendMode
		//BoundingBox
		//TODO: Make this attachable, basically a position pointer
		vec3f mPos;
		
		Particle* mBase;
		std::list<Particle*> mParticles;
		std::vector<GSParticle> mGParticles;
		int mTotalParticles, mMaxParticles, mPerSec;
		int total = 0;

		float mMinPX, mMinPY, mMinPZ, mMinVX, mMinVY, mMinVZ;
		float mMaxPX, mMaxPY, mMaxPZ, mMaxVX, mMaxVY, mMaxVZ;


		//TODO: If animated, need to send offset to geometry shader or pixel shader

		virtual void UpdateParticle(Particle* _p, float _delta);
		void CreateBuffers();
		void CreateTextureResource();
		void UpdateBuffers();
		void CleanUpParticles();
		virtual void EmitParticles();
		void Clear();
	public:
		bool mActive, mEnabled, mReuse;

		ParticleEmitter(int _totalp, int _maxp, int _persec, vec3f _pos);
		virtual ~ParticleEmitter();

		ID3D11Buffer* GetVertexBuffer();
		ID3D11Buffer* GetPixelBuffer();
		ID3D11ShaderResourceView* GetTexture(int _index = 0);
		int GetVertexCount();

		void SetTexture(const char* _tex, int _index = 0);
		void SetTexture(const char* _tex, bool _animate, int _frames, float offset, int _index = 0);
		void SetTexture(const char* _tex, bool _wrap, float _speedx, float _speedy, int _index = 0);
		virtual void SetParticle(Particle* _p);
		void SetPosBounds(vec3f _min, vec3f _max);
		void SetVelBounds(vec3f _min, vec3f _max);
		void SetPos(vec3f _pos);
		void FIRE() { mEnabled = true; }
		void CeaseFire() { mEnabled = false; }
		virtual void Reset();
		void Update(float _delta);

	};

	class VolumeEmitter : public ParticleEmitter
	{
		//TODO: Bounding volume bools and structs, use unions
		enum eVolume { eNONE = -1, eAABB, eSPHERE, eCYLINDER };
		struct bvAABB
		{
			vec4f min, max;
		};
		struct bvSPHERE
		{
			float radius;
		};
		struct bvCylinder
		{
			float radius;
			float length;
			vec4f top, bottom;
			int orientation; //0 - Y, 1 - X, 2 - Z
		};

		eVolume mBoundingVolume;
		union
		{
			bvAABB mRect;
			bvSPHERE mSphr;
			bvCylinder mCylndr;
		};
	public:
		VolumeEmitter();
		VolumeEmitter(int _totalp, int _maxp, int _persec, vec3f _pos);

		void SetBoundingVolume(float _l, float _w, float _h);
		void SetBoundingVolume(float _r);
		void SetBoundingVolume(float _r, float _h, int _dir);
	private:
		virtual void UpdateParticle(Particle* _p, float _delta);
		void BindToAABB(Particle* _p);
		void BindToSPHR(Particle* _p);
		void BindToCYL(Particle* _p);
	};

	class RadialEmitter : public ParticleEmitter
	{

	public:
		RadialEmitter(int _totalp, int _maxp, int _persec, vec3f _pos);
	private:
		virtual void UpdateParticle(Particle* _p, float _delta);
		virtual void EmitParticles();

	};

	class TeleportEffect : public ParticleEmitter
	{
	public:
		float y1, y2;

		TeleportEffect(int _totalp, int _maxp, int _persec , vec3f _pos);
	private:
		virtual void UpdateParticle(Particle* _p, float _delta);
		virtual void EmitParticles();
	};

	class Sparks : public ParticleEmitter
	{
	public:
		Sparks(int _totalp, int _maxp, int _persec, vec3f _pos);
		int mTime = 4;
	private:
		float timer = 0;
		void UpdateParticle(Particle* _p, float _delta);
		void EmitParticles();
	};
}