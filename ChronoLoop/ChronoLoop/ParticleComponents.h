#pragma once
#include <list>
#include <vector>
#include <string>
#include "Rendering\TextureManager.h"
#include "Rendering\Structures.h"
#include "Common\FileIO.h"
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

	struct Particle
	{
		int mLife, mMaxLife;
		//TODO: Opacity and Rotation over time??
		float mSize, mFSize, mSSize;
		vec4f mPos, mPrevPos, mVelocity;
		vec4f mSColor, mEColor, mCurColor;
		bool mActive;

		//Constructors
		Particle();
		Particle(int _life, float _size, vec4f _pos, vec4f _color);
		Particle(int _life, float _size, float _msize, vec4f _pos, vec4f _scolor, vec4f _ecolor);

		Particle& operator=(const Particle& _other);
	};

	class ParticleEmitter
	{
		long mID;
		char* mName;

	protected:
		const char* mTName;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tv;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> text;
		ID3D11Buffer* mVBuffer;
		//BlendMode
		//BoundingBox
		//TODO: Make this attachable, basically a position pointer
		vec4f mPos;

		Particle* mBase;
		std::list<Particle*> mParticles;
		std::vector<GSParticle> mGParticles;
		int mTotalParticles, mMaxParticles, mPerSec;
		vec4f mStartColor, mEndColor;
		bool mIsAnimated;
		float mOffset;
		//TODO: If animated, need to send offset to geometry shader or pixel shader

		virtual void UpdateParticle(Particle* _p, float _delta);
		void CreateBuffers();
		void CreateTextureResource();
		void UpdateBuffers();
		void CleanUpParticles();
		virtual void EmitParticles();

	public:
		bool mActive;

		ParticleEmitter(int _totalp, int _maxp, int _persec, vec4f _pos);
		virtual ~ParticleEmitter();

		ID3D11Buffer* GetVertexBuffer();
		ID3D11ShaderResourceView* GetTexture();
		int GetVertexCount();

		void SetTexture(const char* _tex);
		void SetTexture(const char* _tex, bool _animate, float offset);
		virtual void SetParticle(Particle* _p);

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
			//TODO: Cylinder?
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
		VolumeEmitter(int _totalp, int _maxp, int _persec, vec4f _pos);

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

		float mYLRate, mYHRate;

	public:
		RadialEmitter();
		RadialEmitter(int _totalp, int _maxp, int _persec, vec4f _pos);
	private:
		virtual void UpdateParticle(Particle* _p, float _delta);
		virtual void EmitParticles();

	};

	class Patrick : public ParticleEmitter
	{
	public:
		float y1, y2;

		Patrick();
		Patrick(int _totalp, int _maxp, int _persec, vec4f _pos);
	private:
		virtual void UpdateParticle(Particle* _p, float _delta);
		virtual void EmitParticles();
	};
}