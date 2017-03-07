#pragma once
#include <vector>
#include "ParticleComponents.h"
#include "Rendering\InputLayoutManager.h"

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
		GSMatrix mLeft, mRight;

		ParticleSystem();
		~ParticleSystem();

	public:
		static ParticleSystem* Instance();
		static void Destroy();

		void SetMatrices(matrix4 _lmodel, matrix4 _lview, matrix4 _lproj, matrix4 _rmodel, matrix4 _rview, matrix4 _rproj);
		void SetGeometryShader();
		void SetVertexPixelShader();

		void Update();
		void Render();
		void AddEmitter(ParticleEmitter* _pemitter);
		void RemoveEmitter(ParticleEmitter* _pemitter);
		bool DoesExist(ParticleEmitter* _pemitter);

	};
}