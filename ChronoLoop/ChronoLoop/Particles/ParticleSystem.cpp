#include "ParticleSystem.h"
#include "../Rendering\renderer.h"
#include "../Core\TimeManager.h"
#include "../Rendering/RasterizerStateManager.h"
#include "../Common/Common.h"

namespace Epoch
{
	ParticleSystem* ParticleSystem::mSystem = nullptr;

	ParticleSystem::ParticleSystem()
	{
		SetGeometryShader();
		SetVertexPixelShader();
	}
	ParticleSystem::~ParticleSystem()
	{
		for (int i = 0; i < mPEmitters.size(); ++i)
			delete mPEmitters[i];

		//mPEmitters.clear();
		mILayout->Release();
		mVBuff->Release();
		mVShader->Release();
		mGeometryShader->Release();
		mPShader->Release();
	}

	ParticleSystem* ParticleSystem::Instance()
	{
		if (!mSystem)
			mSystem = new ParticleSystem();

		return mSystem;
	}

	void ParticleSystem::Destroy()
	{
		delete mSystem;
	}

	void ParticleSystem::SetGeometryShader()
	{
		char* buffer;
		int bytes = 0;
		FileIO::LoadBytes("GSParticles.cso", &buffer, bytes);
		Renderer::Instance()->GetDevice()->CreateGeometryShader(buffer, bytes, nullptr, &mGeometryShader);
		SetD3DName(mGeometryShader, "GSParticles.cso");
		delete buffer;
	}
	void ParticleSystem::SetVertexPixelShader()
	{
		char* buffer;
		int bytes = 0;

		FileIO::LoadBytes("ParticleVertexShader.cso", &buffer, bytes);
		Renderer::Instance()->GetDevice()->CreateVertexShader(buffer, bytes, nullptr, &mVShader);
		SetD3DName(mVShader, "ParticleVertexShader.cso");
		D3D11_INPUT_ELEMENT_DESC iDesc[] =
		{
			{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"USELESS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		Renderer::Instance()->GetDevice()->CreateInputLayout(iDesc, ARRAYSIZE(iDesc), buffer, bytes, &mILayout);

		delete buffer;
		FileIO::LoadBytes("ParticlePixelShader.cso", &buffer, bytes);
		Renderer::Instance()->GetDevice()->CreatePixelShader(buffer, bytes, nullptr, &mPShader);
		delete buffer;
		SetD3DName(mPShader, "ParticlePixelShader.cso");
		CD3D11_BUFFER_DESC desc(sizeof(GSMatrix), D3D11_BIND_CONSTANT_BUFFER);
		Renderer::Instance()->GetDevice()->CreateBuffer(&desc, NULL, &mVBuff);
	}

	void ParticleSystem::Update()
	{
		//TODO: Update emitters
		float dt = TimeManager::Instance()->GetDeltaTime();
		for (ParticleEmitter* emit : mPEmitters)
		{
				if (emit->mActive)
				emit->Update(dt);
		}
		for (int i =0; i < mPEmitters.size(); i++)
		{
			if (mPEmitters[i]->mActive == false)
			{
				RemoveEmitter(mPEmitters[i]);
				i--;
			}
		}
	}

	void ParticleSystem::Render()
	{
		//TODO: Set up and render
		if (mPEmitters.size() == 0)
			return;
		auto cntxt = Renderer::Instance()->GetContext();
		RasterizerStateManager::Instance()->ApplyState(RasterState::eRS_FILLED);
		cntxt->IASetInputLayout(mILayout);
		cntxt->VSSetShader(mVShader, NULL, 0);
		//ID3D11Buffer* buff = mVBuff;
		//cntxt->VSSetConstantBuffers(0, 1, &buff);
		cntxt->GSSetShader(mGeometryShader, NULL, 0);
		cntxt->PSSetShader(mPShader, NULL, 0);
		cntxt->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		//cntxt->UpdateSubresource(mVBuff, 1, NULL, &mLeft, 0, 0);
		for (int i = 0; i < mPEmitters.size(); i++)
		{
			//Set vbuffer and gbuffer
			UINT stride = sizeof(GSParticle);
			UINT offset = 0;
			ID3D11Buffer* buffer = mPEmitters[i]->GetVertexBuffer();
			cntxt->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
			ID3D11ShaderResourceView* srv = mPEmitters[i]->GetTexture();
			cntxt->PSSetShaderResources(0, 1, &srv);
			//draw call
			cntxt->Draw(mPEmitters[i]->GetVertexCount(), 0);
			//Unset gbuffer
		}
		cntxt->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cntxt->IASetInputLayout(InputLayoutManager::Instance().GetInputLayout(VertFormat::eVERT_POSNORMTEX));
		cntxt->GSSetShader(NULL, NULL, 0);
		cntxt->VSSetShader(NULL, NULL, 0);
		cntxt->PSSetShader(NULL, NULL, 0);
		cntxt->VSSetConstantBuffers(0, 0, NULL);
	}

	void ParticleSystem::AddEmitter(ParticleEmitter* _pemitter)
	{
				mPEmitters.push_back(_pemitter);
	}

	void ParticleSystem::RemoveEmitter(ParticleEmitter* _pemitter)
	{
		auto iter = std::find(mPEmitters.begin(), mPEmitters.end(), _pemitter);
		if (iter != mPEmitters.end())
		{
			delete *iter;
			mPEmitters.erase(iter);
		}
	}

	bool ParticleSystem::DoesExist(ParticleEmitter* _pemitter)
	{
		if (std::find(mPEmitters.begin(), mPEmitters.end(), _pemitter) != mPEmitters.end())
			return true;

		return false;
	}
}