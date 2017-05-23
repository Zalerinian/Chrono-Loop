#pragma once
#include <vector>
#include "ParticleComponents.h"
#include "../Rendering\InputLayoutManager.h"

struct ID3D11GeometryShader;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

namespace Epoch
{
	class ParticleSystem
	{
	private:
		static ParticleSystem* mSystem;

		std::vector<ParticleEmitter*> mPEmitters;
		ID3D11GeometryShader* mGeometryShader;
		ID3D11VertexShader* mVShader;
		ID3D11PixelShader* mPShader;
		ID3D11Buffer* mVBuff;
		ID3D11InputLayout* mILayout;

		ParticleSystem();
		~ParticleSystem();

	public:
		static ParticleSystem* Instance();
		static void Destroy();

		void SetGeometryShader();
		void SetVertexPixelShader();

		int GetNumEmitters() { return mPEmitters.size(); }
		void Update();
		void Render();
		void AddEmitter(ParticleEmitter* _pemitter);
		void RemoveEmitter(ParticleEmitter* _pemitter);
		void Clear();
		bool DoesExist(ParticleEmitter* _pemitter);

	};
}